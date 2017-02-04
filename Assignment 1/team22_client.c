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

void client_connect(int *socket_fd, struct sockaddr_in *server_address, int argv3, char argv2[4]);
void send_receive_data(int i, int socket_fd, char argv1[16]);
void packetize(char buffer[256], char username[16], int socket_fd);
void depacketize(int socket_fd, char argv1[16]);
void join(int socket_fd1, char username[16]);

//main() begins
int main(int argc, char *argv[])
{

struct sockaddr_in server_address;

int socket_fd=0;
int max_fd, i;


int argv3=atoi(argv[3]);




fd_set main_set, temp_set;

fflush(stdout);
printf("*****************************************************************************\n");
fflush(stdout);
printf("\nHello %s! Please wait while I'm trying to connect you to the Server.\n\n",argv[1]);
fflush(stdout);
printf("*****************************************************************************\n");

client_connect(&socket_fd, &server_address, argv3, argv[2]);//connect new client
sleep(1);
fflush(stdout);

printf("\n%s, You're now successfully connected to the chat room!\n\n", argv[1]);

fflush(stdout);
printf("*****************************************************************************\n");
sleep(1);
printf("\nWelcome to the chat room, %s!\n\n",argv[1]);
fflush(stdout);
sleep(1);


printf("JOIN MESSAGE is being sent. Please wait, %s.\n\n",argv[1]);
sleep(1);
int socket_fd1=socket_fd;
join(socket_fd1,argv[1]);//SEND JOIN MESSAGE

char user_list[1024]={'\0'};

int l;



l=recv(socket_fd1,user_list,1024,0);

if(strcmp(user_list,"error")==0)
{
printf("ERROR! Please check server console for the reason of error.\n\n");
printf("Either same name user or maximum client exceeded!\n\n");
exit(0);
}


printf("*****************************************************************************\n");
printf("\nChat room rules:\n\n");
printf("1. Please do not have your username as 'error'. This is because server is using this keyword for something else.\n\n");
printf("2. Using the message 'quit' will exit you from the chat room.\n\n");

printf("Enjoy chatting!\n\n");

printf("*****************************************************************************\n");

printf("\n\nActive users: %s\n\n",user_list);

FD_ZERO(&main_set);
FD_ZERO(&temp_set);

FD_SET(0,&main_set);
FD_SET(socket_fd,&main_set);

max_fd=socket_fd;
fflush(stdout);
printf("%s:",argv[1]);
fflush(stdout);


while(1)
{

temp_set=main_set;

if(select(max_fd+1,&temp_set,NULL,NULL,NULL)==-1)
{
error("Cannot select()..!");
exit(4);
}


for(i=0;i<=max_fd;i++)
{
if(FD_ISSET(i,&temp_set))
{
send_receive_data(i,socket_fd,argv[1]);//SEND/RECEIVE data from Server
}
}
}


return 0;
}

//End of main()


void client_connect(int *socket_fd, struct sockaddr_in *server_address, int argv3, char argv2[4])
{
if((*socket_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
{
error("Cannot create socket..");
exit(1);
}

server_address->sin_family=AF_INET;
server_address->sin_port=htons(argv3);
server_address->sin_addr.s_addr=inet_addr(argv2);


memset(server_address->sin_zero,'\0',sizeof server_address->sin_zero);

if(connect(*socket_fd,(struct sockaddr *) server_address, sizeof(struct sockaddr))==-1)
{
error("Failed to connect to socket..!");
exit(1);
}
}
//End of client connect()


void send_receive_data(int i, int socket_fd, char argv1[16])
{

char send_buffer[256];
char receive_buffer[256];

int received_bytes;
if(i==0)
{

fgets(send_buffer,256,stdin);
if(strcmp(send_buffer,"quit\n")==0)
exit(0);
else
packetize(send_buffer,argv1,socket_fd);//ENCAPSULATION IMPLEMENTATION USING PACKETIZING
fflush(stdout);
}

else
{

depacketize(socket_fd, argv1);//DECAPSULATION

}
}
//End of send_receive_data()

void packetize(char buffer[256], char username[16], int socket_fd)
{

int buff_len=strlen(buffer);
int user_len=strlen(username);

struct attr_payload
{

char client_username[16];
char message[512];
char reason[32];
int client_count;

};


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

}final_msg,join_msg;

memset(&final_msg, 0, sizeof(final_msg));

strcpy(msg_username.sbcp_attr_payload.client_username,username);
msg_username.attr_type=2;
msg_username.attr_length=user_len+2+2;

strcpy(msg.sbcp_attr_payload.message,buffer);
msg.attr_type=4;
msg.attr_length=buff_len+2+2;

final_msg.ver=3;
final_msg.msg_type=4;
final_msg.msg_length=sizeof(msg_username)+sizeof(msg)+2+2;

final_msg.msg_payload[0]=msg_username;
final_msg.msg_payload[1]=msg;

send(socket_fd, &final_msg,sizeof(final_msg),0);//SEND COMMAND EXECUTED
memset(&final_msg, 0, sizeof(final_msg));
fflush(stdout);
printf("%s:",username);
fflush(stdout);
}
//End of packetize()

void depacketize(int socket_fd, char argv1[16])
{

struct attr_payload
{

char client_username[16];
char message[512];
char reason[32];
int client_count;

};


struct sbcp_attribute
{

int attr_type;
int attr_length;
struct attr_payload sbcp_attr_payload;

};

struct sbcp_message
{

int ver;
int msg_type;
int msg_length;
struct sbcp_attribute msg_payload[4];

}final_msg;

int received_bytes;
char receive_buffer[256];

received_bytes=recv(socket_fd, &final_msg, sizeof(final_msg),0);

strcpy(receive_buffer,final_msg.msg_payload[1].sbcp_attr_payload.message);
receive_buffer[received_bytes]='\0';

printf("\n%s: %s",final_msg.msg_payload[0].sbcp_attr_payload.client_username,receive_buffer);
fflush(stdout);
printf("%s:",argv1);
fflush(stdout);
memset(&final_msg, 0, sizeof(final_msg));

fflush(stdout);

}

//End of depacketize()

void join(int socket_fd1, char username[16])
{


int user_len=strlen(username);

struct attr_payload
{

char client_username[16];
char message[512];
char reason[32];
int client_count;

};


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

}join_msg;

memset(&join_msg, 0, sizeof(join_msg));

strcpy(msg_username.sbcp_attr_payload.client_username,username);
msg_username.attr_type=2;
msg_username.attr_length=user_len+2+2;


join_msg.ver=3;
join_msg.msg_type=2;
join_msg.msg_length=sizeof(msg_username)+2+2;

join_msg.msg_payload[0]=msg_username;

send(socket_fd1, &join_msg,sizeof(join_msg),0);//JOIN COMMAND EXECUTED

memset(&join_msg, 0, sizeof(join_msg));
}
//End of join()
