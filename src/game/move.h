#pragma once

#include <box2d/box2d.h>

#include "../context.h"
#include "bullet.h"
#include "factory.h"
#include "sprite.h"

struct Movable : public Component {
    float radius = 16.0f;
    b2Body *body;
};

class MovableSystem : public System {
  public:
    MovableSystem() {
        require<Movable>();
    }

    void onAdded() override {
        auto &level = Context::get().level;

        for (int y = 0; y < level.tileheight; ++y) {
            for (int x = 0; x < level.tilewidth; ++x) {
                Vector2 coords{x, y};

                if (!level.getRoad(coords) && level.isNextToRoad(coords)) {
                    auto pos = level.getTileCenterPosition(coords);
                    b2BodyDef bodydef;
                    bodydef.position.Set(pos.x, pos.y);
                    bodydef.type = b2_staticBody;
                    bodydef.fixedRotation = true;
                    bodydef.angle = std::numbers::pi / 4.0f;
                    b2Body *body = world.CreateBody(&bodydef);

                    b2PolygonShape shape;

                    auto size = Level::tileSpacing * 0.25f;
                    shape.SetAsBox(size, size);

                    b2FixtureDef fixturedef;
                    fixturedef.shape = &shape;
                    fixturedef.density = 1.0;
                    body->CreateFixture(&fixturedef);

                    /* body->SetTransform(body->GetPosition(), std::numbers::pi / 4.0f); */
                }
            }
        }
    }

    void onRemoved() override {
        /* ClosePhysics(); */
    }

    void onEntityAdded(Entity &entity) override {
        auto &movable = entity.get<Movable>();
        auto r = movable.radius;
        auto &pos = entity.position;

        b2BodyDef bodydef;
        bodydef.position.Set(pos.x, pos.y);
        bodydef.type = b2_dynamicBody;
        bodydef.linearDamping = 10.0f;
        bodydef.fixedRotation = true;
        b2Body *body = world.CreateBody(&bodydef);

        b2CircleShape shape;
        shape.m_radius = r;

        b2FixtureDef fixturedef;
        fixturedef.shape = &shape;
        fixturedef.density = 1.0;
        body->CreateFixture(&fixturedef);

        /* body->SetLinearVelocity({100, 0}); */

        movable.body = body;
    }

    void update(float dt) override {
        System::update(dt);

        time += dt;

        const float step = 1.0f / 60;

        while (time >= step) {
            /* RunPhysicsStep(); */
            world.Step(step, 8, 3);
            time -= step;
        }
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &movable = entity.get<Movable>();
        auto p = movable.body->GetPosition();
        entity.position = {p.x, p.y};
    }

  private:
    b2World world{b2Vec2{0, 0}};
    float time = 0.0f;
};

struct Move : public Component {
    Move(Vector2 f, Vector2 t, float s) : from(f), to(t), speed(s) {
    }

    Vector2 from;
    Vector2 to;
    float speed;
    float time{0};
    float duration;
};

class MoveSystem : public System {
  public:
    MoveSystem() {
        require<Move>();
        require<Movable>();
        require<RotatableSprite>();
    }

    void onEntityAdded(Entity &entity) override {
        auto &rotatable = entity.get<RotatableSprite>();
        auto &move = entity.get<Move>();
        auto &movable = entity.get<Movable>();

        auto delta = move.to - move.from;
        entity.rotation = (std::atan2(delta.y, delta.x) * 180.0f / std::numbers::pi) + 90;

        move.duration = delta.getLength() / move.speed;

        auto v = delta * move.speed;

        movable.body->SetLinearDamping(0.0f);
        movable.body->SetLinearVelocity({v.x, v.y});
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &move = entity.get<Move>();
        auto &movable = entity.get<Movable>();

        if (move.time < move.duration) {
            move.time += dt;
            auto progress = move.time / move.duration;
            /* entity.position = move.from + (move.to - move.from) * progress; */
            auto v = movable.body->GetLinearVelocity();
            entity.rotation = (std::atan2(v.y, v.x) * 180.0f / std::numbers::pi) + 90;

            if (progress >= 1) {

                movable.body->SetLinearVelocity({0, 0});
                movable.body->SetLinearDamping(10.0f);
                entity.remove<Move>();
            }
        }
    }
};
