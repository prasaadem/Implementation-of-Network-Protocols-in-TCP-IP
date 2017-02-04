//Header Files are being included below

# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <unistd.h>
# include <time.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <netdb.h>

//Global parameters declaration
int count;
#define PORT 80
#define USERAGENT "HTMLGET 1.0"

//Each function is self explanatory as per its name. Separate functions are used to execute serverconnect(), newclientaccept(), etc.

void server_connect(int *socket_fd,struct sockaddr_in *server_address,int argv2, char argv1[4]);
void handle(int newsock, fd_set *set);
char * getweb(char buf[1024]);
int create_tcp_socket();
char *get_ip(char *host);
char *build_get_query(char *host, char *page);

//main() function

int main(int argc, char *argv[])
{
	struct sockaddr_in server_address, client_address;
	fd_set main_set;
	fd_set temp_set;
	
	int i;
	int max_fd;
	int argv2=atoi(argv[2]);

	int socket_fd=0;

	//Clearing FD_SET
	FD_ZERO(&main_set);
	FD_ZERO(&temp_set);

	server_connect(&socket_fd, &server_address,argv2,argv[1]);

	FD_SET(0,&main_set);
	FD_SET(socket_fd, &main_set);

	max_fd=socket_fd;

	printf("*****************************************************************************\n");
	fflush(stdout);
	printf("\nStarting the Proxy. Please be patient.\n\n");
	fflush(stdout);
	printf("*****************************************************************************\n");
	sleep(1);
	fflush(stdout);
	printf("\nProxy Listening on IP %s and Port %s.\n\n",argv[1], argv[2]);
	fflush(stdout);
	printf("*****************************************************************************\n\n");

	//Do Forever
	while(1)
	{

		temp_set=main_set;

		if(select(max_fd+1,&temp_set,NULL,NULL,NULL)==-1)
		{
			error("Cannot use select() properly..!");
			exit(4);
		}

		for(i=0;i<=max_fd;i++)
		{
			if(FD_ISSET(i, &temp_set))
			{
				if(i==socket_fd)
				{
		                    /* New connection */
		                    int newsock;
		                    struct sockaddr_in their_addr;
		                    int size = sizeof(struct sockaddr_in);
		                    newsock = accept(socket_fd, (struct sockaddr*)&their_addr, &size);
		                    if (newsock == -1) 
				    {
			                  perror("accept");
			            }
                    		    else 
				    {
                        		printf("Got a connection from %s on port %d\n",inet_ntoa(server_address.sin_addr), htons(server_address.sin_port));
                        		FD_SET(newsock, &main_set);
                        		if (newsock > max_fd) 
					{
		                            max_fd = newsock;
                		        }
                		    }
				}
			else
			{
                    		handle(i, &main_set);
			}
        	}
	    	}
	}

	return 0;
}
//End of main()

void server_connect(int *socket_fd,struct sockaddr_in *server_address, int argv2,char argv1[4])
{
	if((*socket_fd = socket(AF_INET,SOCK_STREAM,0))==-1)
	error("Cannot create socket sucessfully..!");

	//Set the parameters accordingly

	server_address->sin_family=AF_INET;
	server_address->sin_port=htons(argv2);
	server_address->sin_addr.s_addr=inet_addr(argv1);

	memset(server_address->sin_zero, '\0', sizeof server_address->sin_zero);

	int flag=1;

	if(setsockopt(*socket_fd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(int))==-1)
	{
		error("Cannot set socket options!\n");
		exit(1);
	}

	if(bind(*socket_fd, (struct sockaddr *)server_address, sizeof (struct sockaddr))==-1)
	{
		error("Cannot bind port to socket!\n");
		exit(1);
	}

	if(listen(*socket_fd,15) == -1)
	{
		error("Cannot listen on socket!\n");
		exit(1);
	}

	//End of Server_Connect()
}

void handle(int newsock, fd_set *set)
{
	char buf[1024];
	memset(buf, 0, sizeof(buf));
  	int htmlstart = 0;
  	char * htmlcontent;
  	while((recv(newsock, buf,1024, 0)) > 0){

	
	printf("The buffer is : %s\n",buf);
	
	char* web;
    	web = getweb(buf);

	int temp; 	
	temp = send(newsock,web,1024, 0);
    	if(temp == -1)
	{
		perror("Can't send query");
		exit(1);
	}
	else
	{
		printf("Query Sent to server");
	}
}
}
char* getweb(char buf[1024])
{

	    char h[100];
	    char port = 80;
	    char p[100];
	    char useragent[100];
	    sscanf(buf, "GET %s %s %s",h,p,useragent);
	    printf("ip = \"%s\"\n", h);
	    printf("page = \"%s\"\n", p);
	    printf("useragent = \"%s\"\n", useragent);

	  struct sockaddr_in *remote;
	  int sock;
	  int tmpres;
	  char *ip;
	  char *get;
	  char buffer[1024];
	  char *host;
	  char *page;
 
	  host = h;
  	  page = p;
  
  	sock = create_tcp_socket();
  	ip = get_ip(host);
  	fprintf(stderr, "IP is %s\n", ip);
  	remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
  	remote->sin_family = AF_INET;
  	tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
  	if( tmpres < 0)  
  	{
    		perror("Can't set remote->sin_addr.s_addr");
    		exit(1);
  	}else if(tmpres == 0)
  	{
    		fprintf(stderr, "%s is not a valid IP address\n", ip);
    		exit(1);
  	}
  	remote->sin_port = htons(PORT);
 
  	if(connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0){
    	perror("Could not connect");
    	exit(1);
  	}
  	
	get = build_get_query(host, page);
	fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);
 
  	//Send the query to the server
  	int sent = 0;
  	while(sent < strlen(get))
  	{
    		tmpres = send(sock, get+sent, strlen(get)-sent, 0);
    		if(tmpres == -1){
      			perror("Can't send query");
      			exit(1);
    		}
    		sent += tmpres;
  	}
  	//now it is time to receive the page
  	memset(buffer, 0, sizeof(buf));
  	int htmlstart = 0;
  	char * htmlcontent;
  	while((tmpres = recv(sock, buffer,1024, 0)) > 0){
    		if(htmlstart == 0)
    		{
      			htmlcontent = strstr(buffer, "\r\n\r\n");
      			if(htmlcontent != NULL){
        			htmlstart = 1;
        			htmlcontent += 4;
      			}
    		}else{
      			htmlcontent = buffer;
    		}
    		if(htmlstart)
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
		        fprintf(stdout, htmlcontent);
			printf("*****************************************************************************\n\n");
			fflush(stdout);
		 }
 
		memset(buf, 0, tmpres);
  	}
  	if(tmpres < 0)
  	{
  		  perror("Error receiving data");
  	}
  free(get);
  free(remote);
  free(ip);
  close(sock);
 return(htmlcontent);
}
 
 
int create_tcp_socket()
{
  int sock;
  if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    perror("Can't create TCP socket");
    exit(1);
  }
  return sock;
}
 
 
char *get_ip(char *host)
{
  struct hostent *hent;
  int iplen = 15; //XXX.XXX.XXX.XXX
  char *ip = (char *)malloc(iplen+1);
  memset(ip, 0, iplen+1);
  if((hent = gethostbyname(host)) == NULL)
  {
    herror("Can't get IP");
    exit(1);
  }
  if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL)
  {
    perror("Can't resolve host");
    exit(1);
  }
  return ip;
}
 
char *build_get_query(char *host, char *page)
{
  char *query;
  char *getpage = page;
  char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
  if(getpage[0] == '/'){
    getpage = getpage + 1;
    fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
  }
  // -5 is to consider the %s %s %s in tpl and the ending \0
  query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
  sprintf(query, tpl, getpage, host, USERAGENT);
  return query;
}
