#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<errno.h>
int main(int argc, char** argv){
    if(argc==1){
        printf("\n");
    }
    else{
        printf("%s\n", argv[1]);
    }
}