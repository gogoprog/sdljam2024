#pragma once

#include "../context.h"

#include "factory.h"
#include "shake.h"
#include <cstdlib>

struct Life : public Component {
    int team{0};
    int hp{100};
    int maxHp;
};

class LifeSystem : public System {
  public:
    LifeSystem() {
        require<Life>();
    }

    void onEntityAdded(Entity &entity) override {
        auto &life = entity.get<Life>();
        life.maxHp = life.hp;

        if (life.team == 1) {
            if (entity.has<Sprite>()) {
                auto &sprite = entity.get<Sprite>();
                sprite.r = 255;
                sprite.g = 150;
                sprite.b = 100;
            }
        } else {
        }
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &life = entity.get<Life>();

        if (life.hp <= 0) {
            auto &cameraEntity = Context::get().cameraEntity;
            auto &game = Context::get().game;
            auto e = Factory::createExplosion();

            e->position = entity.position;
            engine->addEntity(e);

            engine->removeEntity(entity);

            /* game.stats.money += 20 + 20 * game.waveCount; */

            Context::get().audio.playSound("impact", entity.position);

            if (!cameraEntity->has<Shake>()) {
                cameraEntity->add<Shake>();

                // TODO: Maybe this should be in the constructor for Shake
                // instead with a static variable making it bigger every Nth
                // trigger.
                // I'd also add a freeze-frame effect as well, but that's
                // another topic.
                if (rand() % 100 >= 70) {
                    cameraEntity->get<Shake>().intensity += 0.8f;
                    cameraEntity->get<Shake>().duration += 1.0f;
                }
            }
        }
    }
};
