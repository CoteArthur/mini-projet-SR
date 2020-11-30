all: compile echo

compile:
	@gcc -o server server.c
	@gcc -o client client.c

echo:
	@echo 'Compiled successfully'