AR=ar rc libsystem.a
LIB=libsystem.a
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(LIB)
	@echo "compile libsystem.a"

libsystem.a: $(OBJ)
	@$(AR) $@ $^ 

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o
	@rm -rf *.a

mrproper: clean
	@rm -rf $(LIB)