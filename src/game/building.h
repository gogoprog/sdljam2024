#pragma once

#include "../context.h"
#include "factory.h"
#include "life.h"
#include "vehicle.h"

struct Structure : public Component {
    bool needCanon = false;
    Entity::Ptr canon;
};

struct TankFactory : public Component {
    float timeLeft{0.0f};
};

struct Generator : public Component {
    float timeLeft{0.0f};
};

class BuildingSystem : public System {
  public:
    StructureType typeToBuild;
    BuildingSystem() {
    }

    void onAdded() override {
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;
        auto &game = Context::get().game;
        String str;
        int price;
        switch (typeToBuild) {

            case StructureType::Turret: {
                str = "Turret";
                price = 500;
            } break;
            case StructureType::Generator: {
                str = "Generator";
                price = 300;
            } break;
            case StructureType::TankFactory: {
                str = "Starprt2";
                price = 2000;
            } break;
        }

        auto &atlas = renderer.getAtlas(str);
        auto world_position = Context::get().getMouseWorldPosition();

        auto tile_coords = level.getTileCoords(world_position);
        auto position = level.getTilePosition(tile_coords);

        if (level.canBuildAt(tile_coords) && game.stats.money >= price) {

            renderer.draw(position, atlas, 0);

            if (inputs.isMouseJustPressed(1)) {
                auto e = Factory::createStructure(typeToBuild);
                e->position = position;
                engine->addEntity(e);

                level.lock2x2(tile_coords);
                game.stats.money -= price;

                Context::get().audio.playSound("building");

                engine->changeState(Game::State::PLAYING);
            }

            auto pos = inputs.getMousePosition();
            pos.y += 32;
            renderer.drawText(pos, "build (" + std::to_string(price) + ")");
        } else {
            renderer.draw(position, str, 0, false, 220, 20, 20);
        }

        if (inputs.isKeyJustPressed(SDL_SCANCODE_ESCAPE) || inputs.isMouseJustPressed(3)) {
            Context::get().engine.changeState(Game::State::PLAYING);
        }
    }

  private:
    float timeLeft;
};

class StructureSystem : public System {
  public:
    StructureSystem() {
        require<Structure>();
        priority = 100;
    }

    void onEntityAdded(Entity &entity) override {
        auto &structure = entity.get<Structure>();

        if (structure.needCanon) {
            auto e = Factory::createTurret();
            e->add<Life>().team = entity.get<Life>().team;
            e->position = entity.position;
            engine->addEntity(e);
            structure.canon = e;
        }
    }

    void onEntityRemoved(Entity &entity) override {
        auto &structure = entity.get<Structure>();

        if (structure.canon != nullptr) {
            engine->removeEntity(*structure.canon);
        }

        auto &level = Context::get().level;
        auto tile_coords = level.getTileCoords(entity.position);
        level.unlock2x2(tile_coords);
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &renderer = Context::get().renderer;
        auto &life = entity.get<Life>();
        auto mwp = Context::get().getMouseWorldPosition();
        auto distance = Vector2::getSquareDistance(entity.position, mwp);

        if (distance < 32 * 32) {
            renderer.drawProgressBar(entity.position - Vector2{0, 45}, 80, life.hp / (float)life.maxHp, true);
        }
    }
};

class TankFactorySystem : public System {
    static inline float duration = 10.0f;

  public:
    TankFactorySystem() {
        require<Structure>();
        require<TankFactory>();
        priority = 100;
    }

    void onEntityAdded(Entity &entity) override {
        auto &tf = entity.get<TankFactory>();
        tf.timeLeft = 10.0f;
    }

    void onEntityRemoved(Entity &entity) override {
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &tf = entity.get<TankFactory>();
        auto &life = entity.get<Life>();
        auto &level = Context::get().level;

        auto &game = Context::get().game;

        if (game.stats.money > 500 || life.team == 1) {

            tf.timeLeft -= dt;

            if (tf.timeLeft <= 0.0f) {
                auto e = Factory::createVehicle(life.team);
                e->position = entity.position;
                e->position.y += 64;
                engine->addEntity(e);

                e->add<Target>().tileCoords = level.getTileCoords(entity.position) + Vector2{0, 1};

                tf.timeLeft += duration;

                if (life.team == 0) {
                    game.stats.money -= 500;
                }
            }
        }

        if (life.team == 0) {
            auto &renderer = Context::get().renderer;
            renderer.drawProgressBar(entity.position - Vector2{0, 60}, 80, 1.0f - tf.timeLeft / duration);
        }
    }
};

class GeneratorSystem : public System {
    static inline float duration = 5.0f;

  public:
    GeneratorSystem() {
        require<Structure>();
        require<Generator>();
        priority = 100;
    }

    void onEntityAdded(Entity &entity) override {
        auto &tf = entity.get<Generator>();
        tf.timeLeft = 5.0f;
    }

    void onEntityRemoved(Entity &entity) override {
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &tf = entity.get<Generator>();
        auto &life = entity.get<Life>();

        tf.timeLeft -= dt;

        if (tf.timeLeft <= 0.0f) {

            if (life.team == 0) {
                Context::get().game.stats.money += 100;
            }

            tf.timeLeft += duration;
        }

        if (life.team == 0) {
            auto &renderer = Context::get().renderer;
            renderer.drawProgressBar(entity.position - Vector2{0, 40}, 80, 1.0f - tf.timeLeft / duration);
        }
    }
};
