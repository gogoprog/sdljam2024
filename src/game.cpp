#include "game.h"

#include "context.h"
#include "game/animation.h"
#include "game/camera.h"
#include "game/control.h"
#include "game/factory.h"
#include "game/shake.h"
#include "game/spawn.h"
#include "game/sprite.h"
#include "game/state.h"
#include "game/turret.h"
#include "game/ui.h"
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
    engine.enableSystem<UiSystem>();

    {
        auto e = Factory::createCamera();
        engine.addEntity(e);
        Context::get().cameraEntity = e;
    }
    {
        auto e = Factory::createSpawn();
        e->position = level.getTileCenterPosition(level.beginCoords);
        engine.addEntity(e);
    }
    {
        auto e = Factory::createFlag();
        e->position = level.getTileCenterPosition(level.endCoords);
        e->position.x -= 108;
        e->position.y += 40;
        engine.addEntity(e);
    }
    {
        auto e = Factory::createFlag();
        e->position = level.getTileCenterPosition(level.endCoords);
        e->position.x += 98;
        e->position.y += 40;
        engine.addEntity(e);
    }
    {
        auto e = Factory::createFlag();
        e->position = level.getTileCenterPosition(level.beginCoords);
        e->position.x += 98;
        e->position.y -= 40;
        engine.addEntity(e);
    }
    {
        auto e = Factory::createFlag();
        e->position = level.getTileCenterPosition(level.beginCoords);
        e->position.x -= 108;
        e->position.y -= 40;
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
    stats.money = 1000;
    nextWave();
    changeState(State::INITIATING);
}

void Game::reset() {
    auto &level = Context::get().level;
    auto &engine = Context::get().engine;
    level = Level();

    engine.removeAllEntities();
    engine.removeAllSystems();

    waveCount = 0;

    init();
}

void Game::changeState(const State state) {
    auto &engine = Context::get().engine;

    Context::get().audio.playSound("step");

    switch (state) {
        case State::INITIATING: {
            changeState(State::BUILDING_ROADS);
        } break;

        case State::BUILDING_ROADS: {
            engine.disableSystem<WinningStateSystem>();
            engine.disableSystem<BuildingTurretsStateSystem>();
            engine.enableSystem<RoadBuildingStateSystem>();
        } break;

        case State::BUILDING_TURRETS: {
            engine.disableSystem<RoadBuildingStateSystem>();
            engine.enableSystem<BuildingTurretsStateSystem>();
        } break;

        case State::PLAYING: {
            engine.disableSystem<RoadBuildingStateSystem>();
            engine.disableSystem<BuildingTurretsStateSystem>();
            engine.enableSystem<SpawnSystem>();
            engine.enableSystem<FiringStateSystem>();
        } break;

        case State::WINNING: {
            engine.disableSystem<SpawnSystem>();
            engine.disableSystem<FiringStateSystem>();
            engine.enableSystem<WinningStateSystem>();
        } break;

        case State::LOSING: {
            engine.disableSystem<SpawnSystem>();
            engine.disableSystem<FiringStateSystem>();
            engine.enableSystem<LosingStateSystem>();
        } break;
    }
}

void Game::nextWave() {
    currentWave.units = (waveCount + 1) * 2;
    currentWave.speed = 100 + (waveCount % 10) * 10 + waveCount;
    currentWave.hp = 100 + waveCount * 50;

    waveCount++;
}
