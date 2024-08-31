#pragma once

#include "../context.h"
#include "building.h"
#include "control.h"
#include "factory.h"

class MissionSystem : public System {
  public:
    MissionSystem() {
    }

    void onAdded() override {
        auto &level = Context::get().level;
        auto &game = Context::get().game;

        game.stats.money = 1000;

        for (int i = 0; i < 10; ++i) {
            auto e = Factory::createVehicle(0);
            e->position.x = 200 + rand() % 200;
            e->position.y = 200 + rand() % 200;
            e->name = "playertank" + std::to_string(i);
            engine->addEntity(e);
        }

        if (1)
            for (int i = 0; i < 16; ++i) {
                auto pos = level.getTilePosition(level.startCoords[1]);
                pos.x = pos.x - 500 - rand() % 400;
                pos.y = pos.y + 200 + rand() % 200;
                auto p = level.getTileCoords(pos);

                if (level.isFree(p)) {
                    auto e = Factory::createVehicle(1);
                    e->name = "computertank" + std::to_string(i);
                    e->position = pos;

                    engine->addEntity(e);
                    /* e->add<Target>().tileCoords = level.startCoords[1]; */
                }
            }

        for (int i = 0; i < 3; ++i) {
            auto coords = level.startCoords[1];
            coords.x -= i * 3;
            coords.y += 6;
            auto pos = level.getTilePosition(coords);
            auto e = Factory::createStructure(StructureType::TankFactory);
            e->get<Life>().team = 1;
            e->position = pos;
            engine->addEntity(e);
        }

        {
            auto coords = level.startCoords[1];
            coords.x -= 20;
            coords.y += 6;
            auto pos = level.getTilePosition(coords);
            auto e = Factory::createStructure(StructureType::Turret);
            e->get<Life>().team = 1;
            e->position = pos;
            engine->addEntity(e);
        }

        Context::get().cameraEntity->position = {0, 0};
    }

    void update(const float dt) override {

        time -= dt;

        if (time > 0) {
            auto &renderer = Context::get().renderer;
            auto alpha = time / 3;
            renderer.drawCenteredText(renderer.height / 2, "destroy the enemy", 3.0f, false, alpha);
        }
    }

  private:
    float time{3};
};

class PlayingSystem : public System {
  public:
    PlayingSystem() {
    }

    void onAdded() override {
    }

    void onRemoved() override {
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;

        if (inputs.isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
            Context::get().engine.changeState(Game::State::PAUSE);
        }
    }
};
