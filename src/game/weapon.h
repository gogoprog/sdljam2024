#pragma once

#include "../context.h"
#include "factory.h"

struct Weapon : public Component {
    float timeSinceLastFire{100};
    bool mustFire{false};
    float range = 250;
    float damage = 100;
    float interval = 1;
};

class WeaponSystem : public System {
  public:
    WeaponSystem() {
        require<Weapon>();
    }

    void onEntityAdded(Entity &entity) override {
        auto &level = Context::get().level;
        auto coords = level.getTileCoords(entity.position);
        level.lock2x2(coords);
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &renderer = Context::get().renderer;
        const auto cursor_position = Context::get().getMouseWorldPosition();
        auto &weapon = entity.get<Weapon>();

        if (weapon.timeSinceLastFire > weapon.interval && weapon.mustFire) {
            weapon.timeSinceLastFire = 0;
            auto e = Factory::createBullet(entity, weapon.range, weapon.damage);
            Context::get().engine.addEntity(e);
            weapon.mustFire = false;

            Context::get().audio.playSound("firing");
        }

        weapon.timeSinceLastFire += dt;
    }
};
