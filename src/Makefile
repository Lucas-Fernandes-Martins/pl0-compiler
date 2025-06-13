CC = gcc
CFLAGS = -Wall -Wextra -Isrc

EXEC = compiler

SRC = main.c \
      grammar/grammar_sets.c \
      data_structures/hash_table.c \
      data_structures/transition_table.c \
      analysers/dfa.c \
      analysers/lexer.c \
      analysers/syntatic.c

OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
