import socket
import sys

def parse(data):
	sp = data.split()
	start = data[7:8]
	end = data[15:16]
	period = sp[5]
	i = 0
	answer = " ".join(chr(i) for i in range(ord(start), ord(end)+1,int(period)))

	return answer

ip = sys.argv[1]
port = int(sys.argv[2])
# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = (ip, port)
print >>sys.stderr, 'connecting to %s port %s' % server_address
sock.connect(server_address)

try:
    init = sock.recv(64)
    print init
    
    while(1):
        data = sock.recv(64)
        print data
        answer = parse(data)
        print answer
        sock.sendall(answer + "\n")
       
finally:
    print >>sys.stderr, 'closing socket'
    sock.close()