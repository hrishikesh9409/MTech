import socket
import sys

def fib(n):
	a = 0
	b = 1
	for i in range(n-1):
		a,b = b,a+b
	return a

port = int(sys.argv[1])
server = ("0.0.0.0", port)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 5)
print >> sys.stderr, 'starting up on %s port %s' % server
sock.bind(server)
sock.listen(5)

while True:
    print >> sys.stderr, 'waiting for a connection'
    connection, clientaddr = sock.accept()
    
    try:
        print >> sys.stderr, 'connection from', clientaddr
        data1 = connection.recv(64)
        print >> sys.stderr, "%s" % data1
        while(1):
            data = connection.recv(64)
            
            print >> sys.stderr, "%s" % data
            if data:
            	number = fib(int(data))
            	answer = str(number)
                connection.send(answer + '\n')
            	
            if data[0] == '0':
            	connection.close()
            
    finally:
        connection.close()