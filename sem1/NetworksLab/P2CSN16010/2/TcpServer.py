import socket
import sys

def fib(n):
	a = 0
	b = 1
	for i in range(n-1):
		a,b = b,a+b
	return a

if len(sys.argv) != 2:
	print "Not enough arguments in %s" % (sys.argv[0])
	sys.exit(0)
port = int(sys.argv[1])
server = ("0.0.0.0", port)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 5)
print >> sys.stderr, 'starting up on %s port %s' % server
sock.bind(server)
sock.listen(5)

while True:
    print >> sys.stderr, 'waiting for a connection'
    conxtn, clientaddr = sock.accept()
    
    try:
        print >> sys.stderr, 'connection from', clientaddr
        data1 = conxtn.recv(64)
        print >> sys.stderr, "%s" % data1
        while True:
            data = conxtn.recv(64)
            
            print >> sys.stderr, "%s" % data
            if data:
            	num = fib(int(data))
            	k = str(num)
                conxtn.send(k + '\n')
            	
            if data[0] == '0':
            	conxtn.close()
            
    finally:
        conxtn.close()