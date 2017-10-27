#! /bin/python

import string
import sys
import binascii
import socket
import hashlib

def convertIp2Int(ip):
	binary = ''.join([bin(int(x)+256)[3:] for x in ip.split('.')])
	ipInt = binascii.a2b_qp(binary)
	return ipInt

MOD = 1 << 16

def ones_comp_add16(num1,num2):
    return result if result < MOD else (result+1) % MOD
    result = num1 + num2

def dec_to_bin(x):
    return int(bin(x)[2:])

def hexsize(hex_message):
	return (len(hex_message) * 4)
def hex2bin(hex_message, hex_message_size):
	return (( bin(int(hex_message, 16))[2:] ).zfill(hex_message_size))

def summation(array):
	add = 0
	mod = 1<<16
	for index in range(len(array)):
		add = add + array[index]
		if add >= mod: 
			add = (add + 1) % mod
	return add



message = sys.argv[1]

sport = message[:4]
sport_size = hexsize(sport)
sportbin = hex2bin(sport, sport_size)

dport = message[4:8]
dport_size = hexsize(dport)
dportbin = hex2bin(dport, dport_size)

seq = message[8:16]
seqINT = int(seq, 16)
seq_size = hexsize(seq)
seqbin = hex2bin(seq, seq_size)

seqbin1 = seqbin[:16]
seqbin2 = seqbin[16:32]

ack = message[16:24]
ackINT = int(ack, 16)
ack_size = hexsize(ack)
ackbin = hex2bin(ack, ack_size)

ackbin1 = ackbin[:16]
ackbin2 = ackbin[16:32]

tcpheader = message[24:28]
tcpheader_size = hexsize(ack)
tcpheaderbin = hex2bin(tcpheader, tcpheader_size)

flag = message[26:28]
flagInt = int(flag, 16)

tcpheaderbin1 = tcpheaderbin[:16]
tcpheaderbin2 = tcpheaderbin[16:32]

window = message[28:32]
window_size = hexsize(window)
windowbin = hex2bin(window, window_size)

checksum = message[32:36]

urgentpointer = message[36:40]
urgent_size = hexsize(urgentpointer)
urgentpointerbin = hex2bin(urgentpointer, urgent_size)

protocol = 6
prot = hex(6)
prot_size = hexsize(prot)
protbin = hex2bin(prot, prot_size)
protbin2 = protbin.zfill(16)


offset = message[24:25]
offset_size = hexsize(offset)
offsetbin = hex2bin(offset, offset_size)
offsetbin2 = offsetbin.zfill(16)

offsetint = int(offset, 16)

tcp_header_length = offsetint * 4 * 2

options_length = tcp_header_length - 40


data = len(message) - tcp_header_length
data1 = data/2
tcpcompletelength = tcp_header_length / 2 + data1

###############################################################

offsetdata = message[40:tcp_header_length]
offsetdata_size = hexsize(offsetdata)
offsetdataBin = hex2bin(offsetdata, offsetdata_size)

offsetdataBin1 = offsetdataBin[:16]
offsetdataBin2 = offsetdataBin[16:32]
offsetdataBin3 = offsetdataBin[32:48]
offsetdataBin4 = offsetdataBin[48:64]
offsetdataBin5 = offsetdataBin[64:80]
offsetdataBin6 = offsetdataBin[80:]

offsetdataINT1 = int(offsetdataBin1, 2)
offsetdataINT2 = int(offsetdataBin2, 2)
offsetdataINT3 = int(offsetdataBin3, 2)
offsetdataINT4 = int(offsetdataBin4, 2)
offsetdataINT5 = int(offsetdataBin5, 2)
offsetdataINT6 = int(offsetdataBin6, 2)

offsetdataINT = offsetdataINT1 + offsetdataINT2 + offsetdataINT3 +offsetdataINT4 + offsetdataINT5 + offsetdataINT6


offsetLIST = [offsetdataINT1, offsetdataINT2, offsetdataINT3, offsetdataINT4, offsetdataINT5, offsetdataINT6]


tcpdata = message[tcp_header_length:]
tcpdata_size = hexsize(tcpdata)

i = 0
tcpdata_LIST = []
while(i < len(tcpdata)):
	tcpdata_LIST.append(tcpdata[i] + tcpdata[i+1])
	i = i + 2
tcpdata_LIST2 = tcpdata_LIST + [hex(0)[2:].zfill(2)]
tcpdata_LIST3 = []

j = 0
while(j < int((len(tcpdata)/2))):
	if(int(len(tcpdata)/2) == 0):
		d1 = tcpdata_LIST[j].zfill(2)
		d2 = tcpdata_LIST[j+1].zfill(2)
		d = d1 + d2
		tcpdata_LIST3.append(d)
	else:
		d1 = tcpdata_LIST2[j].zfill(2)
		d2 = tcpdata_LIST2[j+1].zfill(2)
		d = d1 + d2
		tcpdata_LIST3.append(d)
	j = j + 2

tcpdata_LIST4 = []
for value in tcpdata_LIST3:
	tcpdata_LIST4.append(int(value, 16))


# if(int((len(tcpdata)/2)) % 2 != 0):
# 	tcpdata_append = tcpdata + hex(0)[2:].zfill(2)
# if(len(tcpdata) % 2 == 0):
# 	if(len(tcpdata) != 0):
		# tcpdataBin = hex2bin(tcpdata, tcpdata_size)
		# tcpdataBin1 = tcpdataBin[:16]
		# tcpdataBin2 = tcpdataBin[16:]



ip1 = sys.argv[2]
ip2 = sys.argv[3]

ip1Int = convertIp2Int(ip1)
ip2Int = convertIp2Int(ip2)


sportbinInt = int(sportbin, 2)
dportbinInt = int(dportbin, 2)
seqbin1Int = int(seqbin1, 2)
seqbin2Int = int(seqbin2, 2)
ackbin1Int = int(ackbin1, 2)
ackbin2Int = int(ackbin2, 2)
tcpheader1Int = int(tcpheaderbin1, 2)
tcpheader2Int = int(tcpheaderbin2, 2)
windowInt = int(windowbin, 2)
urgentInt = int(urgentpointerbin, 2)
protocolInt = int(protbin2, 2)
offsetInt = int(offsetbin2, 2)
# if(len(tcpdata) == 0):
# 	tcpdataBin1Int = 0
# 	tcpdataBin2Int = 0
# else:
# 	tcpdataBin1Int = int(tcpdataBin1, 2)
# 	tcpdataBin2Int = int(tcpdataBin2, 2)






ip116bit1 = ip1Int[:16]
ip116bit2 = ip1Int[16:32]

ip216bit1 = ip2Int[:16]
ip216bit2 = ip2Int[16:32]

ip11 = int(ip116bit1, 2)
ip12 = int(ip116bit2, 2)
ip21 = int(ip216bit1, 2)
ip22 = int(ip216bit2, 2)


ip1LIST = [ip11, ip12] 
ip2LIST = [ip21, ip22]
ip1 = summation(ip1LIST)
ip2 = summation(ip2LIST)


sequence = [seqbin1Int, seqbin2Int]
seqtotal = summation(sequence)

acknowledgment = [ackbin1Int, ackbin2Int]
acktotal = summation(acknowledgment)

tcpLIST = [tcpheader1Int, tcpheader2Int]
tcptotal = summation(tcpLIST)

# tcpdataLIST = [tcpdataBin1Int, tcpdataBin2Int]
tcpdatatotal = summation(tcpdata_LIST4)

offsetSUM = summation(offsetLIST)

messagelist = [ip1, ip2, sportbinInt, dportbinInt, seqtotal, acktotal, tcptotal, tcpdatatotal, windowInt, urgentInt, protocolInt, offsetSUM, tcpcompletelength]

msglist1 = [ip1, ip2, protocolInt, tcpdatatotal]
msglist2 = [tcpcompletelength, sportbinInt, dportbinInt, seqtotal]
msglist3 = [acktotal, tcptotal]
msglist4 = [windowInt, urgentInt, offsetSUM]

msg = msglist1+msglist2+msglist3+msglist4
totalSUM = summation(msg)

check_sum = hex(totalSUM ^ 65535)[2:]


tcpdataHASH = ''.join(chr(int(tcpdata[i:i+2], 16)) for i in range(0, data, 2))
hash_object = hashlib.sha256(tcpdataHASH.encode('ascii'))
hex_dig = hash_object.hexdigest()

finalmsg = [sportbinInt, dportbinInt, seqINT, ackINT, flagInt, windowInt, int(check_sum, 16), hex_dig]

if(check_sum == checksum):
	for i in range(len(finalmsg)):
		print finalmsg[i]
else:
 	print "Invalid TCP segment" 
