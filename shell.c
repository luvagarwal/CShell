#include "includes.h"
#define BUFFER_SIZE 1000
#define MAX_ARGUMENTS 100

int CTRLC = 0;

char *get_cwd(){
    char *cwd = newbuffer();
    getcwd(cwd, BUFFER_SIZE);
    return cwd;
}

char *pretty_cwd(char *fwd){
    char *cwd = get_cwd();
    if(strlen(cwd) >= strlen(fwd)){
        cwd += strlen(fwd);
        cwd--;
        *cwd = '~';
    }
    return cwd;
}

char* newbuffer(){
    return malloc(sizeof(char) * BUFFER_SIZE);
}

void run_command(char **input_arr){
    if(strcmp(input_arr[0], "cd") == 0){
        chdir(input_arr[1]);
        return;
    }
    pid_t pid = fork();
    if(pid == 0){
        execvp(input_arr[0], input_arr);
        printf("Unable to run command\n");
    }
    wait();
    return;
}
void print_ps1(char *fwd){
    char *hostname = newbuffer();
    gethostname(hostname, BUFFER_SIZE);
    char *cwd = pretty_cwd(fwd);
    printf("<%s:%s>", hostname, cwd);
    free(hostname);
}

void sigintHandler(){
    CTRLC = 1;
    exit(0);
}

void initialize_signals(){
    signal(SIGINT, sigintHandler);
}

char **take_input(){
    char *input = newbuffer();
    char *tmpinput = input;
    char **input_arr = malloc(sizeof(char*) * MAX_ARGUMENTS);
    char **tmp_input_arr = input_arr;
    char c;
    int flag = 0, colorflag = 0;
    while(CTRLC == 0){
        c = getchar();
        if(c == EOF) exit(0);
        if(c == '\n'){
            if(flag == 0){
                *input++ = '\0';
                *input_arr++ = tmpinput;
                if(colorflag == 0){
                    char* cstr = "--color=auto\0";
                    *input_arr++ = cstr;
                    colorflag = 1;
                }
                break;
            }
        }
        if(c == ' ' || c == '\t'){
            if(flag == 0){
                *input++ = '\0';
                *input_arr++ = tmpinput;
                if(colorflag == 0){
                    char* cstr = "--color=auto\0";
                    *input_arr++ = cstr;
                    colorflag = 1;
                }
            }
            flag = 1;
            tmpinput = input;
            continue;
        }
        else flag = 0;
        *input++ = c;
    }
    input_arr = NULL;
    return tmp_input_arr;
}

int main()
{
    initialize_signals();
    char *fwd = get_cwd();
    char **input_arr;
    while(1){
        print_ps1(fwd);
        input_arr = take_input();
        run_command(input_arr);
    }
	return 0;
}
