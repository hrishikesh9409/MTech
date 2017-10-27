CC = gcc

INCLUDE =

LIBS = 

# enter filename without extension
FILENAME = test

all: $(FILENAME).c
	@$(CC) -ggdb -z execstack -fno-stack-protector $(INCLUDE) $(FILENAME).c -m32 -o test $(LIBS)

tdebug:
	@gdb -tui ./$(FILENAME)

pdebug:
	@gdb ./$(FILENAME)

run:
	@./$(FILENAME)

clean:
	rm ./$(FILENAME)
