/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the implementation of the functions that performs the converting the textual information from the
				input file to the different arrays. 
*/


#include "Converting_Text_To_Matrices.h"


// The function allocate memory for the junction array and filling it with the information from the road map written in the input file. 
int Create_Junctions_Array(int num_of_junctions , Junction** ptr_junctions , FILE* graph_file)
{
	char line[LINE_LENGTH];
	char* retstr = NULL ;
	int i = 0;

	*ptr_junctions = (Junction*) malloc(  num_of_junctions * sizeof(Junction)  );
	if (*ptr_junctions == NULL )
	{
		printf("FATAL ERROR: Memory allocation failed\n");
		return(1);
	}

	retstr = fgets(line , LINE_LENGTH , graph_file);
	if (retstr == NULL )
	{
		printf("FATAL ERROR: fgets() failed. Ending program");
		return(1);
	}
	line[  strlen(line) -1  ] = '\0' ;

	(*ptr_junctions)[0].x = atoi (  strtok(line , " ")  );
	(*ptr_junctions)[0].y = atoi (  strtok(NULL , " ")  );
	for ( i = 1  ;  i < num_of_junctions ; i++ )
	{
		(*ptr_junctions)[i].x = atoi (  strtok(NULL , " ")  );
		(*ptr_junctions)[i].y = atoi (  strtok(NULL , " ")  );
	}
	return(0);
}


// The function allocate memory for the graph matrix and filling it with the information from the road map written in the input file. 
int Create_Graph_Matrix (int num_of_junctions , int*** ptr_graph_matrix , FILE* graph_file )
{
	char line[LINE_LENGTH];
	char *retstr = NULL ;
	int i = 0 , j = 0;

	*ptr_graph_matrix = (int**)malloc( num_of_junctions * sizeof(int*) );
	if (*ptr_graph_matrix == NULL )
	{
		printf("FATAL ERROR: Memory allocation failed\n");
		return (1);
	}
	for ( i = 0 ; i < num_of_junctions ; i++ )
	{
		(*ptr_graph_matrix)[i] = (int*)malloc ( num_of_junctions * sizeof (int) );
		if (  (*ptr_graph_matrix)[i]  == NULL  )
		{
			printf("FATAL ERROR: Memory allocation failed\n");
			return (1);
		}
	}

	for ( i = 0 ; i < num_of_junctions ; i++)
	{
		retstr = fgets (line , LINE_LENGTH , graph_file);
		if (retstr == NULL )
		{
			printf("FATAL ERROR: fgets() failed. Ending program");
			return(1);
		}
		if ( feof(graph_file) == 0 )
		{
			line [ strlen(line) -1 ] = '\0';
		}
		(*ptr_graph_matrix)[i][0] = atoi (  strtok( line , " ")  );
		for (  j = 1  ;  j < num_of_junctions  ;  j++  )
			(*ptr_graph_matrix)[i][j] = atoi (  strtok( NULL , " ")  );
	}
	return (0);
}


int Convert_Graphtext_To_Matrices(FILE* graph_file , Junction** ptr_junctions , int*** ptr_graph_matrix , int* ptr_num_of_junctions)
{
	char line[LINE_LENGTH];
	char *retstr = NULL ;
	int line_length = 0;
	int retval=0;

	retstr = fgets (line , LINE_LENGTH , graph_file);
	if (retstr == NULL )
	{
		printf("FATAL ERROR: fgets() failed. Ending program");
		return(1);
	}
	line[  strlen(line) -1  ] = '\0' ;
	*ptr_num_of_junctions = atoi(line);

	retval = Create_Junctions_Array( *ptr_num_of_junctions  ,  ptr_junctions  ,  graph_file );
	if (retval == 1)
		return(1);

	retval = Create_Graph_Matrix ( *ptr_num_of_junctions  ,  ptr_graph_matrix ,  graph_file );
	if (retval == 1)
		return(1);

	return(0);
}
