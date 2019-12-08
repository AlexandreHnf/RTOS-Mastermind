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

        if (!proposalRespectsKnowledge(solution, previousGuesses->at(i), previousEvaluations->at(i))) return false;
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
    nat* referenceCopy = new nat[this->positions];
    for (nat i = 0; i < this->positions; i++)
        referenceCopy[i] = reference[i];

    nat conservedColors = 0;

    for (nat i = 0; i < this->positions; i++) {
        for (nat j = 0; j < this->positions; j++) {
            if (proposal[i] == referenceCopy[j]) {
                referenceCopy[j] = this->positions;
                conservedColors++;
                break;
            }
        }
    }

    nat correctColors = referenceScore[0] + referenceScore[1];

    if (conservedColors < correctColors && correctColors == this->positions) return false;

    if (conservedColors <= correctColors && correctColors != this->positions) return false;

    if (conservedColors == this->positions && correctColors == 0) return false;

    return true;
}
