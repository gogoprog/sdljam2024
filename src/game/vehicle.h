#pragma once

#include "../context.h"
#include "move.h"

struct Vehicle : public Component {
    Path path;
    float speed = 100;
    Vector2 target;
};

class VehicleSystem : public System {
  public:
    VehicleSystem() {
        require<Vehicle>();
        require<Movable>();
    }

    void onEntityAdded(Entity &entity) override {
        /* auto &move = entity.add<Move>(entity.position, Vector2{350, 256}, 100); */
        auto &vehicle = entity.get<Vehicle>();
        auto &level = Context::get().level;
        vehicle.target = level.getTileCoords(entity.position);
        vehicle.target.x += rand() % 20;
        vehicle.target.y += rand() % 20;
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &level = Context::get().level;
        auto &game = Context::get().game;
        auto &vehicle = entity.get<Vehicle>();

        if (!entity.has<Move>()) {

            vehicle.path.resize(0);

            auto start_coords = level.getTileCoords(entity.position);

            if (start_coords == vehicle.target) {
                setRandomTarget(entity);
            } else {
                if (level.findPath(vehicle.path, start_coords, vehicle.target)) {
                    if (vehicle.path.size() > 1) {
                        auto position = level.getTileCenterPosition(vehicle.path[1]);
                        entity.add<Move>(entity.position, position, vehicle.speed);
                    } else if (vehicle.path.size() == 0) {
                    }
                } else {
                    /* auto position = level.getTileCenterPosition(start_coords + directions[rand() % 8]); */
                    /* entity.add<Move>(entity.position, position, vehicle.speed); */
                    setRandomTarget(entity);
                }
            }
        }

        /* for (auto &p : vehicle.path) { */
        /*     auto pos = level.getTileCenterPosition(p); */
        /*     Context::get().renderer.draw(pos, "Bullets", 0); */
        /* } */
    }

    void setRandomTarget(Entity &entity) {
        auto &vehicle = entity.get<Vehicle>();
        auto &level = Context::get().level;
        vehicle.target = level.getTileCoords(entity.position);
        vehicle.target.x += rand() % 20 - 10;
        vehicle.target.y += rand() % 20 - 10;
    }
};
