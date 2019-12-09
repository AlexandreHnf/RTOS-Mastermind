//
// Created by Madalin Ionescu on 2019-12-07.
//

#ifndef RTOS_MASTERMIND_PLAYER_H
#define RTOS_MASTERMIND_PLAYER_H

#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;
using nat = unsigned;
using vg = vector<vector<nat>>;
using sg = vector<nat>;


class Player {

private:
    nat colors;
    nat positions;

    void generatePosition(sg* proposal, nat position, const vg* previousGuesses, const vg* previousEvaluations, bool* found);

public:
    Player() = default;
    Player(nat colors, nat positions);

    bool proposalIsDifferent(sg proposal, nat proposalLen, sg reference) const;

    bool proposalRespectsKnowledge(sg proposal, nat proposalLen, sg reference, sg referenceScore) const;

    bool plausibleSolution(const vg* previousGuesses, const vg* previousEvaluations, sg solution, nat solLen) const;

    sg generatePlausibleSolution(sg posList, const vg* previousGuesses, const vg* previousEvaluations);
};


#endif //RTOS_MASTERMIND_PLAYER_H
