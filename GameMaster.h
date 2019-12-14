#ifndef RTOS_MASTERMIND_GAMEMASTER_H
#define RTOS_MASTERMIND_GAMEMASTER_H

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;
using vg = vector<vector<unsigned>>;
using sg = vector<unsigned>;


class GameMaster {

private:
    unsigned colors;
    unsigned positions;
    unsigned* solution;
    
    // Fills a given position when generating the prefix. Analoguous to the player
    // function used to generate a guess.
    // - v: pointer to working Vector in which to compute the prefix
    // - vv: pointer to a Vector a Vectors saving the prefixes generated
    // - position: position to fill in the working vector
    // - length: length of the sequence to generate
    // - colors: the number of colors to use
    void cartProduct(sg* v, vg* vv, int position, int length, int colors);

public:
    GameMaster() = default;
    GameMaster(unsigned colors, unsigned positions);

    // Returns an array of length 2: 0 -> # perfect colors, 1 -> # correct colors
    sg checkProposedSol(sg proposedSolution);

    // Prints the solution on the standard output stream
    void printSolution();

    // Returns a pointer to the solution.
    unsigned* getSolution() const;

    vg generateFirstPositions(unsigned short posNb);
};


#endif //RTOS_MASTERMIND_GAMEMASTER_H
