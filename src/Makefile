all:
	gcc -o main main.c dfa.c 

test_1:
	./main ../data/ex1

test_2:
	./main ../data/1.txt

test_csv_parsing:
	gcc -o dfa dfa.c headers.h
	./dfa ../data/test.csv
