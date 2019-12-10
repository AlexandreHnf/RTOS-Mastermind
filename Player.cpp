//
// Created by Madalin Ionescu on 2019-12-07.
//

#include "Player.h"

Player::Player(nat colors, nat positions) {
    this->colors = colors;
    this->positions = positions;
}

bool Player::plausibleSolution(const vg* previousGuesses, const vg* previousEvaluations, sg solution, nat solLen) const {
    for (nat i = 0; i < previousGuesses->size(); i++) {
        if (!proposalIsDifferent(solution, solLen, previousGuesses->at(i))) return false;

        if (!proposalRespectsKnowledge(solution, solLen, previousGuesses->at(i), previousEvaluations->at(i))) return false;
    }

    return true;
}

bool Player::proposalIsDifferent(sg proposal, nat proposalLen, sg reference) const {
    if (proposalLen < this->positions) return true;
    for (nat i = 0; i < this->positions; i++)
        if (proposal.at(i) != reference.at(i))
            return true;

    return false;
}

bool Player::proposalRespectsKnowledge(sg proposal, nat proposalLen, sg reference, sg referenceScore) const {
    bool* referenceNotTaken = new bool[reference.size()];
    for (nat i = 0; i < reference.size(); i++)
        referenceNotTaken[i] = true;

    nat conservedColors = 0;

    for (nat i = 0; i < proposalLen; i++) {
        for (nat j = 0; j < reference.size(); j++) {
            if (proposal[i] == reference[j] && referenceNotTaken[j]) {
                referenceNotTaken[j] = false;
                conservedColors++;
                break;
            }
        }
    }

    nat changedColors = proposalLen - conservedColors;

    nat correctColors = referenceScore[0] + referenceScore[1];

    nat wrongColors = reference.size() - correctColors;

    //if (conservedColors < correctColors && correctColors == this->positions) return false;
    if (changedColors > wrongColors && wrongColors == 0) return false;

    //if (conservedColors <= correctColors && correctColors != this->positions) return false;
    if (changedColors >= wrongColors && wrongColors != 0) return false;

    //if (conservedColors == this->positions && correctColors == 0) return false;
    if (changedColors == 0 && wrongColors == reference.size()) return false;

    return true;
}

sg Player::generatePlausibleSolution(sg posList, const vg* previousGuesses, const vg* previousEvaluations) {
    sg proposal;
    proposal.reserve(this->positions);

    for (nat i = 0; i < posList.size(); i++)
        proposal[i] = posList[i];

    bool found = false;
    generatePosition(&proposal, posList.size(), previousGuesses, previousEvaluations, &found);

    if (!found)
        for (nat i = 0; i < this->positions; i++)
            proposal[i] = this->colors;

    return proposal;
}

void Player::generatePosition(sg* proposal, nat position, const vg* previousGuesses, const vg* previousEvaluations, bool* found) {
    if (!*found && plausibleSolution(previousGuesses, previousEvaluations, *proposal, position)) {
        for (nat i = 0; i < this->colors; i++) {
            if (!*found) {
                proposal->at(position) = i;
                if (position == proposal->size() - 1) {
                    // check if the solution is viable here
                    if (plausibleSolution(previousGuesses, previousEvaluations, *proposal, position + 1)) {
                        *found = true;
                    }
                } else generatePosition(proposal, position + 1, previousGuesses, previousEvaluations, found);
            }
        }
    }
}
