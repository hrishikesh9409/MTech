import struct
import binascii
import sys
import socket

def convertIPtoInt(ip_string):
    #Convert an IPv4 address from dotted-quad string format to 32-bit packed binary format
    packed_ip = socket.inet_aton(ip_string)
    #unpack the packed_ip into  a long int
    return (struct.unpack("!L", packed_ip)[0])

def getHeaderInBinary ():
    #convert the two ip address into int and then to hex
    header_bin = [0b0,0b0]

    header_bin[0] = '{0:08x}'.format(convertIPtoInt (sys.argv[2]))     #source ip
    header_bin[1] = '{0:08x}'.format(convertIPtoInt (sys.argv[3]))     #destination ip

    #return header_bin;
    return header_bin;

def getMessageInBinary():
    #extract the message part from the UDP segment to calculate checksum
    msg = sys.argv[1][16:].decode("hex")                    #copy the UDP message
    msg_len = len(msg)                                      #check the length of message
    msg_len_mod2 = msg_len % 2                              #is it even number of octets (a multiple of 16 bits)
    if((msg_len_mod2)!= 0):                                 #if its not even number of octets (not a multiple of 16 bits)
        msg = msg+"\x00"                                    #add 1 byte(octet) of zeroes to the end of the message
    return msg.encode("hex");


def consolidateInput():
    #group together all the values needed to calculate checksum
    header = getHeaderInBinary ()                               #get the ip address in hex
    message = getMessageInBinary()                              #get the message in hex

    message_octet_count =  len(sys.argv[1][16:].decode("hex"))  #length(number of octets) of actual message string
    udp_len = message_octet_count + 8                           #length of message + 8 byte length pf UDP header

    udp_checksum_str = ""

    #pseudo header

    udp_checksum_str += header[0]                       #source ip
    udp_checksum_str += header[1]                       #destination ip
    udp_checksum_str += '{0:02x}'.format(int(0))        #zeroes in pseudo packet
    udp_checksum_str += '{0:02x}'.format(int(17))       #protocol number - 17 for UDP (pseudo header)
    udp_checksum_str += '{0:04x}'.format(int(udp_len))  #length of udp packet (pseudo header)

    #UDP header
    udp_checksum_str += sys.argv[1][:4]                 #source port
    udp_checksum_str += sys.argv[1][4:8]                #destination port
    udp_checksum_str += '{0:04x}'.format(int(udp_len))  #length of udp packet (UDP header)
    udp_checksum_str += message                         #message/data

    #split the entire string into sets of 2hex numbers (16bits) each
    final_checksum_str=''
    for i in range(0,len(udp_checksum_str)):
        if((i%2 == 0 ) and ( i != 0)):
            final_checksum_str += (" " + udp_checksum_str[i])
            continue
        else:
            final_checksum_str += udp_checksum_str[i]

    return final_checksum_str


def carry_around_add(a, b):
    #perform one's complement additon of two hex numbers
    c = a + b                               #addition of first 16 bits
    return (c & 0xffff) + (c >> 16)         #addition of carry

def checksum(data):
    #calculate the checksum of a list of 16 bit binary numbers
    s = 0
    data = data.split()                               #split the string into list of strings indivifual 16 bit numbers
    data = map(lambda x: int(x,16), data)             #convert the individual strings into 16 bit integers
    data = struct.pack("%dB" % len(data), *data)      #pack the sets of integers into list of binary values

    #go through the list of binary numbers, jump 2 at a time since we add 2 numbers at a time
    for i in range(0, len(data), 2):
        w = (ord(data[i]) << 8) + ord(data[i+1])      #m has msg[i] as the MSB 8 bit and msg[i+1] as LSB 8bit
        s = carry_around_add(s, w)                    #do the addition of previous result(s) and the current 16bit sumber(w)
    return ~s & 0xffff                                #return only the last 16bit

def print_data():
    #print the contents of given UDP segment
    print int(sys.argv[1][:4],16)                   #source port
    print int(sys.argv[1][4:8],16)                  #destination port
    print int(sys.argv[1][8:12],16)                 #length
    print hex(int(sys.argv[1][12:16],16))           #checksum
    print sys.argv[1][16:].decode("hex")            #message


def validate():
    #call all the functions
    data = consolidateInput()                       #extract data from the parameters to calculate checksum
    checksum_packet = str(hex(checksum(data))[2:])  #calculate the checksum and ignore the '0x'
    if (checksum_packet == sys.argv[1][12:16]):     #if the checksum in the udp segment is same as calculated checksum
        print_data()                                #print the contents of the UDP segment
    else:
        print ("Invalid UDP segment")               #print the error message


validate()
