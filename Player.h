//
// Created by Madalin Ionescu on 2019-12-07.
//

#ifndef RTOS_MASTERMIND_PLAYER_H
#define RTOS_MASTERMIND_PLAYER_H

#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;


class Player {

private:
    unsigned colors;
    unsigned positions;

public:
    Player() = default;
    Player(unsigned colors, unsigned positions);

    unsigned* pickFirstSolution();

    // function used to analyze the context before launching the threads to find
    // the new solution proposition
    vector<unsigned*> analyzeContext(vector<unsigned*> previousPropositions);

    // picks randomly one of the solutions generated by a processor
    unsigned* pickSolution();

    bool proposalIsDifferent(const unsigned* proposal, const unsigned* reference);

    bool proposalRespectsKnowledge(const unsigned* proposal, const unsigned* reference, const unsigned* referenceScore);

    bool plausibleSolution(const vector<unsigned*> *previousGuesses, const vector<unsigned*> *previousEvaluations, unsigned* solution);
};


#endif //RTOS_MASTERMIND_PLAYER_H
