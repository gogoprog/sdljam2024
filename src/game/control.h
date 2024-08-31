#pragma once

#include "../context.h"
#include "fx.h"
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
        auto &mouse_pos = inputs.getMousePosition();

        if (mouse_pos.x > renderer.width - 150)
            return;

        applySelection = false;

        if (!selecting && inputs.isMouseJustPressed(1)) {
            startMousePosition = mouse_pos;
            endMousePosition = mouse_pos;
            selecting = true;
        }

        if (selecting) {

            renderer.drawQuad(startMousePosition, endMousePosition - startMousePosition, 255, 255, 255);
            endMousePosition = mouse_pos;

            if (inputs.isMouseJustReleased(1)) {
                endMousePosition = mouse_pos;
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

                    Context::get().audio.playSound("select");
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
    Entity::Ptr clickedEntity;

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;
        auto &mouse_pos = inputs.getMousePosition();

        if (mouse_pos.x > renderer.width - 150)
            return;

        clickedEntity = nullptr;

        auto pos = Context::get().getMouseWorldPosition();
        auto coords = level.getTileCoords(pos);

        engine->iterate<Vehicle>([&](auto &e) {
            auto dist = Vector2::getSquareDistance(pos, e->position);

            if (dist < 32 * 32) {
                if (inputs.isMouseJustPressed(3)) {
                    clickedEntity = e;
                }

                renderer.draw(pos, "Cursor1", true);
            }

            return true;
        });

        if (inputs.isMouseJustPressed(3)) {
            if (level.isRoad(coords)) {
                auto rpos = level.getTileCenterPosition(coords);

                auto e = Factory::createSelectFx();
                e->position = rpos;
                e->get<SelectFx>().target = clickedEntity;
                engine->addEntity(e);
            }
        }

        System::update(dt);
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &level = Context::get().level;
        auto &game = Context::get().game;
        auto &vehicle = entity.get<Vehicle>();
        auto &inputs = Context::get().inputs;
        auto &renderer = Context::get().renderer;
        auto &pos = entity.position;
        auto &life = entity.get<Life>();

        if (inputs.isMouseJustPressed(1)) {
            entity.remove<Selected>();
            return;
        }

        if (inputs.isMouseJustPressed(3)) {
            auto pos = Context::get().getMouseWorldPosition();
            auto coords = level.getTileCoords(pos);

            if (level.isRoad(coords)) {
                if (!entity.has<Target>()) {
                    entity.add<Target>();
                }

                if (clickedEntity == nullptr) {
                    entity.get<Target>().tileCoords = coords;
                    entity.get<Target>().entity = nullptr;
                } else {
                    entity.get<Target>().entity = clickedEntity;
                }

                Context::get().audio.playSound("move");
            }
        }

        const int size = 42;

        renderer.drawQuad(pos - Vector2{size, size} / 2, {size, size}, 255, 255, 255, true);

        if (entity.has<Target>()) {
            auto &target = entity.get<Target>();
            auto pos = level.getTileCenterPosition(target.tileCoords);
            if (target.entity != nullptr) {
                pos = target.entity->position;
            }
            renderer.draw(pos, "Cursor1", true);
        }

        renderer.drawProgressBar(entity.position - Vector2{0, 20}, 40, life.hp / (float)life.maxHp, true);
    }
};
