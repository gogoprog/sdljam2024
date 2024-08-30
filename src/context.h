#pragma once

#include "audio.h"
#include "ecs.h"
#include "game.h"
#include "inputs.h"
#include "level.h"
#include "renderer.h"

class Context {
  public:
    Renderer renderer;
    Audio audio;
    Inputs inputs;
    Level level;
    Engine engine;
    Game game;

    SharedPtr<Entity> cameraEntity;

    static Context &get() {
        static Context instance;
        return instance;
    }

    Vector2 getMouseWorldPosition() const {
        auto & camera_position = renderer.getCameraPosition();
        auto & mouse_position = inputs.getMousePosition();

        return mouse_position + camera_position;
    }

    Vector2 getWorldPosition(Vector2 screen_position) const {
        auto camera_position = renderer.getCameraPosition();
        return screen_position + camera_position;
    }
};
