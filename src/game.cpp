#include "game.h"

#include "context.h"
#include "game/animation.h"
#include "game/building.h"
#include "game/camera.h"
#include "game/control.h"
#include "game/editor.h"
#include "game/factory.h"
#include "game/fx.h"
#include "game/hud.h"
#include "game/menu.h"
#include "game/playing.h"
#include "game/shake.h"
#include "game/sprite.h"
#include "game/turret.h"
#include "game/vehicle.h"

Game::Game() = default;
Game::~Game() = default;

void Game::init() {
    auto &engine = Context::get().engine;
    auto &level = Context::get().level;

    engine.enableSystem<TurretSystem>();
    engine.enableSystem<VehicleSystem>();
    engine.enableSystem<VehicleTargetSystem>();
    engine.enableSystem<WeaponSystem>();
    engine.enableSystem<StructureSystem>();
    engine.enableSystem<BulletSystem>();
    engine.enableSystem<LifeSystem>();
    engine.enableSystem<MoveSystem>();
    engine.enableSystem<MovableSystem>();
    engine.enableSystem<CameraSystem>();
    engine.enableSystem<ControlSystem>();
    engine.enableSystem<AnimationSystem>();
    engine.enableSystem<ShakeSystem>();
    engine.enableSystem<SpriteRotaterSystem>();
    engine.enableSystem<SpriteRendererSystem>();
    engine.enableSystem<SelectFxSystem>();

    {
        auto e = Factory::createCamera();
        engine.addEntity(e);
        Context::get().cameraEntity = e;
    }

    for (auto &coord : level.startCoords) {
        auto e = Factory::createFlag();
        e->position = level.getTileCenterPosition(coord);
        engine.addEntity(e);
    }

    if (0)
        for (int i = 0; i < 10; ++i) {
            Vector2 pos = {rand() % 2048, rand() % 2048};

            {
                auto e = Factory::createBase();
                e->position = pos;
                engine.addEntity(e);
            }

            {
                auto e = Factory::createTurret();
                e->position = pos;
                engine.addEntity(e);
            }
        }

    stats.lifes = 25;
    stats.money = 10000;

    {
        engine.setState<MenuSystem>(State::MENU);
        engine.setState<EditorSystem>(State::EDITOR);
        engine.setState<MissionSystem, PlayingSystem, HudSystem, VehicleSelectSystem, VehicleSelectedSystem>(
            State::PLAYING);
        engine.setState<MissionSystem, HudSystem, BuildingSystem>(State::BUILDING);
    }

    engine.changeState(State::MENU);
}

void Game::reset() {
    auto &level = Context::get().level;
    auto &engine = Context::get().engine;

    level.reset();

    engine.removeAllEntities();
    engine.removeAllSystems();

    init();
}
