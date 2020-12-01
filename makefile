all: compile echo

compile:
	@gcc -o server server.c
	@gcc -o client client.c

echo:
	@echo 'Compiled successfully'
	@echo 'Starting server: ./server [port]'
	@echo 'Starting client: ./client [server ip] [port]'