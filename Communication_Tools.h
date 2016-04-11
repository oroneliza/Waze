/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the declarations of the functions that performs different tasks to establish and maintain the connection between 
*				the server and the clients.
*/


#ifndef COMMUNICATION_TOOLS
#define  COMMUNICATION_TOOLS

#include "Bacis_Types.h"


/*
* The function bind the socket to a specific ip address and port.
*
* Input:
* -----
* 1. s - A pointer to the socket we wish to bind.
* 2. server ip address - The IP address of the server as a string.
* 3. port number - The port number we wish to bind the socket.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Bind_Func( SOCKET* s , char *server_ip_address , int port_number );


/*
* The function connect the socket to a specific ip address and port.
*
* Input:
* -----
* 1. s - A pointer to the socket we wish to connect.
* 2. server ip address - The IP address of the server as a string.
* 3. port number - The port number we wish to connect the socket.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Connect_Func ( SOCKET* s , char *server_ip_address , int port_number );


/*
* The function create the main server socket and force him to listen to a specific port number and ip address. 
*
* Input:
* -----
* 1. main socket - A pointer to the socket we wish to create.
* 2. server ip address - The IP address of the server as a string.
* 3. max clients - The maximum number of clients we wish the server will serve.
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Set_Up_Server ( SOCKET* main_socket , char *server_ip_address , int max_clinets );


/*
* The function create a client socket and connect him to a specific port number and ip address.
*
* Input:
* -----
* 1. main socket - A pointer to the socket we wish to create..
* 2. server ip address - The IP address of the server as a string.
* 3. port number - The port number we wish to connect the socket.

* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occured and 0 otherwise .
*/
int Set_Up_Client ( SOCKET* main_socket , char *server_ip_address , int port_number );


/*
* The function send a string through a specific socket and informs if the task succeeded.
*
* Input:
* -----
* 1. str - The string we wish to send.
* 2. sd - The socket we wish to send the string through.
*
* Output:
* -----
* 1. TransferResult_t - Returns the result of the send request.
*/
TransferResult_t SendString( const char *Str, SOCKET sd );


/*
* The function receive a string through a specific socket and informs if the task succeeded.
*
* Input:
* -----
* 1. OutputStrPtr - A pointer to the string we wish to receive.
* 2. sd - The socket we wish to receive the string through.
*
* Output:
* -----
* 1. TransferResult_t - Returns the result of the receive request.
*/
TransferResult_t ReceiveString( char** OutputStrPtr, SOCKET sd );


/*
* The function send a struct of Updated Arc through a specific socket and informs if the task succeeded.
*
* Input:
* -----
* 1. updated arc - The struct we wish to send.
* 2. sd - The socket we wish to send the struct through.
*
* Output:
* -----
* 1. TransferResult_t - Returns the result of the send request.
*/
TransferResult_t Client_Sending_Data ( Updated_Arc updated_arc , SOCKET sd );


/*
* The function receive a struct of Data From Server through a specific socket and informs if the task succeeded.
*
* Input:
* -----
* 1. data from server - A pointer to the struct we wish to receive.
* 2. sd - The socket we wish to receive the struct through.
*
* Output:
* -----
* 1. TransferResult_t - Returns the result of the receive request.
*/
TransferResult_t Client_Receiving_Data ( Data_From_Server *data_from_server , SOCKET sd );


/*
* The function send a struct of data from server through a specific socket and informs if the task succeeded.
*
* Input:
* -----
* 1. data from server - The struct we wish to send.
* 2. sd - The socket we wish to send the struct through.
*
* Output:
* -----
* 1. TransferResult_t - Returns the result of the send request.
*/
TransferResult_t Server_Sending_Data ( Data_From_Server data_from_server , SOCKET sd );


/*
* The function receive a struct of updated arc through a specific socket and informs if the task succeeded.
*
* Input:
* -----
* 1. updated arc - A pointer to the struct we wish to receive.
* 2. sd - The socket we wish to receive the struct through.
*
* Output:
* -----
* 1. TransferResult_t - Returns the result of the receive request.
*/
TransferResult_t Server_Receiving_Data ( Updated_Arc *updated_arc , SOCKET sd );


#endif