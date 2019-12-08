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
using nat = unsigned;

class Game {
private :
    nat positions;
    GameMaster gameMaster;
    Player player;
    nat rounds;
    vector<nat*> previousGuesses;
    vector<nat*> previousEvaluations;

public:
    Game() = default;
    Game(nat colors, nat positions);
    Game(nat colors, nat positions, nat rounds);

    bool isFinished();
    void playRound();
};


#endif //RTOS_MASTERMIND_GAME_H
