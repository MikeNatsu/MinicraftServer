#ifndef MINICRAFTSERVER_ENTITY_H
#define MINICRAFTSERVER_ENTITY_H

#include "MinicraftDef.h"
#include "Dimension.h"
#include "Inventory.h"

#include <unordered_map>
#include <memory>
#include <string_view>
#include <functional>
#include <optional>

namespace mcplus {

    class Entity;

    class ArrowEntity;
    class ItemEntity;
    class Spark;

    class FurnitureEntity;
    class BedEntity;
    class ChestEntity;
    class CrafterEntity;
    class DeathChestEntity;
    class DungeonChestEntity;
    class LanternEntity;
    class SpawnerEntity;
    class TNTEntity;

    class Mob;
    class MobAI;
    class EnemyMob;
    class PassiveMob;
    class AirWizard;
    class Cow;
    class Crepeer;
    class Pig;
    class Player;
    class Sheep;
    class Skeleton;
    class Slime;
    class Snake;
    class Zombie;

    class ParticleEntity;
    class FireParticleEntity;
    class SmashParticleEntity;
    class TextParticleEntity;

    using EntitySolver = std::function<std::shared_ptr<Entity>(EntityId)>;

    class Entity : protected std::enable_shared_from_this<Entity> {
    protected:
        // these are for updates
        static constexpr std::string_view ID         = "eid";
        static constexpr std::string_view X_POSITION = "x";
        static constexpr std::string_view Y_POSITION = "y";
        static constexpr std::string_view LEVEL      = "level";

        struct Field {
            using Extractor = std::function<std::string()>;

            Extractor extractor;

            explicit Field(Extractor extractor);
            Field() = default;
        };

        using UpdateMapper = std::unordered_map<std::string_view, Field>;

        struct Data {
            EntityId id;
            Location2f location;
            bool removed;
            UpdateMapper updateMap;

            Data(EntityId id, const Location2f& location, bool removed, const UpdateMapper& updateMap);
            Data(EntityId id, const Location2f& location, bool removed);
            virtual ~Data() = default;
        };

        std::shared_ptr<Data> _data;

        explicit Entity(std::shared_ptr<Data> data);
        Entity();
    public:
        [[nodiscard]] EntityId id() const;

        const Location2f& getLocation() const;
        void setLocation(const Location2f& location);

        bool isRemoved() const;
        void remove();

        virtual void tick() = 0;
        virtual std::shared_ptr<Entity> get() const = 0;

        [[nodiscard]] virtual std::string raw() const;
        [[nodiscard]] virtual std::string rawUpdate() const;
    };

    class ArrowEntity : public Entity {
        struct ArrowData : Entity::Data {
            std::shared_ptr<Entity> owner;
            Direction attackDirection;
            Damage_t damage;

            Vector2f acceleration;

            ArrowData(EntityId id, const Location2f& location, bool removed, const UpdateMapper& updateMap,
                      std::shared_ptr<Entity> owner, Direction attackDirection, Damage_t damage);
            ArrowData(EntityId id, const Location2f& location, bool removed,
                      std::shared_ptr<Entity> owner, Direction attackDirection, Damage_t damage);
        };

        explicit ArrowEntity(std::shared_ptr<Data> data);
    public:
        explicit ArrowEntity(const Location2f& location,
                             std::shared_ptr<Entity> owner,
                             Direction attackDirection,
                             Damage_t damage);
        ArrowEntity();

        void tick() override;
        std::shared_ptr<Entity> get() const override;

        std::string raw() const override;
    };

    class ItemEntity : public Entity {
    public:
        int32_t lifeTime;
        std::shared_ptr<Item> item;
    };

    class Spark : public Entity {

    };

    class FurnitureEntity : public Entity {

    };

    class BedEntity : public Entity {

    };

    class ChestEntity : public Entity {

    };

    class Mob : public Entity {
        int16_t health;
    };

    class MobAI : public Mob {
        bool ai;
    };

    class AirWizard : public MobAI {

    };

    class HumanEntity : public Mob, public InventoryHolder {
    public:
        [[nodiscard]] const Inventory& getInventory() const override;
        Inventory& getInventory() override;
    };

    class NPC : public HumanEntity {

    };

    class Player : public HumanEntity {

    };

    std::shared_ptr<Entity> createEntity(const std::string& raw, std::optional<EntitySolver> solver = {});

}

#endif //MINICRAFTSERVER_ENTITY_H
