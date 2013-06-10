OBJ=main.o
EXEC=wwm

all: $(EXEC)

$(EXEC): $(OBJ)
	gcc -lxcb -o $(EXEC) $^

%.o: %.c
	gcc -c $<

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
