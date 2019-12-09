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

int main( int argc, char **argv) {
	COLORS = 10;			// Read arguments
	SPOTS = 4;
	NB_FIXED_SPOTS = 1;
	NB_COMBIS = COLORS*SPOTS;

	int ID; // process ID
	int NB_NODES; // nb of instances
	int MASTER = 0;

	MPI_Status status;
	MPI_Init( &argc, &argv ); // init the MPI environment
	MPI_Comm_rank( MPI_COMM_WORLD, &ID ); // ID
	MPI_Comm_size( MPI_COMM_WORLD, &NB_NODES ); // nb of instances

	bool finished = false;
	std::vector<unsigned> prevGuesses;
	// std::vector<unsigned> prevScores;



	int round = 0;
	// do {
		if (ID == MASTER) { // master node 
			if (round == 0) {
				cout << "======= MASTER ======= " << endl;
				GameMaster gameMaster = GameMaster(6, 4);
				gameMaster.printSolution();

				// combis = product(range(COLORS), repeat=SPOTS)

				for (int i = 0; i < 5; i++){
					for (int j = 0; j < SPOTS; j++){
						prevGuesses.push_back(i*SPOTS + j);
					}
				}
			}

			MPI_Barrier(MPI_COMM_WORLD);
			// ====== send prev guesses to all nodes
			// broadcastSend(prevGuesses); 
			
			// int vecSize = prevGuesses.size();
			// MPI_Bcast(&vecSize, 1, MPI_INT , 0, MPI_COMM_WORLD);

			// avec BUFFER
			std::vector<unsigned> buffer[4];
			for (int i = 0; i < 4; i++) {
				std::vector<unsigned> lol;
				lol.push_back(5);
				lol.push_back(6);
				buffer[i] = lol;
			}
			MPI_Bcast(buffer, 4, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);
			cout << "The master have send prev guesses to all nodes" << endl;
			




			// ====== receive the guesses from all nodes
			// std::vector<std::vector<unsigned>> newGuesses; // list of guesses
			// for (int i = 1; i < NB_NODES; i++){
			// 	std::vector<unsigned> guess;
			// 	guess.resize(SPOTS);
			// 	MPI_Recv(&guess[0], SPOTS, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			// 	newGuesses.push_back(guess);
			// 	cout << "after master recv guess" << endl;
			// 	// print new guess 
			// 	for (int j = 0; j < SPOTS; j++){
			// 		cout << guess.at(j) << " ";
			// 	}
			// 	cout << endl;
			// }	
			


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
		} 
		



		else { // challenger nodes
			cout << "======= SLAVE n°" << ID  << " =======" << endl;
			MPI_Barrier(MPI_COMM_WORLD);
			// prevGuesses = broadcastRecvVecOfVec();
			// int vecSize;
			// MPI_Bcast(&vecSize, 1, MPI_INT , MASTER, MPI_COMM_WORLD);
			// cout << "size received from node " << ID << " : " << vecSize << endl; 
			// print(prevGuesses);

			// avec BUFFER
			std::vector<unsigned> buffer[4];
			// for (int i = 0; i < 4; i++) { buffer[i] = 0; }
			MPI_Bcast(buffer, 4, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);
			for (int i = 0; i < 4; i++) { cout << buffer[i].size() << "  ";}
			cout << endl;


			// compute all combiunsignedions and pick one plausible guess

			// new_guess = null;
			// combis = product(range(COLORS), repeat=(SPOTS-NB_FIXED_SPOTS))
			// for combi in combis:
			// 	combi = self.fixed_spots + combi # (0, 1) + (2, 3, 4) = (0, 1, 2, 3, 4)
			// 	if plausible(combi, self.prev_guesses, self.prev_scores):
			// 		# print("Plausible guess :", combi)
			// 		new_guess = combi

			// => recv the guesses list 
			// compute all combinations and pick one plausible guess
			// gatter to master node
			//  MPI_Gather(&sendbuf,sendcnt,sendtype,&recvbuf,recvcount,recvtype,root,comm)
			// newGuessSize = newGuess.size();

			// GATHER
			// std::vector<unsigned> newGuess;
			// for (int i=0; i<SPOTS; i++) {
			// 	newGuess.push_back(ID);
			// }
			// MPI_Send(&newGuess[0], SPOTS, MPI_INT, 0, 0, MPI_COMM_WORLD); // send his guess to the master 
			// cout << "apres send du node " << ID << endl;
		}

		round++;

	// } while (not finished);

	if (ID == 0){
		cout << "Solution found." << endl;
	}

	cout << "ciao ciao bitch " << ID << endl;

	MPI_Finalize();
	return 0;
}



void broadcastSend(std::vector<unsigned> vec){
	int vecSize = vec.size();
	MPI_Bcast(&vecSize, 1, MPI_INT , MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&vec[0], vecSize, MPI_INT , MASTER, MPI_COMM_WORLD);

	// unsigned buffer[vecSize];
	// for (int i = 0; i < vecSize; i++) {buffer[i] = vec.at(i); }
	// MPI_Bcast(buffer, vecSize, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);
}

std::vector<unsigned> broadcastRecvVecOfVec(){
	std::vector<unsigned> vec;
	int vecSize;
	MPI_Bcast(&vecSize, 1, MPI_INT , MASTER, MPI_COMM_WORLD);
	vec.resize(vecSize);
	MPI_Bcast(&vec[0], vecSize, MPI_INT , MASTER, MPI_COMM_WORLD);
	// print(vec);
	return vec;
}

void print(std::vector<unsigned> vec){
	cout << "size of received vector: " << vec.size();
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