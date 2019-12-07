//
// Created by Madalin Ionescu on 2019-12-07.
//

#ifndef RTOS_MASTERMIND_GAME_H
#define RTOS_MASTERMIND_GAME_H

using nat = unsigned;


class Game {
    private GameMaster gameMaster;
    private Player player;
    private nat* previousGuesses;
    private nat rounds;

    public Game(nat colors, nat positions);
    public Game(nat colors, nat positions, nat rounds)

    private bool isFinished();
};


#endif //RTOS_MASTERMIND_GAME_H
