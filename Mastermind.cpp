#include <stdio.h>
#include "mpi.h"
#include "Game.h"
#include <vector>
#include <iostream>

using namespace std;
using nat unsigned;

int main( int argc, char **argv) {

	int COLORS = 10;
	int SPOTS = 4;
	int NB_FIXED_SPOTS = 1;
	int NB_NODES = COLORS**NB_FIXED_SPOTS;   // 4spots, 2fixed => 4**2 nodes

	int rank; // process ID
	int size; // nb of instances

	MPI_Status status;
	MPI_Init( &argc, &argv ); // init the MPI environment
	MPI_Comm_rank( MPI_COMM_WORLD, &rank ); // ID
	MPI_Comm_size( MPI_COMM_WORLD, &size ); // nb of instances

	GameMaster gameMaster = GameMaster(6, 4);
    gameMaster.printSolution();

	// combis = product(range(COLORS), repeat=SPOTS)
	int nbCombis = COLORS*SPOTS;
	cout << nbCombis << "possible combinations" << endl;

	std::vector<nat> prev_guesses;
	std::vector<nat> prev_scores;
	
	bool finished = false;
	
	do {
		if (rank == 0) { // master node 
			// MPI_Recv(&buffer,count,type,source,tag,comm,&status)

			// => recv all guesses from all nodes (blocking recv)
			// assess them
			// send to all nodes the guesses list
		} 

		else { // challenger nodes

			// => recv the guesses list 
			// compute all combinations and pick one plausible guess
			// gatter to master node
		}
		printf( "Process %d got %d\n", rank, value );

	} while (not finished);

	MPI_Finalize();
	return 0;
}
