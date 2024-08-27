#pragma once

#include "../context.h"
#include "move.h"

struct TargetPosition : public Component {
    Vector2 position;
};

struct TargetEntity : public Component {
    Entity::Ptr entity;
};

struct Vehicle : public Component {
    Path path;
    float speed = 100;
};

class VehicleSystem : public System {
  public:
    VehicleSystem() {
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
                    }
                }
            }
        }
    }

    void setRandomTarget(Entity &entity) {
        auto &vehicle = entity.get<Vehicle>();
        auto &level = Context::get().level;
        /* vehicle.target = level.getTileCoords(entity.position); */
        /* vehicle.target.x += rand() % 20 - 10; */
        /* vehicle.target.y += rand() % 20 - 10; */
    }
};
