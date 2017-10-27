


FLAGS = -ggstabs --32
CC = gcc
AS = as
LD = ld
LDFLAGS= -m elf_i386
#OBJ=asm1.o asm2.o asm3.o

%.o: %.s 
	$(AS) -o $@ $(FLAGS) $<

#demo1:$(OBJ)
#	$(LD) -o $@ demo1.o

asm1: asm1.o
	$(LD) $(LDFLAGS) -o $@ $< 

asm2: asm2.o
	$(LD) $(LDFLAGS) -o $@ $<

asm3: asm3.o
	$(LD) $(LDFLAGS) -o $@ $< 

asm4: asm4.o
	$(LD) $(LDFLAGS) -o $@ $< 

asm5: asm5.o
	$(LD) $(LDFLAGS) -o $@ $< 

asm6: asm6.o
	$(LD) $(LDFLAGS) -o $@ $< 

helloworld: helloworld.o
	$(LD) $(LDFLAGS) -o $@ $<
 
vardemo: vardemo.o
	$(LD) $(LDFLAGS) -o $@ $<
all: asm1 asm2 asm3 asm4 asm5 asm6 helloworld vardemo
clean:
	rm -f *.o
	rm -f asm1 asm2 asm3 asm4 asm5 asm6 helloworld vardemo
