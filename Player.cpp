//
// Created by Madalin Ionescu on 2019-12-07.
//

#include "Player.h"

Player::Player(nat colors, nat positions) {
    this->colors = colors;
    this->positions = positions;
}

vector<nat *> Player::analyzeContext(vector<nat *> previousPropositions) {
    return vector<nat *>();
}

nat *Player::pickSolution() {
    return nullptr;
}

nat *Player::pickFirstSolution() {
    nat* solution = new nat[positions];

    for (nat i = 0; i < positions; i++)
        solution[i] = random() % this->positions;

    return solution;
}
