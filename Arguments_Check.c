/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the implementation of the functions that performs the arguments check.
*/


#include "Arguments_Check.h"


//checking the server ip address argument and in case of 'localhost' updating the address to be "127.0.0.1"
int IP_Address_Check ( char* server_ip_address )
{
	if ( strcmp("localhost" , server_ip_address ) != 0 )
	{
		if ( inet_addr ( server_ip_address ) == INADDR_NONE)
		{
			printf("FATAL ERROR: The string \"%s\" cannot be converted into an ip address. Ending program.\n" , server_ip_address  );
			return(1);
		}
	}
	else 
		strcpy ( server_ip_address , LOCALHOST_STRING );
	return (0);
}

//checking the max client argument.
int Max_Clients_Check ( double max_clients )
{
	double test = 0.0 ;

	test = max_clients - (unsigned int)(max_clients);
	if ( test != 0.0 )
	{
		printf( "The max clients variable is not a positive integer!\n");
		return (1);
	}
	return(0);
}

//checking the txt file and opening it.
int Filetxt_Check (char *graph_file_name , FILE** ptr_graph_file )
{

	*ptr_graph_file = fopen ( graph_file_name , "r");
	if ( *ptr_graph_file == NULL )
	{
		printf("FATAL ERROR : Failed opening file");
		return(1);
	}
	return (0);
}


int Arguments_Checks_Server (  char *graph_file_name , FILE** ptr_graph_file , double max_clients , char* server_ip_address )
{
	if (    Filetxt_Check( graph_file_name , ptr_graph_file) == 1   )
		return(1);
		
	if (    Max_Clients_Check ( max_clients) == 1   )
	{
		fclose( *ptr_graph_file );
		return (1);
	}

	if (    IP_Address_Check ( server_ip_address) == 1   )
	{
		fclose( *ptr_graph_file );
		return(1);
	}
	return (0);
}
 

int Arguments_Checks_Client ( char* server_ip_address )
{
	if (    IP_Address_Check ( server_ip_address) == 1   )
		return(1);
	return(0);
}
