#include <stdio.h>
#include "mpi.h"
#include "Game.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include "Mastermind.h"

#define TRUE 1
#define FALSE 0

using namespace std;

int COLORS;
int SPOTS;
int EVALS = 2; // black pins and white pins
int NB_FIXED_SPOTS;
int MASTER_ID = 0;

GameMaster gameMaster;
Player player;

int main( int argc, char **argv) {

	//========= MPI ===============
	int ID; // process ID
	int NB_NODES; // nb of instances

	MPI_Status status;
	MPI_Init( &argc, &argv ); // init the MPI environment
	MPI_Comm_rank( MPI_COMM_WORLD, &ID ); // ID
	MPI_Comm_size( MPI_COMM_WORLD, &NB_NODES ); // nb of instances

	//======= Read arguments =========
	COLORS = NB_NODES-1;
	SPOTS = COLORS/2;
	NB_FIXED_SPOTS = 1;
	if (argc == 2){
		SPOTS = atoi(argv[1]);
	}

	// ======= Init variables ========
	std::chrono::time_point<std::chrono::system_clock> startTime;
	vg prevGuesses;
	vg prevScores;
	int *currentGuesses;
	int round = 0;
	int finished = FALSE;

	do {
		// =======================================================================
		// ============================== MASTER =================================
		// =======================================================================
		if (ID == MASTER_ID) {
	
			if (round == 0) {
				// ==== INITIALISATION FOR THE MASTER_ID ====
				startTime = std::chrono::system_clock::now();

				cout << pow(COLORS, SPOTS) << " possible guesses" << endl;
				gameMaster = GameMaster(COLORS, SPOTS);
				gameMaster.printSolution();
			}
			
			// ====== send to each node its corresponding fixed beginning 
			vg fixedSpots = gameMaster.generateFirstPositions(NB_FIXED_SPOTS);
			sendFixedSpots(fixedSpots);
			
			// ====== send prev guesses and prev scores to all nodes
			broadcastSendVecVec(prevGuesses, SPOTS); 
			broadcastSendVecVec(prevScores, EVALS);

			// ====== receive the guesses from all nodes
			vg newGuesses; // vector of guesses
			for (int i = 1; i < NB_NODES; i++){ // for each slave (every node besides 0)
				std::vector<unsigned> guess;
				guess.resize(SPOTS);
				MPI_Recv(&guess[0], SPOTS, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
				newGuesses.push_back(guess);
			}	

			// cout << "new guesses received :"; print(newGuesses);

			// ====== choose randomly a guess and assess it 
			int rand_i = rand() % (NB_NODES-1); 
			while (newGuesses.at(rand_i).at(0) == COLORS) {  //it means that no plausible sol found
				rand_i = rand() % (NB_NODES-1); // we search again for a random guess
			}
			std::vector<unsigned> chosenGuess = newGuesses.at(rand_i);

			// cout << "chosen guess : "; print(chosenGuess); cout << endl;

			// ======  assess the randomly chosen guess :
			std::vector<unsigned> evaluation = gameMaster.checkProposedSol(chosenGuess);
			
			// cout << "evaluation : "; print(evaluation); cout << endl;

			// ====== update the previous guesses list and the scores list
			prevGuesses.push_back(chosenGuess);
			prevScores.push_back(evaluation);

			// cout << "previous guesses : "; print(prevGuesses);
			// cout << "previous scores : ";  print(prevScores);

			// ======= send the finished value (whether we won or not) to each node
			finished = evaluation.at(0) == SPOTS; // (4, 0)
			// cout << "finished ? " << finished << endl; 
			MPI_Bcast(&finished, 1, MPI_INT , MASTER_ID, MPI_COMM_WORLD);

			MPI_Barrier(MPI_COMM_WORLD); // when Master allow other nodes to continue
		} 
		

		// =======================================================================
		// ============================ CHALLENGER ===============================
		// =======================================================================
		else { 
			player = Player(COLORS, SPOTS); // object player for each node
			
			// receives the fixed Spots 
			std::vector<unsigned> fixedSpot = recvFixedSpots(status);

			// receives the prev guesses list and prev scores list
			prevGuesses = broadcastRecvVecOfVec(SPOTS);
			prevScores = broadcastRecvVecOfVec(EVALS);
			
			// computes all combinations and pick one plausible guess
			std::vector<unsigned> newGuess;
			newGuess = player.generatePlausibleSolution(fixedSpot, &prevGuesses, &prevScores);
			
			// cout << "r" << round << " | new guess of n " << ID << ": "; print(newGuess); cout << endl;

			// sends his guess to the master
			MPI_Send(&newGuess[0], SPOTS, MPI_INT, MASTER_ID, 0, MPI_COMM_WORLD); 

			// receives the finished value 
			MPI_Bcast(&finished, 1, MPI_INT ,MASTER_ID, MPI_COMM_WORLD);
			// if (finished == TRUE) {cout << "node " << ID << " finished" << endl;}

			// waits for the master to finish assessing the guess
			MPI_Barrier(MPI_COMM_WORLD);

		}
		
		round++;

	} while (finished == FALSE);


	// THE LOOP IS FINISHED 
	if (ID == MASTER_ID){
		cout << " ======== Solution found : "; 
		print(prevGuesses.at(prevGuesses.size()-1)); 
		cout << endl;
		auto now = std::chrono::system_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime); 
		cout << "Time : " << elapsedTime.count()/1000.0 << "s" << endl;
	}

	// cout << "GOODBYE from node " << ID << endl;

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
	MPI_Recv(&fixedSpots[0], NB_FIXED_SPOTS, MPI_INT, MASTER_ID, 0, MPI_COMM_WORLD, &status);
	return fixedSpots;
}

void broadcastSend(std::vector<unsigned> vec){
	int vecSize = vec.size();
	MPI_Bcast(&vecSize, 1, MPI_INT , MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast(&vec[0], vecSize, MPI_INT , MASTER_ID, MPI_COMM_WORLD);
}

void broadcastSendVecVec(vg vec, int subListSize){
	int vecSize = vec.size();
	MPI_Bcast(&vecSize, 1, MPI_INT , MASTER_ID, MPI_COMM_WORLD);
	for (int i = 0; i < vecSize; i++){
		MPI_Bcast(&vec[i][0], subListSize, MPI_INT , MASTER_ID, MPI_COMM_WORLD);
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