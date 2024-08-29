#pragma once

#include "types.h"
#include <algorithm>
#include <cassert>
#include <functional>

struct Component {};

class Engine;
class System;

template <typename T> bool contains(Vector<T> &vector, const T &value) {
    auto it = std::find(vector.begin(), vector.end(), value);
    return it != vector.end();
}

class Entity {
    friend class Engine;
    friend class System;

  public:
    using Ptr = SharedPtr<Entity>;

    String name;
    Vector2 position;
    float rotation = 0.0f;

    template <typename T, typename... Args> T &add(Args... args) {
        assert(!has<T>());

        components[typeid(T)] = std::make_unique<T>(args...);

        notifyAdd(typeid(T));

        return (T &)*components[typeid(T)];
    }

    template <typename T> T &get() {
        return (T &)*components[typeid(T)];
    }

    template <typename T> void remove() {
        components.erase(typeid(T));
    }

    template <typename T> bool has() {
        return components.find(typeid(T)) != components.end();
    }

    void notifyAdd(const TypeIndex component_name);
    void notifyRemove(const TypeIndex component_name);

  private:
    Map<TypeIndex, UniquePtr<Component>> components;
    Engine *engine{nullptr};
};

class System {
    friend class Engine;

  public:
    System() = default;
    ~System() = default;

    virtual void onAdded(){};
    virtual void onRemoved(){};

    virtual void update(const float dt);
    virtual void onEntityAdded(Entity &entity){};
    virtual void updateSingle(const float dt, Entity &entity){};

    template <typename T> void require() {
        requiredComponents.push_back(typeid(T));
    }

  protected:
    Vector<TypeIndex> requiredComponents;
    Engine *engine{nullptr};
    Vector<Entity::Ptr> entities;
    int priority{0};
};

class Engine {
    friend class System;

  public:
    template <typename T> void enableSystem(bool global = true) {
        System *value = nullptr;
        for (auto system : allSystems) {
            if (typeid(*system) == typeid(T)) {
                value = system;
            }
        }

        if (value == nullptr) {
            value = new T;
            allSystems.push_back(value);

            if (global) {
                globalSystems.push_back(typeid(*value));
            }
        }

        addSystem(value);
    }

    template <typename T> void disableSystem() {
        System *value = nullptr;
        for (auto system : allSystems) {
            if (typeid(*system) == typeid(T)) {
                value = system;
            }
        }

        if (value == nullptr) {
            value = new T;
            allSystems.push_back(value);
        }

        removeSystem(value);
    }

    void enableSystem(TypeIndex t) {
        printf("Enabling system: %i \n", t.hash_code());
        System *value = nullptr;
        for (auto system : allSystems) {
            if (typeid(*system) == t) {
                value = system;
            }
        }

        addSystem(value);
    }

    void disableSystem(TypeIndex t) {
        printf("Disabling system: %i \n", t.hash_code());
        System *value = nullptr;
        for (auto system : allSystems) {
            if (typeid(*system) == t) {
                value = system;
            }
        }

        removeSystem(value);
    }

    void addEntity(SharedPtr<Entity> entity) {
        entities.push_back(entity);
        entity->engine = this;

        for (auto &kv : entity->components) {
            notifyAdd(*entity, kv.first);
        }
    }

    void removeEntity(Entity &entity) {
        auto end_it = std::remove_if(entities.begin(), entities.end(), [&](auto &se) { return &*se == &entity; });
        entities.resize(end_it - entities.begin());
    }

    void update(const float dt) {
        auto systems_copy = systems;
        for (auto &system : systems_copy) {
            system->update(dt);
        }
    }

    void notifyAdd(Entity &entity, const TypeIndex component_name) {
        for (auto &system : systems) {
            if (std::find(system->requiredComponents.begin(), system->requiredComponents.end(), component_name) !=
                system->requiredComponents.end()) {

                if (std::all_of(system->requiredComponents.begin(), system->requiredComponents.end(), [&](auto name) {
                        auto it = std::find_if(entity.components.begin(), entity.components.end(),
                                               [&](auto &kv) { return kv.first == name; });
                        return it != entity.components.end();
                    })) {
                    system->onEntityAdded(entity);
                }
            }
        }
    }

    template <typename T> void iterate(std::function<bool(Entity &entity)> func) {
        auto entities_copy = entities;
        for (auto &entityptr : entities_copy) {
            auto &entity = *entityptr;

            if (entity.has<T>()) {
                if (!func(entity)) {
                    break;
                }
            }
        }
    }

    void removeAllEntities() {
        entities.clear();
    }

    void removeAllSystems() {
        systems.clear();
    }

    template <typename... T> void setState(int state) {
        (
            [&] {
                System *value = nullptr;

                for (auto system : allSystems) {
                    if (typeid(*system) == typeid(T)) {
                        value = system;
                    }
                }

                if (value == nullptr) {
                    value = new T;
                    allSystems.push_back(value);
                }

                stateSystems[state].push_back(typeid(T));
            }(),
            ...);
    }

    void changeState(int state) {
        auto &stateSystems = this->stateSystems[state];

        for (auto system : systems) {
            auto ti = TypeIndex(typeid(*system));
            if (!contains(stateSystems, ti) && !contains(globalSystems, ti)) {
                disableSystem(ti);
            }
        }

        for (auto system_ti : stateSystems) {
            enableSystem(system_ti);
        }
    }

  private:
    void addSystem(System *system) {
        auto it = std::find(systems.begin(), systems.end(), system);
        if (it == systems.end()) {
            systems.push_back(system);

            std::sort(systems.begin(), systems.end(), [&](auto a, auto b) { return a->priority < b->priority; });
            system->engine = this;
            system->onAdded();
        }
    }

    void removeSystem(System *system) {
        auto it = std::find(systems.begin(), systems.end(), system);
        if (it != systems.end()) {
            systems.erase(std::find(systems.begin(), systems.end(), system));
            system->onRemoved();
        }
    }

    Vector<System *> systems;
    Vector<System *> allSystems;
    Vector<Entity::Ptr> entities;
    Vector<TypeIndex> globalSystems;
    Map<int, Vector<TypeIndex>> stateSystems;
};

inline void Entity::notifyAdd(const TypeIndex component_name) {
    if (engine) {
        engine->notifyAdd(*this, component_name);
    }
}

inline void System::update(const float dt) {
    auto &all_entities = engine->entities;
    entities.resize(0);
    for (auto &entityptr : all_entities) {
        auto &entity = *entityptr;
        if (std::all_of(requiredComponents.begin(), requiredComponents.end(), [&](auto name) {
                auto it = std::find_if(entity.components.begin(), entity.components.end(),
                                       [&](auto &kv) { return kv.first == name; });
                return it != entity.components.end();
            })) {

            entities.push_back(entityptr);
        }
    }

    for (auto &entity : entities) {
        updateSingle(dt, *entity);
    }
}
