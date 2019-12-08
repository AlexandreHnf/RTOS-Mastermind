#include <iostream>
#include "Game.h"

using namespace std;

int main() {
    Player player = Player(8, 4);

    // solution is 4 5 6 7
    // test with 0 1 2 3
    nat* proposal = new nat[4];
    proposal[0] = 1; proposal[1] = 2; proposal[2] = 3; proposal [3] = 4;

    vector<nat*> history;
    vector<nat*> res;
    /*
    // any solution is plausible when there's no guesses history
    cout << "Plausible: " << player.plausibleSolution(&history, &res, proposal) << endl;

    nat* eval = new nat[2];
    eval[0] = 0; eval[1] = 0;
    history.push_back(proposal);
    res.push_back(eval);

    // avoid any permutation of an already tested solution with no correct colors
    cout << "Not plausible: " << player.plausibleSolution(&history, &res, proposal) << endl;

    // tested with 6 5 4 3 -> perfect 1, correct 2
    nat* proposal2 = new nat[4];
    proposal2[0] = 6; proposal2[1] = 5; proposal2[2] = 4; proposal2[3] = 3;
    nat* eval2 = new nat[2];
    eval2[0] = 1; eval2[1] = 2;
    history.push_back(proposal2);
    res.push_back(eval2);

    // avoid any solution which evaluates equally or worse than an already proposed solution
    nat* proposal3 = new nat[4];
    proposal3[0] = 6; proposal3[1] = 5; proposal3[2] = 2; proposal3[3] = 3;
    cout << "Not plausible: " << player.plausibleSolution(&history, &res, proposal3) << endl;
    */
    // test with 7 5 6 4 -> perfect 1, correct 3
    nat* proposal3 = new nat[4];
    proposal3[0] = 7; proposal3[1] = 5; proposal3[2] = 6; proposal3[3] = 4;
    nat* eval3 = new nat[2];
    eval3[0] = 1; eval3[1] = 3;
    history.push_back(proposal3);
    res.push_back(eval3);

    // if the all the colors were found, avoid any solution which doesn't use the same colors
    player.generatePlausibleSolution(0, proposal, &history, &res);

    /*
     * while not game is finished :
     *   game -> play round()
     */
}