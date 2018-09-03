name:=./boptimizer

all: run

run: $(name)
	$(name)

debug: $(name)
	gdb -q $(name)

$(name): $(name).c
	gcc -g -o $(name) $(name).c   -pedantic-errors #-Wall -Wextra

clear:
	rm $(name)
