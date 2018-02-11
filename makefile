all: hello.c
	gcc -Werror -Wall -fsanitize=address -o hello hello.c
