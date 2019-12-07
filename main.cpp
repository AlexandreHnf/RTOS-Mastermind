#include <iostream>
#include "Game.h"

using namespace std;

int main() {
    GameMaster gameMaster = GameMaster(6, 4);
    gameMaster.printSolution();

    Player player = Player(6, 4);
    nat* test = player.pickFirstSolution();

    cout << "Proposed:        ";
    for (int i = 0; i < 4; i++) {
        cout << test[i] << " ";
    }
    cout << endl;

    nat* res = gameMaster.checkProposedSol(test);

    cout << "Perfect: " << res[0] << ", correct colors: " << res[1] << endl;
}