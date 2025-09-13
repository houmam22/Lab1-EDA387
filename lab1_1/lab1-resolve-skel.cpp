/********************************************************* -- SOURCE -{{{1- */
/** Translate host name into IPv4
 *
 * Resolve IPv4 address for a given host name. The host name is specified as
 * the first command line argument to the program. 
 *
 * Build program:
 *  $ g++ -Wall -g -o resolve <file>.cpp
 */
/******************************************************************* -}}}1- */

#include <stdio.h>
#include <stddef.h>

#include <assert.h>
#include <limits.h>
#include <string.h>
#ifdef __unix__
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <unistd.h>
#endif
#ifdef _WIN32
	#include <ws2tcpip.h>
#endif

#if !defined(HOST_NAME_MAX)
#	if defined(__APPLE__)
#		include <sys/param.h>
#		define HOST_NAME_MAX MAXHOSTNAMELEN
#	else  // !__APPLE__
#		define HOST_NAME_MAX 256
#	endif // ~ __APPLE__
#endif // ~ HOST_NAME_MAX

//--//////////////////////////////////////////////////////////////////////////
//--    local declarations          ///{{{1///////////////////////////////////

void print_usage( const char* aProgramName );

//--    local config                ///{{{1///////////////////////////////////


//--    main()                      ///{{{1///////////////////////////////////
int main( int aArgc, char* aArgv[] )
{
	// Initialize Winsock only in windows
	#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return 1;
    }
	#endif
	// Check if the user supplied a command line argument.
	if( aArgc != 2 )
	{
		print_usage( aArgv[0] );
		return 1;
	}

	// The (only) argument is the remote host that we should resolve.
	const char* remoteHostName = aArgv[1];

	// Get the local host's name (i.e. the machine that the program is 
	// currently running on).
	const size_t kHostNameMaxLength = HOST_NAME_MAX+1;
	char localHostName[kHostNameMaxLength];

	if( -1 == gethostname( localHostName, kHostNameMaxLength ) )
	{
		//printf("am here");
		perror( "gethostname(): " );
		#ifdef _WIN32 
		//next line is for windows
		WSACleanup();
		#endif
		return 1;
	}

	// Print the initial message
	printf( "Resolving `%s' from `%s':\n", remoteHostName, localHostName );

	// TODO : add your code here
	
	struct addrinfo hint;
	struct addrinfo *result;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	int status = getaddrinfo(remoteHostName, NULL, &hint, &result);
	if(status){
		printf("getaddrinfo error: %s\n", gai_strerror(status));
		#ifdef _WIN32 
		//next line is for windows
		WSACleanup();getaddri
		#endif
	}

	struct addrinfo *tmp = result;
	printf("IPv4: ");
	
	char addrStr[INET_ADDRSTRLEN];
	struct sockaddr_in *addr = (struct sockaddr_in *)tmp->ai_addr;
	inet_ntop(AF_INET, &addr->sin_addr, addrStr, sizeof(addrStr));
	printf("  %s\n", addrStr);
	freeaddrinfo(result);

	// Ok, we're done. Return success.
	return 0;
}


//--    print_usage()               ///{{{1///////////////////////////////////
void print_usage( const char* aProgramName )
{
	fprintf( stderr, "Usage: %s <hostname>\n", aProgramName );
}


//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
