//
// Created by Madalin Ionescu on 2019-12-07.
//

#include "Player.h"

Player::Player(unsigned colors, unsigned positions) {
    this->colors = colors;
    this->positions = positions;
}

vector<unsigned *> Player::analyzeContext(vector<unsigned *> previousPropositions) {
    return vector<unsigned *>();
}

unsigned *Player::pickSolution() {
    return nullptr;
}

unsigned *Player::pickFirstSolution() {
    unsigned* solution = new unsigned[positions];

    for (unsigned i = 0; i < positions; i++)
        solution[i] = random() % this->positions;

    return solution;
}

bool Player::plausibleSolution(const vector<unsigned*> *previousGuesses, const vector<unsigned*> *previousEvaluations, unsigned *solution) {
    for (unsigned i = 0; i < previousGuesses->size(); i++) {
        cout << "History eval: " << i << endl;
        if (!proposalIsDifferent(solution, previousGuesses->at(i))) return false;

        if (!proposalRespectsKnowledge(solution, previousGuesses->at(i), previousEvaluations->at(i))) return false;
    }

    return true;
}

bool Player::proposalIsDifferent(const unsigned *proposal, const unsigned *reference) {
    for (unsigned i = 0; i < this->positions; i++)
        if (proposal[i] != reference[i])
            return true;

    return false;
}

bool Player::proposalRespectsKnowledge(const unsigned* proposal, const unsigned* reference, const unsigned* referenceScore) {
    unsigned* referenceCopy = new unsigned[this->positions];
    for (unsigned i = 0; i < this->positions; i++)
        referenceCopy[i] = reference[i];

    unsigned conservedColors = 0;

    for (unsigned i = 0; i < this->positions; i++) {
        for (unsigned j = 0; j < this->positions; j++) {
            if (proposal[i] == referenceCopy[j]) {
                referenceCopy[j] = this->positions;
                conservedColors++;
                break;
            }
        }
    }

    unsigned correctColors = referenceScore[0] + referenceScore[1];

    if (conservedColors < correctColors && correctColors == this->positions) return false;

    if (conservedColors <= correctColors && correctColors != this->positions) return false;

    if (conservedColors == this->positions && correctColors == 0) return false;

    return true;
}
