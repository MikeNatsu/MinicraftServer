#include "Packet.h"

#include <stdexcept>
#include <utility>
#include <sstream>

#include "Utils.h"

using namespace mcplus;

static inline void check_raw(PacketId id, const RawPacket& rawPacket) {
    if (id == rawPacket.id) {
        return;
    }

    throw std::invalid_argument("Raw packet is not compatible with packet ID");
}

InvalidPacket::InvalidPacket(const std::string& message) {
    this->message = message;
}

InvalidPacket::InvalidPacket(const RawPacket& raw) {
    this->message = {};

    *this = raw;
}

InvalidPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::INVALID), message};
}

InvalidPacket& InvalidPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::INVALID), raw);

    this->message = raw.data;

    return *this;
}

PingPacket::PingPacket(PingType type) : type(type) {}

PingPacket::PingPacket(const RawPacket& raw) {
    this->type = PingType::AUTO;

    *this = raw;
}

PingPacket::operator RawPacket() const  {
    std::string str;

    if (type == PingType::AUTO) {
        str = "auto";
    } else if (type == PingType::MANUAL) {
        str = "manual";
    } else {
        throw std::runtime_error("Ping Type missed");
    }

    return RawPacket{static_cast<PacketId>(PacketType::PING), str};
}

PingPacket& PingPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::PING), raw);

    auto ping = raw.data;
    if (ping == "auto") {
        this->type = PingType::AUTO;
    } else if (ping == "manual") {
        this->type = PingType::MANUAL;
    } else {
        throw std::invalid_argument("Ping Raw Packet is not valid!");
    }

    return *this;
}

LoginPacket::LoginPacket(const std::string& username, const VersionPack& version) {
    this->username = username;
    this->version = version;
}

LoginPacket::LoginPacket(const RawPacket& raw) {
    this->username = {};

    *this = raw;
}

LoginPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::LOGIN), username + ';' + (std::string) version};
}

LoginPacket& LoginPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::LOGIN), raw);

    std::vector<std::string> splitList{};
    utils::splitString(raw.data, ";", splitList);

    if (splitList.size() != 2) {
        throw std::invalid_argument("Login Raw Packet is not valid!");
    }

    username = splitList[0];
    version = splitList[1];

    return *this;
}

GamePacket::GamePacket(const std::string& mode,
                       int32_t time,
                       float gameSpeed,
                       bool pastDay,
                       int32_t score,
                       int32_t playerCount,
                       int32_t awakenPlayer) {
    this->mode         = mode;
    this->time         = time;
    this->gameSpeed    = gameSpeed;
    this->pastDay      = pastDay;
    this->score        = score;
    this->playerCount  = playerCount;
    this->awakenPlayer = awakenPlayer;
}

GamePacket::GamePacket(const RawPacket& raw) {
    this->mode         = {};
    this->time         = 0;
    this->gameSpeed    = 0;
    this->pastDay      = false;
    this->score        = 0;
    this->playerCount  = 0;
    this->awakenPlayer = 0;

    *this = raw;
}

GamePacket::operator RawPacket() const {
    std::stringstream ss{};

    ss << mode << ';' << time << ';'
       << gameSpeed << ';' << (pastDay ? "true" : "false") << ';'
       << score << ';' << playerCount << ';' << awakenPlayer;

    return RawPacket{static_cast<PacketId>(PacketType::GAME), ss.str()};
}

GamePacket& GamePacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::GAME), raw);

    std::vector<std::string> splitList{};
    utils::splitString(raw.data, ";", splitList);

    if (splitList.size() != 7) {
       throw std::invalid_argument("Game Raw Packet is not valid!");
    }

    this->mode         = splitList[0];
    this->time         = std::strtol(splitList[1].c_str(), nullptr, 10);
    this->gameSpeed    = std::stof(splitList[2]);
    this->pastDay      = splitList[3] == "true";
    this->score        = std::strtol(splitList[4].c_str(), nullptr, 10);
    this->playerCount  = std::strtol(splitList[5].c_str(), nullptr, 10);
    this->awakenPlayer = std::strtol(splitList[6].c_str(), nullptr, 10);

    return *this;
}

InitPacket::InitPacket(int32_t id, int32_t width, int32_t height, int32_t level, int32_t x, int32_t y) {
    this->id     = id;
    this->width  = width;
    this->height = height;
    this->level  = level;
    this->x      = x;
    this->y      = y;
}

InitPacket::InitPacket(const RawPacket& raw) {
    this->id     = 0;
    this->width  = 0;
    this->height = 0;
    this->level  = 0;
    this->x      = 0;
    this->y      = 0;

    *this = raw;
}

InitPacket::operator RawPacket() const {
    std::stringstream ss{};

    ss << id << ',' << width << ',' << height << ',' << level << ',' << x << ',' << y;

    return RawPacket{static_cast<PacketId>(PacketType::INIT), ss.str()};
}

InitPacket& InitPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::INIT), raw);

    std::vector<std::string> splitList{};
    utils::splitString(raw.data, ",", splitList);

    if (splitList.size() != 6) {
        throw std::invalid_argument("Init Raw Packet is not valid!");
    }

    this->id     = std::strtol(splitList[0].c_str(), nullptr, 10);
    this->width  = std::strtol(splitList[1].c_str(), nullptr, 10);
    this->height = std::strtol(splitList[2].c_str(), nullptr, 10);
    this->level  = std::strtol(splitList[3].c_str(), nullptr, 10);
    this->x      = std::strtol(splitList[4].c_str(), nullptr, 10);
    this->y      = std::strtol(splitList[5].c_str(), nullptr, 10);

    return *this;
}

LoadPacket::LoadPacket(int32_t currentLevel) {
    this->currentLevel = currentLevel;
}

LoadPacket::LoadPacket(const RawPacket& raw) {
    this->currentLevel = 0;

    *this = raw;
}

LoadPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::LOAD), std::to_string(currentLevel)};
}

LoadPacket& LoadPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::LOAD), raw);

    this->currentLevel = std::strtol(raw.data.c_str(), nullptr, 10);

    return *this;
}

TilesPacket::TilesPacket(const std::vector<Tile>& tileList) {
    this->tileList = tileList;
}

TilesPacket::TilesPacket(const RawPacket& raw) {
    this->tileList = {};
    
    *this = raw;
}

TilesPacket::operator RawPacket() const {
    std::stringstream ss{};

    if (!tileList.empty()) {
        ss << tileList[0].id << ',' << (int32_t) tileList[0].data;
        std::for_each(tileList.begin() + 1, tileList.end(), [&ss](const auto& tile){ ss << ',' << tile.id << ',' << (int32_t) tile.data; });
    }

    return RawPacket{static_cast<PacketId>(PacketType::TILES), ss.str()};
}

TilesPacket& TilesPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::TILES), raw);

    std::vector<std::string> idDataList{};
    utils::splitString(raw.data, ",", idDataList);

    if (idDataList.size() % 2 == 1) {
        throw std::runtime_error("TilesPacket: data is odd");
    }

    this->tileList.clear();
    this->tileList.reserve(idDataList.size());

    for (auto it = idDataList.begin(); it != idDataList.end(); it += 2) {
        this->tileList.emplace_back((TileId) std::strtol(it->c_str(), nullptr, 10), (uint8_t) std::strtol((it + 1)->c_str(), nullptr, 10));
    }

    return *this;
}

EntitiesPacket::EntitiesPacket(const std::vector<std::shared_ptr<Entity>>& entityList) {
    this->entityList = entityList;
}

EntitiesPacket::EntitiesPacket(const RawPacket& raw) {
    this->entityList = {};

    *this = raw;
}

EntitiesPacket::operator RawPacket() const {
    std::stringstream ss{};

    if (!entityList.empty()) {
        ss << entityList[0]->rawUpdate();
        std::for_each(entityList.begin() + 1, entityList.end(), [&ss](const auto& entity){ ss << ',' << entity->raw(); });
    }

    return RawPacket{static_cast<PacketId>(PacketType::ENTITIES), ss.str()};
}

EntitiesPacket& EntitiesPacket::operator=(const RawPacket & raw) {
    check_raw(static_cast<PacketId>(PacketType::ENTITIES), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ",", dataList);

    this->entityList.clear();
    this->entityList.reserve(dataList.size());

    for (auto it = dataList.begin(); it != dataList.end(); it++) {
        this->entityList.emplace_back(createEntity(*it));
    }

    return *this;
}

TilePacket::TilePacket(WorldId world, int32_t position, const Tile& tile) {
    this->world = world;
    this->position = position;
    this->tile = tile;
}

TilePacket::TilePacket(const RawPacket& raw) {
    this->world = 0;
    this->position = {};
    this->tile = {};

    *this = raw;
}

TilePacket::operator RawPacket() const {
    std::stringstream ss{};

    ss << world << ';' << position << ';' << tile.id << ';' << tile.data;

    return RawPacket{static_cast<PacketId>(PacketType::TILE), ss.str()};
}

TilePacket& TilePacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::TILE), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ";", dataList);

    this->world = strtol(dataList[0].c_str(), nullptr, 10);
    this->position = strtol(dataList[1].c_str(), nullptr, 10);
    this->tile = Tile{static_cast<TileId>(strtol(dataList[2].c_str(), nullptr, 10)), (uint8_t) strtol(dataList[3].c_str(), nullptr, 10)};

    return *this;
}

EntityPacket::EntityPacket(const Entity& entity) {
    this->entity = entity.get();
}

EntityPacket::EntityPacket(const RawPacket& raw) {
    this->entity = {};

    *this = raw;
}

EntityPacket::operator RawPacket() const {
    std::string rawUpdate = entity->rawUpdate();
    return RawPacket{static_cast<PacketId>(PacketType::ENTITY), rawUpdate.empty() ? "-1;" : std::to_string(entity->id()) + ';' + rawUpdate};
}

EntityPacket& EntityPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::ENTITY), raw);

    this->entity = createEntity(raw.data);

    return *this;
}

PlayerPacket::PlayerPacket(const VersionPack& version,
                           int32_t x,
                           int32_t y,
                           int32_t spawnX,
                           int32_t spawnY,
                           int32_t health,
                           int32_t hunger,
                           int32_t armor,
                           int32_t armorDamageBuffer,
                           ItemMaterial curArmor,
                           int32_t score,
                           int32_t level,
                           const std::vector<Potion>& potionList,
                           const Color& shirtColor,
                           bool skinOn,
                           std::vector<Item> inventory) {
    this->version = version;

    this->x = x;
    this->y = y;
    this->spawnX = spawnX;
    this->spawnY = spawnY;
    this->health = health;
    this->hunger = hunger;
    this->armor = armor;
    this->armorDamageBuffer = armorDamageBuffer;
    this->curArmor = curArmor;
    this->score = score;
    this->level = level;

    this->potionList = potionList;
    this->shirtColor = shirtColor;
    this->skinon     = skinOn;

    this->inventory = std::move(inventory);
}

PlayerPacket::PlayerPacket(const RawPacket &raw) {
    this->version = VersionPack{};

    this->x                 = 0;
    this->y                 = 0;
    this->spawnX            = 0;
    this->spawnY            = 0;
    this->health            = 0;
    this->hunger            = 0;
    this->armor             = 0;
    this->armorDamageBuffer = 0;
    this->curArmor          = ItemMaterial::UNKNOWN;
    this->score             = 0;
    this->level             = 0;

    this->potionList = {};
    this->shirtColor = Color{0};
    this->skinon     = false;

    this->inventory = std::vector<Item>();

    *this = raw;
}

PlayerPacket::operator RawPacket() const {
    std::stringstream ss{};

    ss << (std::string) version << '\n';

    ss << x << ',' << y << ','
       << spawnX << ',' << spawnY << ','
       << health << ',' << hunger << ','
       << armor << ',' << armorDamageBuffer << ',' << getItemName(curArmor) << ','
       << score << ',' << level;

    ss << ",PotionEffects[";
    if (potionList.empty()) {
        ss << ']';
    } else {
        ss << getPotionName(potionList[0].type) << ';' << potionList[0].duration;
        std::for_each(potionList.begin() + 1, potionList.end(), [&ss](const auto& potion){ ss << ':' << getPotionName(potion.type) << ';' << potion.duration; });
    }
    ss << ',';
    ss << shirtColor.raw() << ',' << (skinon ? "true" : "false");

    ss << '\n';

    if (inventory.empty()) {
        ss << "NULL";
    } else {
        ss << inventory[0].raw();
        std::for_each(inventory.begin() + 1, inventory.end(), [&ss](const auto& item) { ss << ',' << item.raw(); });
    }

    return RawPacket{static_cast<PacketId>(PacketType::PLAYER), ss.str()};
}

PlayerPacket& PlayerPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::PLAYER), raw);

    std::vector<std::string> globalList{};
    utils::splitString(raw.data, "\n", globalList);

    this->version = globalList[0];

    std::vector<std::string> statList{};
    utils::splitString(globalList[1], ",", statList);

    this->x                 = std::strtol(statList[0].c_str(), nullptr, 10);
    this->y                 = std::strtol(statList[1].c_str(), nullptr, 10);
    this->spawnX            = std::strtol(statList[2].c_str(), nullptr, 10);
    this->spawnY            = std::strtol(statList[3].c_str(), nullptr, 10);
    this->health            = std::strtol(statList[4].c_str(), nullptr, 10);
    this->hunger            = std::strtol(statList[5].c_str(), nullptr, 10);
    this->armor             = std::strtol(statList[6].c_str(), nullptr, 10);
    this->armorDamageBuffer = std::strtol(statList[7].c_str(), nullptr, 10);
    this->curArmor          = getItemMaterial(statList[8]);
    this->score             = std::strtol(statList[9].c_str(), nullptr, 10);
    this->level             = std::strtol(statList[10].c_str(), nullptr, 10);

    std::vector<std::string> potionDataList{};
    utils::splitString(statList[11].substr(14, statList[11].size() - 1), ":", potionDataList);
    this->potionList.clear();
    this->potionList.reserve(potionDataList.size());
    for (const auto& potionData : potionDataList) {
        std::vector<std::string> rawPotion{};
        rawPotion.reserve(2);
        utils::splitString(potionData, ";", rawPotion);

        this->potionList.emplace_back(getPotionType(rawPotion[0]), (int32_t) std::strtol(rawPotion[1].c_str(), nullptr, 10));
    }

    this->shirtColor        = Color{(int32_t) std::strtol(statList[12].c_str(), nullptr, 10)};
    this->skinon            = statList[13] == "true";

    std::vector<std::string> itemList{};
    utils::splitString(globalList[2], ",", itemList);

    this->inventory.clear();
    this->inventory.reserve(itemList.size());

    for (auto it = itemList.begin(); it != itemList.end(); it += 2) {
        this->inventory.emplace_back(static_cast<ItemId>(std::strtol(it->c_str(), nullptr, 10), (uint8_t) std::strtol((it + 1)->c_str(), nullptr, 10)));
    }

    return *this;
}

MovePacket::MovePacket(const FixedLocation& location, Direction direction, WorldId world) {
    this->location  = location;
    this->direction = direction;
    this->world     = world;
}

MovePacket::MovePacket(const RawPacket& raw) {
    this->location  = {};
    this->direction = Direction::NONE;
    this->world     = 0;

    *this = raw;
}

MovePacket::operator RawPacket() const {
    std::stringstream ss{};

    ss << (location.x << 4 | location.xDecimal) << ';' << (location.y << 4 | location.yDecimal) << ';'
       << static_cast<uint8_t>(direction) << ';' << world;

    return RawPacket{static_cast<PacketId>(PacketType::MOVE), ss.str()};
}

MovePacket& MovePacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::MOVE), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ";", dataList);

    int32_t _x = std::strtol(dataList[0].c_str(), nullptr, 10);
    int32_t _y = std::strtol(dataList[1].c_str(), nullptr, 10);

    this->location.x        = _x >> 4;
    this->location.xDecimal = _x & 0xF;

    this->location.y        = _y >> 4;
    this->location.yDecimal = _y & 0xF;

    this->direction = static_cast<Direction>(std::strtol(dataList[2].c_str(), nullptr, 10));
    this->world     = std::strtol(dataList[3].c_str(), nullptr, 10);


    return *this;
}

AddPacket::AddPacket(const Entity& entity) {
    this->entity = entity.get();
}

AddPacket::AddPacket(const RawPacket& raw) {
    this->entity = {};

    *this = raw;
}

AddPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::ADD), entity->raw()};
}

AddPacket& AddPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(raw.id), raw);

    this->entity = createEntity(raw.data);

    return *this;
}

RemovePacket::RemovePacket(EntityId entity, std::optional<WorldId> world) {
    this->entity = entity;
    this->world = world;
}

RemovePacket::RemovePacket(const RawPacket& raw) {
    this->entity = 0;
    this->world = 0;

    *this = raw;
}

RemovePacket::operator RawPacket() const {
    std::stringstream ss{};

    ss << entity;
    if (world.has_value()) {
        ss << ';' << world.value();
    }

    return RawPacket{static_cast<PacketId>(PacketType::REMOVE), ss.str()};
}

RemovePacket& RemovePacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::REMOVE), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ";", dataList);

    std::size_t size = dataList.size();
    if (size == 1) {
        this->entity = strtol(dataList[0].c_str(), nullptr, 10);
        this->world = std::optional<WorldId>();
    } else if (size == 2) {
        this->entity = strtol(dataList[0].c_str(), nullptr, 10);
        this->world = strtol(dataList[1].c_str(), nullptr, 10);
    } else {
        throw std::runtime_error("RemovePacket: invalid data: " + raw.data);
    }

    return *this;
}

DisconnectPacket::DisconnectPacket() = default;

DisconnectPacket::DisconnectPacket(const RawPacket& raw) {
    *this = raw;
}

DisconnectPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::DISCONNECT), ""};
}

DisconnectPacket& DisconnectPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::DISCONNECT), raw);
    return *this;
}

SavePacket::SavePacket() = default;

SavePacket::SavePacket(const RawPacket& raw) {
    *this = raw;
}

SavePacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::SAVE), ""};
}

SavePacket& SavePacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::SAVE), raw);
    return *this;
}

NotifyPacket::NotifyPacket(int32_t notetime, const std::string& note) {
    this->notetime = notetime;
    this->note = note;
}

NotifyPacket::NotifyPacket(const RawPacket& raw) {
    this->notetime = 0;
    this->note = {};

    *this = raw;
}

NotifyPacket::operator RawPacket() const {
    std::stringstream ss{};

    ss << notetime << ';' << note;

    return RawPacket{static_cast<PacketId>(PacketType::NOTIFY), ss.str()};
}

NotifyPacket& NotifyPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::NOTIFY), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ";", dataList);

    this->notetime = strtol(dataList[0].c_str(), nullptr, 10);
    this->note     = dataList[1];

    return *this;
}

InteractPacket::InteractPacket(const Item& item, int32_t stamina, int32_t arrowCount) {
    this->item       = item;
    this->stamina    = stamina;
    this->arrowCount = arrowCount;

    this->isOutgoing = false;
}

InteractPacket::InteractPacket(const Item& item) {
    this->item       = item;
    this->stamina    = 0;
    this->arrowCount = 0;

    this->isOutgoing = true;
}

InteractPacket::InteractPacket(const RawPacket& raw) {
    this->item       = {};
    this->stamina    = 0;
    this->arrowCount = 0;

    this->isOutgoing = false;

    *this = raw;
}

InteractPacket::operator RawPacket() const {
    std::stringstream ss{};

    ss << item.raw();

    if (!isOutgoing) {
        ss << ';' << stamina << ';' << arrowCount;
    }

    return RawPacket{static_cast<PacketId>(PacketType::INTERACT), ss.str()};
}

InteractPacket& InteractPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::INTERACT), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ";", dataList);

    this->item       = Item{dataList[0]};

    std::size_t size = dataList.size();
    if (size == 1) {
        this->isOutgoing = true;
    } else if (size == 3) {
        this->stamina    = std::strtol(dataList[1].c_str(), nullptr, 10);
        this->arrowCount = std::strtol(dataList[2].c_str(), nullptr, 10);
        this->isOutgoing = false;
    } else {
        throw std::runtime_error("InteractPacket: invalid data: " + raw.data);
    }

    return *this;
}

PushPacket::PushPacket(EntityId entity) {
    this->entity = entity;
}

PushPacket::PushPacket(const RawPacket& raw) {
    this->entity = 0;

    *this = raw;
}

PushPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::PUSH), std::to_string(entity)};
}

PushPacket& PushPacket::operator=(const RawPacket &raw) {
    check_raw(static_cast<PacketId>(PacketType::PUSH), raw);

    this->entity = std::strtol(raw.data.c_str(), nullptr, 10);

    return *this;
}

PickupPacket::PickupPacket(EntityId entity) {
    this->entity = entity;
}

PickupPacket::PickupPacket(const RawPacket& raw) {
    this->entity = 0;

    *this = raw;
}

PickupPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::PICKUP), std::to_string(entity)};
}

PickupPacket& PickupPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::PICKUP), raw);

    this->entity = std::strtol(raw.data.c_str(), nullptr, 10);

    return *this;
}

ChestInPacket::ChestInPacket(EntityId chestId, int32_t index, const Item& item) {
    this->chestId = chestId;
    this->index  = index;
    this->item   = item;
}

ChestInPacket::ChestInPacket(const RawPacket& raw) {
    this->chestId = 0;
    this->index  = 0;
    this->item   = {};

    *this = raw;
}

ChestInPacket::operator RawPacket() const {
    std::stringstream ss{};

    ss << chestId << ';' << index << ';' << item.raw();

    return RawPacket{static_cast<PacketId>(PacketType::CHEST_IN), ss.str()};
}

ChestInPacket& ChestInPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::CHEST_IN), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ";", dataList);

    this->chestId = std::strtol(dataList[0].c_str(), nullptr, 10);
    this->index   = std::strtol(dataList[1].c_str(), nullptr, 10);
    this->item    = Item{dataList[2]};

    return *this;
}

ChestOutPacket::ChestOutPacket(const ChestOutPacket::InPacket& in) {
    this->data = in;
}

ChestOutPacket::ChestOutPacket(const ChestOutPacket::OutPacket& out) {
    this->data = out;
}

ChestOutPacket::ChestOutPacket(const RawPacket& raw) {
    *this = raw;
}

ChestOutPacket::operator RawPacket() const {
    std::stringstream ss{};

    /*if (data.type == Type::IN) {
        const ChestOutPacket::InPacket& inputData = *this->data.in;
        
        ss << inputData.chestId << ';' << inputData.itemIndex << ';' << (inputData.wholeStack ? "true" : "false") << inputData.inputIndex;
    } else if (data.type == Type::OUT) {
        const ChestOutPacket::OutPacket& outputData = *this->data.out;

        ss << outputData.item.raw() << ';' << outputData.index;
    }*/

    return RawPacket{static_cast<PacketId>(PacketType::CHEST_OUT), ss.str()};
}

ChestOutPacket& ChestOutPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::CHEST_OUT), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ";", dataList);

    /*std::size_t size = dataList.size();
    if (size == 1 || size == 4) {
        _data->type = Data::Type::IN;
        ChestOutPacket::InPacket& inputData = this->_data->in;

        inputData.chestId        = std::strtol(dataList[0].c_str(), nullptr, 10);
        if (size == 4) {
            inputData.itemIndex  = std::strtol(dataList[1].c_str(), nullptr, 10);
            inputData.wholeStack = dataList[2] == "true";
            inputData.inputIndex = std::strtol(dataList[3].c_str(), nullptr, 10);
        }
    } else if (size == 2) {
        _data->type = Data::Type::OUT;
        ChestOutPacket::OutPacket& outputData = this->_data->out;

        outputData.item  = Item{dataList[0]};
        outputData.index = std::strtol(dataList[1].c_str(), nullptr, 10);
    }*/

    return *this;
}

AddItemsPacket::AddItemsPacket(const std::vector<Item>& itemList) {
    this->itemList = std::vector<Item>(itemList);
}

AddItemsPacket::AddItemsPacket(const RawPacket& raw) {
    this->itemList = std::vector<Item>();

    *this = raw;
}

AddItemsPacket::operator RawPacket() const {
    std::stringstream ss{};

    if (!itemList.empty()) {
        ss << itemList[0].raw();
        std::for_each(itemList.begin(), itemList.end(), [&ss](const auto& item){ ss << ';' << item.raw(); });
    }

    return RawPacket{static_cast<PacketId>(PacketType::ADD_ITEMS), ss.str()};
}

AddItemsPacket& AddItemsPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::ADD_ITEMS), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ";", dataList);

    this->itemList.clear();
    this->itemList.reserve(dataList.size());

    for (const auto& rawItem : dataList) {
        this->itemList.emplace_back(rawItem);
    }

    return *this;
}

BedPacket::BedPacket(bool enabled, EntityId bedId) {
    this->enabled = enabled;
    this->bedId   = bedId;
}

BedPacket::BedPacket(const RawPacket& raw) {
    this->enabled = false;
    this->bedId   = 0;
    
    *this = raw;
}

BedPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::BED), (enabled ? "true;" : "false;") + std::to_string(bedId)};
}

BedPacket& BedPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::BED), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ";", dataList);

    this->enabled = dataList[0] == "true";
    this->bedId   = dataList.size() > 1 ? std::strtol(dataList[1].c_str(), nullptr, 10) : 0;

    return *this;
}

HurtPacket::HurtPacket(EntityId entity, Damage_t damage, Direction direction) {
    this->entity = entity;
    this->damage = damage;
    this->direction = direction;
}

HurtPacket::HurtPacket(const RawPacket& raw) {
    this->entity    = 0;
    this->damage    = 0;
    this->direction = Direction::NONE;

    *this = raw;
}

HurtPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::HURT), std::to_string(entity) + ";" + std::to_string(damage) + ";" + std::to_string(static_cast<int32_t>(direction))};
}

HurtPacket& HurtPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::HURT), raw);

    std::vector<std::string> dataList{};
    utils::splitString(raw.data, ";", dataList);

    this->entity    = std::strtol(dataList[0].c_str(), nullptr, 10);
    this->damage    = std::strtol(dataList[1].c_str(), nullptr, 10);
    this->direction = static_cast<Direction>(std::strtol(dataList[2].c_str(), nullptr, 10));

    return *this;
}

DiePacket::DiePacket() = default;

DiePacket::DiePacket(const RawPacket& raw) {
    *this = raw;
}

DiePacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::DIE), ""};
}

DiePacket& DiePacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::DIE), raw);

    return *this;
}

RespawnPacket::RespawnPacket() = default;

RespawnPacket::RespawnPacket(const RawPacket& raw) {
    *this = raw;
}

RespawnPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::RESPAWN), ""};
}

RespawnPacket& RespawnPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::RESPAWN), raw);

    return *this;
}

DropPacket::DropPacket(const Item& item) {
    this->item = item;
}

DropPacket::DropPacket(const RawPacket& raw) {
    this->item = {};

    *this = raw;
}

DropPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::DROP), item.raw()};
}

DropPacket& DropPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::DROP), raw);

    this->item = Item{raw.data};

    return *this;
}

StaminaPacket::StaminaPacket(int32_t stamina) {
    this->stamina = stamina;
}

StaminaPacket::StaminaPacket(const RawPacket& raw) {
    this->stamina = 0;

    *this = raw;
}

StaminaPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::STAMINA), std::to_string(stamina)};
}

StaminaPacket& StaminaPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::STAMINA), raw);

    this->stamina = std::strtol(raw.data.c_str(), nullptr, 10);

    return *this;
}

ShirtPacket::ShirtPacket(const Color& color) {
    this->color = color;
}

ShirtPacket::ShirtPacket(const RawPacket& raw) {
    this->color = Color{};

    *this = raw;
}

ShirtPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::SHIRT), std::to_string(color.raw())};
}

ShirtPacket& ShirtPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::SHIRT), raw);

    this->color = Color{(int32_t) std::strtol(raw.data.c_str(), nullptr, 10)};

    return *this;
}

StopFishingPacket::StopFishingPacket(EntityId entity) {
    this->entity = entity;
}

StopFishingPacket::StopFishingPacket(const RawPacket& raw) {
    this->entity = 0;

    *this = raw;
}

StopFishingPacket::operator RawPacket() const {
    return RawPacket{static_cast<PacketId>(PacketType::STOPFISHING), std::to_string(entity)};
}

StopFishingPacket& StopFishingPacket::operator=(const RawPacket& raw) {
    check_raw(static_cast<PacketId>(PacketType::STOPFISHING), raw);

    this->entity = std::strtol(raw.data.c_str(), nullptr, 10);

    return *this;
}
