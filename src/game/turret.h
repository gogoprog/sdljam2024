#pragma once

#include "../context.h"
#include "factory.h"
#include "sprite.h"
#include "life.h"

struct TurretStat {
    float interval;
    float range;
    float damage;
};

extern Array<TurretStat, 10> turretStats;

struct Turret : public Component {
    float timeSinceLastFire{100};
    bool mustFire{false};
    int level{0};
};

class TurretSystem : public System {
  public:
    TurretSystem() {
        require<Turret>();
        require<RotatableSprite>();
    }

    void onEntityAdded(Entity &entity) override {
        auto &level = Context::get().level;
        auto coords = level.getTileCoords(entity.position);
        level.lock2x2(coords);
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &renderer = Context::get().renderer;
        const auto cursor_position = Context::get().getMouseWorldPosition();
        auto &turret = entity.get<Turret>();
        auto &rotatable = entity.get<RotatableSprite>();
        auto &stat = turretStats[turret.level];
        auto &life = entity.get<Life>();

        if (turret.timeSinceLastFire > stat.interval && turret.mustFire) {
            turret.timeSinceLastFire = 0;
            auto e = Factory::createBullet(entity, stat.range, stat.damage);
            engine->addEntity(e);
            turret.mustFire = false;

            Context::get().audio.playSound("firing", entity.position);
        }

        turret.timeSinceLastFire += dt;

        if (turret.timeSinceLastFire < 0.2) {
            rotatable.frameOffset = 8;
        } else {
            rotatable.frameOffset = 0;
        }

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

            if (closest_distance < stat.range * stat.range) {
                turret.mustFire = true;
            }
        }
    }
};
