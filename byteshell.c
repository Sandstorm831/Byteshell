#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

char* line;
char **args;
int status;
#define BUFF  64
#define TOK_DELIM " \t\r\n\a\""
extern char **environ;
struct 	Node{
	char *str;
	struct Node* next;
};
char *builtin_func_list[7];
char* alias_func_list[BUFF];
int (*alias_func[BUFF])(char**);
int (*builtin_func[7])(char **);
struct Node* head = NULL;
struct Node* cur=NULL;
int alias_size=0;
char* strAppend(char* str1, char* str2)
{
	char* str3 = (char*)malloc(sizeof(char*)*(strlen(str1)+strlen(str2)));
	strcpy(str3,str1);
	strcpy(str3,str2);
	return str3;
}
int own_history(char** args)
{
	struct Node* ptr =  head;
	int i = 1;
	while(ptr != NULL)
	{
		printf("%s\n", ptr->str);
		ptr = ptr->next;
	}
	return (-1);
}
void add_command_to_history(char* line)
{
	if(head == NULL){
		head = (struct Node*)malloc(sizeof(struct Node));
		head->str = (char*)malloc(0x1000);
		char* str1 = " ";
		strcpy(head->str, line);
		head->next = NULL;
		cur = head;
	}
	else
	{
		struct Node* ptr = (struct Node *)malloc(sizeof(struct Node));
		cur->next = ptr;
		ptr->str = (char*)malloc(0x1000);
		char* str1="";
		strcpy(ptr->str, line);
		ptr->next = NULL;
		cur = ptr;
	}
}
char* read_line(void)
{
    // printf(":)\n");
    int buffsize = 1024;
    int position=0;
    char* buffer=malloc(sizeof(char) * buffsize);
    int c;
    if(!buffer)
    {
        fprintf(stderr, "allocation error\n");
        exit(0);
    }
    while(1)
    {
        c = getchar();
        if(c=='\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
            position++;
        }
        if(position >= buffsize)
        {
            buffsize += 1024;
            buffer = realloc(buffer, buffsize);
        }
        if(!buffer)
        {
            fprintf(stderr, "allocation error\n");
            exit(0);
        }
    }
}
char** split_line(char* line)
{
	int bufsize = 64;
	int i = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		fprintf(stderr, "allocation error in split_line: tokens\n");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, TOK_DELIM);
	while (token != NULL)
	{
		/* handle comments */
		if (token[0] == '#')
		{
			break;
		}
		tokens[i] = token;
		i++;
		if (i >= bufsize)
		{
			bufsize += bufsize;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "reallocation error in split_line: tokens");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, TOK_DELIM);
	}
	tokens[i] = NULL;
	return (tokens);
}
int new_process(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid ==  0)
	{
		/* child process */
		if (execvp(args[0], args) == -1)
		{
			perror("error in new_process: child process");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		/* error forking */
		perror("error in new_process: forking");
	}
	else
	{
		/* parent process */
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return (-1);
}
int own_env(char **args)
{
	int i = 0;
	(void)(**args);

	while (environ[i])
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (-1);
}
int own_exit(char **args)
{
	/* exit with status */
	if (args[1])
	{
		return (atoi(args[1]));
	}
	/* exit success */
	else
	{
		return (0);
	}
}
int own_help(char **args)
{
	long unsigned int i = 0;
	(void)(**args);

	printf("\n---help simple_shell---\n");
	printf("Type a command and its arguments, then hit enter\n");
	printf("Built-in commands:\n");
	for (; i < sizeof(builtin_func_list) / sizeof(char *); i++)
	{
		printf("  -> %s\n", builtin_func_list[i]);
	}
	printf("Use the man command for information on other programs.\n\n");
	return (-1);
}
int own_cd(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "expected argument to \"cd\"\n");
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("error in own_cd.c: changing dir\n");
		}
	}
	return (-1);
}
int own_alias(char** args)
{
	if(args[1]==NULL) 
	{
		fprintf(stderr, "expected argument to \"alias\"\n");
		return (-1);
	}
	int bufsize = 64;
	int i = 0;
	char *argcpy;
	argcpy = (char*)malloc(sizeof(char*)*(strlen(args[1])));
	strcpy(argcpy,args[1]);
	char* mdelim = "=\'";
	char* cds;
	char **cdstok = malloc(bufsize * sizeof(char *));
	if (!cdstok)
	{
		fprintf(stderr, "allocation error in alias: cdstok\n");
		exit(EXIT_FAILURE);
	}
	cds = strtok(argcpy, mdelim);
	while (cds != NULL)
	{
		cdstok[i] = cds;
		i++;
		if (i >= bufsize)
		{
			bufsize += bufsize;
			cdstok = realloc(cdstok, bufsize * sizeof(char *));
			if (!cdstok)
			{
				fprintf(stderr, "reallocation error in alias: cdstok");
				exit(EXIT_FAILURE);
			}
		}
		cds = strtok(NULL, mdelim);
	}
	cdstok[i] = NULL;
	if(cdstok[1]==NULL)
	{
		fprintf(stderr, "alias expects argument =\n");
		return (-1);
	}
	int j=0;
	for(;j<(sizeof(builtin_func_list)/sizeof(char*)); j++)
	{
		if(strcmp(cdstok[1],builtin_func_list[j])==0)
		{
			alias_func[alias_size] = builtin_func[j];
		}
	}
	alias_func_list[alias_size] = cdstok[0];
	alias_size++;
	return (-1);
}
int own_unalias(char** args)
{
	if(args[1]==NULL) 
	{
		fprintf(stderr, "expected argument to \"alias\"\n");
		return (-1);
	}
	for(int j=1;args[j]!=NULL;j++)
	{
		char * fnd = args[j];
		for(int k=0;k<alias_size;k++)
		{
			if(strcmp(fnd,alias_func_list[k])==0)
			{
				for(int l=k; l<alias_size-1; l++)
				{
					alias_func[k] = alias_func[k+1];
					alias_func_list[k] = alias_func_list[k+1];
				}
				alias_size--;
			}
		}
	}
	return (-1);
}
int execute_args(char **args)
{
	builtin_func_list[0] = 	"cd";
	builtin_func_list[1] = 	"env";
	builtin_func_list[2] = 	"help";
	builtin_func_list[3] = 	"exit";
	builtin_func_list[4] = 	"history";
	builtin_func_list[5] = 	"alias";
	builtin_func_list[6] = 	"unalias";

	builtin_func[0] = &own_cd;
	builtin_func[1] = &own_env;
	builtin_func[2] = &own_help;
	builtin_func[3] = &own_exit;
	builtin_func[4] = &own_history;
	builtin_func[5] = &own_alias;
	builtin_func[6] = &own_unalias;

	long unsigned int i = 0;
	int j = 0;

	if (args[0] == NULL)
	{
		/* empty command was entered */
		return (-1);
	}
	/* find if the command is a builtin */
	for (; i < sizeof(builtin_func_list) / sizeof(char *); i++)
	{
		/* if there is a match execute the builtin command */
		if (strcmp(args[0], builtin_func_list[i]) == 0)
		{
			return ((*builtin_func[i])(args));
		}
	}
	/* For Alias function list*/
	for(; j < alias_size; j++)
	{
		if(strcmp(args[0], alias_func_list[j]) == 0)
		{

			return ((*alias_func[j])(args));
		}
	}
	/* create a new process */
	return (new_process(args));
}
int main()
{
    do{
        printf("> ");
        line = read_line();
		add_command_to_history(line);
        args = split_line(line);
		status = execute_args(args);
		/* avoid memory leaks */
		free(line);
		free(args);
		/* exit with status */
		if (status >= 0)
		{
			exit(status);
		}
    }while(status);
}