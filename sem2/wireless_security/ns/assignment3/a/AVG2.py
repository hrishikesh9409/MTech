f = open("data3.temp", 'r')
s = f.readlines()
p = str(s)

total = 0

for line in s:
    #print(int(line))
    printnum = 0
    try: 
        printnum += float(line)
        total += printnum / 100
        #print("Adding: ", printnum)
    except ValueError:
        print("Invalid Literal for Int() With Base 10:", ValueError)

print(total/50)