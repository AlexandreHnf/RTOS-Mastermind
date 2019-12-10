#include <stdio.h>
#include "mpi.h"
#include "Game.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include "Mastermind.h"

using namespace std;


/*
link number of colors, nb of nodes, & nb of fixed spots
do while
link guesses 
report
*/

int TRUE = 1;
int FALSE = 0;
int COLORS;
int SPOTS;
int EVALS = 2; // black pins and white pins
int NB_FIXED_SPOTS;
int NB_COMBIS;
int MASTER = 0;
int NONE = 0;
GameMaster gameMaster;
Player player;

int main( int argc, char **argv) {

	int ID; // process ID
	int NB_NODES; // nb of instances

	MPI_Status status;
	MPI_Init( &argc, &argv ); // init the MPI environment
	MPI_Comm_rank( MPI_COMM_WORLD, &ID ); // ID
	MPI_Comm_size( MPI_COMM_WORLD, &NB_NODES ); // nb of instances

	//======= Read arguments =========
	COLORS = 10;
	SPOTS = 4;
	NB_FIXED_SPOTS = 1;
	NB_COMBIS = COLORS*SPOTS;
	if (argc == 3){
		COLORS = atoi(argv[1]);
		SPOTS  = atoi(argv[2]);
	}

	int finished = FALSE;
	vg prevGuesses;
	vg prevScores;

	int *currentGuesses;
	
	int round = 0;
	do {
		// cout << "Start of Round n°" << round << " for node n°" << ID << endl;
		if (ID == MASTER) {
			// cout << " =========== MASTER ===========" << endl;
			if (round == 0) {
				// ==== INITIALISATION FOR THE MASTER ====
				gameMaster = GameMaster(COLORS, SPOTS);
				gameMaster.printSolution();

				// just to test prev guesses list 
				// for (int i = 0; i < 5; i++){
				// 	std::vector<unsigned> guess;
				// 	std::vector<unsigned> score;
				// 	for (int j = 0; j < SPOTS; j++) { guess.push_back(j); }					
				// 	for (int k = 0; k < EVALS; k++) { score.push_back(k); }
				// 	prevGuesses.push_back(guess);
				// 	prevScores.push_back(score);
				// }
			}
			// send to each node its corresponding fixed begin
			vg fixedSpots = gameMaster.generateFirstPositions(NB_FIXED_SPOTS);

			sendFixedSpots(fixedSpots);
			
			// ====== send prev guesses and prev scores to all nodes
			broadcastSendVecVec(prevGuesses, SPOTS); 
			broadcastSendVecVec(prevScores, EVALS);
			// cout << "The master have sent prev guesses to all nodes" << endl;


			// ====== receive the guesses from all nodes
			vg newGuesses; // list of guesses
			for (int i = 1; i < NB_NODES; i++){
				std::vector<unsigned> guess;
				guess.resize(SPOTS);
				MPI_Recv(&guess[0], SPOTS, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
				newGuesses.push_back(guess);
				// cout << "after master recv guess " << endl;
				// for (int j = 0; j < SPOTS; j++){ cout << guess.at(j) << " ";}
				// cout << endl;
			}	

			cout << "new guesses received :";
			print(newGuesses);

			// ====== choose randomly a guess and assess it 
			int rand_i = rand() % (NB_NODES-1); 
			while (newGuesses.at(rand_i).at(0) == COLORS) {  // it means that no plausible sol found
				rand_i = rand() % (NB_NODES-1); // we search again for a random guess
			}
			std::vector<unsigned> chosenGuess = newGuesses.at(rand_i);


			cout << "chosen guess : ";
			print(chosenGuess);
			cout << endl;

			// ======  assess the guess :
			std::vector<unsigned> evaluation = gameMaster.checkProposedSol(chosenGuess);
			cout << "evaluation : ";
			print(evaluation);
			cout << endl;

			// ====== update the previous guesses list and the scores list
			prevGuesses.push_back(chosenGuess);
			prevScores.push_back(evaluation);

			cout << "previous guesses : ";
			print(prevGuesses);
			cout << "previous scores : ";
			print(prevScores);


			// if victory => finish all 
			// finished = gameMaster.victory(chosenGuess);
			// finished = victory(chosenGuess);
			finished = evaluation.at(0) == SPOTS;
			cout << "finished ? " << finished << endl;

			// send the finished value 
			MPI_Bcast(&finished, 1, MPI_INT , MASTER, MPI_COMM_WORLD);

			MPI_Barrier(MPI_COMM_WORLD); // when Master allow other nodes to continue
		} 
		


		// =======================================================================
		// ============================ CHALLENGER ===============================
		// =======================================================================
		else { 
			//cout << "======= SLAVE n°" << ID  << " =======" << endl;

			player = Player(COLORS, SPOTS); // object player for each node
			
			// receives the fixed Spots 
			std::vector<unsigned> fixedSpot = recvFixedSpots(status);
			//print(fixedSpot);

			// receives the prev guesses list and prev scores list
			prevGuesses = broadcastRecvVecOfVec(SPOTS);
			prevScores = broadcastRecvVecOfVec(EVALS);
			// print(prevGuesses);
			// print(prevScores); 
			
			// compute all combiunsignedions and pick one plausible guess
			// todo : replace it by combinations 
			std::vector<unsigned> newGuess;
			// for (int i=0; i < SPOTS; i++) {
			// 	if (ID == 2) {newGuess.push_back(COLORS);} // NULL is 0 in c++
			// 	else {newGuess.push_back(ID);}
			// }

			newGuess = player.generatePlausibleSolution(fixedSpot, &prevGuesses, &prevScores);
			cout << "r" << round << "new guess of n " << ID << ": ";
			print(newGuess);
			cout << endl;

			// GATHER
			MPI_Send(&newGuess[0], SPOTS, MPI_INT, MASTER, 0, MPI_COMM_WORLD); // send his guess to the master 
			//cout << "process " << ID << " sent his guess to master !" << endl;

			// receives the finished value 
			MPI_Bcast(&finished, 1, MPI_INT ,MASTER, MPI_COMM_WORLD);
			if (finished == TRUE) {cout << "node " << ID << " finished" << endl;}

			MPI_Barrier(MPI_COMM_WORLD);

		}

		// cout << "End of Round n°" << round << " for node n°" << ID << endl;
		round++;

	} while (finished == FALSE);


	// THE LOOP IS FINISHED 
	if (ID == MASTER){

		cout << "Solution found." << endl;
	}

	cout << "GOODBYE process " << ID << endl;

	MPI_Finalize();
	return 0;
}

int victory(std::vector<unsigned> guess) {
	std::vector<unsigned> sol(SPOTS, 4);  // vector of size SPOTS with value 4
	for (int i = 0; i < guess.size(); i++) {
		if (guess.at(i) != sol.at(i)) {
			return FALSE;
		}
	}
	return TRUE;
}

void sendFixedSpots(vg vec) {
	// vec = [ (0, 0), (0, 1), ...] 
	for (int i = 1; i <= vec.size(); i++){
		MPI_Send(&vec[i-1][0], NB_FIXED_SPOTS, MPI_INT, i, 0, MPI_COMM_WORLD); // send his guess to the master 
	}
}

std::vector<unsigned> recvFixedSpots(MPI_Status status){
	// recv (0, 0)
	std::vector<unsigned> fixedSpots;
	fixedSpots.resize(NB_FIXED_SPOTS);
	MPI_Recv(&fixedSpots[0], NB_FIXED_SPOTS, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
	return fixedSpots;
}

void broadcastSend(std::vector<unsigned> vec){
	int vecSize = vec.size();
	MPI_Bcast(&vecSize, 1, MPI_INT , MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&vec[0], vecSize, MPI_INT , MASTER, MPI_COMM_WORLD);
}

void broadcastSendVecVec(vg vec, int subListSize){
	int vecSize = vec.size();
	MPI_Bcast(&vecSize, 1, MPI_INT , MASTER, MPI_COMM_WORLD);
	for (int i = 0; i < vecSize; i++){
		MPI_Bcast(&vec[i][0], subListSize, MPI_INT , MASTER, MPI_COMM_WORLD);
	}
}

vg broadcastRecvVecOfVec(int subListSize){
	vg vec;
	int vecSize = 0;
	MPI_Bcast(&vecSize, 1, MPI_INT , 0, MPI_COMM_WORLD);
	vec.resize(vecSize);
	for (int i = 0; i < vecSize; i++){
		vec[i].resize(subListSize);
		MPI_Bcast(&vec[i][0], subListSize, MPI_INT , 0, MPI_COMM_WORLD);
	}
	return vec;
}

std::vector<unsigned> broadcastRecv() {
	int vecSize = 0;
	MPI_Bcast(&vecSize, 1, MPI_INT , 0, MPI_COMM_WORLD);
	std::vector<unsigned> vec;
	vec.resize(vecSize);
	MPI_Bcast(&vec[0], vecSize, MPI_INT , 0, MPI_COMM_WORLD);
	return vec;
}


void print(vg vec){
	cout << "[";
	for (int i = 0; i < vec.size(); i++){
		print(vec[i]);
		if (i != vec.size()-1)
			cout << ", ";
	}
	cout << "]" << endl;
}

void print(std::vector<unsigned> vec){
	cout << "[";
	for (int i = 0; i < vec.size(); i++){
		cout << vec[i];
		if (i != vec.size() -1)
			cout << ", ";
	}
	cout << "]";
}