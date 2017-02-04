//Header Files are being included below

# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <sys/socket.h>

# include <stdlib.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <netdb.h>


void server_connect(int *socket_fd,struct sockaddr_in *server_address,int argv2, int argv3, char argv1[4]);
void new_client_accept(fd_set *main_set, int *max_fd, int socket_fd, struct sockaddr_in *client_address, char argv2[10], int argv3);

//main() function

int main(int argc, char *argv[])
{

struct sockaddr_in server_address, client_address;
fd_set main_set;
fd_set temp_set;

int i;
int max_fd;
int argv2=atoi(argv[2]);

int argv3=atoi(argv[3]);
int socket_fd=0;

//Clearing FD_SET
FD_ZERO(&main_set);
FD_ZERO(&temp_set);

server_connect(&socket_fd, &server_address,argv2,argv3,argv[1]);

FD_SET(socket_fd, &main_set);

max_fd=socket_fd;



printf("*****************************************************************************\n");
fflush(stdout);
printf("\nStarting the Server. Please be patient.\n\n");
fflush(stdout);
printf("*****************************************************************************\n");
sleep(1);
fflush(stdout);
printf("\nServer Listening on IP %s and Port %s.\n\n",argv[1], argv[2]);
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
new_client_accept(&main_set,&max_fd,socket_fd,&client_address,argv[2],argv3);//accept new client connection
printf("New Client requested data.\n\n");

}

}
}
}

return 0;
}
//End of main()

void server_connect(int *socket_fd,struct sockaddr_in *server_address, int argv2,int argv3, char argv1[4])
{


if((*socket_fd = socket(AF_INET,SOCK_DGRAM,0))<0)
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

}



void new_client_accept(fd_set *main_set, int *max_fd, int socket_fd, struct sockaddr_in *client_address,char argv2[10], int argv3)
{


int new_socket_fd;

socklen_t length;

length=sizeof(struct sockaddr_in);

if((new_socket_fd=socket(PF_INET,SOCK_DGRAM,0))<0)
error("Cannot create socket sucessfully..!");


FD_SET(new_socket_fd,main_set);
if(new_socket_fd>*max_fd)
{
	*max_fd=new_socket_fd;
	
}


int client_len=sizeof(client_address);

char buf[1024];

int n =recvfrom(socket_fd, buf, 1024 ,MSG_DONTWAIT, (struct sockaddr *) &client_address, &client_len);

char *bufindex;

bufindex=buf;

bufindex=bufindex+2;

char filename[12];

strncpy(filename,bufindex,sizeof(filename)-1);

printf("Filename: %s\n", filename);

char file_buf[1024];

FILE *fp;








fp=fopen(filename,"r");

int size=fread(file_buf,1,sizeof(file_buf),fp);
printf("Extracted file data: %s\n",file_buf);

char opcode[4]={'0','3','0','1'};

char send_data[1024];

memcpy((char *) send_data,opcode,4);
memcpy ((char *) send_data+4, file_buf, size);


printf("Extracted file data: %s\n",send_data);

sendto(new_socket_fd, send_data, sizeof(send_data), 0, (struct sockaddr *) &client_address, sizeof(client_address));


memset(send_data,0, sizeof(send_data));

char opcode1[4]={'0','3','0','2'};

memcpy((char *) send_data,opcode1,4);
printf("Extracted file data: %s\n",send_data);
sendto(new_socket_fd, send_data, sizeof(send_data), 0, (struct sockaddr *) &client_address, sizeof(client_address));


fclose(fp);

}





