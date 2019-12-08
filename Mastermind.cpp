#include <stdio.h>
#include "mpi.h"
#include "Game.h"
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

int main( int argc, char **argv) {

	int COLORS = 10;
	int SPOTS = 4;
	int NB_FIXED_SPOTS = 1;
	int NB_NODES = pow(COLORS, NB_FIXED_SPOTS);   // 4spots, 2fixed => 4**2 nodes

	int rank; // process ID
	int size; // nb of instances

	MPI_Status status; // for the send, recv

	MPI_Init( &argc, &argv ); 				// init the MPI environment
	MPI_Comm_rank( MPI_COMM_WORLD, &rank ); // ID
	MPI_Comm_size( MPI_COMM_WORLD, &size ); // nb of instances

	// currentGuesses = (std::vector<unsigned>*) malloc(NB_NODES * sizeof(int)) //MALLOC OMGGGGGGGGGGGGGGGGGGGGGG
	unsigned* currentGuesses = new unsigned[NB_NODES];
	bool finished = false;

	std::vector<unsigned> prevGuesses;
	unsigned prevGuessesSize;
	std::vector<unsigned> prevScores;
	
	// do {
		if (rank == 0) { // master node 
			cout << "I'm the master " << endl;
			GameMaster gameMaster = GameMaster(6, 4);
			gameMaster.printSolution();

			// combis = product(range(COLORS), repeat=SPOTS)
			int nbCombis = COLORS*SPOTS;
			cout << nbCombis << "possible combis" << endl;
			for (int i = 0; i < 5; i++){
				prevGuesses.push_back(i);
			}
			// => recv all guesses from all nodes (blocking recv) or no need cause gather ??
			// for (int i = 1; i<NB_NODES; i++){
				// MPI_Recv(&currentGuesses,count,type,source,MPI_ANY_TAG, MPI_COMM_WORLD, status)	
			// }

			// pick random node_i
			// while not guess of node null 
				// pick random node_i

			// assess them
			// unsigned* evaluation = gameMaster.checkProposedSol(node_i)

			// update the prevGuesses and prevScores
			// prevGuesses.push_back(guess);
			// prevScores.push_back(evaluation);
			
			// send to all nodes the guesses list => how to send a list ??
			// syntax BROADCAST:  MPI_Bcast(&buffer,count,datatype,root,comm)
			// => MPI_Bcast(&prevGuesses, 1 , MPI_INT , 0, MPI_COMM_WORLD);

			// ========= test ============
			// syntax send : MPI_Send(&buffer,count,type,dest,tag,comm)
			prevGuessesSize = prevGuesses.size();
			MPI_Bcast(&prevGuessesSize, 1, MPI_INT , 0, MPI_COMM_WORLD);
			MPI_Bcast(&prevGuesses[0], prevGuesses.size(), MPI_INT , 0, MPI_COMM_WORLD);
		} 

		else { // challenger nodes
			cout << "I'm the slave n°" << rank << endl;
			// => recv the guesses list 
			MPI_Bcast(&prevGuessesSize, 1, MPI_INT , 0, MPI_COMM_WORLD);
			prevGuesses.resize(prevGuessesSize);
			MPI_Bcast(&prevGuesses[0], prevGuessesSize, MPI_INT , 0, MPI_COMM_WORLD);

			for (auto prevGuess : prevGuesses){
				printf("val : %d", prevGuess);
			}

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
			// MPI_Gather(&new_guess, 1, MPI_INT, currentGuess, 1, MPI_INT, 0, MPI_COMM_WORLD);
		}
		// finished = true;
		// printf( "Process %d got %d\n", rank, value );

	// } while (not finished);

	MPI_Finalize();
	return 0;
}
