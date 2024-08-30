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

        for (int i = 0; i < 10; ++i) {
            auto e = Factory::createVehicle();
            e->position.x = 200 + rand() % 200;
            e->position.y = 200 + rand() % 200;
            e->add<Selectable>();
            e->get<Life>().team = 0;
            e->name = "playertank" + std::to_string(i);
            engine->addEntity(e);
        }

        for (int i = 0; i < 100; ++i) {
            /* auto pos = level.getTilePosition(level.startCoords[1]); */
            auto pos = Vector2{450, 450};
            pos.x = pos.x + rand() % 2000;
            pos.y = pos.y + rand() % 2000;
            auto p = level.getTileCoords(pos);

            if (level.isFree(p)) {
                auto e = Factory::createVehicle();
                e->name = "computertank" + std::to_string(i);
                e->position = pos;

                e->get<Life>().team = 1;
                e->get<Sprite>().r = 250;
                e->get<Sprite>().g = 12;
                e->get<Sprite>().b = 12;
                engine->addEntity(e);
                e->add<Target>().tileCoords = level.startCoords[1];
            }
        }
    }
};

class PlayingSystem : public System {
  public:
    PlayingSystem() {
    }

    void onAdded() override {
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;

        if (inputs.isKeyJustPressed(SDL_SCANCODE_1)) {
            engine->changeState(Game::State::BUILDING);
            engine->getSystem<BuildingSystem>().typeToBuild = StructureType::Turret;
        }

        if (inputs.isKeyJustPressed(SDL_SCANCODE_2)) {
            engine->changeState(Game::State::BUILDING);
            engine->getSystem<BuildingSystem>().typeToBuild = StructureType::Generator;
        }

        if (inputs.isKeyJustPressed(SDL_SCANCODE_3)) {
            engine->changeState(Game::State::BUILDING);
            engine->getSystem<BuildingSystem>().typeToBuild = StructureType::TankFactory;
        }

        if (inputs.isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
            Context::get().engine.changeState(Game::State::MENU);
        }
    }
};
