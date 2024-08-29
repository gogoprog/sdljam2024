#pragma once

#include "../context.h"
#include "spawn.h"

class HudSystem : public System {
  public:
    HudSystem() {
        priority = 10;
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &renderer = Context::get().renderer;
        auto &game = Context::get().game;
        auto &level = Context::get().level;
        auto &mouse_position = inputs.getMousePosition();

        renderer.draw({renderer.width - 160, 0}, "Panel");

        {
            char buffer[64];
            sprintf(buffer, "%15d", game.stats.money);
            renderer.drawText({renderer.width - 130, 191}, "money", 1, false);
            renderer.drawText({renderer.width - 130, 218}, buffer, 1, false);
        }
        {
            char buffer[64];
            sprintf(buffer, "%15d", game.stats.lifes);
            renderer.drawText({renderer.width - 130, 188 + 150}, "lifes", 1, false);
            renderer.drawText({renderer.width - 130, 215 + 150}, buffer, 1, false);
        }
        {
            auto minimapSize = 120;

            Vector2 origin = {renderer.width - 138, 4};
            renderer.drawFilledQuad(origin, {minimapSize, minimapSize}, 200, 200, 200);

            float ratio = level.width / (float)minimapSize;

            engine->iterate<Flag>([&](auto &e) {
                renderer.drawFilledQuad(origin + e.position / ratio, {2, 4}, 100, 0, 0);
                return true;
            });

            auto camera = renderer.getCameraPosition();
            camera = camera / ratio;
            camera = camera + origin;

            Vector2 dimensions{renderer.width / ratio, renderer.height / ratio};

            if (camera.x < origin.x) {
                dimensions.x -= origin.x - camera.x;
                camera.x = origin.x;
            }

            if (camera.x + dimensions.x > origin.x + minimapSize) {
                dimensions.x = origin.x + minimapSize - camera.x;
            }

            if (camera.y < origin.y) {
                dimensions.y -= origin.y - camera.y;
                camera.y = origin.y;
            }

            if (camera.y + dimensions.y > origin.y + minimapSize) {
                dimensions.y = origin.y + minimapSize - camera.y;
            }

            renderer.drawQuad(camera, dimensions, 10, 10, 10);

            if (inputs.isMousePressed(1)) {
                if (mouse_position.x > origin.x && mouse_position.x < origin.x + minimapSize &&
                    mouse_position.y > origin.y && mouse_position.y < origin.y + minimapSize) {

                    auto dx = (mouse_position.x - origin.x) / minimapSize;
                    auto dy = (mouse_position.y - origin.y) / minimapSize;

                    auto & campos = Context::get().cameraEntity->position;

                    campos.x = level.width * dx - renderer.width * 0.5;
                    campos.y = level.height * dy - renderer.height * 0.5;

                    printf("%.2f, %.2f\n", campos.x, campos.y);
                }
            }
        }
    }
};
