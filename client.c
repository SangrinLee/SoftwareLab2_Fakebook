#include "a.h"
#include "a.c"

#define MAXLINE 1000
#define NAME_LEN 20

char *EXIT_STRING_2  =  "exit";
int history_fd;

int tcp_connect( int af, char *servip, unsigned short port );
void record_history(char* bufmsg, int bufmsg_len);
void save_history(void);

void errquit( char *mesg )
{
	perror( mesg );
	exit( 1 );
}

void chatting_client();
{
	char bufall[ MAXLINE + NAME_LEN ];
	char *bufmsg;
	char* buff;
	char* buf_last;
	int maxfdp1;
	int s;
	int namelen;
	fd_set read_fds;

	if( argc != 4 )
	{
		printf( "Usage : %s server_ip port ID\n", argv[ 0 ] );
		exit( 1 );
	}

	sprintf( bufall, "[ %s ] : ", argv[ 3 ] );
	namelen = strlen( bufall );
	bufmsg  =  bufall + namelen;

	s  =  tcp_connect( AF_INET, argv[ 1 ], atoi( argv[ 2 ] ) );

	if( s == -1 )
		errquit( "tcp_connect fail" );

	puts( "Connected Server" );
	maxfdp1 = s + 1;
	FD_ZERO( &read_fds );

	while( 1 )
	{
		FD_SET( 0, &read_fds );
		FD_SET( s, &read_fds );

		if( select( maxfdp1, &read_fds, NULL, NULL, NULL )<0 )
			errquit( "select fail" );
		if( FD_ISSET( s, &read_fds ) )
		{
			int nbyte;
			if( ( nbyte  =  recv( s, bufmsg, MAXLINE, 0 ) )>0 )
			{
				bufmsg[ nbyte ] = '\0';

				buff = bufmsg+strlen(bufmsg)-5; 
				if(strcmp("exit\n", bufmsg+strlen(bufmsg)-5 ) == 0 )
				{
					printf("상대방이 방에서 나갔습니다.\n");
				}
				else
				{
					printf( "%s", bufmsg, strlen(bufmsg) ); /////////////////
					record_history(bufmsg, strlen(bufmsg));
				}
			}
		}
		if( FD_ISSET( 0, &read_fds ) )
		{
			if( fgets( bufmsg, MAXLINE, stdin ) ) ///////////////
			{
				if( send( s, bufall, namelen + strlen( bufmsg ), 0 )<0 )
					puts( "error : write error on socket." );
				if( strstr( bufmsg, EXIT_STRING_2 ) != NULL )
				{
					save_history();
					puts( "Good bye." );
					close( s );
					exit( 0 );
				}
				record_history(bufmsg, strlen(bufmsg));
			}
		}
	}
}

int tcp_connect( int af, char *servip, unsigned short port )
{
	struct sockaddr_in servaddr;
	int s;

	if( ( s = socket( af, SOCK_STREAM, 0 ) )<0 )
		return -1;

	bzero( ( char* )&servaddr, sizeof( servaddr ) );
	servaddr.sin_family = af;
	inet_pton( AF_INET, servip, &servaddr.sin_addr );
	servaddr.sin_port  =  htons( port );

	if( connect( s, ( struct sockaddr * )&servaddr, sizeof( servaddr ) )<0 )
		return -1;
	return s;
}

void record_history(char* bufmsg, int bufmsg_len)
{
	history_fd = open("history", O_RDWR | O_APPEND | O_CREAT, 0666);
	write(history_fd, bufmsg, bufmsg_len);
	close(history_fd);
}

void save_history(void)
{
	char yn;
	char file_name[21];

	while(1)
	{
		printf("Do you want to save this chatting history? [y/n]");
		scanf("%c", &yn);

		if(yn == 'n')
		{
			unlink("history");
			break;
		}
		else if(yn == 'y')
		{
			printf("Type a name of the file(using under 21 charaters) ==> ");
			scanf("%s", file_name);

			rename("history", file_name);
			break;
		}
		else
		{
			printf("Type again!\n");
			continue;
		}
	}
}

