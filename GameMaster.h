//
// Created by Madalin Ionescu on 2019-12-07.
//

#ifndef RTOS_MASTERMIND_GAMEMASTER_H
#define RTOS_MASTERMIND_GAMEMASTER_H

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;
using vg = vector<vector<unsigned>>;
using sg = vector<unsigned>;


class GameMaster {

private:
    unsigned colors;
    unsigned positions;
    unsigned* solution;

    void cartProduct(sg* v, vg* vv, int position, int length, int colors);

public:
    GameMaster() = default;
    GameMaster(unsigned colors, unsigned positions);

    // returns an array of length 2: 0 -> # perfect colors, 1 -> # correct colors
    sg checkProposedSol(sg* proposedSolution);

	// bool victory(std::vector<unsigned>);

    void printSolution();

    unsigned* getSolution() const;

    vg generateFirstPositions(unsigned short posNb);
};


#endif //RTOS_MASTERMIND_GAMEMASTER_H
