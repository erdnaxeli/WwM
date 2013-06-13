OBJ=main.o \
	event.o \
	logger.o \
	tools.o \
	window.o \
	tag.o
EXEC=wwm

all: $(EXEC)

debug:
	make DEBUG='-g'

$(EXEC): $(OBJ)
	gcc -Wall -lxcb-util -lxcb -o $(EXEC) $^

%.o: %.c
	gcc -Wall $(DEBUG) -c $<

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
