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

//Server connect function to connect the server to IP and Port
void server_connect(int *socket_fd,struct sockaddr_in *server_address,int argv2, int argv3, char argv1[4]);

//Accepts concurrent clients upto the arg[3] value
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
printf("New Client requested data.\n\n");
new_client_accept(&main_set,&max_fd,socket_fd,&client_address,argv[2],argv3);//accept new client connection


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

//The function that accepts new clients and does file transfer using TFTP

void new_client_accept(fd_set *main_set, int *max_fd, int socket_fd, struct sockaddr_in *client_address,char argv2[10], int argv3)
{


int new_socket_fd;

socklen_t length;

length=sizeof(struct sockaddr_in);

//New socket file descriptor is created here

if((new_socket_fd=socket(AF_INET,SOCK_DGRAM,0))<0)
error("Cannot create socket sucessfully..!");


FD_SET(new_socket_fd,main_set);
if(new_socket_fd>*max_fd)
{
	*max_fd=new_socket_fd;
	
}

//TFTP module begins here

int client_len=sizeof(client_address);

char buf[1024];
char send_data[512];
int opcode =3, block_id=1;
int opcode1, block_id1;
char *point=send_data;
int total_bytes=4;

//Server received RRQ from client

int n =recvfrom(socket_fd, buf, 1024 ,MSG_DONTWAIT, (struct sockaddr *) &client_address, &client_len);

printf("\nReceived RRQ with bytes: %d\n", n);

char *bufindex;

bufindex=buf;

bufindex=bufindex+2;

char filename[12];

strncpy(filename,bufindex,sizeof(filename)-1);

//Extracting filename from the the RRQ of Client

printf("Filename requested by the client is: %s\n", filename);

char file_buf[1024];

FILE *fp;

struct sockaddr_in ack;

//Open the file in read MODE

fp=fopen(filename,"r");

//If File not present then do below, where ERROR message is sent

if(fp==NULL)
{
printf("File not found in Server directory. Aborting task. \n");
opcode =5;
block_id=1;
opcode=htons(opcode);
block_id=htons(block_id);


bzero(send_data,1024);
memcpy(point,&opcode,sizeof(uint16_t));
point+=sizeof(uint16_t);


memcpy(point,&block_id,sizeof(uint16_t));
point+=sizeof(uint16_t);

char error1[50];

strcpy(error1, "Error! File does not exist in Server Directory!");
int err_len=strlen(error1);

memcpy(point,error1,err_len);
point+=err_len;

int zero=0;
zero=htons(zero);

memcpy(point, &zero, sizeof(uint16_t));
point+=sizeof(uint16_t);

total_bytes+=6+err_len;
int res1;
res1=sendto(new_socket_fd, send_data, total_bytes, 0, (struct sockaddr *) &client_address, client_len);

printf("Sent FILENAME not found ERROR Code=1.\n");
return;

//Returns to main()

}


int size;

//Do until no more characters in file

while(1)
{

bzero(file_buf,1024);

size=fread(file_buf,1,512,fp); 	
		
if(size==0)
break;


//Write opcode as 3, and block size for DATA packet

total_bytes=4+size;
printf("Extracted file data: %s\n",file_buf);

opcode1=htons(opcode);
block_id1=htons(block_id);

printf("Block Id: %d\n", block_id);
bzero(send_data,1024);

memcpy(point,&opcode1,sizeof(uint16_t));
point+=sizeof(uint16_t);


memcpy(point,&block_id1,sizeof(uint16_t));
point+=sizeof(uint16_t);
block_id=block_id+1;



memcpy(point,file_buf, size);
point=send_data;

int res;

res=sendto(new_socket_fd, send_data, total_bytes, 0, (struct sockaddr *) &client_address, client_len);

//Send bytes to the Client

printf("Sent this much data: %d\n", res);

int n1;
char rev_data[512];
n1 = recvfrom (new_socket_fd, rev_data, sizeof (rev_data), MSG_DONTWAIT,(struct sockaddr *) &ack,(socklen_t *) & client_len);


//Extract OPCODE From ACK of Client
char ackcode=rev_data[0];
ackcode=ackcode<<4;
ackcode=ackcode & rev_data[1];


//Check if valid ACK
if(ackcode%8!=0)
{
printf("Terminating. Invalid ACK.\n");
return;
}

}

//Send zero DATA packet with just opcode and block

memset(send_data,0, sizeof(send_data));


printf("End of file. Sending last block.\n");

memcpy(point,&opcode1,sizeof(uint16_t));
point+=sizeof(uint16_t);


memcpy(point,&block_id1,sizeof(uint16_t));
point+=sizeof(uint16_t);

sendto(new_socket_fd, send_data, 4, 0, (struct sockaddr *) &client_address, client_len);

fclose(fp);

//End of full file transfer with block size of 512 bytes (total packet size=516) each

}





