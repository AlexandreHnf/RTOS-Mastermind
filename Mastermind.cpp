#include <stdio.h>
#include "mpi.h"
#include "Game.h"
#include <vector>
#include <math.h>
#include <iostream>
#include "Mastermind.h"

using namespace std;

int COLORS;
int SPOTS;
int NB_FIXED_SPOTS;
int NB_NODES;
int NB_COMBIS;

int main( int argc, char **argv) {
	COLORS = 10;			// Read arguments
	SPOTS = 4;
	NB_FIXED_SPOTS = 1;
	NB_NODES = pow(COLORS, NB_FIXED_SPOTS);   // 4spots, 2fixed => 4**2 nodes
	NB_COMBIS = COLORS*SPOTS;

	int rank; // process ID
	int size; // nb of instances

	MPI_Status status; // for the send, recv

	MPI_Init( &argc, &argv ); 				// init the MPI environment
	MPI_Comm_rank( MPI_COMM_WORLD, &rank ); // ID
	MPI_Comm_size( MPI_COMM_WORLD, &size ); // nb of instances

	// vg currentGuesses;
	unsigned *currentGuesses;
	// unsigned* currentGuesses = new unsigned[NB_NODES];
	bool finished = false;

	vg prevGuesses; // [(1, 2, 3), (4, 3, 1), ... ]
	unsigned prevGuessesSize;
	// std::vector<unsigned> prevScores;


	std::vector<unsigned> newGuess; 		// (1, 2, 3)
	
	// do {
		if (rank == 0) { // master node 
			cout << "I'm the master " << endl;
			GameMaster gameMaster = GameMaster(6, 4);
			gameMaster.printSolution();

			// combis = product(range(COLORS), repeat=SPOTS)
			cout << NB_COMBIS << " possible combinations" << endl;
			for (int i = 0; i < 5; i++){
				std::vector<unsigned> lol;
				for (int j = 0; j < SPOTS; j++){
					lol.push_back(j);
				}
				prevGuesses.push_back(lol);
			}
			// => recv all guesses from all nodes (blocking recv) or no need cause gather ??
			// for (int i = 1; i<NB_NODES; i++){
				// MPI_Recv(&currentGuesses,count,type,source,MPI_ANY_TAG, MPI_COMM_WORLD, status)	
			// }
			currentGuesses = (unsigned*) malloc(NB_NODES * SPOTS*sizeof(int));

			// pick random node_i
			// int rand_i = rand() % NB_NODES; 
			// while (currentGuesses.get(i) == null) { 
				// rand_i = rand() % NB_NODES; 
			// }

			// assess them
			// unsigned* evaluation = gameMaster.checkProposedSol(currentGuesses.get(i))

			// update the prevGuesses and prevScores
			// prevGuesses.push_back(guess);
			// prevScores.push_back(evaluation);
			
			// send to all nodes the guesses list => how to send a list ??
			// syntax BROADCAST:  MPI_Bcast(&buffer,count,datatype,root,comm)
			// => MPI_Bcast(&prevGuesses, 1 , MPI_INT , 0, MPI_COMM_WORLD);

			// ========= test ============
			// syntax send : MPI_Send(&buffer,count,type,dest,tag,comm)
			prevGuessesSize = prevGuesses.size();
			broadcastSend(prevGuesses, prevGuessesSize);
		} 

		else { // challenger nodes
			cout << "I'm the slave n°" << rank << endl;
			prevGuesses = broadcastRecvVecOfVec();
			prevGuessesSize = prevGuesses.size();

			print(prevGuesses);
			// compute all combiunsignedions and pick one plausible guess

			// new_guess = null;
			// combis = product(range(COLORS), repeat=(SPOTS-NB_FIXED_SPOTS))
			// for combi in combis:
			// 	combi = self.fixed_spots + combi # (0, 1) + (2, 3, 4) = (0, 1, 2, 3, 4)
			// 	if plausible(combi, self.prev_guesses, self.prev_scores):
			// 		# print("Plausible guess :", combi)
			// 		new_guess = combi

			// gatter to master node
			//  MPI_Gather(&sendbuf,sendcnt,sendtype,&recvbuf,recvcount,recvtype,root,comm)
			// newGuessSize = newGuess.size();

			std::vector<unsigned> newGuess;
			for (int i=0; i<SPOTS; i++) {
				newGuess.push_back(i*rank);
			}
			MPI_Gather(&[newGuess[0], SPOTS, MPI_INT, currentGuesses, NB_NODES, MPI_INT, 0, MPI_COMM_WORLD);
		}

	// } while (not finished);

	MPI_Finalize();
	return 0;
}


void broadcastSend(vg vec, unsigned vecSize){
	MPI_Bcast(&vecSize, 1, MPI_INT , 0, MPI_COMM_WORLD);
	for (int i = 0; i < vecSize; i++){
		MPI_Bcast(&vec[i][0], SPOTS, MPI_INT , 0, MPI_COMM_WORLD);
	}
}

vg broadcastRecvVecOfVec(){
	vg vec;
	int vecSize;
	MPI_Bcast(&vecSize, 1, MPI_INT , 0, MPI_COMM_WORLD);
	vec.resize(vecSize);
	for (int i = 0; i < vecSize; i++){
		vec[i].resize(SPOTS);
		MPI_Bcast(&vec[i][0], SPOTS, MPI_INT , 0, MPI_COMM_WORLD);
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