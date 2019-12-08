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

bool Player::plausibleSolution(const vector<nat*> *previousGuesses, const vector<nat*> *previousEvaluations, nat *solution) {
    for (nat i = 0; i < previousGuesses->size(); i++) {
        if (!proposalIsDifferent(solution, previousGuesses->at(i))) return false;

    }

    return true;
}

bool Player::proposalIsDifferent(const nat *proposal, const nat *reference) {
    for (nat i = 0; i < this->positions; i++)
        if (proposal[i] != reference[i])
            return true;

    return false;
}

bool Player::proposalRespectsKnowledge(const nat* proposal, const nat* reference, const nat* referenceScore) {
    return false;
}
