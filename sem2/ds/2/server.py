import socket
import sys
import os

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = ('localhost', 8080)
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)
d = ""	#variable for reading from file 
p = ""	#variable for storing data received from server side
while True:
    # Wait for a connection
    connection, client_address = sock.accept()
    data = connection.recv(1024)	#receive data
    try:
    	#split received data to take in only file name
    	p = data.split()[1]	
        d = p[1:]
    except IndexError:	#error checking
		connection.sendall("\n404 Not Found\n")
		connection.close()
    if(p):	
    	if(not os.path.isfile(d)):	#error checking for file path
        	connection.sendall("\n404 Not Found\n")
        else:
        	#file operations 
        	f = open(d, 'r')
     		d = f.read()	#read from file
        	connection.sendall(d)	#send data from file to client
        connection.close()	#close connection after processing request
