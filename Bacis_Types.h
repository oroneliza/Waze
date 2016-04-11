/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the declarations of new types and constants.
*/


#ifndef BASIC_TYPES
#define BASIC_TYPES

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LINE_LENGTH 1024
#define SERVER_PORT 51000
#define GPS_PORT 52000
#define LOCALHOST_STRING "127.0.0.1"
#define GPS_SEMAPHORE_NAME "GPS_SEMAPHORE"
#define GPS_PROCESS_NAME "GPS.exe"


/*
* This struct represents a single junction in the road map. The fields are as follows:
* 1. x - The x coordinate of the junction.
* 2. y - The x coordinate of the junction.
*/ 
typedef struct Junction
{
	int x;
	int y;
}Junction;


/*
* This enum is used to represent the state of the current contact between the client and the server.
* TRNS FAILED - Indicate that the connection between the client and the server has failed.
* TRNS DISCONNECTED - Indicate that the connection between the client and the server has lost.
* TRNS SUCCEEDED - Indicate that the connection between the client and the server has succeeded.
* SETUP PROBLEM - Indicate that there was a problem in creating the tools to establish the connection.
*/ 
typedef enum
{ 
	TRNS_FAILED,
	TRNS_DISCONNECTED, 
	TRNS_SUCCEEDED,
	SETUP_PROBLEM
} TransferResult_t;  


/*
* Data From Server - A struct of arguments. The fields are as follows:
* 1. num of junctions - The num of junctions as written in the grpah.txt
* 2. junctions - A pointer to the array of junctions in the road map.
* 3. graph matrix - The matrix holding the traffic congestion of the rad map.
*/
typedef struct Data_From_Server
{
	int num_of_junctions;
	Junction* junctions;
	int** graph_matrix;

}Data_From_Server;


/*
* Updated Arc - A struct of arguments. The fields are as follows:
* 1. source - The first edge of the arc that has to be updated.
* 2. destination - The second edge of the arc that has to be updated.
* 3. delay - A pointer to the semaphores array.
*/
typedef struct Updated_Arc
{
	Junction source;
	Junction destination;
	int delay;
}Updated_Arc;


/*
* Single Thread Arg - A struct of arguments. The fields are as follows:
* 1. s - The accepeted socket so that the thread will be able to communicate with single client. 
* 2. client serial number - The serial number of the client.
* 3. ptr graph matrix - A pointer to the matrix holding the Traffic congestion of the road map.
* 4. mutex graph - A pointer to mutex used to protect from race conditions on the road map.
* 5. output file - A pointer to the server log file.
* 6. mutex file - A pointer to mutex used to protect from race conditions on the server log file.
* 7. num of junctions - The num of junctions as written in the grpah file.
* 8. junctions - The array of junctions in the road map.
* 9. ptr quit - A pointer to flag that indicates that "quit" has being entered by the user in the stdin.
* 10. ptr failure - A pointer to flag that indicates if a fatal error occurred somewhere in the server program.  
*/
typedef struct Single_Thread_Arg
{
	SOCKET s;
	int client_serial_number;
	int *** ptr_graph_matrix;
	HANDLE *mutex_graph;
	FILE *output_file;
	HANDLE *mutex_file;
	int num_of_junctions;
	Junction* junctions;
	BOOL *ptr_quit;
	BOOL *ptr_failure;
}Single_Thread_Arg;



#endif