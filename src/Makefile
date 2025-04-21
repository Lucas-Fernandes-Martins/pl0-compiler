all:
	gcc -o main main.c dfa.c utils.c 

test_1:
	./main ../data/ex1

test_2:
	./main ../data/ex2

test_csv_parsing:
	gcc -o dfa dfa.c headers.h
	./dfa ../data/var.csv

parse:
	python3 parser.py ../state_machines/automata_compiler.jff > ../data/var.csv
		
