/**********************************************************************
 * Copyright (c) 2021
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>

#include <string.h>
#include<sys/wait.h>

#include "types.h"
#include "list_head.h"
#include "parser.h"

struct entry {
	struct list_head list;
	char* string;
};

struct list_head history;

struct entry* origin = (struct entry*)malloc(sizeof(struct entry));
origin->list = &history;

/***********************************************************************
 * run_command()
 *
 * DESCRIPTION
 *   Implement the specified shell features here using the parsed
 *   command tokens.
 *
 * RETURN VALUE
 *   Return 1 on successful command execution
 *   Return 0 when user inputs "exit"
 *   Return <0 on error
 */
int run_command(int nr_tokens, char *tokens[])
{
	if (strcmp(tokens[0], "exit") == 0) return 0;

	else if(strcmp(tokens[0],"cd")==0){
		int return_value=-1;
		if(tokens[1]==NULL || strcmp(tokens[1],"~")==0){
			return_value = chdir(getenv("HOME"));
		}
		else if(strcmp(tokens[1],"-")==0){
			return_value = chdir(getenv("OLDPWD"));
		}
		else if(strcmp(tokens[1], "..")==0){
			char temp[MAX_COMMAND_LEN];
			strcpy(temp, getenv("PWD"));
			char* slash = strchr(temp, '/');
			*slash = '\0';
			return_value = chdir(temp);	
		}
		else if(strcmp(tokens[1], ".")==0){
			return_value = chdir(getenv("PWD"));
		}
		else{
			return_value = chdir(tokens[1]);
		}

		if(return_value == -1){
			fprintf(stderr, "cd: %s: No such file or directory\n", tokens[1]);
			return -1;
		}
		else{
			char cwd[MAX_COMMAND_LEN];
			getcwd(cwd, sizeof(cwd));
			setenv("OLDPWD",getenv("PWD"),1);
			setenv("PWD", cwd,1);
		}
		return 1;

	}

	else if(strcmp(tokens[0],"history")==0){
		int num_temp=0;
		struct entry* temp;
		list_for_each_entry_continue_reverse(temp, &history, list){
			fprintf(stderr, "%2d: %s", num_temp, temp->string);
			num_temp++;
		}
		return 1;
	}
	else if(strcmp(tokens[0],"!")==0){
		int num_temp=0;
		int temp = atoi(tokens[1]);
		char* arr_temp;
		struct entry* entry_temp;
		list_for_each_entry_continue_reverse(entry_temp, &history, list){
			arr_temp = entry_temp->string;
			num_temp++;
			if(num_temp>temp) break;
		}

		return 1;
	}
	
	int pipe_check=0;
	int com_num;
	for(int i=0;i<nr_tokens;i++){
		if(strcmp(tokens[i],"|")==0){
			pipe_check=1;
			com_num= i+1;
			break;
		}
	}

	if(pipe_check==1){
		pid_t pid1;
		pid_t pid2;
		int fd[2];

		pipe(fd);
		pid1 = fork();

		if(pid1==0){
			close(fd[1]);
			dup2(fd[0],STDIN_FILENO);
			close(fd[0]);
			execvp(tokens[0],tokens);
		}
		else{
			pid2=fork();

			if(pid2==0){
				close(fd[0]);
				dup2(fd[1],STDOUT_FILENO);
				close(fd[1]);
				execvp(tokens[com_num],tokens);
			}
			close(fd[0]);
			close(fd[1]);
			waitpid(-1,NULL,0);
			waitpid(-1,NULL,0);
		}
		return 1;
	}

	pid_t pid = fork();

	if(pid>0){
		int status;
		waitpid(pid, &status, 0);

		if(WIFEXITED(status)){
			return 1;
		}
		else{

		}
	}
	else{
		int exe_status=0;
		exe_status=execvp(tokens[0], tokens);
		if(exe_status==-1){
			fprintf(stderr, "Unable to execute %s\n", tokens[0]);
			return -EINVAL;
		}
	}
}


/***********************************************************************
 * struct list_head history
 *
 * DESCRIPTION
 *   Use this list_head to store unlimited command history.
 */
LIST_HEAD(history);


/***********************************************************************
 * append_history()
 *
 * DESCRIPTION
 *   Append @command into the history. The appended command can be later
 *   recalled with "!" built-in command
 */
static void append_history(char * const command)
{
	struct entry* E1 = (struct entry*)malloc(sizeof(struct entry));
	E1->string = (char*)malloc(sizeof(char)*(strlen(command)+1));
	strcpy(E1->string, command);
	list_add(&E1->list, &history);
}


/***********************************************************************
 * initialize()
 *
 * DESCRIPTION
 *   Call-back function for your own initialization code. It is OK to
 *   leave blank if you don't need any initialization.
 *
 * RETURN VALUE
 *   Return 0 on successful initialization.
 *   Return other value on error, which leads the program to exit.
 */
static int initialize(int argc, char * const argv[])
{
	return 0;
}


/***********************************************************************
 * finalize()
 *
 * DESCRIPTION
 *   Callback function for finalizing your code. Like @initialize(),
 *   you may leave this function blank.
 */
static void finalize(int argc, char * const argv[])
{

}


/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING BELOW THIS LINE ******      */
/*          ****** BUT YOU MAY CALL SOME IF YOU WANT TO.. ******      */
static int __process_command(char * command)
{
	char *tokens[MAX_NR_TOKENS] = { NULL };
	int nr_tokens = 0;

	if (parse_command(command, &nr_tokens, tokens) == 0)
		return 1;

	return run_command(nr_tokens, tokens);
}

static bool __verbose = true;
static const char *__color_start = "[0;31;40m";
static const char *__color_end = "[0m";

static void __print_prompt(void)
{
	char *prompt = "$";
	if (!__verbose) return;

	fprintf(stderr, "%s%s%s ", __color_start, prompt, __color_end);
}

/***********************************************************************
 * main() of this program.
 */
int main(int argc, char * const argv[])
{
	char command[MAX_COMMAND_LEN] = { '\0' };
	int ret = 0;
	int opt;

	while ((opt = getopt(argc, argv, "qm")) != -1) {
		switch (opt) {
		case 'q':
			__verbose = false;
			break;
		case 'm':
			__color_start = __color_end = "\0";
			break;
		}
	}

	if ((ret = initialize(argc, argv))) return EXIT_FAILURE;

	/**
	 * Make stdin unbuffered to prevent ghost (buffered) inputs during
	 * abnormal exit after fork()
	 */
	setvbuf(stdin, NULL, _IONBF, 0);

	while (true) {
		__print_prompt();

		if (!fgets(command, sizeof(command), stdin)) break;

		append_history(command);
		ret = __process_command(command);

		if (!ret) break;
	}

	finalize(argc, argv);

	return EXIT_SUCCESS;
}
