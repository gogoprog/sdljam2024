#include "game.h"

#include "context.h"
#include "game/animation.h"
#include "game/building.h"
#include "game/bullet.h"
#include "game/button.h"
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
    engine.enableSystem<TankFactorySystem>();
    engine.enableSystem<GeneratorSystem>();
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
    engine.enableSystem<ButtonSystem>();

    auto e = Factory::createCamera();
    engine.addEntity(e);
    Context::get().cameraEntity = e;

    engine.setState<MenuSystem>(State::MENU);
    engine.setState<PauseSystem, MissionSystem>(State::PAUSE);
    engine.setState<EditorSystem>(State::EDITOR);
    engine.setState<MissionSystem, PlayingSystem, HudSystem, VehicleSelectSystem, VehicleSelectedSystem>(
        State::PLAYING);
    engine.setState<MissionSystem, HudSystem, BuildingSystem>(State::BUILDING);

    engine.changeState(State::MENU);

    Context::get().audio.playMusic();
}

void Game::reset() {
    auto &level = Context::get().level;
    auto &engine = Context::get().engine;

    level.reset();

    engine.removeAllEntities();
    engine.removeAllSystems();

    init();
}
