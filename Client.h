/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the declaration of the function that performs different the client assignment.
*/


#ifndef	  CLIENT
#define   CLIENT

#include "Arguments_Check.h"
#include "Dijkstra's_Algorithm.h"
#include "General_Tools.h"
#include "Communication_Tools.h"



/*
* The function performs the client assignments.
*
* Input:
* -----
* 1. Arg Ip Address - The IP address of the client as a string. 
* 2. Source x - The x coordinate of the source junction as a integer.
* 3. Source y  - The y coordinate of the source junction as a integer.
* 4. Destination x - The x coordinate of the Destination junction as a integer.
* 5. Destination y - The y coordinate of the Destination junction as a integer.
* 6. Client Log File_Name - The name of th client LOG file as a string.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR or a communication problem occurred and 0 otherwise.
*/
int Client_Func ( char* Arg_Ip_Address , int Source_x , int Source_y , int Destination_x , int Destination_y , char * Client_Log_File_Name);



#endif