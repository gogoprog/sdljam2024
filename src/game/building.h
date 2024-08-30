#pragma once

#include "../context.h"
#include "factory.h"

class BuildingSystem : public System {
  public:
    BuildingSystem() {
    }

    void onAdded() override {
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;
        auto &game = Context::get().game;
        auto &atlas = renderer.getAtlas("Turret");
        auto world_position = Context::get().getMouseWorldPosition();

        auto tile_coords = level.getTileCoords(world_position);
        auto position = level.getTilePosition(tile_coords);

        if (level.canBuildAt(tile_coords)) {

            renderer.draw(position, atlas, 0);

            if (inputs.isMouseJustPressed(1)) {
                if (game.stats.money >= game.turretCost) {
                    {
                        auto e = Factory::createBase();
                        e->position = position;
                        engine->addEntity(e);
                    }

                    {
                        auto e = Factory::createTurret();
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
        }
    }

  private:
    float timeLeft;
};
