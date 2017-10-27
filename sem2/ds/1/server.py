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
d = ""
while True:
    # Wait for a connection
    connection, client_address = sock.accept()

    try:
        # Receive the data in small chunks and retransmit it
        while True:

        	data = connection.recv(16)
        	try:
        		p = data.split()[1]
        		d = p[1:]
        	except IndexError:
        		connection.sendall("\n404 Not Found\n")
        		continue

        	if(p):
        		if(not os.path.isfile(d)):
        			connection.sendall("\n404 Not Found\n")
        		else:
        			f = open(d, 'r')
        			d = f.read()
        			connection.sendall(d)

    finally:
    	connection.close()