import binascii
import sys
import socket
import struct

# messageString = sys.argv[5]  
# print sys.argv[5]
# messageString_len = len(messageString)
# messageString_len_mod2 = messageString_len % 2                              #is it even number of octets
# if((messageString_len_mod2)!= 0):                                 #if its odd number of octets
# 	messageString = messageString+"\x00"   
# messageStringTOHEX = messageString.encode("hex");
messageStringTOHEX = sys.argv[5]

#convert 2 ip address into int and then to hex
UDP_Header_bin = [0b0,0b0,0b0,0b0]

packed_ip = socket.inet_aton(sys.argv[1])
packed_ip2 = socket.inet_aton(sys.argv[3])

UDP_Header_bin[0] = '{0:08x}'.format((struct.unpack("!L", packed_ip)[0]))     #source ip
UDP_Header_bin[1] = '{0:04x}'.format(int(sys.argv[2]))                 #source port
UDP_Header_bin[2] = '{0:08x}'.format((struct.unpack("!L", packed_ip2)[0]))     #destination ip
UDP_Header_bin[3] = '{0:04x}'.format(int(sys.argv[4]))                 #destination port

Header_BINARY = UDP_Header_bin;



UDP_Header = Header_BINARY   
message = messageStringTOHEX                     
message_octet_count =  len(sys.argv[5])         

udp_len = message_octet_count + 8              

UDP_Chksm_str = ""

UDP_Chksm_str += UDP_Header[0]                       #source ip
UDP_Chksm_str += UDP_Header[2]                       #destination ip
UDP_Chksm_str += '{0:02x}'.format(int(0))     
UDP_Chksm_str += '{0:02x}'.format(int(17))       #protocol number 
UDP_Chksm_str += '{0:04x}'.format(int(udp_len))  #length of udp packet

    #UDP UDP_Header

UDP_Chksm_str += UDP_Header[1]                       #source port
UDP_Chksm_str += UDP_Header[3]                       #destination port
UDP_Chksm_str += '{0:04x}'.format(int(udp_len))  #length of udp packet (UDP UDP_Header)
UDP_Chksm_str += message                         #message/d

complete_checksum_Str=''
for i in range(0,len(UDP_Chksm_str)):
	if((i%2 == 0 ) and ( i != 0)):
		complete_checksum_Str += (" " + UDP_Chksm_str[i])
		continue
	else:
		complete_checksum_Str += UDP_Chksm_str[i]

checkSUM_STRING = complete_checksum_Str

t = 0
    
d = checkSUM_STRING
d = d.split()
d = map(lambda x: int(x,16), d)
d = struct.pack("%dB" % len(d), *d)       
for i in range(0, len(d), 2):
	y = (ord(d[i]) << 8) + ord(d[i+1])       
	e = t + y 
	t = (e & 0xffff) + (e >> 16)                    

checksum_ordering = ~t & 0xffff  

checksum_udp = checksum_ordering

Output = ""
Output += '{0:04x}'.format(int(sys.argv[2]))      
Output += '{0:04x}'.format(int(sys.argv[4]))   
udp_len = len(sys.argv[5])

Output += '{0:04x}'.format(int(udp_len)); 
Output += '{0:04x}'.format(int(checksum_udp));      

Output += str (sys.argv[5].encode("hex"))     
print Output