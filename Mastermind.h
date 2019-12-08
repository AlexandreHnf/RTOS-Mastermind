#include <stdio.h>
#include "mpi.h"
#include "Game.h"
#include <vector>
#include <math.h>
#include <iostream>

#define vg std::vector<std::vector<unsigned>> //vg = vector fo guesses : [ (g1), (g2)] where g1 = [0, 1, 2]

void broadcastSend(std::vector<std::vector<unsigned>> vec, unsigned vecSize);
vg broadcastRecvVecOfVec();

void print(std::vector<std::vector<unsigned>> vec);