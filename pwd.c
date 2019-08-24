#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<errno.h>
int main(){
    char *directory = (char*)malloc(sizeof(char)*2000);
    getcwd(directory, 2000);
    printf("%s\n", directory);
    return 0;
}