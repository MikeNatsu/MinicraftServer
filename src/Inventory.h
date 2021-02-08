#ifndef MINICRAFTSERVER_INVENTORY_H
#define MINICRAFTSERVER_INVENTORY_H

#include "MinicraftDef.h"
#include "Potion.h"

#include <memory>
#include <string>
#include <cstdint>
#include <vector>
#include <limits>

namespace mcplus {

    struct ItemData;
    struct ItemStackableData;
    struct ItemToolData;
    struct ItemSpawnerData;
    struct ItemPotionData;
    struct ItemFishingRodData;
    struct Item;
    struct InventoryHolder;
    class Inventory;

    enum ItemMaterial : ItemId {
        UNKNOWN                       = 0xFFFF,
        NULL_MATERIAL                 = 0xEFE0,
        POWER_GLOVE                   = 0x0000,
        FISHING_ROD,
        SHEAR,

        FURNITURE_START               = 0x1000,
        FURNITURE_COW_SPAWNER,
        FURNITURE_PIG_SPAWNER,
        FURNITURE_SHEEP_SPAWNER,
        FURNITURE_SLIME_SPAWNER,
        FURNITURE_ZOMBIE_SPAWNER,
        FURNITURE_CREEPER_SPAWNER,
        FURNITURE_SKELETON_SPAWNER,
        FURNITURE_SNAKE_SPAWNER,
        FURNITURE_KNIGHT_SPAWNER,
        FURNITURE_AIR_WIZARD_SPAWNER,
        FURNITURE_CHEST,
        FURNITURE_WORKBENCH,
        FURNITURE_OVEN,
        FURNITURE_FURNACE,
        FURNITURE_ANVIL,
        FURNITURE_ENCHANTER,
        FURNITURE_LOOM,
        FURNITURE_LANTERN,
        FURNITURE_IRON_LANTERN,
        FURNITURE_GOLD_LANTERN,
        FURNITURE_TNT,
        FURNITURE_BED,
        FURNITURE_END,

        BOOK_START                    = 0x2070,
        BOOK,
        BOOK_ANTIDIOUS,
        BOOK_END,

        TOOL_START                   = 0x2200,
        TOOL_SHOVEL,
        TOOL_HOE,
        TOOL_SWORD,
        TOOL_PICKAXE,
        TOOL_AXE,
        TOOL_BOW,
        TOOL_CLAYMORE,
        TOOL_END,

        STACKABLE_START              = 0x2300,
        STACKABLE_WOOD,
        STACKABLE_STONE,
        STACKABLE_LEATHER,
        STACKABLE_WHEAT,
        STACKABLE_KEY,
        STACKABLE_ARROW,
        STACKABLE_STRING,
        STACKABLE_COAL,
        STACKABLE_IRON_ORE,
        STACKABLE_GOLD_ORE,
        STACKABLE_IRON,
        STACKABLE_GOLD,
        STACKABLE_LAPIS,
        STACKABLE_GEM,
        STACKABLE_ROSE,
        STACKABLE_GUN_POWDER,
        STACKABLE_SLIME,
        STACKABLE_GLASS,
        STACKABLE_CLOTH,
        STACKABLE_SCALE,
        STACKABLE_SHARD,

        ARMOR_START,
        ARMOR_LEATHER,
        ARMOR_SNAKE,
        ARMOR_IRON,
        ARMOR_GOLD,
        ARMOR_GEM,
        ARMOR_END,

        BUCKET_START,
        BUCKET_EMPTY,
        BUCKET_WATER,
        BUCKET_LAVA,
        BUCKET_END,

        CLOTHING_START,
        CLOTHING_RED,
        CLOTHING_BLUE,
        CLOTHING_GREEN,
        CLOTHING_YELLOW,
        CLOTHING_BLACK,
        CLOTHING_ORANGE,
        CLOTHING_PURPLE,
        CLOTHING_CYAN,
        CLOTHING_REG,
        CLOTHING_END,

        FOOD_START,
        FOOD_APPLE,
        FOOD_RAW_PORK,
        FOOD_RAW_FISH,
        FOOD_RAW_BEEF,
        FOOD_BREAD,
        FOOD_COOKED_FISH,
        FOOD_COOKED_PORK,
        FOOD_STEAK,
        FOOD_GOLD_APPLE,
        FOOD_END,

        POTION,

        TILE_START,
        TILE_FLOWER,
        TILE_ACORN,
        TILE_DIRT,
        TILE_PLANK,
        TILE_PLANK_WALL,
        TILE_WOOD_DOOR,
        TILE_STONE_BRICK,
        TILE_STONE_WALL,
        TILE_STONE_DOOR,
        TILE_OBSIDIAN_BRICK,
        TILE_OBSIDIAN_WALL,
        TILE_OBSIDIAN_DOOR,
        TILE_WOOL,
        TILE_RED_WOOL,
        TILE_BLUE_WOOL,
        TILE_GREEN_WOOL,
        TILE_YELLOW_WOOL,
        TILE_BLACK_WOOL,
        TILE_SAND,
        TILE_CACTUS,
        TILE_BONE,
        TILE_CLOUD,
        TILE_WHEAT_SEEDS,
        TILE_POTATO,
        TILE_GRASS_SEEDS,

        TORCH_ITEM,

        TILE_END,

        STACKABLE_END,
    };

    struct ItemData {
        virtual ~ItemData() = default;

        virtual bool operator==(const ItemData& anotherData) const;
    };

    struct ItemStackableData : ItemData {
        uint16_t amount;

        explicit ItemStackableData(uint16_t amount);
        ItemStackableData();

        bool operator==(const ItemData& anotherData) const override;
    };

    struct ItemToolData : ItemData {
        enum class Level {
            WOOD, ROCK, IRON, GOLD, GEM
        };

        Level level;
        int32_t durability;

        ItemToolData(Level level, int32_t);
        ItemToolData();

        bool operator==(const ItemData& anotherData) const override;
    };

    struct ItemSpawnerData : ItemData {
        int32_t health;
        int32_t level;
        int32_t maxMobLevel;

        ItemSpawnerData(int32_t health, int32_t level, int32_t maxMobLevel);
        ItemSpawnerData();

        bool operator==(const ItemData& anotherData) const override;
    };

    struct ItemPotionData : ItemStackableData {
        Potion potion;

        explicit ItemPotionData(const Potion& potion);
        ItemPotionData();

        bool operator==(const ItemData& anotherData) const override;
    };

    struct ItemFishingRodData : ItemData {
        enum class Level {
            WOOD,
            IRON,
            GOLD,
            GEM
        };

        Level level;

        explicit ItemFishingRodData(Level level);
        ItemFishingRodData();

        bool operator==(const ItemData& anotherData) const override;
    };

    struct Item {
        explicit Item(ItemId id);
        explicit Item(ItemMaterial itemMaterial);
        explicit Item(const std::string& raw);
        Item(const Item& item);
        Item(Item&& item) noexcept;
        Item();

        [[nodiscard]] ItemId id() const;

        ItemData& data();
        [[nodiscard]] const ItemData& data() const;

        [[nodiscard]] std::string raw() const;

        Item& operator=(const Item& item);
    private:
        ItemId _id;
        std::unique_ptr<ItemData> _data;
    };

    struct InventoryHolder {
        [[nodiscard]] virtual const Inventory& getInventory() const = 0;
        virtual Inventory& getInventory() = 0;
    };

    class Inventory {
        explicit Inventory(const std::string& raw);
        Inventory();

        [[nodiscard]] std::string raw() const;

        [[nodiscard]] std::size_t size() const;
        void add(const Item& item, std::size_t copies = 0);
        void remove(std::size_t startIndex, std::size_t endIndex = std::numeric_limits<std::size_t>::max());
        void remove(std::size_t index);
        void clear();

        std::shared_ptr<const Item> operator[](std::size_t index) const;
        std::shared_ptr<Item> operator[](std::size_t index);
    private:
        std::vector<std::shared_ptr<Item>> itemList;
    };

    const std::string& getItemName(ItemId id);
    ItemMaterial getItemMaterial(const std::string& itemName);
}

#endif // MINICRAFTSERVER_INVENTORY_H