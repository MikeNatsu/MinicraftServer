#ifndef MINICRAFTSERVER_WORLD_H
#define MINICRAFTSERVER_WORLD_H

#include <cstdint>
#include <array>
#include <unordered_map>
#include <vector>

#include "MinicraftDef.h"
#include "Dimension.h"
#include "Entity.h"

namespace mcplus {

    enum class TileMaterial : TileId {
        GRASS,
        DIRT,
        FLOWER,
        HOLE,
        STAIRS_UP,
        STAIRS_DOWN,
        WATER,
        LAVA,
        ROCK,
        TREE,
        TREE_SAPLING,
        SAND,
        CACTUS,
        CACTUS_SAPLING,
        IRON_ORE,
        GOLD_ORE,
        GEM_ORE,
        LAPIS_ORE,
        LAVA_BRICK,
        EXPLODED,
        FARMLAND,
        WHEAT,
        HARD_ROCK,
        INFINITE_FALL,
        CLOUD,
        CLOUD_CACTUS,
        WOOD_DOOR,
        STONE_DOOR,
        OBSIDIAN_DOOR,
        WOOD_FLOOR,
        STONE_FLOOR,
        OBSIDIAN_FLOOR,
        WOOD_WALL,
        STONE_WALL,
        OBSIDIAN_WALL,
        WOOL,
        RED_WOOL,
        BLUE_WOOL,
        GREEN_WOOL,
        YELLOW_WOOL,
        BLACK_WOOL,
        PATH,
        POTATO,
        TORCH
    };

    struct Tile {
        TileId id;
        uint8_t data;

        Tile(TileId id, uint8_t data);
        Tile();
    };

    class Chunk {
        static constexpr std::size_t CHUNK_WIDTH  = 16;
        static constexpr std::size_t CHUNK_HEIGHT = 16;
        static constexpr std::size_t CHUNK_SIZE   = CHUNK_WIDTH * CHUNK_HEIGHT; 

        std::array<Tile, Chunk::CHUNK_SIZE> tiles;
    public:
        Chunk();

        Tile& getTileAt(const Vector2i& pos);
        [[nodiscard]] const Tile& getTileAt(const Vector2i& pos) const;
    };

    class WorldGenerator {

    };

    class World {
        std::string name;
        std::unordered_map<Vector2i, Chunk> loadedChunks;

        std::unordered_map<EntityId, std::shared_ptr<Entity>> entityMap;
    public:
        explicit World(const std::string& name);

        Chunk& getChunkAt(const Vector2i& pos);
        [[nodiscard]] const Chunk& getChunkAt(const Vector2i& pos) const;
    };



    std::string getTileName(TileMaterial tileMaterial);
    TileMaterial getTileMaterial(const std::string& tileName);

}

#endif //MINICRAFTSERVER_WORLD_H
