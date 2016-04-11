/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the implementation of the functions that performs different tasks to establish and maintain the connection between 
*				the server and the clients.
*/


#include "communication_Tools.h"

int Bind_Func( SOCKET* s , char *server_ip_address , int port_number )
{
	SOCKADDR_IN service ;
	int bind_res = 0 ;
	
	service.sin_addr.s_addr = inet_addr ( server_ip_address );
	if ( service.sin_addr.s_addr == INADDR_NONE)
	{
		printf("FATAL ERROR: The string \"%s\" cannot be converted into an ip address. Ending program.\n" ,server_ip_address  );
		return(1);
	}
	service.sin_family = AF_INET;
	service.sin_port = htons (port_number);

	bind_res = bind (*s , (SOCKADDR*)&service , sizeof(service) );
	if (bind_res == SOCKET_ERROR)
	{
		printf("bind() failed with error %d. Ending program.\n",WSAGetLastError() );
		return (1);
	}
	return(0);
}

int Connect_Func ( SOCKET* s , char *server_ip_address , int port_number )
{
	SOCKADDR_IN clientService ; 
	int connect_res = 0;

	clientService.sin_addr.s_addr = inet_addr ( server_ip_address);
	if ( clientService.sin_addr.s_addr == INADDR_NONE)
	{
		printf("FATAL ERROR: The string \"%s\" cannot be converted into an ip address. Ending program.\n" ,server_ip_address  );
		return(1);
	}
	clientService.sin_family = AF_INET;
	clientService.sin_port = htons (port_number);

	connect_res = connect (*s ,  (SOCKADDR*)&clientService , sizeof(clientService) );
	if (connect_res == SOCKET_ERROR)
	{
		printf("connect() failed with error %d. Ending program.\n",WSAGetLastError() );
		return (1);
	}
	return(0);
}

int Set_Up_Client ( SOCKET* ptr_main_socket , char *server_ip_address , int port_number )
{
	WSADATA wsaData;
	int retval = 0 ;

	retval = WSAStartup (MAKEWORD(2,2) , &wsaData );
	if (retval != NO_ERROR)
	{
		printf ( "Error %ld at WSAStartup() , ending program.\n" , WSAGetLastError() );
		return (1);
	}

	*ptr_main_socket = socket ( AF_INET , SOCK_STREAM , IPPROTO_TCP);
	if (*ptr_main_socket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld.\n" , WSAGetLastError() );
		return(1);
	}

	retval = Connect_Func ( ptr_main_socket , server_ip_address , port_number );
	if (retval == 1)
		return (1);
	return(0);
}

int Set_Up_Server ( SOCKET* ptr_main_socket , char *server_ip_address , int max_clinets )
{
	WSADATA wsaData;
	int retval = 0 ;

	retval = WSAStartup (MAKEWORD(2,2) , &wsaData );
	if (retval != NO_ERROR)
	{
		printf ( "Error %ld at WSAStartup() , ending program.\n" , WSAGetLastError() );
		return (1);
	}

	*ptr_main_socket = socket ( AF_INET , SOCK_STREAM , IPPROTO_TCP);
	if (*ptr_main_socket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld.\n" , WSAGetLastError() );
		return(1);
	}

	retval = Bind_Func ( ptr_main_socket , server_ip_address , SERVER_PORT );
	if (retval == 1)
		return (1);
	
	retval = listen (  *ptr_main_socket , max_clinets  );
	if( retval == SOCKET_ERROR)
	{
		printf("Failed listening on socket, error %ld.\n" , WSAGetLastError() );
		//close program
		return(1);
	}
	return(0);
}

//The function send an array of char through a specific socket and informs if the task succeeded.
TransferResult_t SendBuffer( const char* Buffer, int BytesToSend, SOCKET sd )
{
	const char* CurPlacePtr = Buffer;
	int BytesTransferred;
	int RemainingBytesToSend = BytesToSend;
	
	while ( RemainingBytesToSend > 0 )  
	{
		BytesTransferred = send (sd, CurPlacePtr, RemainingBytesToSend, 0);
		if ( BytesTransferred == SOCKET_ERROR ) 
		{
			printf("send() failed, error %d\n", WSAGetLastError() );
			return TRNS_FAILED;
		}
		
		RemainingBytesToSend -= BytesTransferred;
		CurPlacePtr += BytesTransferred; 
	}

	return TRNS_SUCCEEDED;
}

TransferResult_t SendString( const char *Str, SOCKET sd )
{
	int TotalStringSizeInBytes;
	TransferResult_t SendRes;
		
	TotalStringSizeInBytes = (int)( strlen(Str) + 1 ); 

	SendRes = SendBuffer( 
		(const char *)( &TotalStringSizeInBytes ),
		(int)( sizeof(TotalStringSizeInBytes) ), 
		sd );

	if ( SendRes != TRNS_SUCCEEDED ) return SendRes ;

	SendRes = SendBuffer( 
		(const char *)( Str ),
		(int)( TotalStringSizeInBytes ), 
		sd );

	return SendRes;
}

//The function receive an array of char through a specific socket and informs if the task succeeded.
TransferResult_t ReceiveBuffer( char* OutputBuffer, int BytesToReceive, SOCKET sd )
{
	char* CurPlacePtr = OutputBuffer;
	int BytesJustTransferred;
	int RemainingBytesToReceive = BytesToReceive;
	
	while ( RemainingBytesToReceive > 0 )  
	{
		BytesJustTransferred = recv(sd, CurPlacePtr, RemainingBytesToReceive, 0);
		if ( BytesJustTransferred == SOCKET_ERROR ) 
		{
			printf("recv() failed, error %d\n", WSAGetLastError() );
			return TRNS_FAILED;
		}		
		else if ( BytesJustTransferred == 0 )
			return TRNS_DISCONNECTED; 

		RemainingBytesToReceive -= BytesJustTransferred;
		CurPlacePtr += BytesJustTransferred; 
	}

	return TRNS_SUCCEEDED;
}

TransferResult_t ReceiveString( char** OutputStrPtr, SOCKET sd )
{
	int TotalStringSizeInBytes;
	TransferResult_t RecvRes;
	char* StrBuffer = NULL;

	if ( ( OutputStrPtr == NULL ) || ( *OutputStrPtr != NULL ) )
	{
		printf("The first input to ReceiveString() must be " 
			   "a pointer to a char pointer that is initialized to NULL. For example:\n"
			   "\tchar* Buffer = NULL;\n"
			   "\tReceiveString( &Buffer, ___ )\n" );
		return TRNS_FAILED;
	}
		
	RecvRes = ReceiveBuffer( 
		(char *)( &TotalStringSizeInBytes ),
		(int)( sizeof(TotalStringSizeInBytes) ),
		sd );

	if ( RecvRes != TRNS_SUCCEEDED ) return RecvRes;

	StrBuffer = (char*)malloc( TotalStringSizeInBytes * sizeof(char) );

	if ( StrBuffer == NULL )
		return TRNS_FAILED;

	RecvRes = ReceiveBuffer( 
		(char *)( StrBuffer ),
		(int)( TotalStringSizeInBytes), 
		sd );

	if ( RecvRes == TRNS_SUCCEEDED ) 
		{ *OutputStrPtr = StrBuffer; }
	else
	{
		free( StrBuffer );
	}
		
	return RecvRes;
}

TransferResult_t Client_Sending_Data ( Updated_Arc updated_arc , SOCKET sd )
{
	int total_buffer_length = 5 , *buffer_ptr = NULL;
	TransferResult_t send_res ; 

	buffer_ptr = (int*)malloc( total_buffer_length * sizeof(int) );
	if (buffer_ptr == NULL )
	{
		printf("FATAL ERROR: Memory allocation failed.\n");
		return(SETUP_PROBLEM);
	}

	buffer_ptr[0] = updated_arc.source.x;
	buffer_ptr[1] = updated_arc.source.y;
	buffer_ptr[2] = updated_arc.destination.x;
	buffer_ptr[3] = updated_arc.destination.x;
	buffer_ptr[4] = updated_arc.delay;

	send_res = SendBuffer ( 
			(const char*)(buffer_ptr) ,
			(int)(total_buffer_length * sizeof(int) ) ,
			sd);
	free(buffer_ptr);
	return(send_res);
	
}

TransferResult_t Client_Receiving_Data ( Data_From_Server *data_from_server , SOCKET sd )
{
	int total_buffer_length , i = 0 , j , z , *buffer_ptr = NULL;
	TransferResult_t rec_res ; 
	rec_res = ReceiveBuffer(
				(char*)( &total_buffer_length),
				 (int) ( sizeof(total_buffer_length) ),
				 sd );
	if ( rec_res != TRNS_SUCCEEDED ) 
		return rec_res;
	buffer_ptr = (int*) malloc(total_buffer_length * sizeof(int) ) ;
	if ( buffer_ptr == NULL )
	{
		printf("FATAL ERROR: Memory allocation failed.\n");
		return(SETUP_PROBLEM);
	}
	rec_res = ReceiveBuffer(
				(char*)( buffer_ptr),
				 (int) ( total_buffer_length * sizeof(int) ),
				 sd );
	if ( rec_res != TRNS_SUCCEEDED ) 
	{
		free ( buffer_ptr);
		return rec_res;
	}
	data_from_server ->num_of_junctions = buffer_ptr[i];
	i++;
	data_from_server ->junctions = (Junction*) malloc(  data_from_server ->num_of_junctions * sizeof(Junction)  );
	if ( data_from_server ->junctions == NULL )
	{
		printf("FATAL ERROR: Memory allocation failed.\n");
		free ( buffer_ptr);
		return(SETUP_PROBLEM);
	}
	for ( j = 0 ; j < data_from_server ->num_of_junctions ; j++)
	{
		data_from_server ->junctions[j].x = buffer_ptr[i];
		data_from_server ->junctions[j].y = buffer_ptr[i + 1];
		i = i + 2;
	}
	data_from_server ->graph_matrix = (int**) calloc( data_from_server ->num_of_junctions , sizeof(int*) );
	if ( data_from_server ->graph_matrix == NULL )
	{
		printf("FATAL ERROR: Memory allocation failed\n");
		free ( buffer_ptr);
		return(SETUP_PROBLEM);
	}
	for ( j = 0 ; j < data_from_server ->num_of_junctions ; j++ )
	{
		data_from_server ->graph_matrix[j] = (int*) calloc( data_from_server ->num_of_junctions , sizeof (int) );
		if (  data_from_server ->graph_matrix[j]  == NULL  )
		{
			printf("FATAL ERROR: Memory allocation failed\n");
			free ( buffer_ptr);
			return(SETUP_PROBLEM);
		}
	}
	for ( j = 0; j < data_from_server ->num_of_junctions; j++ )
	{
		for ( z = 0; z < data_from_server ->num_of_junctions; z++)
		{
			data_from_server ->graph_matrix[j][z] = buffer_ptr[i];
			i++;
		}
	}
	free (buffer_ptr);
	return rec_res;
}

TransferResult_t Server_Sending_Data ( Data_From_Server data_from_server , SOCKET sd )
{
	int total_buffer_length , i = 0 , j , z , *buffer_ptr = NULL ;
	TransferResult_t send_res ;
	total_buffer_length = ( data_from_server.num_of_junctions * 2 ) + ( (data_from_server.num_of_junctions) * (data_from_server.num_of_junctions) ) + 1 ;
	send_res = SendBuffer(
				(const char*)( &total_buffer_length),
				 (int) ( sizeof(total_buffer_length) ),
				 sd );
	if ( send_res != TRNS_SUCCEEDED ) 
		return send_res;
	buffer_ptr = (int*) malloc( total_buffer_length * sizeof(int) ) ;
	if ( buffer_ptr == NULL )
	{
		printf("FATAL ERROR: Memory allocation failed.\n") ;
		return(SETUP_PROBLEM) ;
	}
	buffer_ptr[i] = data_from_server.num_of_junctions ;
	i++ ;
	for ( j = 0 ; j < data_from_server.num_of_junctions ; j++)
	{
		buffer_ptr[i] = data_from_server.junctions[j].x ;
		buffer_ptr[i+1] = data_from_server.junctions[j].y ;
		i = i + 2 ;
	}
	for (  j = 0  ;   j  <  data_from_server.num_of_junctions  ;   j++  )
	{
		for (  z = 0  ;   z  <  data_from_server.num_of_junctions  ;   z++  )
		{
			buffer_ptr[i] = data_from_server.graph_matrix[j][z] ;
			i++ ;
		}
	}
	send_res = SendBuffer(
				(const char*)( buffer_ptr),
				 (int) (  total_buffer_length * sizeof(int)  ),
				 sd );
	free (buffer_ptr);
	return send_res;
}

TransferResult_t Server_Receiving_Data ( Updated_Arc *updated_arc , SOCKET sd )
{
	int total_buffer_length = 5  , *buffer_ptr = NULL;
	TransferResult_t rec_res ; 
	buffer_ptr = (int*) malloc( total_buffer_length * sizeof(int) );
	if (buffer_ptr == NULL )
	{
		printf("FATAL ERROR: Memory allocation failed.\n");
		return(SETUP_PROBLEM);
	}
	rec_res = ReceiveBuffer(
				(char*)( buffer_ptr),
				 (int) ( total_buffer_length * sizeof(int) ),
				 sd );
	if ( rec_res != TRNS_SUCCEEDED ) 
	{
		free ( buffer_ptr);
		return rec_res;
	}
	updated_arc ->source.x = buffer_ptr[0];
	updated_arc ->source.y = buffer_ptr[1];
	updated_arc ->destination.x = buffer_ptr[2];
	updated_arc ->destination.y = buffer_ptr[3];
	updated_arc ->delay = buffer_ptr[4];
	free ( buffer_ptr);
	return rec_res;
}