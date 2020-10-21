/*
Created by Diwei in 10/18/2020
This file includes the main body of the minishell,
and the functions related to io.
*/

#include "minishell.h"

// globals
volatile sig_atomic_t flag = 1;

void sigint_handler(int sig) {
    write(0, "\n", 1);
    write(1, "\nTerminating through signal handler\n", 36);

    flag = 0;
    return;
}

void init_shell() {
    system("clear");
    printf("******WELCOME TO MY SHELL*******\n");
    return;
}

void print_prompt() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Mini-shell:%s> ", cwd);
}

int get_input(char* cmdInput) {
    fgets(cmdInput, BUFFER_SIZE, stdin);

    if (strchr(cmdInput, '\n') == NULL) {
        int ch;
        int extra_data_found = 0;
        while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
            extra_data_found += 1;
        }
        printf("Extra data found: %d\n", extra_data_found);
        return 1;
    }

    return 0;
}

int parse_input(char* cmdInput, char* cmd[]) {
	char* tok;
	tok = strtok(cmdInput, " \n");

	int size = 0;
	while (tok != NULL) {
		cmd[size] = tok;
		size ++;
		tok = strtok(NULL, " \n");
	}
	cmd[size] = NULL;

	return size;
}

/*
NAME: minishell
DOES: take input from user, parse the input, act based on it, and loop
*/
void minishell(list_t *builtins)
{
    while (flag) {
        print_prompt();

        if (!flag) {
            break;
        }
        
        // get the input
        char cmdInput[BUFFER_SIZE];
        if (get_input(cmdInput)) {
            printf("Input is too long for my Mini-shell, please try other commands.\n");
            continue;
        }
        printf("You typed: %s\n", cmdInput);

        if (!flag) {
            break;
        }

        // parse the input
        char* cmd[TOKEN_SIZE];
        int size = parse_input(cmdInput, cmd);

        if (size == 0) {
            continue;
        }

        // try to run the command as a builtin
        exec_builtin(builtins, cmd[0]);

        // execute the cmd


    }

    return;
}

void main()
{
    signal(SIGINT, sigint_handler);

    list_t* builtins = (list_t*)malloc(sizeof(list_t));
    builtins->value = NULL;
    builtins->next = NULL;

    init_builtins(builtins);

    init_shell();
    minishell(builtins);

    free_builtins(builtins);

    return;
}
