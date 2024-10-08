#pragma once

#include "../context.h"

struct Sprite : public Component {
    String textureName;
    String atlasName;
    int frameIndex{0};
    int layer = 0;
    uint8_t r = 255, g = 255, b = 255;
};

struct RotatableSprite : public Component {
    Vector<int> frames;
    int frameOffset{0};
};

class SpriteRendererSystem : public System {
  public:
    SpriteRendererSystem() {
        require<Sprite>();
    }

    void update(const float dt) override {
        auto &renderer = Context::get().renderer;
        for (auto &items : itemsPerLayer) {
            items.resize(0);
        }

        System::update(dt);

        for (auto &items : itemsPerLayer) {
            for (auto &item : items) {

                auto spr = item.second;

                if (!spr->textureName.empty()) {
                    renderer.draw(item.first, spr->textureName, true);
                } else {
                    renderer.draw(item.first, spr->atlasName, spr->frameIndex, true, spr->r, spr->g, spr->b);
                }
            }
        }
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &sprite = entity.get<Sprite>();
        itemsPerLayer[sprite.layer].emplace_back(entity.position, &sprite);
    }

  private:
    using Item = std::pair<Vector2, Sprite *>;

    Array<Vector<Item>, 5> itemsPerLayer;
};

class SpriteRotaterSystem : public System {
  public:
    SpriteRotaterSystem() {
        require<Sprite>();
        require<RotatableSprite>();
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &rotatable = entity.get<RotatableSprite>();
        auto &sprite = entity.get<Sprite>();

        auto &frames = rotatable.frames;
        auto frame = frames[0];
        entity.rotation = clampAngle(entity.rotation );
        auto step = 45 / 2.0f;
        auto angle = entity.rotation;

        if (angle < step) {
            frame = frames[0];
        } else if (angle < step * 3) {
            frame = frames[1];
        } else if (angle < step * 5) {
            frame = frames[2];
        } else if (angle < step * 7) {
            frame = frames[3];
        } else if (angle < step * 9) {
            frame = frames[4];
        } else if (angle < step * 11) {
            frame = frames[5];
        } else if (angle < step * 13) {
            frame = frames[6];
        } else if (angle < step * 15) {
            frame = frames[7];
        }

        sprite.frameIndex = rotatable.frameOffset + frame;
    }
};
