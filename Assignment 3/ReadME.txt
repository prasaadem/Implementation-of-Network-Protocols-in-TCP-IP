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
-IMPORTANT: Please do not use same serverport when running server second time, until the first server process is killed (NOT SUSPENDED).
-Every client disconnects after it receives the page from proxy, a smooth disconnect happens and resources are deallocated.
-This is an iterative server, no use of fork() in this code.

Creating MAKEFILE:
-The makefile rules are already set.
-Please use following command in the folder where makefile is present:

make -f makefile_server
make -f makefile_client

Then, EXECUTE using the following commands:

./proxy 127.0.0.1 12345
./client 127.0.0.1 12345 www.google.com index.html



Usage:

All commands are executed on standard Terminal of LINUX which supports gcc compiler commands.

-Please use this usage for proxy.c:

gcc proxy.c -o proxy
./proxy <serverIP> <serverPort>

Example:
gcc proxy.c -o proxy
./proxy 127.0.0.1 12345


-Please use this usage for client.c

gcc client.c -o client
./client <serverIP> <serverPort> <host> <page> 

Example:
gcc client.c -o client
./team22_client 127.0.0.1 12345 www.google.com index.html

Special uses:

- SELECT() function has been used in server codes to determine the action.
- SEND() and RECV() are used to send data and receive data respectively into the structs.

Architecture: Modular (Functional) Programming

-We have used modular approach to C Programming in this code. 
-Specific functions like sending SEND messages & RECV messages are implemented separetely in the code for higher clarity.
-This code has comments on most lines to fathom the working of the functions.

Prints on Proxy:

-Proxy starting.
-Listening.
-Shows a connected succesfully message each time a client connects to server IP and server port.
-Shows execution of BUFFER command.
-Shows execution of ERROR command.
-Show execution of webpage downloaded

Prints on Client:

-Client connecting.
-Shows the downloaded webpage received from proxy
