//Header Files are being included below

# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/socket.h>
# include <errno.h>
# include <arpa/inet.h>

//Modular approach to programming involves using separate functions for each task

void client_connect(int *socket_fd, struct sockaddr_in *server_address, int argv2, char argv1[4]);
char *build_get_query(char *host, char *page);
void usage();

#define PAGE "/"
#define USERAGENT "HTMLGET 1.0"

//main() begins
int main(int argc, char *argv[])
{

	struct sockaddr_in server_address;

	int socket_fd=0;
	int max_fd, i;

	int argv2=atoi(argv[2]);

	printf("*****************************************************************************\n");
	fflush(stdout);
	printf("\nHello %s! Please wait while I'm trying to connect you to the Server.\n\n",argv[1]);
	fflush(stdout);
	printf("*****************************************************************************\n");

	client_connect(&socket_fd, &server_address, argv2, argv[1]);//connect new client

	printf("\nYou're now successfully connected to Proxy!\n\n");
	fflush(stdout);
	printf("*****************************************************************************\n");
	fflush(stdout);

	char *query;
	char *host;
  	char *page;
 
  	if(argc == 3)
	{
    		usage();
    		exit(2);
  	}  
  	host = argv[3];
  	if(argc > 3)
	{
    		page = argv[4];
  	}else
	{
    		page = PAGE;
  	}	
	query = build_get_query(host,page);

	char buf[1024];
	sprintf(buf,query);
	fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", query);
	int temp; 	
 	while(1) 
	{
		temp = send(socket_fd, buf, strlen(buf), 0);
	    	if(temp == -1)
		{
			perror("Can't send query");
	                close(socket_fd);
	                exit(1);
	        }
	        else 
		{
	                temp = recv(socket_fd, buf,1024, 0);
	                if ( temp > 0 )
	                {
				printf("*****************************************************************************\n");
				fflush(stdout);
				printf("\nWebContent Received.\n\n");
				fflush(stdout);
				printf("*****************************************************************************\n");
				sleep(1);
				fflush(stdout);
				printf("\nPrinting webcontent for Host:%s & Page:%s\n\n",host, page);
				fflush(stdout);
				printf("*****************************************************************************\n\n");
				fprintf(stdout, buf);
				printf("*****************************************************************************\n\n");
				fflush(stdout);			
			}
	      	}
		break;
    	}
    	close(socket_fd);
	return 0;
}

//End of main()


void client_connect(int *socket_fd, struct sockaddr_in *server_address, int argv2, char argv1[4])
{
	printf("Came to client connect");

	*socket_fd=socket(AF_INET,SOCK_STREAM,0);

	if(*socket_fd == -1)
	{
		error("Cannot create socket..");
		exit(1);
	}

	server_address->sin_family=AF_INET;
	server_address->sin_port=htons(argv2);
	server_address->sin_addr.s_addr=inet_addr(argv1);


	memset(server_address->sin_zero,'\0',sizeof server_address->sin_zero);

	if(connect(*socket_fd,(struct sockaddr *) server_address, sizeof(struct sockaddr_in))==-1)
	{
		error("Failed to connect to socket..!");
		exit(1);
	}
}
//End of cslient connect()


char *build_get_query(char *host, char *page)
{
  	char *query;	
  	char *getpage = page;
  	char *tpl = "GET %s %s %s";
  	if(getpage[0] == '/')
	{
	    	getpage = getpage + 1;
	    	fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
  	}
  	// -5 is to consider the %s %s %s in tpl and the ending \0
  	query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);

	sprintf(query, tpl, host,getpage, USERAGENT);

	//printf("\nThe query is :, %s\n",query);
  	  	
	return query;
}

void usage()
{
	  fprintf(stderr, "USAGE: htmlget host [page]\n\\thost: the website hostname. ex: google.com\n\\tpage: the page to retrieve. ex: index.html, default: /\n");
}
