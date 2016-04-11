/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 4

Description:	This file contains the main function of the program.
*/


#define _CRT_SECURE_NO_WARNINGS

#include "Server.h"
#include "Client.h"
#pragma comment(lib, "Ws2_32.lib")


int main( int argc , char* argv[])
{
	int retval = 0 ;

	if (   strcmp ( argv[1] , "server" ) == 0    )
	{
		retval = Server_Func ( argv[2] , argv[3] ,  atof ( argv[4] )  , argv[5]  );
		if ( retval == 1)
			return(1);
	}
	else if ( strcmp ( argv[1] , "client" ) == 0 ) 
	{
		retval = Client_Func ( argv[2]  , atoi ( argv[3] ) , atoi ( argv[4] ) , atoi ( argv[5] ) , atoi ( argv[6] ) , argv[7] );
		if ( retval == 1)
			return(1);
	}
	else
	{
		printf( "Not a valid input of client/server mode.\n");
		return(1);
	}

	return(0);

}

