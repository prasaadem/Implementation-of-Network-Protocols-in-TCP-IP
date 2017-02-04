README

This FTP server was designed by Team22: Sathyajit and Satya Aditya as a part of the network simulation assignment for ECEN 602 at TAMU.

MakeFile and Source Code are uploaded.


Testing: 

- We tested this using the Xinetd TFTP standalone client and found it to be working.
- Unlike conventional TFTP standard, this server blocks all data into a single block and transmits it to the client.
- Channel is assumed to be lossless, i.e. no ACK mechanism is implemented on this server.


Server:

- Server listens on any ephimeral port for accepting RRQs.
- Usage: ./team22_ftpserver 127.0.0.1 12222 12
- Usgae: ./name of server <serverIP> <serverPort> <Maximum number of clients>

Please feel free to reach me at sathyajit92@gmail.com for any clarifications.

