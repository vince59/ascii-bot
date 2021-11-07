EXEC=simu
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)
	@echo "compile simulator"

simu: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)
	@mv ./simu ../../

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)