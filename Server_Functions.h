/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the declarations of the functions that performs different tasks of the server assignment.
*/


#ifndef SERVER_THREAD_FUNC
#define SERVER_THREAD_FUNC

#include "Bacis_Types.h"
#include "communication_Tools.h"
#include "General_Tools.h"


/*
* The function creates the GPS semaphore anD creates the GPS process.
*
* Input:
* -----
* 1. max clients - The number of the max clients that the server can serve.
* 2. server ip address - The IP address of the server.
* 3. ptr GPS semaphore - A pointer to the semaphore create by the server in order to signal the GPS that quit recieved by the user.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Create_GPS_process (int max_clients , char* server_ip_address , HANDLE *ptr_GPS_semaphore ) ;


/*
* The function intialize: 1. The threads of the server (allocates memory for the array, runs the quit thread and intialize 
*							 all the client threads arguments.
*						  2. The two mutexes that protecting the graph matrix and the output file.
*
* Input:
* -----
* 1. ptr threads - A pointer to the array of threads (used as an additional output).
* 2. ptr args - A pointer to array of Single Thread Arg (used as an additional output).
* 3. max clients - The number of the max clients taht the server can serve.
* 4. ptr quit - A pointer to flag that indicates that "quit" has being entered by the user in the stdin. 
* 5. ptr mutex graph - A pointer to mutex used to protect from race conditions on the road map.
* 6. ptr mutex file -  A pointer to mutex used to protect from race conditions on the server log file.
* 7. junctions - The array of junctions in the road map.
* 8. num of junctions - The num of junctions as written in the grpah file
* 9. output file - A pointer to the server log file.
* 10. ptr graph matrix - A pointer to the matrix holding the Traffic congestion of the road map.
* 11. ptr failure - A pointer to flag that indicates if a fatal error occurred somewhere in the server program. 
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Initialize_Threads_And_Mutex ( HANDLE** ptr_threads , Single_Thread_Arg **ptr_args , int max_clients ,  BOOL* ptr_quit ,
								   HANDLE *ptr_mutex_graph , HANDLE *ptr_mutex_file , Junction *junctions , int num_of_junctions ,
								   FILE* output_file , int*** ptr_graph_matrix , BOOL* ptr_failure );


/*
* The function finds the first available thread in order to serve client that were accepted.
*
* Input:
* -----
* 1. threads - The array of threads. 
* 2. max clients - The number of the max clients that the server can serve.

* Output:
* -----
* 1. integer - The index in the array of the first available thread.
			   (If the return value is max_clients+1 it indicate that all theard are occupied).
*/
int Find_First_Unused_Theard_Slot( HANDLE *threads , int max_clients );


/*
* The function runs a single thread that will communicate with a single client.
*
* Input:
* -----
* 1. (*func)(Single_Thread_Arg*) - The function that the thread will perform. The func detailed below.
* 2. arg - A pointer to the struct of argument that the thread need. Detailed in Basic Types module.
*
* Output:
* -----
* 1. HANDLE to the thread. The returend value is NULL if the function fails.
*/
HANDLE Run_Single_Client_Thread ( int (*func)(Single_Thread_Arg*) , Single_Thread_Arg  *arg);


/*
* The function checks if one or more threads finished running due FATAL ERROR.
*
* Input:
* -----
* 1. threads - The array of threads
* 2. max clients - The number of the max clients taht the server can serve.
*
* Output:
* -----
* 1. BOOL - TRUE if a FATAL ERROR occured in one of the thread and FALSE otherwise.
*/
BOOL Check_For_Failed_Threads ( HANDLE* threads , int max_clients );


/*
* The function perform the single client communication assignment. 
*
* Input:
* -----
* 1. arg - A pointer to the struct of argument that the thread need. Detailed in Basic Types module.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Single_Client_Func ( Single_Thread_Arg   *arg );


#endif