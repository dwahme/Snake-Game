all: main.c
	gcc -Werror -Wall -fsanitize=address -o main main.c
