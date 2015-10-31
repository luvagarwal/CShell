#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

char *get_cwd();
char *pretty_cwd(char *);
char* newbuffer();
void run_command(char **);
void print_ps1(char *);
void initialize_signals();
char **take_input();