parse:
	python3 ./src/parser.py ./state_machines/automata_compiler.jff > ./data/var.csv

all:
	gcc -o ./main ./src/main.c ./src/dfa.c ./src/utils.c

run:	all
	./main  $(FILE_PATH)
		
