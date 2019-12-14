#include "GameMaster.h"

GameMaster::GameMaster(unsigned colors, unsigned positions) {
    this->colors = colors;
    this->positions = positions;
    this->solution = new unsigned[positions];

    srandom((int) time(0));

    for (unsigned i = 0; i < this->positions; i++)
        solution[i] = random() % this->colors;
}

sg GameMaster::checkProposedSol(sg proposedSolution) {
    sg res; // 0 -> perfect matches; 1 -> correct colors
    res.resize(2); res[0] = 0; res[1] = 0;
    unsigned* solution = new unsigned[this->positions];

    for (unsigned i = 0; i < proposedSolution.size(); i++)
        if (proposedSolution.at(i) == this->solution[i]) {
            res[0]++;
            proposedSolution.at(i) = this->colors;
            solution[i] = this->colors;
        } else solution[i] = this->solution[i];

    for (unsigned i = 0; i < this->positions; i++)
        if (proposedSolution.at(i) != this->colors)
            for (unsigned j = 0; j < this->positions; j++)
                if (proposedSolution.at(i) == solution[j]) {
                    res[1]++;
                    solution[j] = this->colors;
                    break;
                }
    return res;
}

void GameMaster::printSolution() {
    cout << "The solution is: ";

    for (unsigned i = 0; i < this->positions; i++)
        cout << this->solution[i] << " ";

    cout << endl;
}

unsigned* GameMaster::getSolution() const {
    return solution;
}

vg GameMaster::generateFirstPositions(unsigned short posNb) {
    sg working;
    vg beginnings;

    for (int i = 0; i < posNb; i++)
        working.push_back(0);

    cartProduct(&working, &beginnings, 0, posNb, this->colors);

    return beginnings;
}

void GameMaster::cartProduct(sg *v, vg *vv, int position, int length, int colors) {
    int i;
    for (i = 0; i < colors; i++) {
        v->at(position) = i;
        if (position == length - 1) {
            vv->push_back(*v);
        }
        else cartProduct(v, vv, position + 1, length, colors);
    }
}


