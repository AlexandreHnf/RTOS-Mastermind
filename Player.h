//
// Created by Madalin Ionescu on 2019-12-07.
//

#ifndef RTOS_MASTERMIND_PLAYER_H
#define RTOS_MASTERMIND_PLAYER_H

#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;
// natural numbers
using nat = unsigned;
// Vector of Guesses
using vg = vector<vector<nat>>;
// Simple Guess
using sg = vector<nat>;


class Player {

private:
    nat colors;     // the number of colors in the solution
    nat positions;  // the length of the solution

    // Recursive function which fills a position in a guess and calls itself to fill the following
    // - proposal: pointer to the working vector storing the partial guess
    // - position: the position to fill in the proposal
    // - previousGuesses: pointer to a vector containing the previous guesses
    // - previousEvaluations: pointer to a vector containing the vealuations of the previous guesses
    // - found: pointer to a boolean indicating if a solution was already found. Used to stop the recursion
    void generatePosition(sg* proposal, nat position, const vg* previousGuesses, const vg* previousEvaluations, bool* found);

public:
    Player() = default;
    Player(nat colors, nat positions);

    // Returns true if the proposed guess is different than a reference
    // - proposal: the proposed solution (may be partial)
    // - proposalLen: the len of the proposed solution (may be partial - only the first k positions set)
    // - reference: the reference to which to compare the proposal
    bool proposalIsDifferent(sg proposal, nat proposalLen, sg reference) const;

    // Returns true if the proposed guess is at least as good as the reference  
    // - proposal: the proposed solution (may be partial)
    // - proposalLen: the len of the proposed solution (may be partial - only the first k positions set)
    // - reference: the reference to which to compare the proposal
    // - referenceScore: the evaluation of the reference (the reference is a guess from the guesses' history)
    bool proposalRespectsKnowledge(sg proposal, nat proposalLen, sg reference, sg referenceScore) const;

    // Returns true if the guess is different than the already proposed solutions
    // (using proposalIsDifferent) and if it respect what we learned from the 
    // previous guesses (using proposalRespectsKnowledge)
    // - previousGuesses: pointer to a vector containing the previous guesses
    // - previousEvaluations: pointer to a vector containing the vealuations of the previous guesses
    // - solution: the guess to be tested
    // - solLen: the lenght of the guess to test - may be partia (i.e. only the first k positions filled)
    bool plausibleSolution(const vg* previousGuesses, const vg* previousEvaluations, sg solution, nat solLen) const;

    // Generates a plausible solution based on a prefix and on the previous guesses.
    // - posList: vector of the elements of the prefix
    // - previousGuesses: pointer to a vector containing the previous guesses
    // - previousEvaluations: pointer to a vector containing the vealuations of the previous guesses
    sg generatePlausibleSolution(sg posList, const vg* previousGuesses, const vg* previousEvaluations);
};


#endif //RTOS_MASTERMIND_PLAYER_H
