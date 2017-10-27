import socket
import sys

def conv(input):
	s = input.split()
	val = []
	letters = ''
	j = 0
	for i in range(len(input)):
		if input[i] == ' ':
			val.append(input[i+1])

	diff = ord(val[2]) - ord(val[0])
	for i in range(0,diff+1,int(s[5])):
		letters += chr(ord(val[0]) + i) + ' '

	return letters

if len(sys.argv) != 3:
        print "Not enough arguments in %s" % (sys.argv[0])
        sys.exit(1)

host = sys.argv[1]
port = int(sys.argv[2])       
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server = (host, port)
print >> sys.stderr, 'connecting to %s port %s' % server
sock.connect(server)
c = ''
try:
	val = sock.recv(64)
	print val

	while True:
		data = sock.recv(64)
		# print >> sys.stderr, "%s" % data
		print data
		ans = conv(data)
		sock.sendall(ans + "\n")

finally:
		print >> sys.stderr, 'closing socket'
		sock.close()
		
		Start: H, end: J, period: 1
