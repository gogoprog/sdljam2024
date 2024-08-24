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

    Stats stats;

    int turretCost{250};
};
