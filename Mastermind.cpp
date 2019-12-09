#include <stdio.h>
#include "mpi.h"
#include "Game.h"
#include <vector>
#include <iostream>
#include "Mastermind.h"

using namespace std;

int COLORS;
int SPOTS;
int EVALS = 2; // black pins and white pins
int NB_FIXED_SPOTS;
int NB_COMBIS;
int MASTER = 0;
int NONE = 0;
GameMaster gameMaster;

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
	vg prevGuesses;
	vg prevScores;

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
				gameMaster = GameMaster(6, 4);
				gameMaster.printSolution();

				// combis = product(range(COLORS), repeat=SPOTS)

				for (int i = 0; i < 5; i++){
					std::vector<unsigned> guess;
					std::vector<unsigned> score;
					for (int j = 0; j < SPOTS; j++) { guess.push_back(j); }					
					for (int k = 0; k < EVALS; k++) {score.push_back(2); }
					prevGuesses.push_back(guess);
					prevScores.push_back(score);
				}
			}

			// ====== send prev guesses and prev scores to all nodes
			// broadcastSend(prevGuesses, SPOTS); 
			broadcastSend(prevScores, EVALS);
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
			// 	rand_i = rand() % NB_NODES; 
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

			// prevGuesses = broadcastRecvVecOfVec(SPOTS);
			prevScores = broadcastRecvVecOfVec(EVALS);
			// print(prevGuesses);
			print(prevScores);


			// compute all combiunsignedions and pick one plausible guess
			// todo : replace it by combinations 
			std::vector<unsigned> newGuess;
			for (int i=0; i<SPOTS; i++) {
				if (ID == 2) {newGuess.push_back(NONE);} // NULL is 0 in c++
				else {newGuess.push_back(ID);}
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

void broadcastSend(vg vec, int subListSize){
	int vecSize = vec.size();
	MPI_Bcast(&vecSize, 1, MPI_INT , 0, MPI_COMM_WORLD);
	for (int i = 0; i < vecSize; i++){
		MPI_Bcast(&vec[i][0], subListSize, MPI_INT , 0, MPI_COMM_WORLD);
	}
}


vg broadcastRecvVecOfVec(int subListSize){
	vg vec;
	int vecSize;
	MPI_Bcast(&vecSize, 1, MPI_INT , 0, MPI_COMM_WORLD);
	vec.resize(vecSize);
	for (int i = 0; i < vecSize; i++){
		vec[i].resize(subListSize);
		MPI_Bcast(&vec[i][0], subListSize, MPI_INT , 0, MPI_COMM_WORLD);
	}
	return vec;
}


void print(vg vec){
	cout << "{";
	for (int i = 0; i < vec.size(); i++){
		cout << "[";
		for (int j = 0; j < vec[i].size(); j++){
			cout << vec[i][j];
			if (j != vec[i].size()-1)
				cout << ", ";
		}
		cout << "]";
		if (i != vec.size()-1)
			cout << ", ";
	}
	cout << "}" << endl;
}