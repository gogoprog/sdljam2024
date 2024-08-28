#pragma once

#include "../context.h"
#include "control.h"
#include "factory.h"

class PlayingSystem : public System {
  public:
    PlayingSystem() {
    }

    void onAdded() override {
        auto &level = Context::get().level;

        for (int i = 0; i < 12; ++i) {
            auto e = Factory::createVehicle();
            e->position.x = 200 + rand() % 200;
            e->position.y = 200 + rand() % 200;
            e->add<Selectable>();
            e->get<Life>().team = 0;
            e->name = "playertank" + std::to_string(i);
            engine->addEntity(e);
        }

        for (int i = 0; i < 18; ++i) {
            auto e = Factory::createVehicle();
            e->name = "computertank" + std::to_string(i);
            /* auto pos = level.getTilePosition(level.startCoords[1]); */
            auto pos = Vector2{450, 450};
            e->position.x = pos.x + rand() % 200;
            e->position.y = pos.y + rand() % 200;
            e->get<Life>().team = 1;
            e->get<Sprite>().r = 250;
            e->get<Sprite>().g = 12;
            e->get<Sprite>().b = 12;
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
