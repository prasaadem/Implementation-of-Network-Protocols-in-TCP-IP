************************************************************************************************************************************************************************
						README
************************************************************************************************************************************************************************

This code is a part of the Network simulation Assignment for ECEN 602 at Texas A&M University. 
It has been sucessfully compiled, executed and tested on gcc compiler (part of standard LINUX).

Common Errors and Catches:

-Please email me at sathyajit92@gmail.com for any questions. We've tested this extensively.
-If data is not input correctly on the command line as per the ordering given below, it throws a segmentation error. 
-This is NOT to be assumed as an error.
-If data is missing from the command line, it throws segmentation error too.
-If a user with a name same as an existing active chat room user joins, he will be required to reenter chat room using a valid name.
-Username can be any size until 1024 characters.
-Maximum number of clients active on chat room = argv[3]. If argv[3]+1 client joins, it will throw an ERROR on server and client console.
-IMPORTANT: Please do not use same serverport when running server second time, until the first server process is killed (NOT SUSPENDED).
-If a client disconnects using keyword 'quit', a smooth disconnect happens and resources are deallocated. If not, the other clients enter into hang state.
-This is an iterative server, no use of fork() in this code.

Creating MAKEFILE:
-The makefile rules are already set.
-Please use following command in the folder where makefile is present:

make -f makefile_server
make -f makefile_client

Then, EXECUTE using the following commands:

./team22_server 127.0.0.1 12345 12
./team22_client sathyajit 127.0.0.1 12345



Usage:

All commands are executed on standard Terminal of LINUX which supports gcc compiler commands.

-Please use this usage for team22_server.c:

gcc team22_server.c -o team22_server
./team22_server <serverIP> <serverPort> <maximum queqe of listen()>

Example:
gcc team22_server.c -o team22_server
./team22_server 127.0.0.1 12345 12


-Please use this usage for team22_client.c

gcc team22_client.c -o team22_client
./team22_client <Client_Username> <serverIP> <serverPort> 

Example:
gcc team22_client.c -o team22_client
./team22_client sathyajit 127.0.0.1 12345

Special uses:

- SELECT() function has been used in both client and server codes to determine the action.
- SEND() and RECV() are used to send data and receive data respectively into the structs.

Architecture: Modular (Functional) Programming

-We have used modular approach to C Programming in this code. 
-Specific functions like sending JOIN, FWD and SEND messages are implemented separetely in the code for higher clarity.
-This code has comments on most lines to fathom the working of the functions.

Data usage: Data Structures (DS)

-Encapsulation of messages into network packets is implemented using DS.
-SBCP frame format for packetization is used.
-As per the requirement, two level encapsulation scheme is employed.

COMMANDS Implemented:

JOIN COMMAND: When a new user connects to the chat room, a JOIN struct is sent across to the server.
This contains the username of the client.

SEND COMMAND: When a user types a chat message, this is encapsulated in the SEND struct and sent to the server.
This contains the username and the chat message of a single client.

FWD COMMAND: Server broadcasts the chat message to the clients using the FWD struct.

ERROR COMMAND: When a client with the same name as a already active client joins the session, Server sends a ERROR command.
This client is disconnected from the server and chat room. 

Prints on Server:

-Server starting.
-Listening.
-Shows a connected succesfully message each time a client connects to server IP and server port.
-Shows execution of FWD command.
-Shows execution of ERROR command.

Prints on Client:

-Client connecting.
-JOIN Message.
-Chat room rules. (Please take a look at them before assuming ERROR!)
-Active users on the chat room.
-ERROR when client uses the same username as an already active client.

