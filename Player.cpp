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

bool Player::plausibleSolution(const vector<nat*> *previousGuesses, const vector<nat*> *previousEvaluations, const nat *solution) const {
    for (nat i = 0; i < previousGuesses->size(); i++) {
        if (!proposalIsDifferent(solution, previousGuesses->at(i))) return false;

        if (!proposalRespectsKnowledge(solution, previousGuesses->at(i), previousEvaluations->at(i))) return false;
    }

    return true;
}

bool Player::proposalIsDifferent(const nat *proposal, const nat *reference) const {
    for (nat i = 0; i < this->positions; i++)
        if (proposal[i] != reference[i])
            return true;

    return false;
}

bool Player::proposalRespectsKnowledge(const nat* proposal, const nat* reference, const nat* referenceScore) const {
    bool* referenceNotTaken = new bool[this->positions];
    for (nat i = 0; i < this->positions; i++)
        referenceNotTaken[i] = true;

    nat conservedColors = 0;

    for (nat i = 0; i < this->positions; i++) {
        for (nat j = 0; j < this->positions; j++) {
            if (proposal[i] == reference[j] && referenceNotTaken[j]) {
                referenceNotTaken[j] = false;
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

nat *Player::generatePlausibleSolution(nat fixedPositions, const nat *posList, const vector<nat*> *previousGuesses, const vector<nat*> *previousEvaluations) {
    nat* proposal = new nat[this->positions];

    for (nat i = 0; i < fixedPositions; i++)
        proposal[i] = posList[i];

    generatePosition(proposal, fixedPositions, previousGuesses, previousEvaluations);
}

void Player::generatePosition(nat *proposal, nat position, const vector<nat*> *previousGuesses, const vector<nat*> *previousEvaluations) {
    for (nat i = 0; i < this->colors; i++) {
        proposal[position] = i;
        if (position == this->positions - 1) {
            // check if the solution is viable here
            if (plausibleSolution(previousGuesses, previousEvaluations, proposal)) {
                for (int i = 0; i < this->positions; i++)
                    cout << proposal[i] << " ";
                cout << endl;
                return;
            }
        }
        else generatePosition(proposal, position + 1, previousGuesses, previousEvaluations);
    }
}
