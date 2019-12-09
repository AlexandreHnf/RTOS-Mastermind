//
// Created by Madalin Ionescu on 2019-12-07.
//

#ifndef RTOS_MASTERMIND_GAMEMASTER_H
#define RTOS_MASTERMIND_GAMEMASTER_H

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
// #define unsigned unsigned int // shorter name for unsignedural numbers


class GameMaster {

private:
    unsigned colors;
    unsigned positions;
    unsigned* solution;

public:
    GameMaster() = default;
    GameMaster(unsigned colors, unsigned positions);

    // returns an array of length 2: 0 -> # perfect colors, 1 -> # correct colors
    unsigned* checkProposedSol(unsigned* proposedSolution);

	// bool victory(std::vector<unsigned>);

    void printSolution();

    unsigned* getSolution() const;
};


#endif //RTOS_MASTERMIND_GAMEMASTER_H
