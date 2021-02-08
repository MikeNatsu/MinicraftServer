#include "Inventory.h"

#include <unordered_map>
#include <sstream>
#include "Utils.h"

using namespace mcplus;

static bool checkBelongTo(ItemMaterial start, ItemMaterial end, ItemId id);
static std::string getToolLevelName(ItemToolData::Level level);
static std::string getFishingRodLevelName(ItemFishingRodData::Level level);

bool ItemData::operator==(const ItemData& anotherData) const {
    return true;
}

ItemStackableData::ItemStackableData(uint16_t amount) {
    this->amount = amount;
}

ItemStackableData::ItemStackableData() {
    this->amount = 0;
}

bool ItemStackableData::operator==(const ItemData& anotherData) const {
    const auto* anotherStackableData = dynamic_cast<const ItemStackableData*>(&anotherData);
    return anotherStackableData != nullptr && amount == anotherStackableData->amount;
}

ItemToolData::ItemToolData(ItemToolData::Level level, int32_t durability) {
    this->level = level;
    this->durability = durability;
}

ItemToolData::ItemToolData() {
    this->level = ItemToolData::Level::WOOD;
    this->durability = 0;
}

bool ItemToolData::operator==(const ItemData& anotherData) const {
    const auto* anotherToolData = dynamic_cast<const ItemToolData*>(&anotherData);
    return anotherToolData != nullptr && level == anotherToolData->level && durability == anotherToolData->durability;
}

ItemSpawnerData::ItemSpawnerData(int32_t health, int32_t level, int32_t maxMobLevel) {
    this->health = health;
    this->level = level;
    this->maxMobLevel = maxMobLevel;
}

ItemSpawnerData::ItemSpawnerData() {
    this->health = 0;
    this->level = 0;
    this->maxMobLevel = 0;
}

bool ItemSpawnerData::operator==(const ItemData& anotherData) const {
    const auto* anotherSpawnerData = dynamic_cast<const ItemSpawnerData*>(&anotherData);
    return anotherSpawnerData != nullptr && health == anotherSpawnerData->health && level == anotherSpawnerData->level && maxMobLevel == anotherSpawnerData->maxMobLevel;
}

ItemPotionData::ItemPotionData(const Potion &potion) {
    this->potion = potion;
}

ItemPotionData::ItemPotionData() {
    this->potion = {};
}

bool ItemPotionData::operator==(const ItemData &anotherData) const {
    const auto* anotherPotionData = dynamic_cast<const ItemPotionData*>(&anotherData);
    return anotherPotionData != nullptr && potion == anotherPotionData->potion;
}

ItemFishingRodData::ItemFishingRodData(ItemFishingRodData::Level level) {
    this->level = level;
}

ItemFishingRodData::ItemFishingRodData() {
    this->level = ItemFishingRodData::Level::WOOD;
}

bool ItemFishingRodData::operator==(const ItemData& anotherData) const {
    const auto* anotherFishingRodData = dynamic_cast<const ItemFishingRodData*>(&anotherData);
    return anotherFishingRodData != nullptr && level == anotherFishingRodData->level;
}

Item::Item(ItemId id) {
    this->_id = id;

    if (checkBelongTo(ItemMaterial::STACKABLE_START, ItemMaterial::STACKABLE_END, id)) {
        if (static_cast<ItemId>(ItemMaterial::POTION) == id) {
            this->_data = std::make_unique<ItemPotionData>();
        } else {
            this->_data = std::make_unique<ItemStackableData>();
        }
    } else if (checkBelongTo(ItemMaterial::TOOL_START, ItemMaterial::TOOL_END, id)) {
        this->_data = std::make_unique<ItemToolData>();
    } else {
        this->_data = std::make_unique<ItemData>();
    }
}

Item::Item(ItemMaterial itemMaterial) : Item(static_cast<ItemId>(itemMaterial)) {

}

Item::Item(const std::string& raw) : Item(0) {

}

Item::Item(const Item &item) {
    this->_id = item._id;
    this->_data = std::make_unique<ItemData>(*item._data);
}

Item::Item(Item &&item) noexcept : _id(item._id), _data(std::move(item._data)) {

}

Item::Item() {
    this->_id = ItemMaterial::NULL_MATERIAL;
    this->_data = std::make_unique<ItemData>();
}

ItemId Item::id() const {
    return _id;
}

ItemData& Item::data() {
    return *_data;
}

const ItemData& Item::data() const {
    return *_data;
}

std::string Item::raw() const {
    std::stringstream ss{};

    if (checkBelongTo(ItemMaterial::TOOL_START, ItemMaterial::TOOL_END, _id)) {
        auto& toolData = dynamic_cast<ItemToolData&>(*_data);
        ss << getToolLevelName(toolData.level) << ' ' << getItemName(_id);
    } else if (checkBelongTo(ItemMaterial::STACKABLE_START, ItemMaterial::STACKABLE_END, _id)) {
        auto& stackableData = dynamic_cast<ItemStackableData&>(*_data);

        if (_id == static_cast<ItemId>(ItemMaterial::POTION)) {
            auto& potionData = dynamic_cast<ItemPotionData&>(stackableData);
            ss << getPotionName(potionData.potion.type);
        } else {
            ss << getItemName(_id);
        }

        ss << '_' << stackableData.amount;
    } else {
        ss << getItemName(_id);
    }

    return ss.str();
}

Item &Item::operator=(const Item &item) {
    this->_id = item._id;
    this->_data = std::make_unique<ItemData>(*item._data);

    return *this;
}

const std::string& mcplus::getItemName(ItemId id) {
    static std::unordered_map<TileId, std::string> _data{
        {ItemMaterial::UNKNOWN,                      "Unknown Blank"},
        {ItemMaterial::NULL_MATERIAL,                "NULL"},
        {ItemMaterial::POWER_GLOVE,                  "Power Glove"},
        {ItemMaterial::FURNITURE_COW_SPAWNER,        "Cow Spawner"},
        {ItemMaterial::FURNITURE_PIG_SPAWNER,        "Pig Spawner"},
        {ItemMaterial::FURNITURE_SHEEP_SPAWNER,      "Sheep Spawner"},
        {ItemMaterial::FURNITURE_SLIME_SPAWNER,      "Slime Spawner"},
        {ItemMaterial::FURNITURE_ZOMBIE_SPAWNER,     "Zombie Spawner"},
        {ItemMaterial::FURNITURE_CREEPER_SPAWNER,    "Creeper Spawner"},
        {ItemMaterial::FURNITURE_SKELETON_SPAWNER,   "Skeleton Spawner"},
        {ItemMaterial::FURNITURE_SNAKE_SPAWNER,      "Snake Spawner"},
        {ItemMaterial::FURNITURE_KNIGHT_SPAWNER,     "Knight Spawner"},
        {ItemMaterial::FURNITURE_AIR_WIZARD_SPAWNER, "AirWizard Spawner"},
        {ItemMaterial::FURNITURE_CHEST,              "Chest"},
        {ItemMaterial::FURNITURE_WORKBENCH,          "Workbench"},
        {ItemMaterial::FURNITURE_OVEN,               "Oven"},
        {ItemMaterial::FURNITURE_FURNACE,            "Furnace"},
        {ItemMaterial::FURNITURE_ANVIL,              "Anvil"},
        {ItemMaterial::FURNITURE_ENCHANTER,          "Enchanter"},
        {ItemMaterial::FURNITURE_LOOM,               "Loom"},
        {ItemMaterial::FURNITURE_LANTERN,            "Lantern"},
        {ItemMaterial::FURNITURE_IRON_LANTERN,       "Iron Lantern"},
        {ItemMaterial::FURNITURE_GOLD_LANTERN,       "Gold Lantern"},
        {ItemMaterial::FURNITURE_TNT,                "Tnt"},
        {ItemMaterial::FURNITURE_BED,                "Bed"},
        {ItemMaterial::TORCH_ITEM,                   "Torch"},
        {ItemMaterial::BUCKET_EMPTY,                 "Bucket Empty"},
        {ItemMaterial::BUCKET_WATER,                 "Bucket Water"},
        {ItemMaterial::BUCKET_LAVA,                  "Bucket Lava"},
        {ItemMaterial::BOOK,                         "Book"},
        {ItemMaterial::BOOK_ANTIDIOUS,               "Book Antidious"}
    };

    return _data[id];
}

ItemMaterial mcplus::getItemMaterial(const std::string& itemName) {
    static std::unordered_map<std::string, ItemMaterial> _data{
            {"Unknown Blank", ItemMaterial::UNKNOWN},
            {"NULL", ItemMaterial::NULL_MATERIAL},
            {"Power Glove", ItemMaterial::POWER_GLOVE},
            {"Cow Spawner", ItemMaterial::FURNITURE_COW_SPAWNER},
            {"Pig Spawner", ItemMaterial::FURNITURE_PIG_SPAWNER},
            {"Sheep Spawner", ItemMaterial::FURNITURE_SHEEP_SPAWNER},
            {"Slime Spawner", ItemMaterial::FURNITURE_SLIME_SPAWNER},
            {"Zombie Spawner", ItemMaterial::FURNITURE_ZOMBIE_SPAWNER},
            {"Creeper Spawner", ItemMaterial::FURNITURE_CREEPER_SPAWNER},
            {"Skeleton Spawner", ItemMaterial::FURNITURE_SKELETON_SPAWNER},
            {"Snake Spawner", ItemMaterial::FURNITURE_SNAKE_SPAWNER},
            {"Knight Spawner", ItemMaterial::FURNITURE_KNIGHT_SPAWNER},
            {"AirWizard Spawner", ItemMaterial::FURNITURE_AIR_WIZARD_SPAWNER},
            {"Chest", ItemMaterial::FURNITURE_CHEST, },
            {"Workbench", ItemMaterial::FURNITURE_WORKBENCH},
            {"Oven", ItemMaterial::FURNITURE_OVEN},
            {"Furnace", ItemMaterial::FURNITURE_FURNACE},
            {"Anvil", ItemMaterial::FURNITURE_ANVIL},
            {"Enchanter", ItemMaterial::FURNITURE_ENCHANTER},
            {"Loom", ItemMaterial::FURNITURE_LOOM},
            {"Lantern", ItemMaterial::FURNITURE_LANTERN},
            {"Iron Lantern", ItemMaterial::FURNITURE_IRON_LANTERN},
            {"Gold Lantern", ItemMaterial::FURNITURE_GOLD_LANTERN},
            {"Tnt", ItemMaterial::FURNITURE_TNT},
            {"Bed", ItemMaterial::FURNITURE_BED},
            {"Torch", ItemMaterial::TORCH_ITEM},
            {"Bucket Empty", ItemMaterial::BUCKET_EMPTY},
            {"Bucket Water", ItemMaterial::BUCKET_WATER},
            {"Bucket Lava", ItemMaterial::BUCKET_LAVA},
            {"Book", ItemMaterial::BOOK},
            {"Book Antidious", ItemMaterial::BOOK_ANTIDIOUS}
    };

    return _data[itemName];
}

static bool checkBelongTo(ItemMaterial start, ItemMaterial end, ItemId id) {
    return static_cast<ItemId>(start) > id && id < static_cast<ItemId>(end);
}

static std::string getToolLevelName(ItemToolData::Level level) {
    static std::unordered_map<ItemToolData::Level, std::string> _data{
            {ItemToolData::Level::WOOD, "Wood"},
            {ItemToolData::Level::ROCK, "Rock"},
            {ItemToolData::Level::IRON, "Iron"},
            {ItemToolData::Level::GOLD, "Gold"},
            {ItemToolData::Level::GEM,  "Gem"}
    };

    return _data[level];
}

static std::string getFishingRodLevelName(ItemFishingRodData::Level level) {
    static std::unordered_map<ItemFishingRodData::Level, std::string> _data{
            {ItemFishingRodData::Level::WOOD, "Wood"},
            {ItemFishingRodData::Level::IRON, "Iron"},
            {ItemFishingRodData::Level::GOLD, "Gold"},
            {ItemFishingRodData::Level::GEM,  "Gem"}
    };

    return _data[level];
}

Inventory::Inventory(const std::string &raw) {
    std::vector<std::string> dataList{};
    utils::splitString(raw, ":", dataList);

    this->itemList = {};
    this->itemList.reserve(dataList.size());

    for (const auto& data : dataList) {
        this->itemList.emplace_back(std::make_shared<Item>(data));
    }
}

Inventory::Inventory() = default;

std::string Inventory::raw() const {
    std::stringstream ss{};

    if (!itemList.empty()) {
        ss << itemList[0]->raw();
        std::for_each(itemList.begin() + 1, itemList.end(), [&ss](const auto& item){ ss << ':' << item->raw(); });
    }

    return ss.str();
}

std::size_t Inventory::size() const {
    return itemList.size();
}

void Inventory::add(const Item &item, std::size_t copies) {
    if (checkBelongTo(ItemMaterial::STACKABLE_START, ItemMaterial::STACKABLE_END, item.id())) {
        int32_t newAmount = dynamic_cast<const ItemStackableData&>(item.data()).amount * (copies + 1);

        for (auto& inventoryItem : itemList) {
            if (inventoryItem->id() == item.id()) {
                dynamic_cast<ItemStackableData&>(inventoryItem->data()).amount += newAmount;
                return;
            }
        }

        auto inventoryItem = std::make_shared<Item>(item);
        dynamic_cast<ItemStackableData&>(inventoryItem->data()).amount += newAmount;
        itemList.emplace_back(inventoryItem);
    } else {
        copies += 1;
        for (std::size_t i = 0; i < copies; i++) {
            this->itemList.emplace_back(std::make_shared<Item>(item));
        }
    }
}

void Inventory::remove(std::size_t startIndex, std::size_t endIndex) {
    itemList.erase(itemList.begin() + startIndex, itemList.begin() + endIndex);
}

void Inventory::remove(std::size_t startIndex) {
    itemList.erase(itemList.begin() + startIndex);
}

void Inventory::clear() {
    itemList.clear();
}

std::shared_ptr<const Item> Inventory::operator[](std::size_t index) const {
    return itemList[index];
}

std::shared_ptr<Item> Inventory::operator[](std::size_t index) {
    return itemList[index];
}
