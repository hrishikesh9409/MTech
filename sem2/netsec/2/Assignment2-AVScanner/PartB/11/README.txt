The comparison of the dump of 1.bin and hello is given in the hexdump.png image. Both the dumps are identical although 1.bin has more padded 00 bytes in addition to the similar dump to the executable hello.

To run the program do the following :
	1. make bin
	2. make virus
	3. ./mkbin and enter the number of executables to be made 
	4. ./virus will read the contents from the "corrupted" hello executable, store it in memory and corrupt all .bin files in the directory
	5. The output of the program is an stdout to the user prompting "BUSTED" as given in output.png

Note : the mkbin executable creates binary files based on the number entered when asked. A random value is also inserted into each binary file.