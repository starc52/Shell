#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<fcntl.h>
#include<errno.h>
#include<signal.h>
int setenvSimulator(char *varName, char *varValue);
int unsetenvSimulator(char *varName);
int checkRedirect(char *fullCommand, char *inputFile, char *outputFile, char *action, int *append);//checks for redirection symbols, and gives input, output, and action accordingly. note that input, output are null pointers if there doesnt exist one. action is not trimmed.
int cd(char * path, char * homeDirectory, char *newDirectory);//given a path(path) entered by user, the actual home directory(homeDirectory), and a buffer(newDirectory) for storing the changed directory in the terms of the revised directory structure.
int directoryChanger(char *actDirectory, char* path, char * correctedPath);//given an actual directory path, it converts the path into one with the revised ~.
int commandParse(char *command, char **revisedCommand, int *numOfArguments, int *errorInFormat);//it parses the command into (command and arguments) and adds it to the a buffer(revisedCommand)
int executeCommand(char *command, char **args, char *directory, int procedure, char *inputFile, char *outputFile, int append, int redirect);//executes commands designed by me
int executeSystemCommand(char * command, char **args, int procedure, int numOfArguments, char *inputFile, char *outputFile, int append, int redirect);//executes various other commands.
int reverseDirectoryChanger(char * homeDirectory, char *givenDirectory, char *revisedDirectory);//converts the directory from new ~ format to the original format.
int main(){
    signal(SIGINT, SIG_IGN);
    char *user = (char *)malloc(sizeof(char)*100);
    char *computer = (char *)malloc(sizeof(char)*200);
    char *directory = (char *)malloc(sizeof(char)*1000);
    char *command = (char *)malloc(sizeof(char)*1000);
    char **revisedCommand = (char **)malloc(sizeof(char*)*15);
    for(int i=0;i<15;i++){
        revisedCommand[i]= (char *)malloc(sizeof(char)*1000);
    }
    char homeDirectory[1000];
    char *currentDirectory = (char *)malloc(sizeof(char)*1000);
    int numOfArguments=0;
    getlogin_r(user, 100);
    gethostname(computer, 200);
    getcwd(currentDirectory, 1000);
    getcwd(homeDirectory, 1000);//holds the value of the new ~
    //strcat(currentDirectory, "/");//holds the value of the current drectory regardless of ~, with an extra /.
    //strcat(homeDirectory, "/");
    while(1){
        directoryChanger(homeDirectory, currentDirectory, directory);
        if(strcmp(directory, "~")!=0){
            directory[strlen(directory)-1]='\0';
        }
        printf("<%s@%s:%s>", user, computer, directory);
        for(int i=0;i<1000;i++){
            command[i]='\0';
        }
        for(int ing=0;ing<15;ing++){
            revisedCommand[ing][0]='\0';
        }
        scanf("%[^\n]", command);
        getchar();
        char *token;
        token = strtok(command, ";");
        while(token!=NULL){
            //printf("%s is the token\n", token);
            char *inputFile=(char *)malloc(sizeof(char)*1000);
            char *outputFile=(char *)malloc(sizeof(char)*1000);
            char *action=(char*)malloc(sizeof(char)*2000);
            int append = 0, errorInFormat=0;
            int redirect = checkRedirect(token, inputFile, outputFile, action, &append);
            commandParse(action, revisedCommand, &numOfArguments, &errorInFormat);
            if(errorInFormat==1){
                free(action);
                free(inputFile);
                free(outputFile);
                break;
            }
            // for(int h=0;h<numOfArguments;h++){
            //     printf("%d %s\n", h, revisedCommand[h]);
            // }
            if(strcmp(revisedCommand[0], "quit")==0){
                exit(0);
            }
            if(revisedCommand[numOfArguments-1][0]!='&'){
                if(strcmp(revisedCommand[0], "cd")==0 || strcmp(revisedCommand[0], "ls")==0 || strcmp(revisedCommand[0], "echo")==0 ||strcmp(revisedCommand[0], "pwd")==0 || strcmp(revisedCommand[0], "pinfo")==0||strcmp(revisedCommand[0], "setenv")==0||strcmp(revisedCommand[0], "unsetenv")==0){
                    if(strcmp(revisedCommand[0], "cd")!=0&&strcmp(revisedCommand[0], "setenv")!=0&&strcmp(revisedCommand[0], "unsetenv")!=0){
                        // for(int ing =0;ing<numOfArguments;ing++){
                        //     printf("here %d %s\n",ing, revisedCommand[ing]);
                        // }
                        executeCommand(revisedCommand[0], revisedCommand, homeDirectory,  0, inputFile, outputFile, append, redirect);
                    }
                    else if(strcmp(revisedCommand[0], "cd")==0){
                        cd(revisedCommand[1], homeDirectory, currentDirectory);
                    }
                    else if(strcmp(revisedCommand[0], "setenv")==0){
                        setenvSimulator(revisedCommand[1], revisedCommand[2]);
                    }
                    else if(strcmp(revisedCommand[0], "unsetenv")==0){
                        unsetenvSimulator(revisedCommand[1]);
                    }
                }
                else{
                    executeSystemCommand(revisedCommand[0], revisedCommand, 0, numOfArguments, inputFile, outputFile, append, redirect);
                }
            }
            else{
                executeSystemCommand(revisedCommand[0], revisedCommand, 1, numOfArguments, inputFile, outputFile, append, redirect);
            }
            token = strtok(NULL, ";");
            free(action);
            free(inputFile);
            free(outputFile);
        }
        getcwd(currentDirectory, 1000); 
        strcat(currentDirectory, "/");          
    }
    return 0;
}
int setenvSimulator(char * varName, char * varValue){
    setenv(varName, varValue, 1);
    return 0;
}
int unsetenvSimulator(char * varName){
    unsetenv(varName);
    return 0;
}
int checkRedirect(char *fullCommand, char *inputFile, char *outputFile, char *action, int *append){
    int length =strlen(fullCommand);
    int lessPos=-1, greaterPos=-1;
    for(int itera1=0;itera1<length;itera1++){
        if(fullCommand[itera1]=='>'&&fullCommand[itera1+1]=='>'){
            *append=1;
            break;
        }
    }
    if(*append!=1){
        for(int h=0;h<length;h++){
            if(fullCommand[h]=='<'){
                lessPos=h;
            }
            else if(fullCommand[h]=='>'){
                greaterPos=h;
            }
        }
        if(lessPos==-1){
            if(greaterPos==-1){
                int i=0;
                for(i=0;i<length;i++){
                    if(fullCommand[i]!=' '&&fullCommand[i]!='\t'){
                        break;
                    }
                }
                int j=0;
                for(j=length-1;j>=0;j--){
                    if(fullCommand[j]!=' '&&fullCommand[j]!='\t'){
                        break;
                    }
                }
                inputFile=NULL;
                outputFile=NULL;
                int cursor=0;
                for(int ind=i;ind<=j;ind++){
                    action[cursor]=fullCommand[ind];
                    cursor++;
                }
                action[cursor]='\0';
                return 0;
            }
            else{
                int i=0;
                for(i=greaterPos+1;i<length;i++){
                    if(fullCommand[i]!=' '&&fullCommand[i]!='\t'){
                        break;
                    }
                }
                int j=0;
                for(j=length-1;j>greaterPos;j--){
                    if(fullCommand[j]!=' '&&fullCommand[j]!='\t'){
                        break;
                    }
                }
                inputFile=NULL;
                int cursor=0;
                for(int ind=i;ind<=j;ind++){
                    outputFile[cursor]=fullCommand[ind];
                    cursor++;
                }
                outputFile[cursor]='\0';
                cursor=0;
                for(int ind=0;ind<greaterPos;ind++){
                    action[cursor]=fullCommand[ind];
                    cursor++;
                }
                action[cursor]='\0';
                return 1;
            }
        }
        else{
            if(greaterPos==-1){
                int i=0;
                for(i=lessPos+1;i<length;i++){
                    if(fullCommand[i]!=' '&&fullCommand[i]!='\t'){
                        break;
                    }
                }
                int j=0;
                for(j=length-1;j>lessPos;j--){
                    if(fullCommand[j]!=' '&&fullCommand[j]!='\t'){
                        break;
                    }
                }
                int cursor=0;
                for(int ind=i;ind<=j;ind++){
                    inputFile[cursor]=fullCommand[ind];
                    cursor++;
                }
                inputFile[cursor]='\0';
                outputFile=NULL;
                cursor=0;
                for(int ind=0;ind<lessPos;ind++){
                    action[cursor]=fullCommand[ind];
                    cursor++;
                }
                action[cursor]='\0';
                return 2;
            }
            else{
                int i1=0, i2=0;
                for(i2=greaterPos+1;i2<length;i2++){
                    if(fullCommand[i2]!=' '&&fullCommand[i2]!='\t'){
                        break;
                    }
                }
                int j1=0, j2=0;
                for(j2=length-1;j2>greaterPos;j2--){
                    if(fullCommand[j2]!=' '&&fullCommand[j2]!='\t'){
                        break;
                    }
                }
                for(i1=lessPos+1;i1<greaterPos;i1++){
                    if(fullCommand[i1]!=' '&&fullCommand[i1]!='\t'){
                        break;
                    }
                }
                for(j1=greaterPos-1;j1>lessPos;j1--){
                    if(fullCommand[j1]!=' '&&fullCommand[j1]!='\t'){
                        break;
                    }
                }
                int cursor=0;
                for(int ind=i1;ind<=j1;ind++){
                    inputFile[cursor]=fullCommand[ind];
                    cursor++;
                }
                inputFile[cursor]='\0';
                cursor=0;
                for(int ind=i2;ind<=j2;ind++){
                    outputFile[cursor]=fullCommand[ind];
                    cursor++;
                }
                outputFile[cursor]='\0';
                cursor=0;
                for(int ind=0;ind<lessPos;ind++){
                    action[cursor]=fullCommand[ind];
                    cursor++;
                }
                action[cursor]='\0';
                return 3;
            }
        }
    }
    else{
        for(int h=0;h<length;h++){
            if(fullCommand[h]=='<'){
                lessPos=h;
            }
            else if(fullCommand[h]=='>'){
                greaterPos=h;
            }
        }
        if(lessPos==-1){
            int i=0;
            for(i=greaterPos+1;i<length;i++){
                if(fullCommand[i]!=' '&&fullCommand[i]!='\t'){
                    break;
                }
            }
            int j=0;
            for(j=length-1;j>greaterPos;j--){
                if(fullCommand[j]!=' '&&fullCommand[j]!='\t'){
                    break;
                }
            }
            inputFile=NULL;
            int cursor=0;
            for(int ind=i;ind<=j;ind++){
                outputFile[cursor]=fullCommand[ind];
                cursor++;
            }
            outputFile[cursor]='\0';
            cursor=0;
            for(int ind=0;ind<greaterPos-1;ind++){
                action[cursor]=fullCommand[ind];
                cursor++;
            }
            action[cursor]='\0';
            return 1;
        }
        else{
            int i1=0, i2=0;
            for(i2=greaterPos+1;i2<length;i2++){
                if(fullCommand[i2]!=' '&&fullCommand[i2]!='\t'){
                    break;
                }
            }
            int j1=0, j2=0;
            for(j2=length-1;j2>greaterPos;j2--){
                if(fullCommand[j2]!=' '&&fullCommand[j2]!='\t'){
                    break;
                }
            }
            for(i1=lessPos+1;i1<greaterPos-1;i1++){
                if(fullCommand[i1]!=' '&&fullCommand[i1]!='\t'){
                    break;
                }
            }
            for(j1=greaterPos-2;j1>lessPos;j1--){
                if(fullCommand[j1]!=' '&&fullCommand[j1]!='\t'){
                    break;
                }
            }
            int cursor=0;
            for(int ind=i1;ind<=j1;ind++){
                inputFile[cursor]=fullCommand[ind];
                cursor++;
            }
            inputFile[cursor]='\0';
            cursor=0;
            for(int ind=i2;ind<=j2;ind++){
                outputFile[cursor]=fullCommand[ind];
                cursor++;
            }
            outputFile[cursor]='\0';
            cursor=0;
            for(int ind=0;ind<lessPos;ind++){
                action[cursor]=fullCommand[ind];
                cursor++;
            }
            action[cursor]='\0';
            return 3;
        }
    }
}
int commandParse(char *command, char **revisedCommand, int *numOfArguments, int * errorInFormat){
    int k=0;
    int echoAlert=0;
    int lsAlert=0;
    int setenvAlert=0, unsetenvAlert=0;
    int directoryArg=0;
    while(command[k]=='\t'||command[k]==' '){
        k++;
    }
    int n=0;
    while(command[k]!='\t'&&command[k]!=' '&&command[k]!='\0'){
        revisedCommand[0][n]=command[k];
        n++;
        k++;
    }
    revisedCommand[0][n]='\0';
    *numOfArguments=1;
    if(strcmp(revisedCommand[0], "echo")==0){
        echoAlert=1;
    }
    if(strcmp(revisedCommand[0], "ls")==0){
        lsAlert=1;
    }
    if(strcmp(revisedCommand[0], "setenv")==0){
        setenvAlert=1;
    }
    if(strcmp(revisedCommand[0], "unsetenv")==0){
        unsetenvAlert=1;
    }
    while(command[k]=='\t'||command[k]==' '){
        k++;
    }
    if(echoAlert==1){
        int m=0;
        while(k<strlen(command)){
            if(command[k]!='\"'&&command[k]!='\''){
                revisedCommand[1][m]=command[k];
                m++;
            }
            k++;
        }
        revisedCommand[1][m]='\0';
        *numOfArguments++;   
    }
    else if(lsAlert==1){
        int m=0; int lAlert=0; int aAlert=0;
        while(k<strlen(command)){
            if(command[k]=='-'){
                if(command[k+1]=='l'){
                    lAlert=1;
                    if(command[k+2]=='a'){
                        aAlert=1;
                    }
                    k+=3;
                }
                else if(command[k+1]=='a'){
                    aAlert=1;
                    if(command[k+2]=='l'){
                        lAlert=1;
                    }
                    k+=3;
                }
                while(command[k]=='\t'||command[k]==' '){
                    k++;
                }
            }
            else{
                directoryArg=1;
                while(command[k]!='\t'&&command[k]!=' '&&command[k]!='\0'){
                    revisedCommand[1][m]=command[k];
                    m++;
                    k++;
                }
                revisedCommand[1][m]='\0';
                while(command[k]=='\t'||command[k]==' '){
                    k++;
                }
            }
            if(k<strlen(command)){
                if(command[k]=='-'){
                    if(command[k+1]=='l'){
                        lAlert=1;
                        if(command[k+2]=='a'){
                            aAlert=1;
                        }
                        k+=3;
                    }
                    else if(command[k+1]=='a'){
                        aAlert=1;
                        if(command[k+2]=='l'){
                            lAlert=1;
                        }
                        k+=3;
                    }
                    while(command[k]=='\t'||command[k]==' '){
                        k++;
                    }
                }
                else{
                    directoryArg=1;
                    while(command[k]!='\t'&&command[k]!=' '&&command[k]!='\0'){
                        revisedCommand[1][m]=command[k];
                        m++;
                        k++;
                    }
                    revisedCommand[1][m]='\0';
                    while(command[k]=='\t'||command[k]==' '){
                        k++;
                    }   
                }
                if(command[k]=='-'){
                    if(command[k+1]=='l'){
                        lAlert=1;
                        if(command[k+2]=='a'){
                            aAlert=1;
                        }
                        k+=3;
                    }
                    else if(command[k+1]=='a'){
                        aAlert=1;
                        if(command[k+2]=='l'){
                            lAlert=1;
                        }
                        k+=3;
                    }
                    while(command[k]=='\t'||command[k]==' '){
                        k++;
                    }
                }
                else{
                    directoryArg=1;
                    while(command[k]!='\t'&&command[k]!=' '&&command[k]!='\0'){
                        revisedCommand[1][m]=command[k];
                        m++;
                        k++;
                    }
                    revisedCommand[1][m]='\0';
                    while(command[k]=='\t'||command[k]==' '){
                        k++;
                    }
                }
            } 
        }
        if(lAlert==1&&aAlert==0){
            if(directoryArg==0){
                strcpy(revisedCommand[1], "-l");
                *numOfArguments++;
            }
            else{
                strcpy(revisedCommand[2], "-l");
                *numOfArguments+=2;
            }
        }
        else if(lAlert==0 &&aAlert==1){
            if(directoryArg==0){
                strcpy(revisedCommand[1], "-a");
                *numOfArguments++;
            }
            else{
                strcpy(revisedCommand[2], "-a");
                *numOfArguments+=2;
            }
        }
        else if(lAlert==0&&aAlert==0){
            if(directoryArg==0){
                strcpy(revisedCommand[1], "\0");
                *numOfArguments++;
            }
            else{
                strcpy(revisedCommand[2], "\0");
                *numOfArguments+=2;
            }
        }
        else{
            if(directoryArg==0){
                strcpy(revisedCommand[1], "-la");
                *numOfArguments++;
            }
            else{
                strcpy(revisedCommand[2], "-la");
                *numOfArguments+=2;
            }
        }
    }
    else if(setenvAlert==1){
        if(k>=strlen(command)){
            printf("here\n");
            printf("Error: Usage is setenv var[value]\n");
            *errorInFormat=1;
            return 0;
        }
        int m=0;
        while(command[k]!='['){
            revisedCommand[1][m]=command[k];
            m++;
            k++;
        }
        revisedCommand[1][m]='\0';
        k++;
        m=0;
        while(command[k]!=']'){
            revisedCommand[2][m]=command[k];
            m++;
            k++;
        }
        revisedCommand[2][m]='\0';
        k++;
        while(command[k]==' '||command[k]=='\t'){
            k++;
        }
        if((command[k]!=' '&&command[k]!='\t'&&command[k]!='\0')&&k<strlen(command)){
            printf("Error: Usage is setenv var[value]\n");
            *errorInFormat=1;
            return 0;
        }
    }
    else if(unsetenvAlert==1){
        if(k>=strlen(command)){
            printf("Error: Usage is unsetenv var\n");
            *errorInFormat=1;
            return 0;
        }
        int m=0;
        while(command[k]!=' '&&command[k]!='\t'&&command[k]!='\0'){
            revisedCommand[1][m]=command[k];
            m++;
            k++;
        }
        revisedCommand[1][m]='\0';
        while(command[k]==' '||command[k]=='\t'){
            k++;
        }
        if(command[k]!=' '&&command[k]!='\t'&&command[k]!='\0'&&k<strlen(command)){
            printf("Error: Usage is unsetenv var\n");
            *errorInFormat=1;
            return 0;
        }
    }
    else{
        int m=0;
        int n=0;
        while(k<strlen(command)){
            n++;
            m=0;
            while(command[k]!='\t'&&command[k]!=' '){
                revisedCommand[n][m]=command[k];
                k++;
                m++;
            }
            revisedCommand[n][m]='\0';
            while(command[k]=='\t'||command[k]==' '){
                k++;
            }
        }
        *numOfArguments+=n;
        *numOfArguments++;
    }
    return 0;
}
int executeCommand(char *command, char **args, char *directory, int procedure, char *inputFile, char *outputFile, int append, int redirect){
    if(strcmp(command, "ls")!=0){
        char *path = (char *)malloc(sizeof(char)*1000);
        strcpy(path, directory);
        strcat(path, "/");
        strcat(path, command);
        int fdin, fdout;
        if(redirect==3){
            fdin = open(inputFile, O_RDONLY);
            if(fdin<0){
                printf("Input File Error:%s\n", strerror(errno));
                exit(-1);
            }
            if(append==1){
                fdout = open(outputFile, O_CREAT|O_RDWR|O_APPEND, 0700);
            }
            else{
                fdout = open(outputFile, O_TRUNC|O_CREAT|O_RDWR, 0700);
            }
            if(fdout<0){
                printf("Output File Error:%s\n", strerror(errno));
                exit(-1);
            }
        }
        else if(redirect==2){
            fdin = open(inputFile, O_RDONLY);
            if(fdin<0){
                printf("Input File Error:%s\n", strerror(errno));
                exit(-1);
            }
        }
        else if(redirect==1){
            if(append==1){
                fdout = open(outputFile, O_CREAT|O_RDWR|O_APPEND, 0700);
            }
            else{
                fdout = open(outputFile, O_TRUNC|O_CREAT|O_RDWR, 0700);
            }
            if(fdout<0){
                printf("Output File Error:%s\n", strerror(errno));
                exit(-1);
            }
        }
        pid_t childPID = fork();
        if(redirect==3){
            printf("here1 %s %s\n", inputFile, outputFile);            
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                dup2(fdin, 0);
                dup2(fdout, 1);
                if(args[1][0]=='\0'){
                    char *arg[]={command, directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char *arg[]={command, args[1], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
        else if(redirect==2){
            printf("here2 %s\n", inputFile);            
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                dup2(fdin, 0);
                if(args[1][0]=='\0'){
                    char *arg[]={command, directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char *arg[]={command, args[1], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
        else if(redirect==1){
            printf("here3 %s\n", outputFile);                        
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                dup2(fdout, 1);
                if(args[1][0]=='\0'){
                    char *arg[]={command, directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char *arg[]={command, args[1], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
        else if(redirect==0){
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                if(args[1][0]=='\0'){
                    char *arg[]={command, directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char *arg[]={command, args[1], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
    }
    else{
        char *path = (char *)malloc(sizeof(char)*1000);
        strcpy(path, directory);
        strcat(path, "/");
        strcat(path, command);
        int fdin, fdout;
        if(redirect==3){
            fdin = open(inputFile, O_RDONLY);
            if(fdin<0){
                printf("Input File Error:%s\n", strerror(errno));
                exit(-1);
            }
            if(append==1){
                fdout = open(outputFile, O_CREAT|O_RDWR|O_APPEND, 0700);
            }
            else{
                fdout = open(outputFile, O_TRUNC|O_CREAT|O_RDWR, 0700);
            }
            if(fdout<0){
                printf("Output File Error:%s\n", strerror(errno));
                exit(-1);
            }
        }
        else if(redirect==2){
            fdin = open(inputFile, O_RDONLY);
            if(fdin<0){
                printf("Input File Error:%s\n", strerror(errno));
                exit(-1);
            }
        }
        else if(redirect==1){
            if(append==1){
                fdout = open(outputFile, O_CREAT|O_RDWR|O_APPEND, 0700);
            }
            else{
                fdout = open(outputFile, O_TRUNC|O_CREAT|O_RDWR, 0700);
            }
            if(fdout<0){
                printf("Output File Error:%s\n", strerror(errno));
                exit(-1);
            }
        }
        pid_t childPID = fork();
        if(redirect==3){
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                dup2(fdin, 0);
                dup2(fdout, 1);
                if(args[1][0]=='\0' &&args[2][0]=='\0'){
                    char *arg[]={command, directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else if(args[1][0]=='\0'&&args[2][0]!='\0'){
                    char *arg[]={command, args[2], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else if(args[1][0]!='\0'&&args[2][0]=='\0'){
                    char *arg[]={command,args[1],directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char *arg[]={command,args[1], args[2], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
        else if(redirect==2){
            printf("here5 %s \n", inputFile);
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                dup2(fdin, 0);
                if(args[1][0]=='\0' &&args[2][0]=='\0'){
                    char *arg[]={command, directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else if(args[1][0]=='\0'&&args[2][0]!='\0'){
                    char *arg[]={command, args[2], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else if(args[1][0]!='\0'&&args[2][0]=='\0'){
                    char *arg[]={command,args[1],directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char *arg[]={command,args[1], args[2], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
        else if(redirect==1){
            printf("here 6%s\n", outputFile);                        
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                dup2(fdout, 1);
                if(args[1][0]=='\0' &&args[2][0]=='\0'){
                    char *arg[]={command, directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else if(args[1][0]=='\0'&&args[2][0]!='\0'){
                    char *arg[]={command, args[2], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else if(args[1][0]!='\0'&&args[2][0]=='\0'){
                    char *arg[]={command,args[1],directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char *arg[]={command,args[1], args[2], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
        else if(redirect==0){
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                if(args[1][0]=='\0' &&args[2][0]=='\0'){
                    char *arg[]={command, directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else if(args[1][0]=='\0'&&args[2][0]!='\0'){
                    char *arg[]={command, args[2], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else if(args[1][0]!='\0'&&args[2][0]=='\0'){
                    char *arg[]={command,args[1],directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char *arg[]={command,args[1], args[2], directory, NULL};
                    if(execv(path, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
    }
    return 0;
}
int executeSystemCommand(char * command, char **args, int procedure, int numOfArguments, char *inputFile, char *outputFile, int append, int redirect){
    if(procedure==0){
        int fdin, fdout;
        if(redirect==3){
            fdin = open(inputFile, O_RDONLY);
            if(fdin<0){
                printf("Input File Error:%s\n", strerror(errno));
                exit(-1);
            }
            if(append==1){
                fdout = open(outputFile, O_CREAT|O_RDWR|O_APPEND, 0700);
            }
            else{
                fdout = open(outputFile, O_TRUNC|O_CREAT|O_RDWR, 0700);
            }
            if(fdout<0){
                printf("Output File Error:%s\n", strerror(errno));
                exit(-1);
            }
        }
        else if(redirect==2){
            fdin = open(inputFile, O_RDONLY);
            if(fdin<0){
                printf("Input File Error:%s\n", strerror(errno));
                exit(-1);
            }
        }
        else if(redirect==1){
            if(append==1){
                fdout = open(outputFile, O_CREAT|O_RDWR|O_APPEND, 0700);
            }
            else{
                fdout = open(outputFile, O_TRUNC|O_CREAT|O_RDWR, 0700);
            }
            if(fdout<0){
                printf("Output File Error:%s\n", strerror(errno));
                exit(-1);
            }
        }
        pid_t childPID = fork();
        if(redirect==3){
            signal(SIGINT, SIG_DFL);
            printf("here 7%s %s\n", inputFile, outputFile);
            if(childPID == 0){
                dup2(fdin, 0);
                dup2(fdout, 1);
                if(numOfArguments>=2){
                    char** arg=(char **)malloc(sizeof(char*)*(numOfArguments+1));
                    for(int i=0;i<numOfArguments;i++){
                        arg[i]=(char *)malloc(sizeof(char)*1000);
                        strcpy(arg[i], args[i]);
                    }
                    arg[numOfArguments]=NULL;
                    if(execvp(command, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char * arg[] = {command, NULL};
                    if(execvp(command, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
        else if(redirect==2){
            printf("here8 %s\n", inputFile);            
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                dup2(fdin, 0);
                if(numOfArguments>=2){
                    char** arg=(char **)malloc(sizeof(char*)*(numOfArguments+1));
                    for(int i=0;i<numOfArguments;i++){
                        arg[i]=(char *)malloc(sizeof(char)*1000);
                        strcpy(arg[i], args[i]);
                    }
                    arg[numOfArguments]=NULL;
                    if(execvp(command, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char * arg[] = {command, NULL};
                    if(execvp(command, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }   
        else if(redirect==1){
            printf("here9 %s\n", outputFile);            
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                dup2(fdout, 1);
                if(numOfArguments>=2){
                    char** arg=(char **)malloc(sizeof(char*)*(numOfArguments+1));
                    for(int i=0;i<numOfArguments;i++){
                        arg[i]=(char *)malloc(sizeof(char)*1000);
                        strcpy(arg[i], args[i]);
                    }
                    arg[numOfArguments]=NULL;
                    if(execvp(command, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char * arg[] = {command, NULL};
                    if(execvp(command, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
        else if(redirect==0){
            if(childPID == 0){
                signal(SIGINT, SIG_DFL);
                if(numOfArguments>=2){
                    char** arg=(char **)malloc(sizeof(char*)*(numOfArguments+1));
                    for(int i=0;i<numOfArguments;i++){
                        arg[i]=(char *)malloc(sizeof(char)*1000);
                        strcpy(arg[i], args[i]);
                    }
                    arg[numOfArguments]=NULL;
                    if(execvp(command, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char * arg[] = {command, NULL};
                    if(execvp(command, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                signal(SIGINT, SIG_IGN);;;
                wait(NULL);
            }
        }
    }
    else{
        int wstatus;
        pid_t fstGen = fork();
        if(fstGen==0){
            pid_t sndGen = fork();
            if(sndGen == 0){
                if(numOfArguments>=3){
                    char** arg=(char **)malloc(sizeof(char*)*(numOfArguments));
                    for(int i=0;i<numOfArguments-1;i++){
                        arg[i]=(char *)malloc(sizeof(char)*1000);
                        strcpy(arg[i], args[i]);
                    }
                    arg[numOfArguments-1]=NULL;
                    if(execvp(command, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
                else{
                    char * arg[] = {command, NULL};
                    if(execvp(command, arg)<0){
                        printf("Error:%s\n", strerror(errno));
                        exit(-1);
                    }
                }
            }
            else{
                do {
                    pid_t w = waitpid(sndGen, &wstatus, WUNTRACED | WCONTINUED);
                    if (w == -1) {
                        perror("waitpid");
                        exit(-1);
                    }
                    if (WIFEXITED(wstatus)) {
                        printf("%s exited normally, with pid %d\n",command, sndGen);
                    } 
               } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
               exit(0);
            }
        }
    }
    return 0;
}
int directoryChanger(char *actDirectory, char* path, char * correctedPath){
    int length = strlen(actDirectory);
    int pathLength = strlen(path);
    if(pathLength<length){
        strcpy(correctedPath, path);
        return 0;
    }
    else if(pathLength>length){
        int subString=1;
        for(int i=0;i<length;i++){
            if(actDirectory[i]!=path[i]){
                subString=0;
            }
        }
        if(subString==0){
            strcpy(correctedPath, path);
            return 0;
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
    else{
        int subString=1;
        for(int i=0;i<length;i++){
            if(actDirectory[i]!=path[i]){
                subString=0;
            }
        }
        if(subString==0){
            strcpy(correctedPath, path);
            return 0;
        }
        else{
            strcpy(correctedPath, "~");
        }
    }
    return 0;
}
int reverseDirectoryChanger(char * homeDirectory, char *givenDirectory, char *revisedDirectory){
    if(givenDirectory[0]=='~'){
        strcpy(revisedDirectory, homeDirectory);
        int length= strlen(revisedDirectory);
        for(int i=1;i<strlen(givenDirectory);i++){
            revisedDirectory[length+i-1]=givenDirectory[i];
        }
    }
    else{
        strcpy(revisedDirectory, givenDirectory);
    }
    return 0;
}
int cd(char * path, char *homeDirectory, char * newDirectory){
    reverseDirectoryChanger(homeDirectory, path, newDirectory);
    if(chdir(newDirectory)<0){
        perror("chdir");
    }
    return 0;
}