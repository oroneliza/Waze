/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the declarations of the functions that performs the converting the textual information from the
				input file to the different arrays. 
*/


#ifndef CONVERTING_TEXT_TO_MATRICES
#define  CONVERTING_TEXT_TO_MATRICES

#include "Bacis_Types.h"


/*
* The function convetrs the textual information from the input file to be represented by different arrays. 
*
* Input:
* -----
* 1. graph file - A pointer to input file that contains the information about the road map.
* 2. ptr junctions - A pointer to the array of junctions in the road map (used as an additional output).
* 3. ptr graph matrix - A pointer to the matrix holding the Traffic congestion of the road map (used as an additional output).
* 4. ptr num of junctions - A pointer to the number of junctions in the road map.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Convert_Graphtext_To_Matrices(FILE* graph_file , Junction** ptr_junctions , int*** ptr_graph_matrix , int* ptr_num_of_junctions);


#endif