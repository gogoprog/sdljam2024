#pragma once

#include "../context.h"

class MenuSystem : public System {
  public:
    MenuSystem() {
        componentsNames.push_back("Control");
    }

    void onAdded() override {
    }

    void update(const float dt) override {
        System::update(dt);

        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;
        auto world_position = Context::get().getMouseWorldPosition();
        auto &terrain = renderer.getTerrain("StoneSnow");

        renderer.drawCenteredText(80, "island war", 5);
        renderer.drawCenteredText(200, "a sdl2 based rts game", 1);

        renderer.drawCenteredText(300, "play", 2);

        renderer.drawText(Vector2(800, 750), "dedicated to my love : clementine", 1);

        if (inputs.isKeyJustPressed(SDL_SCANCODE_RETURN)) {
            Context::get().game.changeState(Game::State::PLAYING);
        }
    }

    void updateSingle(const float dt, Entity &entity) override {
    }

  private:
    float timeLeft;
};
