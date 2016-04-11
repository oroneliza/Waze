/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the implementation of the function that performs the server assignment.
*/


#include "Server.h"

int Server_Func ( char *arg_ip_address , char *graph_file_name ,  double arg_max_clients , char *server_log_file_name)
{
	Junction *junctions = NULL;
	HANDLE mutex_graph = NULL, mutex_file = NULL ; 
	HANDLE GSP_semaphore = NULL ;
	int** graph_matrix = NULL ;
	char server_ip_address[50]; 
	int max_clients = 0 ,num_of_junctions = 0 , retval = 0 , Int = 0 , client_serial_number = 1 , i = 0;
	FILE* graph_file = NULL , *output_file = NULL ;
	SOCKET main_socket = INVALID_SOCKET , accept_socket = INVALID_SOCKET;
	HANDLE *threads = NULL ;
	BOOL quit = FALSE , failure = FALSE ; 
	FD_SET read_set;
	struct timeval select_timeout ;
	Single_Thread_Arg *args_array = NULL ;
	DWORD wait_res = 0 , exit_code_quit = 0 ;


	strcpy (server_ip_address , arg_ip_address);
	retval = Arguments_Checks_Server( graph_file_name , &graph_file , arg_max_clients , server_ip_address  );
	if (retval == 1)
	{
		return(1);
	}
	
	max_clients = (int)arg_max_clients;
	output_file = fopen ( server_log_file_name , "w");
	if ( output_file == NULL )
	{
		printf("FATAL ERROR : Opening file failed. Ending program.\n");
		fclose(graph_file);
		return(1);
	}

	retval = Create_GPS_process (max_clients , server_ip_address , &GSP_semaphore );
	if (retval == 1)
	{
		fclose(graph_file);
		fclose(output_file);
		return(1);
	}
	
	retval = Convert_Graphtext_To_Matrices( graph_file , &junctions , &graph_matrix ,  &num_of_junctions );
	if (retval == 1)
	{
		Closing_Program_Server (junctions , &mutex_graph , &mutex_file , graph_matrix ,
							    graph_file , output_file , main_socket , threads ,
							    args_array ,max_clients , num_of_junctions , &GSP_semaphore );
		return (1);
	}

	retval = Initialize_Threads_And_Mutex ( &threads , &args_array , max_clients , &quit , &mutex_graph , &mutex_file ,
											junctions , num_of_junctions , output_file , &graph_matrix , &failure );
	if (retval == 1)
	{
		Closing_Program_Server (junctions , &mutex_graph , &mutex_file , graph_matrix ,
							    graph_file , output_file , main_socket , threads ,
							    args_array ,max_clients , num_of_junctions , &GSP_semaphore );
		return(1);
	}
	
	retval = Set_Up_Server ( &main_socket , server_ip_address , max_clients );
	if (retval == 1)
	{
		Closing_Program_Server (junctions , &mutex_graph , &mutex_file , graph_matrix ,
							    graph_file , output_file , main_socket , threads ,
							    args_array ,max_clients , num_of_junctions , &GSP_semaphore );
		return(1);
	}
	


	select_timeout.tv_sec = 1;
	select_timeout.tv_usec = 0;
	
	while ( quit == FALSE  &&  failure == FALSE  )
	{
		FD_ZERO ( &read_set );
		FD_SET ( main_socket  ,  &read_set  );
		retval = select ( 0 , &read_set , NULL , NULL , &select_timeout );
		if (retval == SOCKET_ERROR)
		{
			printf("FATAL ERROR: select() failed, error code: %d.\n" , GetLastError () ); 
			failure = TRUE;
			break;
		}
		else if( retval == 0 )
			continue;

		accept_socket = accept ( main_socket , NULL , NULL );
		if (accept_socket == INVALID_SOCKET )
		{
			printf("FATAL ERROR: Accepting connection with client failed, error code %ld.\n" , WSAGetLastError() );
			failure = TRUE;
			break;
		}

		Int = Find_First_Unused_Theard_Slot( threads , max_clients );
		if ( Int == max_clients + 1  )
		{
			printf("No slots available for client , dropping the connection.\n");
			retval = closesocket( accept_socket );
			if ( retval == SOCKET_ERROR )
			{
				printf("Error at closesocket(): %ld.\n" , WSAGetLastError() );
				failure = TRUE;
				break;	
			}
			continue;
		}
		else
		{
			args_array[Int].s = accept_socket;
			args_array[Int].client_serial_number = client_serial_number;
			threads[Int] = Run_Single_Client_Thread ( Single_Client_Func , &(args_array[Int]) );
			if ( threads[Int] == NULL )
			{
				printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
				failure = TRUE;
				break;
			}
			client_serial_number++ ;
		}
		failure = Check_For_Failed_Threads ( threads , max_clients );
	}

	for ( i = 1   ;   i < max_clients + 1   ;   i++ )
	{
		if (threads[i] != NULL )
		{
			wait_res = WaitForSingleObject ( threads[i] , INFINITE );
			if (wait_res == WAIT_FAILED )
			{
				failure = TRUE;
				printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
			}
		}
	}


	if ( failure == TRUE )
	{
		TerminateThread ( threads[0] , exit_code_quit );
		Closing_Program_Server (junctions , &mutex_graph , &mutex_file , graph_matrix ,
								graph_file , output_file , main_socket , threads ,
								args_array ,max_clients , num_of_junctions , &GSP_semaphore );
		return(1);
	}
	
	retval = Print_Graph_Into_Log_File ( output_file , graph_matrix , num_of_junctions , junctions );
	if( retval == 1 )
	{
		Closing_Program_Server (junctions , &mutex_graph , &mutex_file , graph_matrix ,
								graph_file , output_file , main_socket , threads ,
								args_array ,max_clients , num_of_junctions , &GSP_semaphore );
				
		return (1);
	}
	retval = Closing_Program_Server (junctions , &mutex_graph , &mutex_file , graph_matrix ,
							     	 graph_file , output_file , main_socket , threads ,
							         args_array ,max_clients , num_of_junctions , &GSP_semaphore );
	if(retval == 1)
		return(1);
	return(0);
}