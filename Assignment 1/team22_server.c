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

//Global parameters declaration
int count;
void join_reply(int socket_fd1, int argv3);
char users[1024];


//Structures for implementing packetization and data encapsulation as per the SBCP protocol requirements

struct attr_payload
{

char client_username[16];
char message[512];
char reason[32];
int client_count;

}a_payload;


struct sbcp_attribute
{

int attr_type;
int attr_length;
struct attr_payload sbcp_attr_payload;

}msg_username, msg, msg_reason;

struct sbcp_message
{

int ver;
int msg_type;
int msg_length;
struct sbcp_attribute msg_payload[4];

}final_msg, join_msg;


//Each function is self explanatory as per its name. Separate functions are used to execute serverconnect(), newclientaccept(), etc.

void server_connect(int *socket_fd,struct sockaddr_in *server_address,int argv2, int argv3, char argv1[4]);
void new_client_accept(fd_set *main_set, int *max_fd, int socket_fd, struct sockaddr_in *client_address, char argv2[10], int argv3);
void send_receive_data(int i, fd_set *main_set, int socket_fd, int max_fd);
void broadcast_data (int j, int i, int socket_fd, int received_bytes, struct sbcp_message receive_buffer, fd_set *main_set);


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
}

else
send_receive_data(i,&main_set,socket_fd,max_fd);
}
}
}

return 0;
}
//End of main()

void server_connect(int *socket_fd,struct sockaddr_in *server_address, int argv2,int argv3, char argv1[4])
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

if(listen(*socket_fd,argv3) == -1)
{
error("Cannot listen on socket!\n");
exit(1);
}

//End of Server_Connect()

}


void new_client_accept(fd_set *main_set, int *max_fd, int socket_fd, struct sockaddr_in *client_address,char argv2[10], int argv3)
{
int new_socket_fd;

socklen_t length;


length=sizeof(struct sockaddr_in);

if((new_socket_fd=accept(socket_fd,(struct sockaddr*)client_address,&length))==-1)
{
error("Cannot accept this client..");
exit(1);
}
else
{
FD_SET(new_socket_fd,main_set);
if(new_socket_fd>*max_fd)
{
	*max_fd=new_socket_fd;
	
}

printf("Connection established to Socket (%s : %s) on client port %d\n",inet_ntoa(client_address->sin_addr),argv2,ntohs(client_address->sin_port));
int new_socket_fd1=new_socket_fd;
join_reply(new_socket_fd1,argv3);
}
}
//End of new_client_accept()



void send_receive_data(int i, fd_set *main_set, int socket_fd, int max_fd)
{

int received_bytes;

memset(&final_msg, 0, sizeof(final_msg));

if((received_bytes= recv(i, &final_msg, sizeof(final_msg), 0)) <=0)
{
if(received_bytes==0)
{
printf("Hanged %d socket!\n", i);
}
else
{
error("Cannot receive from socket..!");
}

close(i);

FD_CLR(i,main_set);
}

else
{
int j;
for(j=0;j<=max_fd;j++)
{
broadcast_data(j,i,socket_fd,received_bytes,final_msg,main_set);
}
fflush(stdout);
printf("\n*****************************************************************************");
printf("\n\nFWD MESSAGE Excecuted. All clients have MESSAGE now.\n");
fflush(stdout);
}
}

//End of send_receive_data()


void broadcast_data (int j, int i, int socket_fd, int received_bytes, struct sbcp_message final_msg, fd_set *main_set)
{

if(FD_ISSET(j,main_set))
{
if(j!=socket_fd && j!=i)
{
if(send(j,&final_msg,received_bytes,0)==-1)//FWD COMMAND EXECUTED
{
error("Cannot broadcast..!");
}

memset(&final_msg, 0, sizeof(final_msg));
}
}
//End of broadcast_data()


}

void join_reply(int socket_fd1, int argv3)
{

int received_bytes;

memset(&join_msg, 0, sizeof(join_msg));

if((received_bytes= recv(socket_fd1, &join_msg, sizeof(join_msg), 0)) <=0)
{
if(received_bytes==0)
{
printf("Hanged %d socket!\n!", socket_fd1);
}
else
{
error("Cannot receive from socket..!");
}

close(socket_fd1);

//FD_CLR(i,main_set);

}

char name[16];

strcpy(name,join_msg.msg_payload[0].sbcp_attr_payload.client_username);

if(strstr(users,name))
{

fflush(stdout);
printf("\n*****************************************************************************");
printf("\n\nERROR(REASON=SAME NAME) MESSAGE Executed. Disconnected Client.\n");
fflush(stdout);

char error[]="error";
send(socket_fd1,error,strlen(error),0);//ERROR-SAME NAME USER COMMAND EXECUTED
memset(&join_msg, 0, sizeof(join_msg));
}

else
{

if(count>argv3-1)
{
printf("\n*****************************************************************************");
printf("\n\nERROR(REASON=MAX CLIENTS EXCEEDED) MESSAGE Executed. Disconnected Client.\n");
fflush(stdout);

char error[]="error";
send(socket_fd1,error,strlen(error),0);//ERROR-MAXIMUM CLIENTS EXCEEEDED COMMAND EXECUTED
memset(&join_msg, 0, sizeof(join_msg));
}

else
{

strcat(users,join_msg.msg_payload[0].sbcp_attr_payload.client_username);

int m=strlen(users);

send(socket_fd1,users,strlen(users),0);
users[m]=',';
memset(&join_msg, 0, sizeof(join_msg));
count++;

}
}

}

//End of join_reply()
