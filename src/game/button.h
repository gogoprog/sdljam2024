#pragma once

#include "../context.h"
#include "shake.h"

struct Button : public Component {
    bool background = true;
    String text = "unnamed";
    std::function<void()> onClick;
    float scale = 1.0f;
    bool alignCenter = false;
    bool alignRight = false;
    float right = 0.0f;
};

class ButtonSystem : public System {
  public:
    ButtonSystem() {
        require<Button>();
        priority = 999;
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &pos = entity.position;
        auto &renderer = Context::get().renderer;
        auto &inputs = Context::get().inputs;
        auto &button = entity.get<Button>();
        auto &mpos = inputs.getMousePosition();
        SDL_Rect rect;

        renderer.computeTextRect(rect, pos, button.text, button.scale);

        if (button.alignRight) {
            pos.x = renderer.width - button.right;
        }

        if (button.alignCenter) {
            pos.x = renderer.width / 2.0f - rect.w / 2.0f;
        }

        renderer.drawText(pos, button.text, button.scale);

        if (mpos.x >= rect.x && mpos.x <= rect.x + rect.w && mpos.y >= rect.y && mpos.y <= rect.y + rect.h) {
            renderer.drawQuad({rect.x, rect.y}, {rect.w, rect.h}, 255, 255, 255);

            if (inputs.isMouseJustPressed(1)) {
                button.onClick();
            }
        }
    }
};
