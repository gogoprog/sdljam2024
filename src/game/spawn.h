#pragma once

#include "../context.h"
#include "factory.h"
#include "move.h"
#include "vehicle.h"

struct Flag : public Component {
};

struct Spawn : public Component {
    float timeSinceLastSpawn{10000};
};

class SpawnSystem : public System {
  public:
    SpawnSystem() {
        require<Spawn>();
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &game = Context::get().game;
        /* auto &wave = game.currentWave; */

        auto &spawn = entity.get<Spawn>();

        spawn.timeSinceLastSpawn += dt;

        if (spawn.timeSinceLastSpawn > 1.0) {


            spawn.timeSinceLastSpawn = 0;
        }
    }
};
