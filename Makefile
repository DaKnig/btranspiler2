name:=./boptimizer
executable:=bin/$(name)
output:=misc/output
expected:=misc/output

all: compile

compile: $(executable)

check: $(executable)
	$(executable)>$(output)
	diff $(expected) $(output)

run: $(executable)
	$(executable)

debug: $(executable)
	gdb -q $(executable)

$(executable): $(name).c
	gcc -g -o $(executable) $(name).c   -pedantic-errors -Wall -Wextra

clear:
	rm bin/*
