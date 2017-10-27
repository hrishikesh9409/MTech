import string
import sys
import binascii

total = len(sys.argv)
# ip = raw_input()
ip = sys.argv[1]
binary = ''.join([bin(int(x)+256)[3:] for x in ip.split('.')])
a = binascii.a2b_qp(binary)
print a[0:15]
temp1 = a[0:15]
temp2 = a[16:31]
b = int(temp1, 2)
c = int(temp2, 2)
print b
print temp2

# def IP_to_HEX(ip):
# 	a = ip.split('.')
# 	b = hex(int(a[0]))[2:].zfill(2) + hex(int(a[1]))[2:].zfill(2) + hex(int(a[2]))[2:].zfill(2) + hex(int(a[3]))[2:].zfill(2)
# 	b = b.replace('0x', '')
# 	b = b.upper(
# 	c = [format(int(x), '08b') for x in ip.split('.')]
# 	d = ' ' .join(format(int(x), '08b') for x in ip.split('.'))
# 	print b
# 	print c
# 	return;

MOD = 1 << 16

def ones_comp_add16(num1,num2):
    result = num1 + num2
    return result if result < MOD else (result+1) % MOD

n1 = b
n2 = c
result = ones_comp_add16(n1,n2)

print('''\
  {:016b}
+ {:016b}
------------------
  {:016b}'''.format(n1,n2,result))

print result

# IP_to_HEX(ip)
# cmdargs = str(sys.argv)

# print ("The total numbers of args passed to the script: %d " % total)
# print ("Args list: %s " % cmdargs)