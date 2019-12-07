//
// Created by Madalin Ionescu on 2019-12-07.
//

#ifndef RTOS_MASTERMIND_GAMEMASTER_H
#define RTOS_MASTERMIND_GAMEMASTER_H

using nat = unsigned; // shorter name for natural numbers


class GameMaster {

    public GameMaster(nat colors, nat positions)

    private nat* solution;

    // returns an array of length 2: 0 -> # perfect colors, 1 -> # correct colors
    private nat* checkProposedSol(nat* proposedSolution);
};


#endif //RTOS_MASTERMIND_GAMEMASTER_H
