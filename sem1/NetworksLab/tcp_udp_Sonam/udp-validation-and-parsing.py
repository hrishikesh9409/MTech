import string
import re
import binascii
import hashlib
import sys

def ip_to_hex(ip):
   i = ip.split('.')
   h = '{:02X}{:02X}{:02X}{:02X}'.format(*map(int, i))
   return h

def sum_r(list):
   total = 0
   mod = 1<<16
   for value in list:
      total = total + value
      if total>= mod:
         total=(total+1)% mod    
   return total

segment = str(sys.argv[1])
src_ip = str(sys.argv[2])
des_ip = str(sys.argv[3])

sip_hex = ip_to_hex(src_ip)
dip_hex = ip_to_hex(des_ip)

sip_list = []
sip_list.append(int(sip_hex[:4],16))
sip_list.append(int(sip_hex[4:],16))

dip_list = []
dip_list.append(int(dip_hex[:4],16))
dip_list.append(int(dip_hex[4:],16))

sum_sip = hex(sum_r(sip_list))[2:]
sum_dip = hex(sum_r(dip_list))[2:]

sport_hex = segment[:4]
dport_hex = segment[4:8]
length_hex =segment[8:12]
checksum_hex = segment[12:16]
data_hex= segment[16:]

protocal_hex = (hex(17)[2:]).zfill(4)

msg_length = int(length_hex,16)- 8

len_data_hex = len(data_hex)
j=0
data_hex_list = []

while j < len_data_hex:
   data_hex_list.append(data_hex[j]+data_hex[j+1])
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

sum_msg = hex(sum_r(msg_dec_list))[2:]

hex_list = []
hex_list.append(sum_sip)
hex_list.append(sum_dip)
hex_list.append(sport_hex)
hex_list.append(dport_hex)
hex_list.append(protocal_hex)
hex_list.append(length_hex)
hex_list.append(length_hex)
hex_list.append(sum_msg)

l_hex = []
for h in hex_list:
   l_hex.append(int(h,16))

check_sum = hex(sum_r(l_hex)^65535)[2:]

msg = ''.join(chr(int(data_hex[i:i+2], 16)) for i in range(0, len_data_hex, 2))
hash_object = hashlib.sha256(msg.encode('ascii'))
hex_dig = hash_object.hexdigest()

udp_segment = []
udp_segment.append(int(sport_hex,16))
udp_segment.append(int(dport_hex,16))
udp_segment.append(int(length_hex,16))
udp_segment.append('0x'+checksum_hex)
udp_segment.append(hex_dig)

if checksum_hex == check_sum:
   for items in udp_segment:
      print(items)
else:
   print('Invalid UDP segment')
