#pragma once

#include "../context.h"

class MenuSystem : public System {
  public:
    MenuSystem() {
        require<Control>();
    }

    void onAdded() override {
    }

    void update(const float dt) override {
        System::update(dt);

        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;
        auto world_position = Context::get().getMouseWorldPosition();

        renderer.drawCenteredText(80, "island war", 5, false, 0.5f);
        renderer.drawCenteredText(200, "a sdl2 based rts game", 1);

        renderer.drawCenteredText(300, "play", 2);

        renderer.drawText(Vector2(800, 750), "dedicated to my love : clementine", 1);

        if (inputs.isKeyJustPressed(SDL_SCANCODE_RETURN)) {
            Context::get().game.changeState(Game::State::PLAYING);
        }

        if (inputs.isKeyJustPressed(SDL_SCANCODE_E)) {
            Context::get().game.changeState(Game::State::EDITOR);
        }
    }

    void updateSingle(const float dt, Entity &entity) override {
        time += dt;

        entity.position = Vector2(std::sin(time) * 200, std::cos(time) * 200);
    }

  private:
    float time{0};
};