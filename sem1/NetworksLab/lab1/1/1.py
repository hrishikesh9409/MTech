import string
import sys
import binascii

################################################################
def summation(array):
	add = 0
	mod = 1<<16
	for index in range(len(array)):
		add = add + array[index]
		if add >= mod: 
			add = (add + 1) % mod
	return add

def Port_HEX(port):
   p = (hex(port)[2:]).zfill(4)
   return p

def IP2Hex(ip):
	a = ip.split('.')
	b = hex(int(a[0]))[2:].zfill(2) + hex(int(a[1]))[2:].zfill(2) + hex(int(a[2]))[2:].zfill(2) + hex(int(a[3]))[2:].zfill(2)
	b = b.replace('0x', '')
	b = b.upper()	
	return b
################################################################

sourceIP = sys.argv[1]
sourcePort = int(sys.argv[2])
destIP = sys.argv[3]
destPort = int(sys.argv[4])
message = str(sys.argv[5])

message_length = int(len(message)/2)
length = 8 + message_length
length_hex = hex(length)[2:].zfill(4)

sourceIP_HEX = IP2Hex(sourceIP)
destIP_HEX = IP2Hex(destIP)

sourceIP_LIST = []
sourceIP_LIST.append(int(sourceIP_HEX[:4], 16))
sourceIP_LIST.append(int(sourceIP_HEX[4:], 16))

destIP_LIST = []
destIP_LIST.append(int(destIP_HEX[:4], 16))
destIP_LIST.append(int(destIP_HEX[4:], 16))

sourcePort_HEX = Port_HEX(sourcePort)
destPort_HEX = Port_HEX(destPort)

protocol_HEX = (hex(17)[2:].zfill(4))


################################################################
i = 0
totalDATA = []

while(i < len(message)):
	totalDATA.append(message[i] + message[i + 1])
	i = i + 2

j = 0
totalDATA2 = totalDATA + [hex(0)[2:].zfill(2)]
totalDATA3 = []
while(j < message_length):
	if(message_length % 2 == 0):
		d1 = totalDATA[j].zfill(2)
		d2 = totalDATA[j+1].zfill(2)
		d = d1 + d2
		totalDATA3.append(d)
	else:
		d1 = totalDATA2[j].zfill(2)
		d2 = totalDATA2[j+1].zfill(2)
		d = d1 + d2
		totalDATA3.append(d)
	j = j + 2

totalDATA4 = []
for value in totalDATA3:
	totalDATA4.append(int(value, 16))

################################################################

summation_sourceIP = hex(summation(sourceIP_LIST))[2:]
summation_destIP = hex(summation(destIP_LIST))[2:]
summation_message = hex(summation(totalDATA4))[2:]

summation_LIST = []
summation_LIST.append(summation_sourceIP)
summation_LIST.append(summation_destIP)
summation_LIST.append(sourcePort_HEX)
summation_LIST.append(destPort_HEX)
summation_LIST.append(protocol_HEX)
summation_LIST.append(length_hex)
summation_LIST.append(length_hex)
summation_LIST.append(summation_message)

LIST = []
for value in summation_LIST:
	LIST.append(int(value, 16))

checkSum = hex(summation(LIST)^65535)[2:]

segLIST = []
segLIST.append(sourcePort_HEX)
segLIST.append(destPort_HEX)
segLIST.append(length_hex)
segLIST.append(checkSum)

segLIST = "".join(segLIST)

print segLIST + message

