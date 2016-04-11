/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the declarations of the functions that perform the searching junction , closing program ant printing assignments.
*/


#ifndef GENERAL_TOOLS
#define  GENERAL_TOOLS

#include "Bacis_Types.h"


/*
* Server Print Mode - An enum contains the fields as follows:
* 1. SUCCESSFULLY CONNECTED - Signal that the server successfully connected to client.
* 2. GRAPH SENT - Signaled that the graph were sent successfully.
* 3. UPDATED ARC - Signaled that an arc were up.
* 4. CLIENT DICONNECTED - signaled that the client closed the connection.
*/
typedef enum Server_Print_Mode
{
	SUCCESSFULLY_CONNECTED,
	GRAPH_SENT,
	UPDATED_ARC,
	CLIENT_DICONNECTED

}Server_Print_Mode; 


/*
* Close Status - An enum contains the fields as follows:
* 1. PLUS GPS SERVER - Signaled that we also requested to close the server socket and the GPS socket.
* 2. PLUS SERVER - Signaled that we also requested to close the server socket.
* 3. STAND ALONE - Signaled that we requested to close only the program allocation resources.
*/
typedef enum
{
	PLUS_GPS_SERVER = 0 ,
	PLUS_SERVER ,
	STAND_ALONE
}Close_Status;


/*
* Current Status - An enum contains the fields as follows:
* 1. SUCCESSFULLY LOGGED TO SERVER - Signaled that we successfully logged to the server.
* 2. FAILED TO CONNECT TO SERVER - Signaled that we faild to establish a connection between the client and the server.
* 3. RECEIVED MAP ROAD - Signaled that we received the data from server struct from the server.
* 4. BAD COORDINATES - Signaled that we receiveed a bad coordinates from the operation user.
* 5. CALCULATED PATH - Signaled that we calculated the shortest path and we are ready to print it.
* 6. SUCCESSFULLY LOOGED TO GPS - Signaled that we successfully logged to the GPS.
* 7. FAILED TO CONNECT TO GPS - Signaled that we failed to connect to the GPS.
* 8. GPS TIME - Signaled that we received the current time from the GPS.
* 9. FAILED TO RECEIVE TIME - Signaled that we failed to receive the current time from the GPS.. 
* 10. FAILED TO UPDATE SERVER - Signaled that we failed to update the server(the connection was lost).
* 11. YOU HAVE REACHED - Signaled that we have reached our destination.
* 12. FAILED TO REACH - Signaled that we failed to reach our destination.
*/
typedef enum
{
	SUCCESSFULLY_LOGGED_TO_SERVER = 0 ,
	FAILED_TO_CONNECT_TO_SERVER ,
	RECEIVED_MAP_ROAD ,
	BAD_COORDINATES ,
	CALCULATED_PATH ,
	SUCCESSFULLY_LOOGED_TO_GPS ,
	FAILED_TO_CONNECT_TO_GPS ,
	GPS_TIME ,
	FAILED_TO_RECEIVE_TIME ,
	FAILED_TO_UPDATE_SERVER ,
	YOU_HAVE_REACHED ,
	FAILED_TO_REACH 
}Current_Status;


/*
* The function check if the requested source and destination junctions exsit in the array junctions.
*
* Input:
* -----
* 1. junctions - An array that contains the junctions.
* 2. source - A junction struct that we wish to search.
* 3. destination - A junction struct that we wish to search.
* 4. source index - A pointer to the index of the source junction we will find.  
* 5. destination index - A pointer to the index of the destination junction we will find.
* 6. Num Of Junctions - The number of junctions that exist.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Check_If_Junction_Exist (Junction* junctions , Junction source , Junction destination , int * source_index , int* destination_index ,
	int Num_Of_Junctions );


/*
* The function prints the current status of the server to the server LOG file and to the sdtout.
*
* Input:
* -----
* 1. server print mode - enum defined by us indicates that status that should be printed.
* 2. output file - A pointer to the server LOG file.
* 3. client serial number - The number of the single client.
* 4. data from client - The struct that contains the information regarding the arc that were updated.  
* 5. new weight - the new traffic conjestion of the arc.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Print_Server_Mode ( Server_Print_Mode server_print_mode , FILE* output_file , HANDLE* mutex_file, int client_serial_number , 
						Updated_Arc data_from_client , int new_weight  );


/*
* The function prints the updated information about the road map before finally closing the server program.
*
* Input:
* -----
* 1. output file - A pointer to server log file.
* 2. graph matrix - The matrix holding the Traffic congestion of the road map
* 3. num of junctions - The number of junctions in the road map.
* 4. junctions - The array of junctions in the road map.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Print_Graph_Into_Log_File ( FILE* output_file , int** graph_matrix , int num_of_junctions , Junction* junctions );


/*
* The function closes and frees all the resources allocated to the program.
*
* Input:
* -----
* 1. junctions - The array of junctions in the road map.
* 2. mutex graph - A mutex used to protect from race conditions on the road map.
* 3. mutex file - A mutex used to protect from race conditions on the server log file.
* 4. graph matrix - The matrix holding the Traffic congestion of the road map.
* 5. graph file - A pointer to the input file (used as an additional output).
* 6. output_file - A pointer to server log file.
* 7. main socket - The main listening socket of the server.
* 8. threads - The array of threads 
* 9. args array - An array of the threads arguments. 
* 10. max clients - The number of the max clients taht the server can serve.
* 11. num of junctions - The number of junctions in the road map.
* 12. ptr GPS semaphore - A pointer to the semaphore create by the server in order to signal the GPS the quit recieved by the user.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Closing_Program_Server (Junction* junctions , HANDLE* mutex_graph , HANDLE* mutex_file , int** graph_matrix ,
							 FILE *graph_file , FILE* output_file , SOCKET main_socket , HANDLE* threads ,
							 Single_Thread_Arg *args_array , int max_clients , int num_of_junctions , HANDLE *ptr_GPS_semaphore );


/*
* The function closes and frees all the resources allocated to the program.
*
* Input:
* -----
* 1. GPS Socket - The GPS socket we wish to close.
* 2. Server Socket - The server socket we wish to close.
* 3. data from server - The struct that contains the pointers to the matrix and array we allocated and wish to free.
* 4. close status - Signaled if we need to close and free the frogram with the sockets or not.
* 5. shortest path arry - A pointer to the array we allocated for the junctions.
* 6. Client File - A pointer to the client file we need to close.
* 7. GPS time - A pointer to the string we allocated to receive the GPS time.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Close_Program_Client ( SOCKET GPS_Socket , SOCKET Server_Socket , Data_From_Server data_from_server , Close_Status close_status ,
							int *shortest_path_arry , FILE* Client_File , char *GPS_time );


/*
* The function prints the current status of the client to the client LOG file and to the sdtout.
*
* Input:
* -----
* 1. current status - enum defined by us indicates that status that should be printed..
* 2. Client Log File - A pointer to the client LOG file.
* 3. junctions - The array of junctions needed to print.
* 4. shortest path array - The array of indexs that represents the shortest path route.
* 5. Source - The junction struct of the source.
* 6. GPS new time - The time that we received from the GPS. 
* 7. Route Length - The number of arcs that exist in the shortest path.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Print_Client_Mode ( Current_Status current_status , FILE* Client_Log_File , Junction *junctions , int *shortest_path_array , Junction Source ,
						int GPS_new_time , int Route_Length );

#endif