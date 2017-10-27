import re
import sys
import socket

if (not len(sys.argv) == 2):
    print "Usage: ip2hex.py hostname|ip address/mask"
    sys.exit(1)

try:
    (in_str, mask) = sys.argv[1].split("/")
    # sanity check mask
    mask = int(mask)
    if (mask > 32 or mask < 0):
        print "Mask out of range"
        sys.exit(1)
except ValueError:
    mask = 0
    in_str = sys.argv[1]

try:
    ip_addr = socket.gethostbyname(in_str)
except:
    print "Invalid address!"
    sys.exit(1)

#gethostbyname really checks this for us, but you never know
ip_regex = re.compile('(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.' \
                      '(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.' \
                      '(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.' \
                      '(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)')
ip_match = ip_regex.match(ip_addr)

if (ip_match == None):
    print "Invalid address"
    sys.exit(1)

hex_ip_addr = 0
for i in range(1,5):
    hex_ip_addr += int(ip_match.group(i)) << (4-i)*8

fmt = "%%0%dX" % ((32 - mask) / 4)
print fmt % (hex_ip_addr >> mask)