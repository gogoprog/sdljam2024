#pragma once

#include "../context.h"

class EditorSystem : public System {
  public:
    EditorSystem() {
        priority = 10;
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &renderer = Context::get().renderer;
        auto &game = Context::get().game;
        auto &level = Context::get().level;
        auto &mouse_position = inputs.getMousePosition();
        auto world_position = Context::get().getMouseWorldPosition();

        auto tile_coords = level.getTileCoords(world_position);
        auto position = level.getTilePosition(tile_coords);

        auto &terrain = renderer.getTerrain("Snd2Watr");

        if (!inputs.isKeyPressed(SDL_SCANCODE_LSHIFT)) {
            if (inputs.isMousePressed(1)) {
                level.setRoad(tile_coords, true);
            }
            renderer.draw(position, terrain, Tile::FILL2);
        } else {

            if (inputs.isMousePressed(1)) {
                level.setRoad(tile_coords, false);
            }
            renderer.draw(position, terrain, Tile::FILL1);
        }

        if (inputs.isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
            Context::get().game.changeState(Game::State::MENU);
        }

        if (inputs.isKeyJustPressed(SDL_SCANCODE_W)) {
            level.dump();
        }
    }
};
