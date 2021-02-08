#include "Entity.h"
#include "Utils.h"

#include <sstream>
#include <utility>

using namespace mcplus;

static EntityId next_entity_id = 0;

using EntityCreator = std::function<std::shared_ptr<Entity>(const std::string&, std::optional<EntitySolver> solver)>;

static Location2f getLocationFromRaw(const std::string& raw);
static std::shared_ptr<Entity> createArrowEntity(const std::string& raw, std::optional<EntitySolver> solver);
static std::shared_ptr<Entity> createItemEntity(const std::string& raw, std::optional<EntitySolver> solver);

Entity::Field::Field(Entity::Field::Extractor extractor) : extractor(std::move(extractor)) {

}

Entity::Data::Data(EntityId id, const Location2f& location, bool removed, const Entity::UpdateMapper& updateMap) {
    this->id        = id;
    this->location  = location;
    this->removed   = removed;
    this->updateMap = updateMap;
}

Entity::Data::Data(EntityId id, const Location2f& location, bool removed) {
    this->id        = id;
    this->location  = location;
    this->removed   = removed;
    this->updateMap = {};
}

Entity::Entity(std::shared_ptr<Data> data) {
    this->_data = std::move(data);
}

Entity::Entity() {
    this->_data = std::make_shared<Entity::Data>(next_entity_id++, Location2f{}, false);
}

EntityId Entity::id() const {
    return _data->id;
}

const Location2f& Entity::getLocation() const {
    return _data->location;
}

void Entity::setLocation(const Location2f& location) {
    auto fixedLocation = static_cast<FixedLocation>(location);

    auto& actualLocation = _data->location;
    auto& updateMap      = _data->updateMap;

    if (actualLocation.x != location.x) {
        updateMap[Entity::X_POSITION] = Field([fixedLocation]() {
            return std::to_string(fixedLocation.x << 4 | fixedLocation.xDecimal);
        });
    }
    if (actualLocation.y != location.y) {
        updateMap[Entity::Y_POSITION] = Field([fixedLocation]() {
            return std::to_string(fixedLocation.y << 4 | fixedLocation.yDecimal);
        });
    }
    if (actualLocation.world != location.world) {
        updateMap[Entity::LEVEL] = Field([location]() {
            return std::to_string(location.world);
        });
    }

    actualLocation = location;
}

bool Entity::isRemoved() const {
    return _data->removed;
}

void Entity::remove() {
    _data->removed = true;
}

std::string Entity::raw() const {
    std::stringstream ss{};

    auto fixedLocation = static_cast<FixedLocation>(_data->location);

    ss << (fixedLocation.x << 4 | fixedLocation.xDecimal) << ':';
    ss << (fixedLocation.y << 4 | fixedLocation.yDecimal);

    return ss.str();
}

std::string Entity::rawUpdate() const {
    std::stringstream ss{};

    auto& updateMap = _data->updateMap;

    if (!updateMap.empty()) {
        auto it = updateMap.begin();
        ss << it->first << ',' << it->second.extractor();

        for (it = updateMap.erase(it); it != updateMap.end(); it = updateMap.erase(it)) {
            ss << ';' << it->first << ',' << it->second.extractor();
        }
    }

    return ss.str();
}

ArrowEntity::ArrowData::ArrowData(EntityId id, const Location2f& location, bool removed,
                                  const Entity::UpdateMapper& updateMap, std::shared_ptr<Entity> owner,
                                  Direction attackDirection, Damage_t damage) : Entity::Data(id, location, removed, updateMap) {
    this->owner = std::move(owner);
    this->attackDirection = attackDirection;
    this->damage = damage;
}

ArrowEntity::ArrowData::ArrowData(EntityId id, const Location2f& location, bool removed, std::shared_ptr<Entity> owner,
                                  Direction attackDirection, Damage_t damage) : Entity::Data(id, location, removed) {
    this->owner = std::move(owner);
    this->attackDirection = attackDirection;
    this->damage = damage;
}

ArrowEntity::ArrowEntity(std::shared_ptr<Entity::Data> data) : Entity(std::move(data)) {

}

ArrowEntity::ArrowEntity(const Location2f& location,
                         std::shared_ptr<Entity> owner,
                         Direction attackDirection,
                         Damage_t damage) {
    this->_data = std::make_shared<ArrowEntity::ArrowData>(next_entity_id++, location, false, std::move(owner), attackDirection, damage);
}

ArrowEntity::ArrowEntity() : Entity() {
    this->_data = std::make_shared<ArrowEntity::ArrowData>(next_entity_id++, Location2f{}, false, nullptr, Direction::NONE, 0);
}

void ArrowEntity::tick() {

}

std::shared_ptr<Entity> ArrowEntity::get() const {
    return std::shared_ptr<Entity>{new ArrowEntity()};
}

std::string ArrowEntity::raw() const {
    std::stringstream ss{};

    const auto* data = dynamic_cast<ArrowEntity::ArrowData*>(_data.get());
    const auto& owner = data->owner;
    const auto& attackDirection = data->attackDirection;
    const auto& damage = data->damage;

    ss << "Arrow["
       << Entity::raw()                         << ':'
       << id()                                  << ':'
       << (owner ? owner->id() : 0)             << ':'
       << static_cast<int32_t>(attackDirection) << ':'
       << damage                                << ':'
       << getLocation().world                   << ']';

    return ss.str();
}

std::shared_ptr<Entity> mcplus::createEntity(const std::string& raw, std::optional<EntitySolver> solver) {
    static std::unordered_map<std::string, EntityCreator> _data{
            {"Arrow", createArrowEntity}
    };

    std::string name = raw.substr(0, raw.find('['));
    return _data[name](raw.substr(name.size() + 1, raw.size() - 1), std::move(solver));
}

static Location2f getLocationFromRaw(const std::string& raw) {
    FixedLocation fixedLocation{static_cast<int32_t>(std::strtol(raw.substr(0, raw.find(':')).c_str(), nullptr, 10)),
                                static_cast<int32_t>(std::strtol(raw.substr(raw.find(':') + 1).c_str(), nullptr, 10))};
    return {0, fixedLocation, Direction::NONE};
}

static std::shared_ptr<Entity> createArrowEntity(const std::string& raw, std::optional<EntitySolver> solver) {
    std::vector<std::string> dataList{};
    dataList.reserve(5);
    utils::splitString(raw, ":", dataList);

    std::shared_ptr<Entity> solvedEntity{};
    EntityId id = std::strtol(dataList[2].c_str(), nullptr, 10);

    if (solver.has_value()) {
        solvedEntity = solver.value()(id);
    }

    return std::make_shared<ArrowEntity>(getLocationFromRaw(dataList[0] + ':' + dataList[1]),
                                         solvedEntity,
                                         static_cast<Direction>(std::strtol(dataList[3].c_str(), nullptr, 10)),
                                         std::strtol(dataList[4].c_str(), nullptr, 10));
}

static std::shared_ptr<Entity> createItemEntity(const std::string& raw) {
    return {};
}

