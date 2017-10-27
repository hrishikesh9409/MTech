import socket
import binascii
import struct

import sys

def ChecksumCheck(d):

    s = 0
    d = d.split()                             
    d = map(lambda x: int(x,16), d)           
    d = struct.pack("%dB" % len(d), *d)      

    for i in range(0, len(d), 2):
        y = (ord(d[i]) << 8) + ord(d[i+1])   
        e = s + y
        s = (e & 0xffff) + (e >> 16)                  
    return ~s & 0xffff 


messageString = sys.argv[1][16:].decode("hex")                    
messageString_len = len(messageString)                                   
messageString_len_mod2 = messageString_len % 2                             
if((messageString_len_mod2)!= 0):                                 
    messageString = messageString+"\x00"                                    
messageStringTOHEX= messageString.encode("hex");

UDP_Header_bin = [0b0,0b0]
packed_ip1 = socket.inet_aton(sys.argv[2])
packed_ip2 = socket.inet_aton(sys.argv[3])

UDP_Header_bin[0] = '{0:08x}'.format(struct.unpack("!L", packed_ip1)[0])     #source ip
UDP_Header_bin[1] = '{0:08x}'.format(struct.unpack("!L", packed_ip2)[0])

Header_BINARY = UDP_Header_bin;



UDP_Header = Header_BINARY          
message = messageStringTOHEX                          

message_octet_count =  len(sys.argv[1][16:].decode("hex"))  
udp_len = message_octet_count + 8            

UDP_Chksm_str = ""

  #pseudo UDP_Header

UDP_Chksm_str += UDP_Header[0]                       #source ip
UDP_Chksm_str += UDP_Header[1]                       #destination ip
UDP_Chksm_str += '{0:02x}'.format(int(0))        
UDP_Chksm_str += '{0:02x}'.format(int(17))       #protocol number 
UDP_Chksm_str += '{0:04x}'.format(int(udp_len))  #length of udp packet (pseudo UDP_Header)

    #UDP UDP_Header
UDP_Chksm_str += sys.argv[1][:4]                 #source port
UDP_Chksm_str += sys.argv[1][4:8]                #destination port
UDP_Chksm_str += '{0:04x}'.format(int(udp_len))  #length of udp packet (UDP UDP_Header)
UDP_Chksm_str += message                         

complete_checksum=''
for i in range(0,len(UDP_Chksm_str)):
    if((i%2 == 0 ) and ( i != 0)):
        complete_checksum += (" " + UDP_Chksm_str[i])
        continue
    else:
        complete_checksum += UDP_Chksm_str[i]

checkSUM_STRING = complete_checksum


d = checkSUM_STRING                      
checksum_packet = str(hex(ChecksumCheck(d))[2:])  
if (checksum_packet == sys.argv[1][12:16]):     
    print int(sys.argv[1][:4],16)                   #source port
    print int(sys.argv[1][4:8],16)                  #destination port
    print int(sys.argv[1][8:12],16)                 #length
    print hex(int(sys.argv[1][12:16],16))           #checksum
    print sys.argv[1][16:].decode("hex")                             
else:
    print ("Invalid UDP_segment")              