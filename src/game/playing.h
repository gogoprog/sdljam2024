#pragma once

#include "../context.h"
#include "factory.h"

class PlayingSystem : public System {
  public:
    PlayingSystem() {
    }

    void onAdded() override {
        for (int i = 0; i < 128; ++i) {
            auto e = Factory::createVehicle();
            e->position.x = 200 + rand() % 200;
            e->position.y = 200 + rand() % 200;
            engine->addEntity(e);
        }
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;

        if (inputs.isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
            Context::get().game.changeState(Game::State::MENU);
        }
    }
};
