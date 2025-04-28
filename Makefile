parse:
	python3 ./state_machines/parser.py ./state_machines/automata_compiler.jff > ./data/transition_matrix.csv

all:
	gcc -o ./main ./src/main.c ./src/transition_matrix_creator.c

run:	all
	./main  $(FILE_PATH)
		
