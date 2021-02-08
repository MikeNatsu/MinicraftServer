#ifndef MCPLUS_PACKET_HEADER
#define MCPLUS_PACKET_HEADER

#include "MinicraftDef.h"
#include "Protocol.h"
#include "Dimension.h"
#include "World.h"
#include "Entity.h"
#include "Inventory.h"
#include "Potion.h"

#include <memory>
#include <vector>
#include <optional>
#include <variant>

namespace mcplus {
    /**
     * Type: Input/Output
     * Description: It indicates that either player or server occurred an error
     */
    struct InvalidPacket;
    /**
     * Type: Input/Output
     * Description: It used by server, measuring latency between player and server,
     *              when server send a ping package, player will re-sent it again
     */
    struct PingPacket;
    /**
     * Type: Input/Output
     * Description: Receive login credentials from player, if server send this packet,
     *              it means that login failed, otherwise no
     */
    struct LoginPacket;
    /**
     * Type: Output
     * Description: Send the game information to player
     */
    struct GamePacket;
    /**
     * Type: Output
     * Description: Send as world as player initial information to player
     */
    struct InitPacket;
    /**
     * Type: Input
     * Description: It indicates that player is requesting to server the tiles and entities
     *              from the world where player is on
     */
    struct LoadPacket;
    /**
     * Type: Output
     * Description: Send to player the whole world's tiles
     */
    struct TilesPacket;
    /**
     * Type: Output
     * Description: Send to player the whole world's entities
     */
    struct EntitiesPacket;
    /**
     * Type: Output
     * Description: It indicates that a tile was changed/updated
     */
    struct TilePacket;
    /**
     * Type: Input/Output
     * Description: It indicates that an entity was changed/updated, but
     *              if player doesn't have that entity, it will request
     *              to send that entity data through this
     */
    struct EntityPacket;
    /**
     * Type: Output
     * Description: Send the player data to player
     */
    struct PlayerPacket;
    /**
     * Type: Input
     * Description: It indicates that player moved around the world
     */
    struct MovePacket;
    /**
     * Type: Output
     * Description: Send to player a new entity
     */
    struct AddPacket;
    /**
     * Type: Output
     * Description: Notify to player that entity should be marked as removed
     */
    struct RemovePacket;
    /**
     * Type: Input/Output
     * Description: It indicates that player is quitting from server or server
     *              made player to disconnect
     */
    struct DisconnectPacket;
    /**
     * Type: Input
     * Description: Player requests to save data
     */
    struct SavePacket;
    /**
     * Type: Output
     * Description: Send a notification to player
     */
    struct NotifyPacket;
    /**
     * Type: Input/Output
     * Description: Player requests an interaction given X data, thereby server
     *              will respond again with a resolved held item
     */
    struct InteractPacket;
    /**
     * Type: Input
     * Description: It indicates that player has pushed an entity
     */
    struct PushPacket;
    /**
     * Type: Input
     * Description: It indicates that player is picking up an item from floor
     */
    struct PickupPacket;
    /**
     * Type: Input
     * Description: Player is saving an item to a chest
     */
    struct ChestInPacket;
    /**
     * Type: Input/Output
     * Description: It can mean two things, if player is leaving an item in a chest
     *              then player will be sending item index, input index and if to get
     *              the whole stack. If it's a death chest, then server will just send
     *              to player all items from that death chest
     */
    struct ChestOutPacket;
    /**
     * Type: Output
     * Description: Server will be adding items to player inventory
     */
    struct AddItemsPacket;
    /**
     * Type: Input
     * Description: It indicates that a player went to sleep in a bed or exited from bed
     */
    struct BedPacket;
    /**
     * Type: ??
     * Description ??
     */
    struct PotionPacket;
    /**
     * Type: Output
     * Description: Send to player a hurt from an entity
     */
    struct HurtPacket;
    /**
     * Type: Input
     * Description: It indicates that a player died
     */
    struct DiePacket;
    /**
     * Type: Input
     * Description: It indicates that a player is trying to respawn again
     */
    struct RespawnPacket;
    /**
     * Type: Input
     * Description: It indicates that a player dropped an item
     */
    struct DropPacket;
    /**
     * Type: Output
     * Description: Send how many stamina to take way from player
     */
    struct StaminaPacket;
    /**
     * Type: Input
     * Description: It indicates that player updated their shirt color
     */
    struct ShirtPacket;
    /**
     * Type: Output
     * Description: It indicates that an entity stopped fishing
     */
    struct StopFishingPacket;

    // Start Extension

    // End Extension

    /**
     * What kind of ping is sent.
     *
     * Auto if it was done by a timer or Manual if it was done
     * by a command or external cause
     */
    enum class PingType {
        AUTO,
        MANUAL
    };

    struct InvalidPacket : Packet {
        std::string message;

        explicit InvalidPacket(const std::string& message);
        explicit InvalidPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        InvalidPacket& operator=(const RawPacket& raw) override;
    };

    struct PingPacket : Packet {
        PingType type;

        explicit PingPacket(PingType type);
        explicit PingPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        PingPacket& operator=(const RawPacket& raw) override;
    };

    struct LoginPacket : Packet {
        std::string username;
        VersionPack version;

        LoginPacket(const std::string& username, const VersionPack& version);
        explicit LoginPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        LoginPacket& operator=(const RawPacket& raw) override;
    };

    struct GamePacket : Packet {
        std::string mode;
        int32_t time;
        float gameSpeed;
        bool pastDay;
        int32_t score;
        int32_t playerCount;
        int32_t awakenPlayer;

        GamePacket(const std::string& mode,
                   int32_t time,
                   float gameSpeed,
                   bool pastDay,
                   int32_t score,
                   int32_t playerCount,
                   int32_t awakenPlayer);
        explicit GamePacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        GamePacket& operator=(const RawPacket& raw) override;
    };

    struct InitPacket : Packet {
        int32_t id;
        int32_t width;
        int32_t height;
        int32_t level;
        int32_t x;
        int32_t y;

        InitPacket(int32_t id, int32_t width, int32_t height, int32_t level, int32_t x, int32_t y);
        explicit InitPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        InitPacket& operator=(const RawPacket& raw) override;
    };

    struct LoadPacket : Packet {
        int32_t currentLevel;

        explicit LoadPacket(int32_t currentLevel);
        explicit LoadPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        LoadPacket& operator=(const RawPacket& raw) override;
    };

    struct TilesPacket : Packet {
        std::vector<Tile> tileList;

        explicit TilesPacket(const std::vector<Tile>& tileList);
        explicit TilesPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        TilesPacket& operator=(const RawPacket& raw) override;
    };

    struct EntitiesPacket : Packet {
        std::vector<std::shared_ptr<Entity>> entityList;

        explicit EntitiesPacket(const std::vector<std::shared_ptr<Entity>>& entityList);
        explicit EntitiesPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        EntitiesPacket& operator=(const RawPacket& raw) override;
    };

    struct TilePacket : Packet {
        WorldId world;
        int32_t position; // x + width * y
        Tile tile;

        TilePacket(WorldId world, int32_t position, const Tile& tile);
        explicit TilePacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        TilePacket& operator=(const RawPacket& raw) override;
    };

    struct EntityPacket : Packet {
        std::shared_ptr<Entity> entity;

        explicit EntityPacket(const Entity& entity);
        explicit EntityPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        EntityPacket& operator=(const RawPacket& raw) override;
    };

    struct PlayerPacket : Packet {
        VersionPack version;

        int32_t x;
        int32_t y;
        int32_t spawnX;
        int32_t spawnY;
        int32_t health;
        int32_t hunger;
        int32_t armor;
        int32_t armorDamageBuffer;
        ItemMaterial curArmor;
        int32_t score;
        int32_t level;

        std::vector<Potion> potionList;
        Color shirtColor;
        bool skinon;

        std::vector<Item> inventory;

        PlayerPacket(const VersionPack& versionPack,
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
                     std::vector<Item> inventory);
        explicit PlayerPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        PlayerPacket& operator=(const RawPacket& raw) override;
    };

    struct MovePacket : Packet {
        FixedLocation location;

        Direction direction;
        WorldId world;

        MovePacket(const FixedLocation& location, Direction direction, WorldId world);
        explicit MovePacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        MovePacket& operator=(const RawPacket& raw) override;
    };

    struct AddPacket : Packet {
        std::shared_ptr<Entity> entity;

        explicit AddPacket(const Entity& entity);
        explicit AddPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        AddPacket& operator=(const RawPacket& raw) override;
    };

    struct RemovePacket : Packet {
        EntityId entity;
        std::optional<WorldId> world;

        RemovePacket(EntityId entity, std::optional<WorldId> world);
        explicit RemovePacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        RemovePacket& operator=(const RawPacket& raw) override;
    };

    struct DisconnectPacket : Packet {
        // no info

        DisconnectPacket();
        explicit DisconnectPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        DisconnectPacket& operator=(const RawPacket& raw) override;
    };

    struct SavePacket : Packet {
        // no info

        /*
         * actually, clients send their data, and we wouldn't like that a
         * hack client can send fake data
         */

        SavePacket();
        explicit SavePacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        SavePacket& operator=(const RawPacket& raw) override;
    };

    struct NotifyPacket : Packet {
        int32_t notetime;
        std::string note;

        NotifyPacket(int32_t notetime, const std::string& note);
        explicit NotifyPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        NotifyPacket& operator=(const RawPacket& raw) override;
    };

    struct InteractPacket : Packet {
        Item item;
        int32_t stamina;
        int32_t arrowCount;

        InteractPacket(const Item& item, int32_t stamina, int32_t arrowCount);
        explicit InteractPacket(const Item& item);
        explicit InteractPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        InteractPacket& operator=(const RawPacket& raw) override;
    private:
        bool isOutgoing;
    };

    struct PushPacket : Packet {
        EntityId entity;

        explicit PushPacket(EntityId entity);
        explicit PushPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        PushPacket& operator=(const RawPacket& raw) override;
    };

    struct PickupPacket : Packet {
        EntityId entity;

        explicit PickupPacket(EntityId entity);
        explicit PickupPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        PickupPacket& operator=(const RawPacket& raw) override;
    };

    struct ChestInPacket : Packet {
        EntityId chestId;
        int32_t index;
        Item item;

        ChestInPacket(EntityId chestId, int32_t index, const Item& item);
        explicit ChestInPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        ChestInPacket& operator=(const RawPacket& raw) override;
    };

    struct ChestOutPacket : Packet {
        struct InPacket {
            EntityId chestId;
            int32_t itemIndex;
            int32_t inputIndex;
            bool wholeStack;
        };
        struct OutPacket {
            //Item item;
            int32_t index;
        };

        std::variant<InPacket, OutPacket> data;

        explicit ChestOutPacket(const ChestOutPacket::InPacket& in);
        explicit ChestOutPacket(const ChestOutPacket::OutPacket& out);
        explicit ChestOutPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        ChestOutPacket& operator=(const RawPacket& raw) override;
    };

    struct AddItemsPacket : Packet {
        std::vector<Item> itemList;

        explicit AddItemsPacket(const std::vector<Item>& itemList);
        explicit AddItemsPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        AddItemsPacket& operator=(const RawPacket& raw) override;
    };

    struct BedPacket : Packet {
        bool enabled;
        EntityId bedId;

        BedPacket(bool enabled, EntityId bedId);
        explicit BedPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        BedPacket& operator=(const RawPacket& raw) override;
    };

    struct PotionPacket : Packet {
        PotionType type;
        bool enabled;

        PotionPacket(PotionType type, bool enabled);
        explicit PotionPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        PotionPacket& operator=(const RawPacket& raw) override;
    };

    struct HurtPacket : Packet {
        EntityId entity;
        Damage_t damage;
        Direction direction;

        HurtPacket(EntityId entity, Damage_t damage, Direction direction);
        explicit HurtPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        HurtPacket& operator=(const RawPacket& raw) override;
    };

    struct DiePacket : Packet {
        // no info

        // actually player sends a Death Chest with their items

        explicit DiePacket();
        explicit DiePacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        DiePacket& operator=(const RawPacket& raw) override;
    };

    struct RespawnPacket : Packet {
        // no info

        RespawnPacket();
        explicit RespawnPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        RespawnPacket& operator=(const RawPacket& raw) override;
    };

    struct DropPacket : Packet {
        Item item;

        explicit DropPacket(const Item& item);
        explicit DropPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        DropPacket& operator=(const RawPacket& raw) override;
    };

    struct StaminaPacket : Packet {
        int32_t stamina;

        explicit StaminaPacket(int32_t stamina);
        explicit StaminaPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        StaminaPacket& operator=(const RawPacket& raw) override;
    };

    struct ShirtPacket : Packet {
        Color color;

        explicit ShirtPacket(const Color& color);
        explicit ShirtPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        ShirtPacket& operator=(const RawPacket& raw) override;
    };

    struct StopFishingPacket : Packet {
        EntityId entity;

        explicit StopFishingPacket(EntityId entity);
        explicit StopFishingPacket(const RawPacket& raw);

        explicit operator RawPacket() const override;
        StopFishingPacket& operator=(const RawPacket& raw) override;
    };

}

#endif // MCPLUS_PACKET_HEADER
