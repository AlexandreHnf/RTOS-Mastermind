//
// Created by Madalin Ionescu on 2019-12-07.
//

#include "GameMaster.h"

GameMaster::GameMaster(unsigned colors, unsigned positions) {
    this->colors = colors;
    this->positions = positions;
    this->solution = new unsigned[positions];

    srandom((int) time(0));

    for (unsigned i = 0; i < this->positions; i++)
        solution[i] = random() % this->colors;
}

unsigned* GameMaster::checkProposedSol(unsigned* proposedSolution) {
    unsigned* res = new unsigned[2]; // 0 -> perfect; 1 -> correct color
    res[0] = 0; res[1] = 0;
    unsigned* solution = new unsigned[this->positions];

    for (unsigned i = 0; i < this->positions; i++)
        if (proposedSolution[i] == this->solution[i]) {
            res[0]++;
            proposedSolution[i] = this->colors;
            solution[i] = this->colors;
        } else solution[i] = this->solution[i];

    for (unsigned i = 0; i < this->positions; i++)
        if (proposedSolution[i] != this->colors)
            for (unsigned j = 0; j < this->positions; j++)
                if (proposedSolution[i] == solution[j]) {
                    res[1]++;
                    solution[j] = this->colors;
                    break;
                }

    return res;
}

void GameMaster::printSolution() {
    cout << "The solution is: ";

    for (unsigned i = 0; i < this->positions; i++)
        cout << this->solution[i] << " ";

    cout << endl;
}

unsigned* GameMaster::getSolution() const {
    return solution;
}
