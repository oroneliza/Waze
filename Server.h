/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the declaration of the function that performs the server assignment.
*/


#ifndef SERVER
#define SERVER

#include "Arguments_Check.h"
#include "Converting_Text_To_Matrices.h"
#include "Server_Functions.h"


/*
* The function performs the server assignment.
*
* Input:
* -----
* 1. server ip address - The IP address of the server.
* 2. graph file name - The name of the input file that contains the information about the road map.
* 3. max client - The number of the max clients that the server can serve.
* 4. server log file name - The name of the server log file.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Server_Func ( char *server_ip_address , char *graph_file_name ,  double max_client , char *server_log_file_name);

#endif