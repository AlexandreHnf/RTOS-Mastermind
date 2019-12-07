//
// Created by Madalin Ionescu on 2019-12-07.
//

#include "Game.h"

Game::Game(nat colors, nat positions) {
    Game(colors, positions, UINT_MAX);
}

Game::Game(nat colors, nat positions, nat rounds) {
    this->gameMaster = GameMaster(colors, positions);
    this->player = Player(colors, positions);
    this->rounds = rounds;
}

bool Game::isFinished() {
    return false;
}
