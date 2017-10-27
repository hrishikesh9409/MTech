import struct
import sys
import socket
import binascii

def convertIPtoInt(ip_string):
    #Convert an IPv4 address from dotted-quad string format to 32-bit packed binary format
    packed_ip = socket.inet_aton(ip_string)
    #unpack the packed_ip into  a long int
    return (struct.unpack("!L", packed_ip)[0])

def getHeaderInBinary ():
    #convert the two ip address into int and then to hex
    header_bin = [0b0,0b0,0b0,0b0]

    header_bin[0] = '{0:08x}'.format(convertIPtoInt (sys.argv[1]))     #source ip
    header_bin[1] = '{0:04x}'.format(int(sys.argv[2]))                 #source port
    header_bin[2] = '{0:08x}'.format(convertIPtoInt (sys.argv[3]))     #destination ip
    header_bin[3] = '{0:04x}'.format(int(sys.argv[4]))                 #destination port

    #return header_bin;
    return header_bin;

def getMessageInBinary():
    #extract the message part from the UDP segment to calculate checksum
    msg = sys.argv[5]                                       #copy the UDP message
    msg_len = len(msg)                                      #check the length of message
    msg_len_mod2 = msg_len % 2                              #is it even number of octets
    if((msg_len_mod2)!= 0):                                 #if its not even number of octets
        msg = msg+"\x00"                                    #add 1 byte(octet) of zeroes to the end of the message
    # '''
    # msg_len_appended = len(msg)                             #calculate the new length

    # msg_bin = int(binascii.hexlify(msg),16)                 #convert into binary
    # format_pattern = '{0:0'+str(msg_len_appended * 2)+'x}'  #make sure it prints all the leading zeroes

    # #return the stream of bits
    # return format_pattern.format(msg_bin)
    # '''
    return msg.encode("hex");
def consolidateInput():
    #group together all the values needed to calculate checksum
    header = getHeaderInBinary ()                       #get the ip address in hex
    message = getMessageInBinary()                      #get the message in hex
    message_octet_count =  len(sys.argv[5])             #length(number of octets) of actual message string

    udp_len = message_octet_count + 8                   #length of message + 8 byte length pf UDP header

    udp_checksum_str = ""

    #pseudo header

    udp_checksum_str += header[0]                       #source ip
    udp_checksum_str += header[2]                       #destination ip
    udp_checksum_str += '{0:02x}'.format(int(0))        #zeroes in pseudo packet
    udp_checksum_str += '{0:02x}'.format(int(17))       #protocol number - 17 for UDP (pseudo header)
    udp_checksum_str += '{0:04x}'.format(int(udp_len))  #length of udp packet (pseudo header)

    #UDP header

    udp_checksum_str += header[1]                       #source port
    udp_checksum_str += header[3]                       #destination port
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

def checksum():
    #calculate the checksum of a list of 16 bit binary numbers
    s = 0
    #go through the list of binary numbers, jump 2 at a time
    data = consolidateInput()
    data = data.split()
    data = map(lambda x: int(x,16), data)
    data = struct.pack("%dB" % len(data), *data)        #pack the sets of integers into list of binary values

    for i in range(0, len(data), 2):
        w = (ord(data[i]) << 8) + ord(data[i+1])        #m has msg[i] as the MSB 8 bit and msg[i+1] as LSB 8bit
        s = carry_around_add(s, w)                      #do the addition of previous result(s) and the current 16bit sumber(w)
    return ~s & 0xffff                                  #return only the last 16bit


def printOutput():
    #call the components
    checksum_udp = checksum()
    #checksum_udp = hex(checksum_udp)[2:]

    final_output = ""
    final_output += '{0:04x}'.format(int(sys.argv[2]))          #source port
    final_output += '{0:04x}'.format(int(sys.argv[4]))          #destination port
    udp_len = len(sys.argv[5])

    final_output += '{0:04x}'.format(int(udp_len));             #length of UDP segment
    final_output += '{0:04x}'.format(int(checksum_udp));        #checksum

    # '''
    # format_pattern = '{0:0'+str(udp_len)+'x}'               #format for message
    # msg_bin = int(binascii.hexlify(sys.argv[5]),16)             #converting message to string
    # final_output += format_pattern.format(msg_bin)              #message
    # '''

    final_output += str (sys.argv[5].encode("hex"))             #encode the output to hex
    return (final_output)

msgout = printOutput()
print msgout