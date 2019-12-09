#include <stdio.h>
#include "mpi.h"
#include "Game.h"
#include <vector>
#include <iostream>
#include "Mastermind.h"

using namespace std;

int COLORS;
int SPOTS;
int NB_FIXED_SPOTS;
int NB_COMBIS;
int MASTER = 0;

int main( int argc, char **argv) {
	COLORS = 10;			// Read arguments
	SPOTS = 4;
	NB_FIXED_SPOTS = 1;
	NB_COMBIS = COLORS*SPOTS;

	int ID; // process ID
	int NB_NODES; // nb of instances

	MPI_Status status;
	MPI_Init( &argc, &argv ); // init the MPI environment
	MPI_Comm_rank( MPI_COMM_WORLD, &ID ); // ID
	MPI_Comm_size( MPI_COMM_WORLD, &NB_NODES ); // nb of instances

	bool finished = false;
	std::vector<unsigned> prevGuesses;
	// std::vector<unsigned> prevScores;

	int *currentGuesses;
	

	int round = 0;
	// do {
		// =======================================================================
		// ============================== MASTER =================================
		// =======================================================================
		if (ID == MASTER) {

			cout << "======= MASTER ======= " << endl;
			if (round == 0) {
				// ==== INITIALISATION FOR THE MASTER ====
				GameMaster gameMaster = GameMaster(6, 4);
				gameMaster.printSolution();

				// combis = product(range(COLORS), repeat=SPOTS)

				for (int i = 0; i < 5; i++){
					for (int j = 0; j < SPOTS; j++){
						prevGuesses.push_back(i*SPOTS + j);
					}
				}
			}

			// ====== send prev guesses to all nodes
			broadcastSend(prevGuesses); 
			// MPI_Barrier(MPI_COMM_WORLD); // allow other nodes to use that guesses
			cout << "The master have sent prev guesses to all nodes" << endl;


			// ====== receive the guesses from all nodes
			std::vector<std::vector<unsigned>> newGuesses; // list of guesses
			for (int i = 1; i < NB_NODES; i++){
				std::vector<unsigned> guess;
				guess.resize(SPOTS);
				MPI_Recv(&guess[0], SPOTS, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
				newGuesses.push_back(guess);
				// cout << "after master recv guess " << endl;
				for (int j = 0; j < SPOTS; j++){ cout << guess.at(j) << " ";}
				cout << endl;
			}	
			

			// ====== choose randomly a guess and assess it 
			// int rand_i = rand() % NB_NODES; 
			// while (currentGuesses.get(i) == null) { 
				// rand_i = rand() % NB_NODES; 
			// }		

			// ======  assess the guess :
			// unsigned* evaluation = gameMaster.checkProposedSol(currentGuesses.get(i))

			// ====== update the previous guesses list
			// for (int i = 0; i<SPOTS; i++){
			// 	prevGuesses.push_back(choosenGuess.get(i));
			// }			

			MPI_Barrier(MPI_COMM_WORLD); // when Master allow other nodes to continue
		} 
		


		// =======================================================================
		// ============================ CHALLENGER ===============================
		// =======================================================================
		else { 
			cout << "======= SLAVE n°" << ID  << " =======" << endl;

			// MPI_Barrier(MPI_COMM_WORLD);
			prevGuesses = broadcastRecvVecOfVec();
			print(prevGuesses);


			// compute all combiunsignedions and pick one plausible guess
			// todo : replace it by combinations 
			std::vector<unsigned> newGuess;
			for (int i=0; i<SPOTS; i++) {
				newGuess.push_back(ID);
			}
			
			// GATHER
			MPI_Send(&newGuess[0], SPOTS, MPI_INT, MASTER, 0, MPI_COMM_WORLD); // send his guess to the master 
			cout << "process " << ID << " sent his guess to master !" <<endl;
			MPI_Barrier(MPI_COMM_WORLD);

		}

		round++;

	// } while (not finished);


	// THE LOOP IS FINISHED 
	if (ID == 0){
		cout << "Solution found." << endl;
	}

	cout << "GOODBYE process " << ID << endl;

	MPI_Finalize();
	return 0;
}



void broadcastSend(std::vector<unsigned> vec){
	int vecSize = vec.size();
	MPI_Bcast(&vecSize, 1, MPI_INT , MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&vec[0], vecSize, MPI_INT , MASTER, MPI_COMM_WORLD);
}

std::vector<unsigned> broadcastRecvVecOfVec(){
	std::vector<unsigned> vec;
	int vecSize;
	MPI_Bcast(&vecSize, 1, MPI_INT , MASTER, MPI_COMM_WORLD);
	vec.resize(vecSize);
	MPI_Bcast(&vec[0], vecSize, MPI_INT , MASTER, MPI_COMM_WORLD);
	return vec;
}

void print(std::vector<unsigned> vec){
	cout << "[";
	for (int i = 0; i < vec.size(); i++){
		if (i % SPOTS == 0){
			cout << "[";
		}
		cout << vec[i] << ",";
		if (i % SPOTS == SPOTS-1){
			cout << "],";
		}
	}
	cout << "]" << endl;
}