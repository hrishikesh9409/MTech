To run the program

1. compile and execute egg
2. compile and execute envaddr to obtain the address to the env variable
3. copy the address and paste it in exploit.py code 
4. execute the following command : python exploit.py > badfile
5. compile and execute stack (enable fno-stack-protector -z execstack flags and also -m32 flag) 