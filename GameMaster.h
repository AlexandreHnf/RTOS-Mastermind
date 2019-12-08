//
// Created by Madalin Ionescu on 2019-12-07.
//

#ifndef RTOS_MASTERMIND_GAMEMASTER_H
#define RTOS_MASTERMIND_GAMEMASTER_H

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using nat = unsigned; // shorter name for natural numbers


class GameMaster {

private:
    nat colors;
    nat positions;
    nat* solution;

public:
    GameMaster() = default;
    GameMaster(nat colors, nat positions);

    // returns an array of length 2: 0 -> # perfect colors, 1 -> # correct colors
    nat* checkProposedSol(nat* proposedSolution);

    void printSolution();

    nat* getSolution() const;
};


#endif //RTOS_MASTERMIND_GAMEMASTER_H
