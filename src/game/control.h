#pragma once

#include "../context.h"
#include "vehicle.h"

struct Control : public Component {
    Vector2 startPosition;
    Vector2 startMousePosition;
};

struct Selectable : public Component {
    Vector2 startPosition;
    Vector2 startMousePosition;
};

class ControlSystem : public System {
  public:
    ControlSystem() {
        require<Control>();
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &inputs = Context::get().inputs;
        auto &control = entity.get<Control>();
        auto speed = 1000;

        if (inputs.isKeyPressed(SDL_SCANCODE_RIGHT) or inputs.isKeyPressed(SDL_SCANCODE_D)) {
            entity.position.x += dt * speed;
        }

        if (inputs.isKeyPressed(SDL_SCANCODE_LEFT) or inputs.isKeyPressed(SDL_SCANCODE_A)) {
            entity.position.x -= dt * speed;
        }

        if (inputs.isKeyPressed(SDL_SCANCODE_UP) or inputs.isKeyPressed(SDL_SCANCODE_W)) {
            entity.position.y -= dt * speed;
        }

        if (inputs.isKeyPressed(SDL_SCANCODE_DOWN) or inputs.isKeyPressed(SDL_SCANCODE_S)) {
            entity.position.y += dt * speed;
        }

        if (inputs.isMouseJustPressed(3)) {
            control.startMousePosition = inputs.getMousePosition();
            control.startPosition = entity.position;
        } else if (inputs.isMousePressed(3)) {
            auto mouse_pos = inputs.getMousePosition();
            auto delta = mouse_pos - control.startMousePosition;
            entity.position = control.startPosition - delta;
        }
    }
};


class VehicleSelectSystem : public System {
  public:
    VehicleSelectSystem() {
        require<Vehicle>();
        require<Selectable>();
    }

    void onEntityAdded(Entity &entity) override {
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &level = Context::get().level;
        auto &game = Context::get().game;
        auto &vehicle = entity.get<Vehicle>();

    }
};
