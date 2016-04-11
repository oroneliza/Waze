/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the declarations of the functions that performs the arguments check.
*/


#ifndef ARGUMENTS_CHECK
#define  ARGUMENTS_CHECK

#include "Bacis_Types.h"


/*
* The function checks if 3 inputs recieved from the user are valid.
*
* Input:
* -----
* 1. graph file name - The name of the input file that contains the information about the road map.
* 2. ptr input file - A pointer to the input file (used as an additional output).
* 3. max clients - A floating point number of the max clients that the server can serve.
* 4. server ip address - The IP address of the server.
*
* Output:
* -----
* 1. integer - Returns 1 if the arguments are not valid and 0 otherwise .
*			   **** closing file in case that the two other arguments are invalid is made indise the function.
*/
int Arguments_Checks_Server (  char *graph_file_name , FILE** ptr_graph_file , double max_clients , char* server_ip_address );


/*
 * The fuction checks if the server ip addresss recieved from the user is valid. 
 * 
 * Input:
 * -----
 * 1. server ip address - The ip address of the server.
 *
 * Output:
 * -----
 * 1. Integer - Returns 1 if the arguments are not valid and 0 otherwise .
*/
int Arguments_Checks_Client ( char* ptr_server_ip_address );

#endif