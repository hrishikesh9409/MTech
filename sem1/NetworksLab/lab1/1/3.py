import string
import sys
import binascii
import hashlib

def summation(array):
	add = 0
	mod = 1<<16
	for index in range(len(array)):
		add = add + array[index]
		if add >= mod: 
			add = (add + 1) % mod
	return add

def IP2Hex(ip):
	a = ip.split('.')
	b = hex(int(a[0]))[2:].zfill(2) + hex(int(a[1]))[2:].zfill(2) + hex(int(a[2]))[2:].zfill(2) + hex(int(a[3]))[2:].zfill(2)
	b = b.replace('0x', '')
	b = b.upper()	
	return b



bigData = sys.argv[1]
sourceIP = sys.argv[2]
destIP = sys.argv[3]
sourcePort = bigData[:4]
destPort = bigData[4:8]
length_hex = bigData[8:12]
check_Sum = bigData[12:16]
data_HEX = bigData[16:]

udpdataHASH = ''.join(chr(int(data_HEX[i:i+2], 16)) for i in range(0, len(data_HEX), 2))
hash_object = hashlib.sha256(udpdataHASH)
hex_dig = hash_object.hexdigest()

sourceIP_HEX = IP2Hex(sourceIP)
destIP_HEX = IP2Hex(destIP)

sourceIP_LIST = []
sourceIP_LIST.append(int(sourceIP_HEX[:4], 16))
sourceIP_LIST.append(int(sourceIP_HEX[4:], 16))
summation_sourceIP = hex(summation(sourceIP_LIST))[2:]

destIP_LIST = []
destIP_LIST.append(int(destIP_HEX[:4], 16))
destIP_LIST.append(int(destIP_HEX[4:], 16))
summation_destIP = hex(summation(destIP_LIST))[2:]


protocol_HEX = (hex(17)[2:1].zfill(4))

message_length = int(length_hex, 16) - 8
length_data_hex = len(data_HEX)

i = 0
totalDATA = []

while(i < length_data_hex):
	totalDATA.append(data_HEX[i] + data_HEX[i + 1])
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

summation_message = hex(summation(totalDATA4))[2:]

summation_LIST = []
summation_LIST.append(summation_sourceIP)
summation_LIST.append(summation_destIP)
summation_LIST.append(sourcePort)
summation_LIST.append(destPort)
summation_LIST.append('0011')
summation_LIST.append(length_hex)
summation_LIST.append(length_hex)
summation_LIST.append(summation_message)

LIST = []
for value in summation_LIST:
	LIST.append(int(value, 16))

checkSum = hex(summation(LIST)^65535)[2:]

segLIST = []
segLIST.append(int(sourcePort, 16))
segLIST.append(int(destPort, 16))
segLIST.append(int(length_hex, 16))
segLIST.append('0x' + checkSum)
segLIST.append(hex_dig)

if(checkSum == check_Sum):
	print int(sourcePort, 16)
	print int(destPort, 16)
	print int(length_hex, 16)
	print hex(int(check_Sum, 16))
	print hex_dig
else:
 	print "Invalid UDP segment"