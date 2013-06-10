OBJ=main.o \
	event.o
EXEC=wwm

all: $(EXEC)

$(EXEC): $(OBJ)
	gcc -Wall -lxcb-util -lxcb -o $(EXEC) $^

%.o: %.c
	gcc -Wall -c $<

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
