#pragma once

#include "../context.h"
#include "factory.h"

struct Structure : public Component {
    bool needCanon = false;
};

class BuildingSystem : public System {
  public:
    StructureType typeToBuild;
    BuildingSystem() {
    }

    void onAdded() override {
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;
        auto &game = Context::get().game;
        String str;
        switch (typeToBuild) {

            case StructureType::Turret: {
                str = "Turret";
            } break;
            case StructureType::Generator: {
                str = "Generator";
            } break;
            case StructureType::TankFactory: {
                str = "Starprt2";
            } break;
        }

        auto &atlas = renderer.getAtlas(str);
        auto world_position = Context::get().getMouseWorldPosition();

        auto tile_coords = level.getTileCoords(world_position);
        auto position = level.getTilePosition(tile_coords);

        if (level.canBuildAt(tile_coords)) {

            renderer.draw(position, atlas, 0);

            if (inputs.isMouseJustPressed(1)) {
                if (game.stats.money >= game.turretCost) {
                    {
                        auto e = Factory::createStructure(typeToBuild);
                        e->position = position;
                        engine->addEntity(e);
                    }

                    level.lock2x2(tile_coords);
                    game.stats.money -= game.turretCost;

                    engine->changeState(Game::State::PLAYING);
                }
            }

            auto pos = inputs.getMousePosition();
            pos.y += 32;
            renderer.drawText(pos, "build (" + std::to_string(game.turretCost) + ")");
        } else {

            renderer.draw(position, str, 0, false, 220, 20, 20);
        }

        if (inputs.isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
            Context::get().engine.changeState(Game::State::PLAYING);
        }
    }

  private:
    float timeLeft;
};

class StructureSystem : public System {
  public:
    StructureSystem() {
        require<Structure>();
    }

    void onEntityAdded(Entity &entity) override {
        auto &structure = entity.get<Structure>();

        if (structure.needCanon) {
            auto e = Factory::createTurret();
            e->position = entity.position;
            engine->addEntity(e);
        }
    }

    void updateSingle(const float dt, Entity &entity) override {
    }
};
