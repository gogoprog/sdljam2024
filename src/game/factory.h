#pragma once

#include "../ecs.h"
#include "../types.h"

enum class StructureType { Turret, Generator, TankFactory };

class Factory {
  public:
    static Entity::Ptr createCamera();
    static Entity::Ptr createBase();
    static Entity::Ptr createTurret();
    static Entity::Ptr createBullet(const Entity &source, float range, float damage);
    static Entity::Ptr createVehicle(const int team);
    static Entity::Ptr createSpawn();
    static Entity::Ptr createSmallExplosion();
    static Entity::Ptr createExplosion();
    static Entity::Ptr createFlag();
    static Entity::Ptr createSelectFx();
    static Entity::Ptr createStructure(const StructureType type);
    static Entity::Ptr createButton(const String text, std::function<void()> callback);
};
