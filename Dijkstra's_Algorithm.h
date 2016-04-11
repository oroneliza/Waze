/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the declaration of the function that performs the Dijkstra Algorithm.
*/


#ifndef DIJKSTRA_ALGORITHM
#define   DIJKSTRA_ALGORITHM

#include "Bacis_Types.h"


/*
* The function find the shortest path from the source to the target junction and insert that path to an array and return the numbers of arcs 
* in that path.
*
* Input:
* -----
* 1. graph matrix - The matrix that represents the road map and the weight of the arcs between each junction.
* 2. shortest path - An array of int that will contains the indexs of the junctions in the shortest path (used as an additional output).
* 3. num of junctions - The number of junctions in the road map.
* 4. source - The index of the source junction in the array.
* 5. target - The index of the target junction in the array.
*
* Output:
* -----
* 1. integer - Returns the number of arcs between the junctions in the shortest path and -1 in case of FATLA ERROR.

*/
int Find_Shortest_Path (int** graph_matrix, int* shortest_path , int num_of_junctions, int source , int target );


#endif