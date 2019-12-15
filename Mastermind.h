#include <stdio.h>
#include "mpi.h"
#include "Player.h"
#include "GameMaster.h"
#include <vector>
#include <math.h>
#include <iostream>

#define vg std::vector<std::vector<unsigned>> //vg = vector fo guesses : [ (g1), (g2)] where g1 = [0, 1, 2]

void broadcastSendVecVec(vg, int);
vg broadcastRecvVecOfVec(int);

void broadcastSend(std::vector<unsigned>);
std::vector<unsigned> broadcastRecv();

void sendFixedSpots(vg);
std::vector<unsigned> recvFixedSpots(MPI_Status);

int victory(std::vector<unsigned>);

void print(vg);
void print(std::vector<unsigned>);