Steps to execute the programs : 

	1. make
	2. make bin
	3. make virus
	4. ./mkbin --> enter the number of binary files to be created
	5. ./scan --> run the AV Scanner program (should be run first ahead of the virus program)
	6. ./virus --> infects all the binary files present within the directory

Note : the mkbin executable creates binary files based on the number entered when asked. A random value is also inserted into each binary file.