#pragma once

#include "../context.h"
#include "button.h"
#include "control.h"
#include "factory.h"

class MenuSystem : public System {
  public:
    MenuSystem() {
        require<Control>();
    }

    void onAdded() override {
        auto &renderer = Context::get().renderer;
        {
            auto e = Factory::createButton("play", []() { Context::get().engine.changeState(Game::State::PLAYING); });
            e->get<Button>().scale = 2.0f;
            e->position = {renderer.width / 2, 350};
            buttons.push_back(e);
            engine->addEntity(e);
        }
        {
            auto e = Factory::createButton("editor", []() { Context::get().engine.changeState(Game::State::EDITOR); });
            e->get<Button>().scale = 1.0f;
            e->position = {renderer.width / 2, 450};
            buttons.push_back(e);
            engine->addEntity(e);
        }
    }

    void onRemoved() override {
        for (auto e : buttons) {
            engine->removeEntity(*e);
        }
    }

    void update(const float dt) override {
        System::update(dt);

        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;
        auto world_position = Context::get().getMouseWorldPosition();

        auto size = 800;
        renderer.drawFilledQuad(Vector2{renderer.width / 2 - size / 2, 50}, Vector2{size, 700}, 20, 20, 20, 0.9, false);

        renderer.drawCenteredText(80, "island war", 5, false, 0.5f);
        renderer.drawCenteredText(200, "a sdl2 based rts game", 1);

        renderer.drawText(Vector2(renderer.width - 440, renderer.height - 20), "dedicated to my love : clementine", 1);

        if (inputs.isKeyJustPressed(SDL_SCANCODE_RETURN)) {
            Context::get().engine.changeState(Game::State::PLAYING);
        }

        if (inputs.isKeyJustPressed(SDL_SCANCODE_E)) {
            Context::get().engine.changeState(Game::State::EDITOR);
        }
    }

    void updateSingle(const float dt, Entity &entity) override {
        time += dt;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;
        auto move = 100;
        auto origin = Vector2(level.width - renderer.width, level.height - renderer.height) * 0.5f;

        entity.position = origin + Vector2(std::sin(time) * move, std::cos(time) * move);
    }

  private:
    float time{0};
    Vector<Entity::Ptr> buttons;
};

class PauseSystem : public System {
  public:
    PauseSystem() {
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

        renderer.drawCenteredText(300, "resume", 2);

        if (inputs.isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
            Context::get().engine.changeState(Game::State::PLAYING);
        }
    }

  private:
};
