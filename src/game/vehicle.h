#pragma once

#include "../context.h"
#include "life.h"
#include "move.h"
#include "weapon.h"

struct Target : public Component {
    Vector2 tileCoords;
    Entity::Ptr entity{nullptr};
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
        auto &renderer = Context::get().renderer;

        if (!entity.has<Target>() && !entity.has<Move>()) {
            Entity::Ptr closest = nullptr;
            float closest_distance = std::numeric_limits<float>::max();

            engine->iterate<Life>([&](Entity::Ptr &other) {
                if (other.get() != &entity) {
                    auto &other_life = other->get<Life>();

                    if (other_life.team != life.team) {
                        auto distance = Vector2::getSquareDistance(entity.position, other->position);

                        if (distance < closest_distance) {
                            closest = other;
                            closest_distance = distance;
                        }
                    }
                }
                return true;
            });

            if (closest != nullptr) {
                auto delta = closest->position - entity.position;
                entity.rotation = (std::atan2(delta.y, delta.x) * 180.0f / std::numbers::pi) + 90;

                if (closest_distance < vehicle.fireDistance * vehicle.fireDistance) {
                    entity.get<Weapon>().mustFire = true;
                }
            }

            // BotSystem

            if (life.team == 1) {
                auto x = rand() % level.tilewidth;
                auto y = rand() % level.tileheight;

                if (level.isFree({x, y})) {
                    entity.add<Target>().tileCoords = {x, y};
                }
            }
        } else {
            if (entity.has<Target>()) {
                auto other = entity.get<Target>().entity;
                if (other != nullptr) {

                    auto &other_life = other->get<Life>();
                    if (other_life.team != life.team) {
                        auto distance = Vector2::getSquareDistance(entity.position, other->position);
                        if (distance < vehicle.fireDistance * vehicle.fireDistance) {
                            entity.get<Weapon>().mustFire = true;
                        }
                    }
                }
            }
        }

        auto mwp = Context::get().getMouseWorldPosition();
        auto distance = Vector2::getSquareDistance(entity.position, mwp);

        if (distance < 32 * 32) {
            renderer.drawProgressBar(entity.position - Vector2{0, 20}, 40, life.hp / (float)life.maxHp, true);
        }
    }
};

class VehicleTargetSystem : public System {
  public:
    VehicleTargetSystem() {
        require<Vehicle>();
        require<Movable>();
        require<Target>();
    }

    void onEntityAdded(Entity &entity) override {
        auto &vehicle = entity.get<Vehicle>();
        auto &level = Context::get().level;
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &level = Context::get().level;
        auto &game = Context::get().game;
        auto &vehicle = entity.get<Vehicle>();
        auto &target = entity.get<Target>();

        Vector2 target_coords;

        if (target.entity != nullptr) {
            target_coords = level.getTileCoords(target.entity->position);
        } else {
            target_coords = target.tileCoords;
        }

        if (!entity.has<Move>()) {

            vehicle.path.resize(0);

            auto start_coords = level.getTileCoords(entity.position);

            if (start_coords == target_coords) {
                entity.remove<Target>();
            } else {
                if (level.findPath(vehicle.path, start_coords, target_coords)) {
                    if (vehicle.path.size() > 1) {
                        auto position = level.getTileCenterPosition(vehicle.path[1]);
                        entity.add<Move>(entity.position, position, vehicle.speed);
                    } else if (vehicle.path.size() == 0) {
                        entity.remove<Target>();
                    }

                } else {
                    entity.remove<Target>();
                }
            }
        }

        if (Context::get().inputs.isKeyPressed(SDL_SCANCODE_F1)) {
            for (int i = 1; i < vehicle.path.size(); ++i) {
                auto from = level.getTileCenterPosition(vehicle.path[i - 1]);
                auto to = level.getTileCenterPosition(vehicle.path[i]);

                Context::get().renderer.drawLine(from, to, 0, 0, 200, true);
            }
        }
    }
};
