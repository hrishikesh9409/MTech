import string
import re
import binascii
import sys

def ip_to_hex(ip):
   i = ip.split('.')
   h = '{:02X}{:02X}{:02X}{:02X}'.format(*map(int, i))
   return h

def port_to_hex(port):
   p = (hex(port)[2:]).zfill(4)
   return p

def sum_r(list):
   total = 0
   mod = 1<<16
   for value in list:
      total = total + value
      if total>= mod:
         total=(total+1)% mod    
   return total

src_ip = str(sys.argv[1])
src_port =int(str(sys.argv[2]))
des_ip =str(sys.argv[3])
des_port =int(str(sys.argv[4]))
msg = str(sys.argv[5])

msg_length = int((len(msg))/2)
length = 8 + msg_length
len_hex = hex(length)[2:].zfill(4)

sip_hex = ip_to_hex(src_ip)
dip_hex = ip_to_hex(des_ip)

sip_list = []
sip_list.append(int(sip_hex[:4],16))
sip_list.append(int(sip_hex[4:],16))

dip_list = []
dip_list.append(int(dip_hex[:4],16))
dip_list.append(int(dip_hex[4:],16))

sport_hex = port_to_hex(src_port)
dport_hex = port_to_hex(des_port)

protocal_hex = (hex(17)[2:]).zfill(4)

len_data_hex = len(msg)
j=0
data_hex_list = []

while j < len_data_hex:
   data_hex_list.append(msg[j]+msg[j+1])
   j=j+2

i=0
msg_hex_append= data_hex_list + [hex(0)[2:].zfill(2)]
msg_list=[]
while i< msg_length:
    if(msg_length%2==0):
        c1 = data_hex_list[i].zfill(2)
        c2 = data_hex_list[i+1].zfill(2)
        c = c1+c2
        msg_list.append(c)
    else:
        c1= msg_hex_append[i].zfill(2)
        c2 = msg_hex_append[i+1].zfill(2)
        c=c1+c2
        msg_list.append(c)
    i=i+2

msg_dec_list = []    
for val in msg_list:
   msg_dec_list.append(int(val,16))

sum_sip = hex(sum_r(sip_list))[2:]
sum_dip = hex(sum_r(dip_list))[2:]
sum_msg = hex(sum_r(msg_dec_list))[2:]

hex_list = []
hex_list.append(sum_sip)
hex_list.append(sum_dip)
hex_list.append(sport_hex)
hex_list.append(dport_hex)
hex_list.append(protocal_hex)
hex_list.append(len_hex)
hex_list.append(len_hex)
hex_list.append(sum_msg)

l_hex = []
for h in hex_list:
   l_hex.append(int(h,16))

check_sum = hex(sum_r(l_hex)^65535)[2:]

segment=[]
segment.append(sport_hex)
segment.append(dport_hex)
segment.append(len_hex)
segment.append(check_sum)

segment ="".join(segment)

print(segment+msg)
