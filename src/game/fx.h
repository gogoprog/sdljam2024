#pragma once

#include "../context.h"

struct SelectFx : public Component {
    float time{0.0f};
    float duration{1.0f};
};

class SelectFxSystem : public System {
  public:
    SelectFxSystem() {
        require<SelectFx>();
    }

    void onEntityAdded(Entity &entity) override {
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &renderer = Context::get().renderer;
        auto &pos = entity.position;
        auto &fx = entity.get<SelectFx>();

        fx.time += dt;

        float a = 196;
        float b = 2;
        float f = std::min(fx.time / fx.duration, 1.0f);

        auto ff = std::sin((f * std::numbers::pi) / 2.0f);
        float size = a + (b - a) * ff;

        renderer.drawQuad(pos - Vector2{size, size} / 2, {size, size}, 55, 200, 55, true);

        if (f >= 1.0f) {
            engine->removeEntity(entity);
        }
    }
};
