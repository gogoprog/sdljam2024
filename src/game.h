#pragma once

#include "types.h"

struct Wave {
    int units;
    int speed;
    int hp;
};

struct Stats {
    int lifes;
    int money;
};

class Game {
  public:
    enum class State {
        INITIATING,
        MENU,
        EDITOR,
        PLAYING,
        WINNING,
        LOSING,
    };

    Game();
    ~Game();

    void init();
    void reset();
    void changeState(const State state);
    void nextWave();

    Stats stats;
    Wave currentWave;
    int waveCount{0};

    int turretCost{250};
};
