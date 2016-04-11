/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the implementation of the functions that performs different tasks of the server assignment.
*/


#include "Server_Functions.h"


int Create_GPS_process (int max_clients , char* server_ip_address , HANDLE *ptr_GPS_semaphore ) 
{
	PROCESS_INFORMATION proc_info;
	SECURITY_ATTRIBUTES security;
	char command_line[50];
	BOOL retval = 0;
	STARTUPINFO	startinfo = { sizeof( STARTUPINFO ), NULL, 0 }; 

	security.nLength = sizeof(security);
	security.lpSecurityDescriptor = NULL ;
	security.bInheritHandle = TRUE ;
	*ptr_GPS_semaphore = CreateSemaphore(&security , 0 , max_clients , GPS_SEMAPHORE_NAME );
	if ( *ptr_GPS_semaphore == NULL )
	{
		printf("FATAL ERROR: CreatSemaphore() failed, error code: %d.\n" , GetLastError () );
		return(1);
	}
		
	sprintf (command_line , "%s %s %d" , GPS_PROCESS_NAME , server_ip_address , max_clients); 
	retval = CreateProcess( NULL, command_line, NULL, NULL, FALSE, CREATE_NEW_CONSOLE,	    
							NULL, NULL, &startinfo,	&proc_info );
	if (retval == 0 )
	{
		printf("FATAL ERROR: CreateProcess() failed, error code: %d.\n" , GetLastError () );
		CloseHandle(*ptr_GPS_semaphore);
		return(1);
	}
	return (0);
}


//Performs the reading "quit" from stdin task. Is being called by the Initialize_Threads_And_Mutex function.
void Quit_Thread ( BOOL* ptr_quit )
{
	char line [6];
	while ( *ptr_quit  == FALSE )
	{
		gets (line);
		if ( strcmp(line , "quit") == 0 )
			*ptr_quit = TRUE;
	}
	return ;
}


// Creates an handle for a single thread running the Quit Func.
HANDLE Run_Quit_Single_Thread ( void (*func)(BOOL*) , BOOL *ptr_quit)
{
	return CreateThread ( NULL  ,  0  ,  (LPTHREAD_START_ROUTINE)func  ,  ptr_quit  ,  0  ,  NULL  );
}


HANDLE Run_Single_Client_Thread ( int (*func)(Single_Thread_Arg*) , Single_Thread_Arg  *arg)
{
	return CreateThread ( NULL  ,  0  ,  (LPTHREAD_START_ROUTINE)func  ,  arg  ,  0  ,  NULL  );
}


int Initialize_Threads_And_Mutex ( HANDLE** ptr_threads , Single_Thread_Arg **ptr_args , int max_clients ,  BOOL* ptr_quit ,
								   HANDLE *ptr_mutex_graph , HANDLE *ptr_mutex_file ,Junction *junctions , int num_of_junctions ,
								   FILE* output_file , int*** ptr_graph_matrix , BOOL* ptr_failure )
{
	int i = 0;
	*ptr_threads = (HANDLE*)malloc ( ( max_clients + 1 ) * sizeof(HANDLE) );
	*ptr_args = (Single_Thread_Arg*)malloc( ( max_clients + 1 ) * sizeof(Single_Thread_Arg) );
	if ( *ptr_threads == NULL || *ptr_args == NULL  )
	{
		printf("FATAL ERROR : Memory allocation failed.\n");
		return(1);
	}

	(*ptr_threads) [0] = Run_Quit_Single_Thread ( Quit_Thread, ptr_quit ) ;
	if ( (*ptr_threads) [0] == NULL )
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
		return(1);
	}
	for ( i = 1   ;   i < max_clients + 1   ;   i++  )
	{
		(*ptr_threads)[i] = NULL ;
		(*ptr_args)[i].junctions = junctions;
		(*ptr_args)[i].mutex_file = ptr_mutex_file;
		(*ptr_args)[i].mutex_graph = ptr_mutex_graph;
		(*ptr_args)[i].num_of_junctions = num_of_junctions;
		(*ptr_args)[i].output_file = output_file ;
		(*ptr_args)[i].ptr_graph_matrix = ptr_graph_matrix;
		(*ptr_args)[i].ptr_quit = ptr_quit;
		(*ptr_args)[i].ptr_failure = ptr_failure;
	}

	*ptr_mutex_graph = CreateMutex(NULL , 0 ,NULL);
	if ( *ptr_mutex_graph == NULL )
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
		return(1);
	}
		
	*ptr_mutex_file = CreateMutex(NULL , 0 ,NULL);
	if ( *ptr_mutex_file == NULL )
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
		return(1);
	}
	
	return (0);
}


int Find_First_Unused_Theard_Slot( HANDLE *threads , int max_clients )
{
	int i = 0 ;
	DWORD wait_res = 0;

	for ( i = 1   ;   i < max_clients + 1   ;   i++ )
	{
		if ( threads[i] == NULL )
			return (i);
		else
		{
			wait_res = WaitForSingleObject ( threads[i] , 0 );
			if (wait_res == WAIT_OBJECT_0)
			{
				CloseHandle(threads[i]);
				threads[i] = NULL ;
				break;
			}
		}
	}
	return (i);
}


//Prepare the data that the server should send the client at the beginning of thier communication.
// It copies the graph matrix in order to release it as quickly as possible and not holdint it in all the sending data process.
//(The memory allocation free is done after sending the data to client).
int Preparing_Send_Data ( Data_From_Server *ptr_data_from_server , int num_of_junctions , Junction* junctions ,
						  int** original_graph , HANDLE *mutex_graph )
{
	int i = 0 , j = 0 ;
	DWORD res = 0;

	ptr_data_from_server->graph_matrix = (int**)malloc( num_of_junctions * sizeof(int*) );
	if ( ptr_data_from_server->graph_matrix == NULL)
	{
		printf("FATAL ERROR : Memory allocation failed.\n");
		return(1);
	}
	for ( i = 0  ;  i < num_of_junctions  ; i++ )
	{
		  (ptr_data_from_server->graph_matrix)[i] = (int*)malloc (num_of_junctions * sizeof(int) );
		if ( (ptr_data_from_server->graph_matrix)[i] == NULL )
		{
			printf("FATAL ERROR : Memory allocation failed.\n");
			return(1);
		}
	}

	res = WaitForSingleObject( *mutex_graph , INFINITE );
	if (res == WAIT_FAILED )
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
		return(1);
	}

	for ( i = 0  ;  i < num_of_junctions  ;  i++ )
	{
		for ( j = 0  ;  j < num_of_junctions  ;  j++ )
			 (ptr_data_from_server->graph_matrix)[i][j] = original_graph[i][j];
	}
	res = ReleaseMutex( *mutex_graph );
	if (res == 0 )
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
		return(1);
	}

	ptr_data_from_server->num_of_junctions = num_of_junctions;
	ptr_data_from_server->junctions = junctions;
	return(0);
}


BOOL Check_For_Failed_Threads ( HANDLE* threads , int max_clients )
{
	int i = 0 ;
	DWORD   exit_code = 0 ;
	BOOL success = 0 ;

	for ( i = 1  ;  i < max_clients + 1   ;   i++ )
	{
		if (threads[i] != NULL )
		{
			success = GetExitCodeThread ( threads[i] , &exit_code );
			if ( exit_code == 1 || success == 0 )
			{
				if (success == 0)
				{
					printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
					return (TRUE);
				}
			}
		}
	}
	return(FALSE);
}


int Single_Client_Func ( Single_Thread_Arg   *arg )
{
	int retval = 0 , source = 0 , destination = 0 , new_weight = 0;
	DWORD res = 0 ;
	TransferResult_t send_res, recv_res;
	Data_From_Server data_from_server;
	Updated_Arc data_from_client = { {0,0} , {0,0} , 0 };

	retval = Print_Server_Mode ( SUCCESSFULLY_CONNECTED , arg->output_file , arg->mutex_file , arg->client_serial_number ,
							     data_from_client , 0 );
	if (retval == 1 )
	{
		*(arg->ptr_failure) = TRUE;
		closesocket ( arg->s);
		return(1);
	}
									
	retval = Preparing_Send_Data (&data_from_server ,arg->num_of_junctions , arg->junctions , *(arg->ptr_graph_matrix) , arg->mutex_graph);
	if(retval == 1)
	{
		*(arg->ptr_failure) = TRUE;
		closesocket ( arg->s);
		free (data_from_server.graph_matrix);
		return(1);
	}

	send_res = Server_Sending_Data ( data_from_server , arg->s );
	if ( send_res == SETUP_PROBLEM   ||   send_res == TRNS_FAILED )
	{
		*(arg->ptr_failure) = TRUE;
		if ( send_res == SETUP_PROBLEM)
			printf("FATAL ERROR: Memory allocation failed.\n");
		else
			printf("FATAL ERROR: Service socket error while writing, closing thread.\n");
		closesocket ( arg->s );
		free (data_from_server.graph_matrix);
		return(1);
	}
	free (data_from_server.graph_matrix);
	
	retval = Print_Server_Mode ( GRAPH_SENT , arg->output_file , arg->mutex_file , arg->client_serial_number ,
							     data_from_client , 0 );
	if (retval == 1 )
	{
		*(arg->ptr_failure) = TRUE;
		closesocket ( arg->s);
		return(1);
	}

	while(  *(arg->ptr_quit) == FALSE   &&   *(arg->ptr_failure) == FALSE  ) 
	{		
		recv_res = Server_Receiving_Data(  &data_from_client ,  arg->s  );
		if ( recv_res == SETUP_PROBLEM   ||   recv_res == TRNS_FAILED )
		{
			*(arg->ptr_failure) = TRUE;
			if ( recv_res == SETUP_PROBLEM)
				printf("FATAL ERROR: Memory allocation failed.\n");
			else
				printf("FATAL ERROR: Service socket error while reading, closing thread.\n");
			closesocket (arg->s);
			return(1);
		}
		if (recv_res == TRNS_DISCONNECTED)
		{
			retval = Print_Server_Mode ( CLIENT_DICONNECTED , arg->output_file , arg->mutex_file , arg->client_serial_number ,
							     data_from_client , 0 );
			if (retval == 1 )
			{
				*(arg->ptr_failure) = TRUE;
				closesocket (arg->s);
				return(1);
			}

			retval = closesocket ( arg->s);
			if ( retval == SOCKET_ERROR )
			{
				*(arg->ptr_failure) = TRUE;
				printf("Error at closesocket(): %ld.\n" , WSAGetLastError() );
				return(1);
			}
			return(0);
		}
		
		Check_If_Junction_Exist (arg->junctions , data_from_client.source , data_from_client.destination , &source , &destination , arg->num_of_junctions );
		res = WaitForSingleObject ( *(arg->mutex_graph) , INFINITE);
		if (res == WAIT_FAILED )
		{
			*(arg->ptr_failure) = TRUE;
			printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
			closesocket ( arg->s);
			return(1);
		}

		new_weight = ceil( 0.75 * (  *(arg->ptr_graph_matrix)  )[source][destination] + 
						   0.25 * data_from_client.delay  );

		(  *(arg->ptr_graph_matrix)  )[source][destination] = new_weight;
		(  *(arg->ptr_graph_matrix)  )[destination][source] = new_weight;
		res = ReleaseMutex( *(arg->mutex_graph) );
		if (res == 0 )
		{
			*(arg->ptr_failure) = TRUE;
			printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
			closesocket ( arg->s);
			return(1);
		}
		retval = Print_Server_Mode ( UPDATED_ARC , arg->output_file , arg->mutex_file , arg->client_serial_number ,
							     data_from_client , new_weight );
		if (retval == 1 )
		{
			*(arg->ptr_failure) = TRUE;
			closesocket ( arg->s);
			return(1);
		}
	}

	retval = closesocket ( arg->s);
	if ( *(arg->ptr_failure) == TRUE )
		return(1);
	else
	{
		if ( retval == SOCKET_ERROR )
		{
			*(arg->ptr_failure) = TRUE;
			printf("Error at closesocket(): %ld.\n" , WSAGetLastError() );
			return(1);
		}
		return(0);
	}
		
}



