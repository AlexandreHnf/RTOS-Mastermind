//
// Created by Madalin Ionescu on 2019-12-07.
//

#ifndef RTOS_MASTERMIND_GAME_H
#define RTOS_MASTERMIND_GAME_H

#include <limits.h>
#include <vector>

#include "GameMaster.h"
#include "Player.h"

using namespace std;

class Game {
private :
    unsigned positions;
    GameMaster gameMaster;
    Player player;
    unsigned rounds;
    vector<unsigned*> previousGuesses;

public:
    Game() = default;
    Game(unsigned colors, unsigned positions);
    Game(unsigned colors, unsigned positions, unsigned rounds);

    bool isFinished();
    void playRound();
};


#endif //RTOS_MASTERMIND_GAME_H
