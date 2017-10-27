print "Enter identifier space"
m = input()
number = input("Enter number of nodes: " )

node = []
i = 0

while i < number:
	b = input("Enter node value: " )
	node.append(b)
	i = i + 1

node.sort()
print node



