#include "factory.h"

#include "animation.h"
#include "building.h"
#include "bullet.h"
#include "button.h"
#include "camera.h"
#include "control.h"
#include "fx.h"
#include "hittable.h"
#include "spawn.h"
#include "sprite.h"
#include "turret.h"
#include "vehicle.h"
#include "weapon.h"

const Array<int, 8> turret_frames = {2, 3, 5, 6, 7, 8, 9, 4};
const Array<int, 8> bullet_frames = {1, 2, 14, 26, 25, 24, 12, 0};
const Array<int, 8> tank_frames = {1, 2, 5, 8, 7, 6, 3, 0};

SharedPtr<Entity> Factory::createCamera() {
    auto e = std::make_shared<Entity>();
    e->add<Camera>();
    e->add<Control>();
    e->position = {0, 0};

    return e;
}

SharedPtr<Entity> Factory::createBase() {
    auto e = std::make_shared<Entity>();
    e->add<Sprite>();
    e->get<Sprite>().atlasName = "Turret";
    e->get<Sprite>().frameIndex = 0;

    return e;
}

SharedPtr<Entity> Factory::createTurret() {
    auto e = std::make_shared<Entity>();
    e->add<Turret>();
    e->add<Sprite>();
    e->get<Sprite>().atlasName = "Turret";
    e->get<Sprite>().frameIndex = 2;
    e->get<Sprite>().layer = 2;
    e->add<RotatableSprite>();
    /* e->get<RotatableSprite>().frames = std::span(turret_frames); */
    e->get<RotatableSprite>().frames = Vector<int>{turret_frames.begin(), turret_frames.end()};

    return e;
}

SharedPtr<Entity> Factory::createBullet(const Entity &source, float range, float damage) {
    auto e = std::make_shared<Entity>();
    e->add<Bullet>();
    e->add<Sprite>();
    e->get<Sprite>().atlasName = "Bullets";
    e->get<Sprite>().frameIndex = 0;
    e->get<Sprite>().layer = 4;
    e->add<RotatableSprite>();
    /* e->get<RotatableSprite>().frames = std::span(bullet_frames); */
    e->get<RotatableSprite>().frames = Vector<int>{bullet_frames.begin(), bullet_frames.end()};

    auto &entity = source;
    auto speed = 500;
    auto angle = (entity.rotation - 90) * std::numbers::pi / 180.0f;
    auto velocity = Vector2(std::cos(angle) * speed, std::sin(angle) * speed);
    auto direction = velocity.getNormalized();
    e->get<Bullet>().velocity = velocity;
    e->get<Bullet>().lifetimeLeft = range / speed;
    e->get<Bullet>().damage = damage;
    e->rotation = entity.rotation;
    e->position = entity.position + direction * 32;

    return e;
}

SharedPtr<Entity> Factory::createVehicle(const int team) {
    auto e = std::make_shared<Entity>();
    e->add<Vehicle>();
    e->add<Sprite>();
    e->get<Sprite>().atlasName = "Tank11";
    e->get<Sprite>().frameIndex = 0;
    e->get<Sprite>().layer = 2;
    e->add<RotatableSprite>();
    /* e->get<RotatableSprite>().frames = std::span(tank_frames); */
    e->get<RotatableSprite>().frames = Vector<int>{tank_frames.begin(), tank_frames.end()};
    e->add<Movable>();
    e->add<Hittable>();
    e->get<Hittable>().radius = 20;
    e->add<Life>().team = team;

    if (team == 0) {
        e->add<Selectable>();
    }

    e->add<Weapon>();

    return e;
}

SharedPtr<Entity> Factory::createSpawn() {
    auto e = std::make_shared<Entity>();
    e->add<Spawn>();

    return e;
}

SharedPtr<Entity> Factory::createSmallExplosion() {
    auto e = std::make_shared<Entity>();
    e->add<Sprite>();
    e->get<Sprite>().atlasName = "expSmall";
    e->get<Sprite>().frameIndex = 0;
    e->get<Sprite>().layer = 4;
    e->add<Animation>();
    e->get<Animation>().frameRate = 15;
    e->get<Animation>().autoRemove = true;
    return e;
}

SharedPtr<Entity> Factory::createExplosion() {
    auto e = std::make_shared<Entity>();
    e->add<Sprite>();
    e->get<Sprite>().atlasName = "exploBig";
    e->get<Sprite>().frameIndex = 0;
    e->get<Sprite>().layer = 4;
    e->add<Animation>();
    e->get<Animation>().frameRate = 15;
    e->get<Animation>().autoRemove = true;
    return e;
}

SharedPtr<Entity> Factory::createFlag() {
    auto e = std::make_shared<Entity>();
    e->add<Sprite>();
    e->get<Sprite>().atlasName = "Flag";
    e->get<Sprite>().frameIndex = 0;
    e->get<Sprite>().layer = 4;
    e->add<Animation>();
    e->get<Animation>().frameRate = 5;
    e->get<Animation>().autoRemove = false;
    e->get<Animation>().loop = true;
    e->add<Flag>();
    return e;
}

SharedPtr<Entity> Factory::createSelectFx() {
    auto e = std::make_shared<Entity>();
    e->add<SelectFx>();
    return e;
}

Entity::Ptr Factory::createStructure(const StructureType type) {
    auto e = std::make_shared<Entity>();

    e->add<Structure>();
    e->add<Life>();
    e->get<Life>().hp = 1000;
    e->add<Hittable>();
    e->get<Hittable>().radius = 56;
    e->add<Sprite>();
    e->get<Sprite>().layer = 1;

    switch (type) {
        case StructureType::Turret: {
            e->get<Sprite>().atlasName = "Turret";
            e->get<Sprite>().frameIndex = 0;
            e->get<Structure>().needCanon = true;
        } break;
        case StructureType::Generator: {
            e->get<Sprite>().atlasName = "Generator";
            e->get<Sprite>().frameIndex = 0;
            e->add<Animation>();
            e->get<Animation>().frameRate = 15;
            e->get<Animation>().loop = true;
            e->add<Generator>();
        } break;
        case StructureType::TankFactory: {
            e->get<Sprite>().atlasName = "Starprt2";
            e->get<Sprite>().frameIndex = 0;
            e->add<Animation>();
            e->get<Animation>().frameRate = 15;
            e->get<Animation>().loop = true;
            e->add<TankFactory>();
        } break;
    }

    return e;
}

Entity::Ptr Factory::createButton(const String text, std::function<void()> callback) {
    auto e = std::make_shared<Entity>();
    e->add<Button>();
    e->get<Button>().text = text;
    e->get<Button>().onClick = callback;
    return e;
}
