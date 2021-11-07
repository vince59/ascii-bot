EXEC=mapper
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)
	@echo "compile mapper"

mapper: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)
	@mv ./mapper ../../

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)