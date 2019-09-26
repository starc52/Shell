#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<errno.h>
#include<fcntl.h>
int directoryChanger(char * actDirectory, char *path, char *correctedPath);
int main(int argc, char **argv){
    // for(int i=0;i<argc;i++){
    //     printf("in pinfo %d %s\n",i, argv[i]);
    // }
    pid_t processID;
    char directory[1000];
    strcpy(directory, argv[argc-1]);
    int temp=0;
    if(argc==2){
        processID=getppid();
    }
    else{
        int i=0;
        int pow10=1;
        while(i<strlen(argv[1])){
            temp=temp+((argv[1][strlen(argv[1])-i-1]-'0')*pow10);
            i++;
            pow10*=10;
        }
        processID=temp;
    }
    char path[1000];
    strcpy(path, "/proc/");
    char num2str[1000];
    sprintf(num2str, "%d", processID);
    strcat(path, num2str);
    strcat(path, "/");
    strcat(path, "status");
    strcat(path, "\0");
    char pathNew[1000];
    char buffer[1000];
    strcpy(pathNew, "/proc/");
    strcat(pathNew, num2str);
    strcat(pathNew, "/");
    strcat(pathNew, "exe");
    readlink(pathNew,buffer, 1000);
    strcat(buffer, "\0");
    char status[2];
    char VMSizeValue[300];
    FILE *fd = fopen(path, "r");
    if(fd == NULL){
        printf("Error: No such process\n");
        exit(-2);
    }
    else{
        fscanf(fd, "Name:\t %*s\nUmask:\t%*s\nState:\t%s (%*[^)])\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\nVmSize:\t%[^\n]\n", status, VMSizeValue);
        //printf("%s %s\n", status, VMSizeValue);
    }
    char correctedPath[1000];
    directoryChanger(directory, buffer, correctedPath);
    printf("pid -- %d\n", processID);
    printf("Process Status -- %s\nMemory -- %s\nExecutable Path -- %s\n", status, VMSizeValue, correctedPath);
    fclose(fd);
    return 0;
}
int directoryChanger(char *actDirectory, char* path, char * correctedPath){
    int length = strlen(actDirectory);
    int pathLength = strlen(path);
    if(pathLength<length){
        strcpy(correctedPath, path);
    }
    else{
        int subString=1;
        for(int i=0;i<length;i++){
            if(actDirectory[i]!=path[i]){
                subString=0;
            }
        }
        if(subString==0){
            strcpy(correctedPath, path);
        }
        else{
            strcpy(correctedPath, "~");
            int j=1;
            for(int i=length;i<pathLength;i++){
                correctedPath[j]=path[i];
                j++;
            }
            correctedPath[j]='\0';
        }
    }
}