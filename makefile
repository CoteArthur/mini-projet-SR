all: compile echo

compile:
	@gcc -c file-reading-utils.c
	@gcc -c server.c
	@gcc -o server file-reading-utils.o server.o
	@rm ./file-reading-utils.o
	@rm ./server.o
	@gcc -o client client.c

echo:
	@echo 'Compiled successfully'
	@echo 'Starting server: ./server [port]'
	@echo 'Starting client: ./client [server ip] [port]'