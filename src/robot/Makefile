EXEC=bot
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)
	@echo "compile robot"

bot: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)
	@mv ./bot ../../

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)