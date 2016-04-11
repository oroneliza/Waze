/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the implementation of the functions that performs the Dijkstra Algorithm.
*/


#include "Dijkstra's_Algorithm.h"


//Fills the shortest path array in the schematic order of the junctions in the path
//and returns the number of arcs in the path.
int Create_Path ( int* previous , int* 	shortest_path , int source , int target )
{
	int path_length = 0 , curr_vertex = target ,i = 0 ;
	
	while ( curr_vertex != source )
	{
		curr_vertex = previous[curr_vertex] ;
		path_length ++;
	}

	curr_vertex = target;
	shortest_path[ path_length - 1 ] = target ;
	for ( i = path_length - 2   ;   i > -1   ;   i--  )
	{
		shortest_path[i] = previous [curr_vertex ];
		curr_vertex = previous[ curr_vertex ];
	}
	return( path_length );
}


//Finds the junction with the minimum distance in the distance array which is still in the queue 
//and returns it's index in the junction array.
int Find_Min_Junction_In_Queue ( int* distance , BOOL* queue , int num_of_junctions )
{
	int i = 0 , j = 0 , min = -1 , min_junction = 0 ;

	for ( i = 0  ;  i < num_of_junctions  ; i++ )
	{
		if (queue[i] == TRUE && distance[i] != -1 )
		{
			min = distance[i] ;
			min_junction = i ;
			break;
		}
	}
		
	for ( j = i + 1  ;  j < num_of_junctions  ; j++  )
	{
		if (queue[j] == TRUE    &&    distance[j] != -1     &&     distance[j] < min )
		{
			min = distance [j];
			min_junction = j;			
		}
	}

	queue[min_junction] = FALSE;
	return (min_junction);
}


int Find_Shortest_Path (int** graph_matrix , int* shortest_path , int num_of_junctions, int source , int target )
{
	BOOL* queue = NULL ;
	int *distance = NULL , *previous = NULL , curr_vertex = 0 , alt = 0 ;
	int i=0 , path_length = 0;

	queue = (BOOL*)malloc (num_of_junctions * sizeof(BOOL)  );
	if (queue == NULL)
	{
		printf("FATAL ERROR: Memory allocation failed\n");
		return (-1);
	}
	
	distance = (int*)malloc (num_of_junctions * sizeof(int) );
	if (distance == NULL)
	{
		printf("FATAL ERROR: Memory allocation failed\n");
		return (-1);
	}
	
	previous = (int*)malloc (num_of_junctions * sizeof(int) );
	if (previous == NULL)
	{
		printf("FATAL ERROR: Memory allocation failed\n");
		return (-1);
	}

	for ( i = 0 ; i < num_of_junctions ; i++ )
	{
		if (i == source)
			distance[i] = 0 ;
		else
			distance[i] = -1 ;
		previous[i] = -1 ;
		queue[i] = TRUE ;
	}

	while ( ( curr_vertex = Find_Min_Junction_In_Queue(distance , queue , num_of_junctions) )   !=   target  )
	{
		for ( i = 0  ;  i < num_of_junctions  ;  i++  )
		{
			if ( graph_matrix[curr_vertex][i] != -1   &&   queue[i] == TRUE  )
			{
				alt = distance[curr_vertex] + graph_matrix[curr_vertex][i];
				if (  alt < distance[i]   ||   distance[i] == -1   )
				{
					distance[i] = alt;
					previous[i] = curr_vertex;
				}
			}
		}
	}
	path_length = Create_Path (previous , shortest_path , source , target );
	free( queue );
	free( distance );
	free( previous );
	return (  path_length   );
}













			






