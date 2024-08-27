#pragma once

#include "../context.h"
#include "move.h"
#include "weapon.h"

struct TargetPosition : public Component {
    Vector2 position;
};

struct TargetEntity : public Component {
    Entity::Ptr entity;
};

struct Vehicle : public Component {
    Path path;
    float speed = 100;
    float fireDistance = 250;
};

class VehicleSystem : public System {
  public:
    VehicleSystem() {
        require<Vehicle>();
        require<Weapon>();
    }

    void onEntityAdded(Entity &entity) override {
        auto &vehicle = entity.get<Vehicle>();
        auto &level = Context::get().level;
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &level = Context::get().level;
        auto &game = Context::get().game;
        auto &vehicle = entity.get<Vehicle>();
        auto &life = entity.get<Life>();

        if (!entity.has<TargetPosition>() && !entity.has<Move>()) {
            Entity::Ptr closest = nullptr;
            float closest_distance = std::numeric_limits<float>::max();

            for (auto &other : entities) {
                auto &other_life = other->get<Life>();

                if (other_life.team != life.team) {
                    auto distance = Vector2::getSquareDistance(entity.position, other->position);

                    if (distance < closest_distance) {
                        closest = other;
                        closest_distance = distance;
                    }
                }
            }

            if (closest != nullptr) {
                auto delta = closest->position - entity.position;
                entity.rotation = (std::atan2(delta.y, delta.x) * 180.0f / std::numbers::pi) + 90;

                if (closest_distance < vehicle.fireDistance * vehicle.fireDistance) {
                    entity.get<Weapon>().mustFire = true;
                }
            }
        }
    }
};

class VehicleTargetPositionSystem : public System {
  public:
    VehicleTargetPositionSystem() {
        require<Vehicle>();
        require<Movable>();
        require<TargetPosition>();
    }

    void onEntityAdded(Entity &entity) override {
        auto &vehicle = entity.get<Vehicle>();
        auto &level = Context::get().level;
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &level = Context::get().level;
        auto &game = Context::get().game;
        auto &vehicle = entity.get<Vehicle>();
        auto &target = entity.get<TargetPosition>();

        if (!entity.has<Move>()) {

            vehicle.path.resize(0);

            auto start_coords = level.getTileCoords(entity.position);

            if (start_coords == target.position) {
                entity.remove<TargetPosition>();
            } else {
                if (level.findPath(vehicle.path, start_coords, target.position)) {
                    if (vehicle.path.size() > 1) {
                        auto position = level.getTileCenterPosition(vehicle.path[1]);
                        entity.add<Move>(entity.position, position, vehicle.speed);
                    } else if (vehicle.path.size() == 0) {
                        entity.remove<TargetPosition>();
                    }
                } else {
                    entity.remove<TargetPosition>();
                }
            }
        }
    }
};
