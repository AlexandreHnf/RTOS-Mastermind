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
    //true if the position in reference was not already matched to one in the proposal, false otherwise
    for (nat i = 0; i < reference.size(); i++)
        referenceNotTaken[i] = true;

    nat conservedColors = 0;

    for (nat i = 0; i < proposalLen; i++) {
        for (nat j = 0; j < reference.size(); j++) {
            if (proposal[i] == reference[j] && referenceNotTaken[j]) {
                //do not match the same position in reference twice
                referenceNotTaken[j] = false;   //mark the position in reference
                conservedColors++;
                break;
            }
        }
    }

    nat changedColors = proposalLen - conservedColors;

    nat correctColors = referenceScore[0] + referenceScore[1];

    nat wrongColors = reference.size() - correctColors;

    //if (conservedColors < correctColors && correctColors == this->positions) return false;
    if (changedColors > wrongColors) return false;

    //if (conservedColors <= correctColors && correctColors != this->positions) return false;
    //if (changedColors > wrongColors && wrongColors != 0) return false;

    //if (conservedColors == this->positions && correctColors == 0) return false;
    if (changedColors == 0 && wrongColors == reference.size()) return false;

    return true;
}

sg Player::generatePlausibleSolution(sg posList, const vg* previousGuesses, const vg* previousEvaluations) {
    sg proposal;

    //set the prefix
    for (nat i = 0; i < posList.size(); i++)
        proposal.push_back(posList[i]);
    //create the other positions - reserve method fails
    for (nat i = posList.size(); i < this->positions; i++)
        proposal.push_back(this->colors);

    bool found = false;
    //start filling the positions following the fixed prefix 
    generatePosition(&proposal, posList.size(), previousGuesses, previousEvaluations, &found);

    if (!found) //solution not found: replace every position with a marker value indicating that
        for (nat i = 0; i < this->positions; i++)
            proposal[i] = this->colors;

    return proposal;
}

void Player::generatePosition(sg* proposal, nat position, const vg* previousGuesses, const vg* previousEvaluations, bool* found) {
    //recursive backtracking function
    if (!*found && plausibleSolution(previousGuesses, previousEvaluations, *proposal, position)) {
        //stop when the solution is found
        for (nat i = 0; i < this->colors; i++) {
            if (!*found) {
                //do not modify anything after the solution was found
                proposal->at(position) = i;
                if (position == proposal->size() - 1) {
                    //all the positions filled: check if the solution is viable
                    if (plausibleSolution(previousGuesses, previousEvaluations, *proposal, position + 1)) {
                        //accept the solution and stop the recursion
                        *found = true;
                    }
                } else generatePosition(proposal, position + 1, previousGuesses, previousEvaluations, found);
            }
        }
    }
}
