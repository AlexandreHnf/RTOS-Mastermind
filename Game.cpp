//
// Created by Madalin Ionescu on 2019-12-07.
//

#include "Game.h"

Game::Game(unsigned colors, unsigned positions) {
    Game(colors, positions, UINT_MAX);
}

Game::Game(unsigned colors, unsigned positions, unsigned rounds) {
    this->positions = positions;
    this->gameMaster = GameMaster(colors, positions);
    this->player = Player(colors, positions);
    this->rounds = rounds;
}

bool Game::isFinished() {
    unsigned* solution = this->gameMaster.getSolution();
    unsigned* proposal = this->previousGuesses[this->previousGuesses.size() - 1];

    bool identical = true;

    for (unsigned i = 0; i < this->positions; i++)
        if (proposal[i] != solution[i])
            identical = false;

    return identical;
}

void Game::playRound() {
    /*
     * if no previous guesses
     *   player make a random guess
     * otherwise
     *   generate plausible guesses on threads
     *   pick randomly a guess
     *
     * game master evaluate the guess
     * save the guess and its evaluation
     *
     */
}
