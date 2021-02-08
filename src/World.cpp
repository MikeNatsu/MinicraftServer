#include "World.h"
#include <unordered_map>

using namespace mcplus;

Tile::Tile(TileId id, uint8_t data) {
    this->id   = id;
    this->data = data;
}

Tile::Tile() {
    this->id   = 0;
    this->data = 0;
}

Chunk::Chunk() {
    tiles.fill({0, 0});
}

Tile& Chunk::getTileAt(const Vector2i& pos) {
    return tiles[pos.x + (pos.y * 16)];
}

const Tile& Chunk::getTileAt(const Vector2i& pos) const {
    return tiles[pos.x + (pos.y * 16)];
}

World::World(const std::string& name) {
    this->name = name;
    loadedChunks = {};
}

Chunk& World::getChunkAt(const Vector2i& pos) {
    return loadedChunks[pos];
}

const Chunk& World::getChunkAt(const Vector2i& pos) const {
    return loadedChunks.at(pos);
}

std::string mcplus::getTileName(TileMaterial tileMaterial) {
    static std::unordered_map<TileMaterial, std::string> _data{
            {TileMaterial::GRASS,          "Grass"},
            {TileMaterial::DIRT,           "Dirt"},
            {TileMaterial::FLOWER,         "Flower"},
            {TileMaterial::HOLE,           "Hole"},
            {TileMaterial::STAIRS_UP,      "Stairs Up"},
            {TileMaterial::STAIRS_DOWN,    "Stairs Down"},
            {TileMaterial::WATER,          "Water"},
            {TileMaterial::LAVA,           "Lava"},
            {TileMaterial::ROCK,           "Rock"},
            {TileMaterial::TREE,           "Tree"},
            {TileMaterial::TREE_SAPLING,   "Tree Sapling"},
            {TileMaterial::SAND,           "Sand"},
            {TileMaterial::CACTUS,         "Cactus"},
            {TileMaterial::CACTUS_SAPLING, "Cactus Sapling"},
            {TileMaterial::IRON_ORE,       "Iron Ore"},
            {TileMaterial::GOLD_ORE,       "Gold Ore"},
            {TileMaterial::GEM_ORE,        "Gem Ore"},
            {TileMaterial::LAPIS_ORE,      "Lapis Ore"},
            {TileMaterial::LAVA_BRICK,     "Lava Brick"},
            {TileMaterial::EXPLODED,       "Explode"},
            {TileMaterial::FARMLAND,       "Farmland"},
            {TileMaterial::WHEAT,          "Wheat"},
            {TileMaterial::HARD_ROCK,      "Hard Rock"},
            {TileMaterial::INFINITE_FALL,  "Infinite Fall"},
            {TileMaterial::CLOUD,          "Cloud"},
            {TileMaterial::CLOUD_CACTUS,   "Cloud Cactus"},
            {TileMaterial::WOOD_DOOR,      "Wood Door"},
            {TileMaterial::STONE_DOOR,     "Stone Door"},
            {TileMaterial::OBSIDIAN_DOOR,  "Obsidian Door"},
            {TileMaterial::WOOD_FLOOR,     "Wood Floor"},
            {TileMaterial::STONE_FLOOR,    "Stone Floor"},
            {TileMaterial::OBSIDIAN_FLOOR, "Obsidian Floor"},
            {TileMaterial::WOOD_WALL,      "Wood Wall"},
            {TileMaterial::STONE_WALL,     "Stone Wall"},
            {TileMaterial::OBSIDIAN_WALL,  "Obsidian Wall"},
            {TileMaterial::WOOL,           "Wool"},
            {TileMaterial::RED_WOOL,       "Red Wool"},
            {TileMaterial::BLUE_WOOL,      "Blue Wool"},
            {TileMaterial::GREEN_WOOL,     "Green Wool"},
            {TileMaterial::YELLOW_WOOL,    "Yellow Wool"},
            {TileMaterial::BLACK_WOOL,     "Black Wool"},
            {TileMaterial::PATH,           "Path"},
            {TileMaterial::POTATO,         "Potato"},
            {TileMaterial::TORCH,          "Torch"}
    };

    return _data[tileMaterial];
}

TileMaterial mcplus::getTileMaterial(const std::string& tileName) {
    static std::unordered_map<std::string, TileMaterial> _data{
            {"Grass",          TileMaterial::GRASS},
            {"Dirt",           TileMaterial::DIRT},
            {"Flower",         TileMaterial::FLOWER},
            {"Hole",           TileMaterial::HOLE},
            {"Stairs Up",      TileMaterial::STAIRS_UP},
            {"Stairs Down",    TileMaterial::STAIRS_DOWN},
            {"Water",          TileMaterial::WATER},
            {"Lava",           TileMaterial::LAVA},
            {"Rock",           TileMaterial::ROCK},
            {"Tree",           TileMaterial::TREE},
            {"Tree Sapling",   TileMaterial::TREE_SAPLING},
            {"Sand",           TileMaterial::SAND},
            {"Cactus",         TileMaterial::CACTUS},
            {"Cactus Sapling", TileMaterial::CACTUS_SAPLING},
            {"Iron Ore",       TileMaterial::IRON_ORE},
            {"Iron Ore",       TileMaterial::GOLD_ORE},
            {"Gem Ore",        TileMaterial::GEM_ORE},
            {"Lapis Ore",      TileMaterial::LAPIS_ORE},
            {"Lava Brick",     TileMaterial::LAVA_BRICK},
            {"Explode",        TileMaterial::EXPLODED},
            {"Farmland",       TileMaterial::FARMLAND},
            {"Wheat",          TileMaterial::WHEAT},
            {"Hard Rock",      TileMaterial::HARD_ROCK},
            {"Infinite Fall",  TileMaterial::INFINITE_FALL},
            {"Cloud",          TileMaterial::CLOUD},
            {"Cloud Cactus",   TileMaterial::CLOUD_CACTUS},
            {"Wood Door",      TileMaterial::WOOD_DOOR},
            {"Stone Door",     TileMaterial::STONE_DOOR},
            {"Obsidian Door",  TileMaterial::OBSIDIAN_DOOR},
            {"Wood Floor",     TileMaterial::WOOD_FLOOR},
            {"Stone Floor",    TileMaterial::STONE_FLOOR},
            {"Obsidian Floor", TileMaterial::OBSIDIAN_FLOOR},
            {"Wood Wall",      TileMaterial::WOOD_WALL},
            {"Stone Wall",     TileMaterial::STONE_WALL},
            {"Obsidian Wall",  TileMaterial::OBSIDIAN_WALL},
            {"Wool",           TileMaterial::WOOL},
            {"Red Wool",       TileMaterial::RED_WOOL},
            {"Blue Wool",      TileMaterial::BLUE_WOOL},
            {"Green Wool",     TileMaterial::GREEN_WOOL},
            {"Yellow Wool",    TileMaterial::YELLOW_WOOL},
            {"Black Wool",     TileMaterial::BLACK_WOOL},
            {"Path",           TileMaterial::PATH},
            {"Potato",         TileMaterial::POTATO},
            {"Torch",          TileMaterial::TORCH}
    };

    return _data[tileName];
}
