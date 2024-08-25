#include "game.h"

#include "context.h"
#include "game/animation.h"
#include "game/camera.h"
#include "game/control.h"
#include "game/editor.h"
#include "game/factory.h"
#include "game/hud.h"
#include "game/menu.h"
#include "game/playing.h"
#include "game/shake.h"
#include "game/spawn.h"
#include "game/sprite.h"
#include "game/state.h"
#include "game/turret.h"
#include "game/vehicle.h"

Game::Game() = default;
Game::~Game() = default;

void Game::init() {
    auto &engine = Context::get().engine;
    auto &level = Context::get().level;

    engine.enableSystem<TurretSystem>();
    engine.enableSystem<VehicleSystem>();
    engine.enableSystem<BulletSystem>();
    engine.enableSystem<LifeSystem>();
    engine.enableSystem<MoveSystem>();
    engine.enableSystem<CameraSystem>();
    engine.enableSystem<ControlSystem>();
    engine.enableSystem<AnimationSystem>();
    engine.enableSystem<ShakeSystem>();
    engine.enableSystem<SpriteRotaterSystem>();
    engine.enableSystem<SpriteRendererSystem>();

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

    if (1)
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
    stats.money = 1000;
    changeState(State::INITIATING);
}

void Game::reset() {
    auto &level = Context::get().level;
    auto &engine = Context::get().engine;

    level.reset();

    engine.removeAllEntities();
    engine.removeAllSystems();

    init();
}

void Game::changeState(const State state) {
    auto &engine = Context::get().engine;

    Context::get().audio.playSound("step");

    switch (state) {
        case State::INITIATING: {
            changeState(State::MENU);
        } break;

        case State::MENU: {
            engine.enableSystem<MenuSystem>();
            engine.disableSystem<EditorSystem>();
            engine.disableSystem<HudSystem>();
            engine.disableSystem<PlayingSystem>();
            engine.disableSystem<VehicleSelectSystem>();
            engine.disableSystem<VehicleSelectedSystem>();
        } break;

        case State::PLAYING: {
            engine.disableSystem<MenuSystem>();
            engine.disableSystem<EditorSystem>();
            engine.enableSystem<HudSystem>();
            engine.enableSystem<PlayingSystem>();
            engine.enableSystem<VehicleSelectSystem>();
            engine.enableSystem<VehicleSelectedSystem>();
        } break;

        case State::EDITOR: {
            engine.disableSystem<MenuSystem>();
            engine.enableSystem<EditorSystem>();
        } break;
    }
}
