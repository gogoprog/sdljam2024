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
    enum State {
        INITIATING,
        MENU,
        PAUSE,
        EDITOR,
        PLAYING,
        BUILDING,
        WINNING,
        LOSING,
    };

    Game();
    ~Game();

    void init();
    void reset();

    Stats stats;
};
