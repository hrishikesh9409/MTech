import string
import re
import binascii
import hashlib

#Convert IP Addresses to coresponding hex values
def ip_to_hex(ip):
   i = ip.split('.')
   h = '{:02X}{:02X}{:02X}{:02X}'.format(*map(int, i))
   return h
 
#Addition routine
def sum_r(list):
   total = 0
   mod = 1<<16
   for value in list:
      total = total + value
      if total>= mod:
         total=(total+1)% mod    
   return total
   
#Inputing the values
segment = input('Enter segment: ')
src_ip  = input('Enter source IP: ')
des_ip  = input('Enter destination IP: ')

#Extracting values from segment
sport_hex            = segment[:4]
dport_hex            = segment[4:8]
sequence_hex         = segment[8:16]
acknowlege_hex       = segment[16:24]
offset_hex           = segment[24:25].zfill(4)
of_re_fl_hex         = segment[24:28]
flag_hex             = segment[26:28].zfill(4)
window_hex           = segment[28:32]
checksum_hex         = segment[32:36]
urgent_pointer_hex   = segment[36:40]
tcp_segment_length   = int(segment[24:25])*8
option               = segment[40:tcp_segment_length]
data_hex             = segment[tcp_segment_length:]

#Conveting Source IP to hex and converting the hex two 16 bit numbers and adding them
sip_hex = ip_to_hex(src_ip)
sip_list = []
sip_list.append(int(sip_hex[:4],16))
sip_list.append(int(sip_hex[4:],16))
sum_sip = hex(sum_r(sip_list))[2:]

#Conveting Destination IP to hex and converting the hex two 16 bit numbers and adding them
dip_hex = ip_to_hex(des_ip)
dip_list = []
dip_list.append(int(dip_hex[:4],16))
dip_list.append(int(dip_hex[4:],16))
sum_dip = hex(sum_r(dip_list))[2:]

#Conveting Sequence Number to two hex number and adding them  
seq_list=[]
seq_list.append(int(sequence_hex[:4],16))
seq_list.append(int(sequence_hex[4:],16))
sum_seq = hex(sum_r(seq_list))[2:]

#Conveting Acknowledge Number to two 16 bit hex number and adding them 
ack_list=[]
ack_list.append(int(acknowlege_hex[:4],16))
ack_list.append(int(acknowlege_hex[4:],16))
sum_ack = hex(sum_r(ack_list))[2:]

#Conveting Option to 16 bit hex numbers and adding them 
option1_list=[]
len_option = len(option)
k=0
if len_option%4==0:
   while k < len_option:
      option1_list.append(option[k]+option[k+1]+option[k+2]+option[k+3])
      k=k+4
else:
   while len_option%4!=0:
      option.zfill(1)
   while k < len_option:
      option1_list.append(option[k]+option[k+1]+option[k+2]+option[k+3])
      k=k+4
option_dec_list=[]
for val in option1_list:
   option_dec_list.append(int(val,16))
sum_option = hex(sum_r(option_dec_list))[2:]   

#Finding length of data segment
len_data_hex = len(data_hex)

#Conveting data segment to corresponding message and calculating its length
msg = ''.join(chr(int(data_hex[i:i+2], 16)) for i in range(0, len_data_hex, 2))
msg_length = len(msg)

#Conveting Data Segment to 16 bit numbers and padding zeroes whereever necessary and adding them
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

#Specifing Reserve,Protocal Number and Length of segment
reserve_hex = hex(0)[2:].zfill(4)
protocal_hex = (hex(6)[2:]).zfill(4)
length_hex = hex((int(offset_hex,16)*4) + msg_length)[2:].zfill(4)

#Appending every values to lists for finding the total sum
hex_list1 = []
hex_list2 = []
hex_list3 = []
hex_list4 = []
hex_list1.append(sum_sip)
hex_list1.append(sum_dip)
hex_list1.append(reserve_hex)
hex_list1.append(protocal_hex)
hex_list2.append(length_hex)
hex_list2.append(sport_hex)
hex_list2.append(dport_hex)
hex_list2.append(sum_seq)
hex_list3.append(sum_ack)
hex_list3.append(of_re_fl_hex)
hex_list4.append(window_hex)
hex_list4.append(urgent_pointer_hex)
hex_list4.append(sum_option)
hex_list4.append(sum_msg)

l_hex1 = []
for h in hex_list1:
   l_hex1.append(int(h,16))
l_hex2 = []
for h in hex_list2:
   l_hex2.append(int(h,16))
l_hex3 = []
for h in hex_list3:
   l_hex3.append(int(h,16))
l_hex4 = []
for h in hex_list4:
   l_hex4.append(int(h,16))

sum1= sum_r(l_hex1)
sum2= sum_r(l_hex2)
sum3= sum_r(l_hex3)
sum4= sum_r(l_hex4)
sum_list=[]
sum_list.append(sum1)
sum_list.append(sum2)
sum_list.append(sum3)
sum_list.append(sum4)

#Taking complement to find checksum
check_sum = hex(sum_r(sum_list)^65535)[2:]
#print(check_sum)

#Hashing the message
msg = ''.join(chr(int(data_hex[i:i+2], 16)) for i in range(0, len_data_hex, 2))
hash_object = hashlib.sha256(msg.encode('ascii'))
hex_dig = hash_object.hexdigest()
#print(hex_dig)

#Appending values to a list for output
tcp_segment = []
tcp_segment.append(int(sport_hex,16))
tcp_segment.append(int(dport_hex,16))
tcp_segment.append(int(sequence_hex,16))
tcp_segment.append(int(acknowlege_hex,16))
tcp_segment.append(int(flag_hex,16))
tcp_segment.append(int(window_hex,16))
tcp_segment.append(int(checksum_hex,16))
tcp_segment.append(hex_dig)

#Validating Segment
if checksum_hex == check_sum:
   for items in tcp_segment:
      print(items)
else:
   print('Invalid TCP segment')
