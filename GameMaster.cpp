//
// Created by Madalin Ionescu on 2019-12-07.
//

#include "GameMaster.h"

GameMaster::GameMaster(nat colors, nat positions) {
    this->colors = colors;
    this->positions = positions;
    this->solution = new nat[positions];

    srandom((int) time(0));

    for (nat i = 0; i < this->positions; i++)
        solution[i] = random() % this->colors;
}

nat* GameMaster::checkProposedSol(nat* proposedSolution) {
    nat* res = new nat[2]; // 0 -> perfect; 1 -> correct color
    res[0] = 0; res[1] = 0;
    nat* solution = new nat[this->positions];

    for (nat i = 0; i < this->positions; i++)
        if (proposedSolution[i] == this->solution[i]) {
            res[0]++;
            proposedSolution[i] = this->colors;
            solution[i] = this->colors;
        } else solution[i] = this->solution[i];

    for (nat i = 0; i < this->positions; i++)
        if (proposedSolution[i] != this->colors)
            for (nat j = 0; j < this->positions; j++)
                if (proposedSolution[i] == solution[j]) {
                    res[1]++;
                    solution[j] = this->colors;
                    break;
                }

    return res;
}

void GameMaster::printSolution() {
    cout << "The solution is: ";

    for (nat i = 0; i < this->positions; i++)
        cout << this->solution[i] << " ";

    cout << endl;
}

nat* GameMaster::getSolution() const {
    return solution;
}
