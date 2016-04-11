/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the implementation of the function that performs different tasks of the client assignment.
*/


#include "Client.h"

int Client_Func ( char* Arg_Ip_Address , int Source_x , int Source_y , int Destination_x , int Destination_y , char * Client_Log_File_Name)
{
	Junction Source;
	Junction Destination ;
	TransferResult_t RecvRes , send_res ;
	Updated_Arc updated_arc ;
	int  ret_val , i , num_of_arcs = 0 , delay , GPS_new_time = 0 , GPS_old_time = 0 , source_index , destination_index , error_server = 0 , 
			*shortest_path_array = NULL; 
	char server_ip_address [50] , *GPS_time = NULL ; 
	FILE *Client_File = NULL ; 
	SOCKET Server_Socket = INVALID_SOCKET , GPS_Socket  = INVALID_SOCKET;
	Data_From_Server  data_from_server ;
	data_from_server.graph_matrix = NULL ;
	data_from_server.junctions = NULL ;
	Source.x = Source_x ;
	Source.y = Source_y ;
	Destination.x = Destination_x ;
	Destination.y = Destination_y ;



	strcpy ( server_ip_address , Arg_Ip_Address);
	ret_val = Arguments_Checks_Client(  server_ip_address  );
	if ( ret_val == 1 )
	{
		return(1);
	}

	Client_File = fopen(Client_Log_File_Name , "w" );
	if ( Client_File == NULL )
	{
		printf( "FATAL ERROR : could't create a LOG client file\n" );
		return(1);
	}

	ret_val = Set_Up_Client ( &Server_Socket , server_ip_address , SERVER_PORT );
	if ( ret_val == 1 )
	{
		Print_Client_Mode ( FAILED_TO_CONNECT_TO_SERVER , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ; 
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , STAND_ALONE , shortest_path_array ,  Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	ret_val = Print_Client_Mode ( SUCCESSFULLY_LOGGED_TO_SERVER , Client_File , data_from_server.junctions ,
									shortest_path_array ,Source , GPS_new_time , 0 ) ;
	if ( ret_val == 1 )
	{
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_SERVER , shortest_path_array , Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	RecvRes = Client_Receiving_Data ( &data_from_server , Server_Socket ) ;
	if ( RecvRes != TRNS_SUCCEEDED )
	{
		Print_Client_Mode ( FAILED_TO_CONNECT_TO_SERVER , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ; 
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_SERVER , shortest_path_array , Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	ret_val = Print_Client_Mode ( RECEIVED_MAP_ROAD , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ;
	if ( ret_val == 1 )
	{
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_SERVER , shortest_path_array , Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	ret_val = Check_If_Junction_Exist ( data_from_server.junctions , Source , Destination , &source_index , &destination_index ,
										data_from_server.num_of_junctions ) ;
	if ( ret_val == 1 )
	{
		Print_Client_Mode ( BAD_COORDINATES , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ; 
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_SERVER , shortest_path_array , Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	shortest_path_array = (int*) malloc( data_from_server.num_of_junctions * sizeof(int) );
	if ( shortest_path_array == NULL )
	{
		printf( " FATAL ERROR: memory allocation failed\n");
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_SERVER , shortest_path_array , Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	num_of_arcs = Find_Shortest_Path ( data_from_server.graph_matrix , shortest_path_array , data_from_server.num_of_junctions , source_index ,
										destination_index );
	 if ( num_of_arcs == -1 )
	 {
		 ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_SERVER , shortest_path_array , Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	 }

	ret_val = Print_Client_Mode ( CALCULATED_PATH , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time ,
									num_of_arcs ) ;
	if ( ret_val == 1 )
	{
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_SERVER , shortest_path_array , Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	ret_val = Set_Up_Client ( &GPS_Socket , server_ip_address , GPS_PORT );
	if ( ret_val == 1 )
	{
		Print_Client_Mode ( FAILED_TO_CONNECT_TO_GPS , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ; 
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_SERVER , shortest_path_array ,  Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	ret_val = Print_Client_Mode ( SUCCESSFULLY_LOOGED_TO_GPS , Client_File , data_from_server.junctions ,
									shortest_path_array , Source , GPS_new_time , 0 ) ;
	if ( ret_val == 1 )
	{
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_GPS_SERVER ,
											shortest_path_array , Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	RecvRes = ReceiveString ( &GPS_time , GPS_Socket );
	if ( RecvRes != TRNS_SUCCEEDED )
	{
		Print_Client_Mode ( FAILED_TO_RECEIVE_TIME , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ;
		Print_Client_Mode ( FAILED_TO_REACH , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ;
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_GPS_SERVER , shortest_path_array ,  Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	GPS_new_time = atoi ( GPS_time );
	free ( GPS_time );
	GPS_time = NULL ;
	GPS_old_time = GPS_new_time;

	ret_val = Print_Client_Mode ( GPS_TIME , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ;
	if ( ret_val == 1 )
	{
		Print_Client_Mode ( FAILED_TO_REACH , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ;
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_GPS_SERVER ,
											shortest_path_array , Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	for ( i = 0 ; i < num_of_arcs ; i++ )
	{

		RecvRes = ReceiveString ( &GPS_time , GPS_Socket );
		if ( RecvRes != TRNS_SUCCEEDED )
		{
			Print_Client_Mode ( FAILED_TO_RECEIVE_TIME , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ;
			Print_Client_Mode ( FAILED_TO_REACH , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ;
			ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_SERVER , shortest_path_array ,  Client_File , GPS_time ) ;
			if ( ret_val == 1 )
				printf( "FATAL ERROR : could't close Assignments\n" );

			return(1);
		}

		GPS_new_time = atoi ( GPS_time );
		free ( GPS_time );
		GPS_time = NULL ;
		delay = GPS_new_time - GPS_old_time ;
		GPS_old_time = GPS_new_time ;

		ret_val = Print_Client_Mode ( GPS_TIME , Client_File , data_from_server.junctions , shortest_path_array , 
									data_from_server.junctions [ shortest_path_array[i] ] , GPS_new_time , 0 ) ;
		if ( ret_val == 1 )
		{
			Print_Client_Mode ( FAILED_TO_REACH , Client_File , data_from_server.junctions , shortest_path_array , Source , GPS_new_time , 0 ) ;
			ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_GPS_SERVER ,
											shortest_path_array , Client_File , GPS_time ) ;
			if ( ret_val == 1 )
				printf( "FATAL ERROR : could't close Assignments\n" );

			return(1);
		}

		if ( error_server == 1 )
			continue;

		if ( i == 0)
			updated_arc.source = Source ;

		else
			updated_arc.source = data_from_server.junctions [ shortest_path_array[i-1] ] ;
		updated_arc.destination = data_from_server.junctions [ shortest_path_array[i] ] ;
		updated_arc.delay = delay;

		send_res = Client_Sending_Data ( updated_arc , Server_Socket );
		if ( send_res != TRNS_SUCCEEDED )
		{
			ret_val = Print_Client_Mode ( FAILED_TO_UPDATE_SERVER , Client_File , data_from_server.junctions ,
										shortest_path_array , Source , GPS_new_time , 0 ) ;
			if ( ret_val == 1 )
			{
				ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_GPS_SERVER ,
											shortest_path_array , Client_File , GPS_time ) ;
				if ( ret_val == 1 )
					printf( "FATAL ERROR : could't close Assignments\n" );

				return(1);
			}

			error_server = 1 ;
		}

	}

	ret_val = Print_Client_Mode ( YOU_HAVE_REACHED , Client_File , data_from_server.junctions ,
										shortest_path_array , Source , GPS_new_time , 0 ) ;
	if ( ret_val == 1 )
	{
		ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_GPS_SERVER ,
											shortest_path_array , Client_File , GPS_time ) ;
		if ( ret_val == 1 )
			printf( "FATAL ERROR : could't close Assignments\n" );

		return(1);
	}

	ret_val = Close_Program_Client ( GPS_Socket , Server_Socket , data_from_server , PLUS_GPS_SERVER ,
											shortest_path_array , Client_File , GPS_time ) ;
	if ( ret_val == 1 )
	{
		printf( "FATAL ERROR : could't close Assignments\n" );
		return(1);
	}

	return (0);

}



	