#pragma once

#include "../context.h"
#include "vehicle.h"

struct Control : public Component {
    Vector2 startPosition;
    Vector2 startMousePosition;
};

struct Selectable : public Component {};

struct Selected : public Component {};

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

    bool selecting = false;
    bool applySelection = false;
    Vector2 startMousePosition;
    Vector2 endMousePosition;

    void onEntityAdded(Entity &entity) override {
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &renderer = Context::get().renderer;

        applySelection = false;

        if (!selecting && inputs.isMouseJustPressed(1)) {
            /* startMousePosition = Context::get().getMouseWorldPosition(); */
            startMousePosition = inputs.getMousePosition();
            endMousePosition = inputs.getMousePosition();
            selecting = true;
        }

        if (selecting) {

            renderer.drawQuad(startMousePosition, endMousePosition - startMousePosition, 255, 255, 255);
            endMousePosition = inputs.getMousePosition();

            if (inputs.isMouseJustReleased(1)) {
                /* endMousePosition = Context::get().getMouseWorldPosition(); */

                endMousePosition = inputs.getMousePosition();
                selecting = false;
                applySelection = true;
            }
        }

        System::update(dt);
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &level = Context::get().level;
        auto &game = Context::get().game;
        auto &vehicle = entity.get<Vehicle>();

        auto &pos = entity.position;

        if (applySelection) {
            Vector2 min = {std::min(startMousePosition.x, endMousePosition.x),
                           std::min(startMousePosition.y, endMousePosition.y)};
            Vector2 max = {std::max(startMousePosition.x, endMousePosition.x),
                           std::max(startMousePosition.y, endMousePosition.y)};

            min = Context::get().getWorldPosition(min);
            max = Context::get().getWorldPosition(max);

            if (pos.x > min.x && pos.x < max.x && pos.y > min.y && pos.y < max.y) {

                if (!entity.has<Selected>()) {
                    entity.add<Selected>();
                }
            }
        }
    }
};

class VehicleSelectedSystem : public System {
  public:
    VehicleSelectedSystem() {
        require<Vehicle>();
        require<Selected>();
    }

    bool selecting = false;
    bool applySelection = false;
    Vector2 startMousePosition;
    Vector2 endMousePosition;

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;

        System::update(dt);

        if (inputs.isMouseJustPressed(3)) {
            auto pos = Context::get().getMouseWorldPosition();
            auto coords = level.getTileCoords(pos);
            auto rpos = level.getTileCenterPosition(coords);

            auto e = Factory::createSelectFx();
            e->position = rpos;
            engine->addEntity(e);
        }
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &level = Context::get().level;
        auto &game = Context::get().game;
        auto &vehicle = entity.get<Vehicle>();
        auto &inputs = Context::get().inputs;
        auto &renderer = Context::get().renderer;
        auto &pos = entity.position;

        if (inputs.isMouseJustPressed(1)) {
            entity.remove<Selected>();
            return;
        }

        if (inputs.isMouseJustPressed(3)) {
            auto pos = Context::get().getMouseWorldPosition();
            auto coords = level.getTileCoords(pos);
            vehicle.target = coords;
        }

        const int size = 42;

        renderer.drawQuad(pos - Vector2{size, size} / 2, {size, size}, 255, 255, 255, true);

        if (entity.has<Move>()) {
            auto rtarget = level.getTileCenterPosition(vehicle.target);
            renderer.draw(rtarget, "Cursor1", true);
        }
    }
};
