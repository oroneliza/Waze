/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the implementation of the functions that performs the searching junction, closing program and printing assignments.
*/


#include "General_Tools.h"


// constant strings that being printed in the server program. 
static const char* Print_Sentence[] = {"Recieived connection from client%d\n" ,
										"Sent the road map to client%d\n" ,	
										"Client %d reported %d %d %d %d %d new weight is %d\n" ,
										"Closed connection from client%d\n" }; 


// constant strings that being printed in the client program. 
const char* Client_Print_Status[] = {"Successfully logged into server\n" ,
									"Failed to connect to server\n" ,
									"Received the road map from server\n" ,
									"Bad coordinates\n" ,
									"Calculated path:" ,
									"Successfully logged into GPS\n" ,
									"Failed to connect to GPS\n" ,
									"GPS time at %d %d is %d\n" ,
									"Failed to receive time from GPS\n" ,
									"Failed to update server\n" ,
									"You have reached destination\n" ,
									"Failed to reach destination\n" };


int Check_If_Junction_Exist (Junction* junctions , Junction source , Junction destination , int * source_index , int* destination_index ,
	int Num_Of_Junctions )
{
	int i , flag_i = 0 , flag_j = 0 ;
	for ( i = 0 ; i < Num_Of_Junctions ; i++)
	{
		if ( ( junctions[i].x == source.x ) && ( junctions[i].y == source.y ) )
		{
			*source_index = i ;
			flag_i = 1 ;
		}
		if ( ( junctions[i].x == destination.x ) && ( junctions[i].y == destination.y ) )
		{
			*destination_index = i ;
			flag_j = 1 ;
		}
	}

	if ( ( flag_i == 1 ) && ( flag_j == 1 ) )
		return(0);
	else 
		return (1);

}


int Print_Graph_Into_Log_File ( FILE* output_file , int** graph_matrix , int num_of_junctions , Junction* junctions )
{
	int i = 0 , j = 0 , retval = 0 ;

	char* str = NULL ;
	char temp[12];

	str = (char*)malloc ( num_of_junctions * 12 * sizeof(char) );

	// prints the number of junctions
	fprintf(output_file , "%d\n" , num_of_junctions);
	if ( retval < 0)
	{
		printf("FATAL ERROR: fprintf() failed. Ending program");
		return(1);
	}

	// prints the junctions
	sprintf( str , "%d %d" , junctions[0].x , junctions[0].y); 
	for ( i = 1  ;  i < num_of_junctions  ;  i++ )
	{
		sprintf ( temp , " %d %d" ,  junctions[i].x , junctions[i].y);
		strcat ( str , temp );
	}
	strcat ( str , "\n");
	retval = fprintf( output_file , str );
	if ( retval <  0 )
	{
		printf("FATAL ERROR: fprintf() failed. Ending program");
		return(1);
	}
	
	// prints the road map
	for ( i = 0   ;   i < num_of_junctions   ;   i++  )
	{
		sprintf ( str , "%d" , graph_matrix[i][0] );
		for ( j = 1     ; j < num_of_junctions   ;   j++ )
		{
			sprintf ( temp , " %d" , graph_matrix[i][j]);
			strcat ( str , temp);

		}
		if ( i != num_of_junctions -1 )
		{
			strcat ( str , "\n" );
		}
		retval = fprintf( output_file , str );
		if ( retval < 0)
		{
			printf("FATAL ERROR: fprintf() failed. Ending program");
			return(1);
		}
	}
	free(str);
	return(0);
}


int Print_Server_Mode ( Server_Print_Mode server_print_mode , FILE* output_file , HANDLE* mutex_file, int client_serial_number , 
						Updated_Arc data_from_client , int new_weight  )
{
	int retval = 0 ;
	DWORD res = 0;

	res = WaitForSingleObject ( *mutex_file , INFINITE );
	if (res == WAIT_FAILED )
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
		return(1);
	}


	switch ( server_print_mode )
	{
	case SUCCESSFULLY_CONNECTED:
		printf(  Print_Sentence[SUCCESSFULLY_CONNECTED] , client_serial_number );
		retval = fprintf( output_file , Print_Sentence[SUCCESSFULLY_CONNECTED] , client_serial_number );
		if( retval < 0 )
		{
			printf("FATAL ERROR: fprintf() failed. Ending program");
			return(1);
		}
		break;

	case GRAPH_SENT:
		printf(  Print_Sentence[GRAPH_SENT] , client_serial_number);
		retval = fprintf( output_file , Print_Sentence[GRAPH_SENT] , client_serial_number );
		if( retval < 0 )
		{
			printf("FATAL ERROR: fprintf() failed. Ending program");
			return(1);
		}
		break;


	case UPDATED_ARC:
		printf(  Print_Sentence[UPDATED_ARC] , client_serial_number , data_from_client.source.x , data_from_client.source.y ,
											  data_from_client.destination.x , data_from_client.destination.y ,
									     	  data_from_client.delay , new_weight  );
		retval = fprintf( output_file ,  Print_Sentence[UPDATED_ARC] , client_serial_number , data_from_client.source.x , data_from_client.source.y ,
										 data_from_client.destination.x , data_from_client.destination.y ,
									     data_from_client.delay , new_weight  );
		if( retval < 0 )
		{
			printf("FATAL ERROR: fprintf() failed. Ending program");
			return(1);
		}
		break;

	case CLIENT_DICONNECTED:
		printf(  Print_Sentence[CLIENT_DICONNECTED] , client_serial_number);
		retval = fprintf( output_file , Print_Sentence[CLIENT_DICONNECTED] , client_serial_number );
		if( retval < 0 )
		{
			printf("FATAL ERROR: fprintf() failed. Ending program");
			return(1);
		}
		break;
		
	}

	res = ReleaseMutex( *mutex_file );
	if (res == 0 )
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program.\n " , GetLastError()  );
		return(1);
	}

	return(0);
}


int Closing_Program_Server (Junction* junctions , HANDLE* mutex_graph , HANDLE* mutex_file , int** graph_matrix ,
							 FILE *graph_file , FILE* output_file , SOCKET main_socket , HANDLE* threads ,
							 Single_Thread_Arg *args_array , int max_clients , int num_of_junctions , HANDLE *ptr_GPS_semaphore )
{
	int i = 0 , j = 0 ;
	DWORD res = 0;
	BOOL fatal_error_Flag = FALSE ;


	// close GPS by releasing semaphore
	if ( ReleaseSemaphore( *ptr_GPS_semaphore , max_clients , NULL ) == 0)
		fatal_error_Flag = TRUE;
	if ( CloseHandle( *ptr_GPS_semaphore ) == 0 )
		fatal_error_Flag = TRUE;

	//close socket
	if (closesocket (main_socket) == SOCKET_ERROR)
		fatal_error_Flag = TRUE;


	//close junctions
	free (junctions);


	// close road map
	for ( i = 0  ;  i < num_of_junctions  ;  i++ )
		free ( graph_matrix[i] );
	free( graph_matrix );


	//close threads
	for ( i = 0  ;  i < max_clients +1  ;  i++ )
	{
		if (threads[i] != NULL )
		{
			if (CloseHandle(threads[i]) == 0)
				fatal_error_Flag = TRUE;
		}
	}
	free(threads);


	//close arg array
	free (args_array);


	//close mutexes
	if ( CloseHandle( *mutex_graph ) == 0 ||   CloseHandle( *mutex_file ) == 0 )
		fatal_error_Flag = TRUE;


	//close files
	if ( fclose (graph_file ) == EOF   ||   fclose( output_file ) == EOF )
		fatal_error_Flag = TRUE;

	
	//close communication 
	if ( WSACleanup () == SOCKET_ERROR )
		fatal_error_Flag = TRUE;
	return ( fatal_error_Flag );
}


//The function receive the pointers and structs that being allocated and free them.
void Close_Args ( Data_From_Server data_from_server , int *shortest_path_array , char *GPS_time )
{
	int i ;
	for ( i = 0 ;  i < data_from_server.num_of_junctions  ;  i++)
	{
		if ( data_from_server.graph_matrix[i] != NULL )
			free( data_from_server.graph_matrix[i] );
	}
	if ( data_from_server.junctions != NULL )
		free ( data_from_server.junctions );
	if ( shortest_path_array != NULL )
		free ( shortest_path_array );
	if ( GPS_time != NULL )
		free ( GPS_time );
	return;
}


int Close_Program_Client ( SOCKET GPS_Socket , SOCKET Server_Socket , Data_From_Server data_from_server , Close_Status close_status ,
							int *shortest_path_arry , FILE* Client_File , char *GPS_time )
{
	int A = 0 , B = 0 , C = 0 , D  = 0;
	switch( close_status )
	{

	case PLUS_GPS_SERVER:
		if ( GPS_Socket != INVALID_SOCKET )
			A = closesocket( GPS_Socket );
		if ( Server_Socket != INVALID_SOCKET )
			B = closesocket( Server_Socket );
		C = WSACleanup();
		Close_Args( data_from_server , shortest_path_arry , GPS_time );
		if ( Client_File != NULL )
			D = fclose ( Client_File );
		if  ( ( A != 0) || ( B != 0 ) || ( C != 0 ) || ( D != 0 ) )
			return (1);
		return (0);

	case PLUS_SERVER:
		if ( Server_Socket != INVALID_SOCKET )
			B = closesocket( Server_Socket );
		C = WSACleanup();
		Close_Args( data_from_server , shortest_path_arry , GPS_time);
		if ( Client_File != NULL )
			D = fclose ( Client_File );
		if  ( ( B != 0 ) || ( C != 0 ) || ( D != 0 ) )
			return (1);
		return (0);

	case STAND_ALONE:
		C = WSACleanup();
		Close_Args( data_from_server , shortest_path_arry , GPS_time);
		if ( Client_File != NULL )
			D = fclose ( Client_File );
		if  ( ( C != 0 ) || ( D != 0 ) )
			return (1);
		return (0);
	}
	return (0);
}


int Print_Client_Mode ( Current_Status current_status , FILE* Client_Log_File , Junction *junctions , int *shortest_path_array , Junction Source ,
						int GPS_new_time , int Route_Length )
{
	int i , ret_val ;
	switch ( current_status )
	{

	case CALCULATED_PATH:
		printf( Client_Print_Status [current_status] );
		ret_val = fprintf( Client_Log_File , Client_Print_Status [current_status] );
		if ( ret_val < 0 )
			{
				printf("FATAL ERROR: fprintf() failed. Ending program");
				return (1);
			}
		printf( " %d %d" , Source.x , Source.y );
		ret_val = fprintf( Client_Log_File , " %d %d" , Source.x , Source.y );
		if ( ret_val < 0 )
			{
				printf("FATAL ERROR: fprintf() failed. Ending program");
				return (1);
			}
		for ( i = 0 ; i < Route_Length ; i++)
		{
			printf( " %d %d" , junctions [shortest_path_array[i] ].x , junctions [shortest_path_array[i] ].y );
			ret_val = fprintf( Client_Log_File , " %d %d" , junctions [shortest_path_array[i] ].x , junctions [shortest_path_array[i] ].y );
			if ( ret_val < 0 )
			{
				printf("FATAL ERROR: fprintf() failed. Ending program");
				return (1);
			}
		}
		printf("\n");
		ret_val = fprintf( Client_Log_File , "\n");
		if ( ret_val < 0 )
			{
				printf("FATAL ERROR: fprintf() failed. Ending program");
				return (1);
			}
		return(0);

	case GPS_TIME:
		printf( Client_Print_Status [current_status] , Source.x , Source.y , GPS_new_time );
		ret_val = fprintf( Client_Log_File ,  Client_Print_Status [current_status] , Source.x , Source.y , GPS_new_time );
		if ( ret_val < 0 )
			{
				printf("FATAL ERROR: fprintf() failed. Ending program");
				return (1);
			}
		return(0);

	default:
		printf( "%s" , Client_Print_Status [current_status] );
		ret_val = fprintf( Client_Log_File , "%s" , Client_Print_Status [current_status] );
		if ( ret_val < 0 )
			{
				printf("FATAL ERROR: fprintf() failed. Ending program");
				return (1);
			}
		return (0);
	}
}

