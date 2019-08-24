#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<errno.h>
int cd(char * path, char * homeDirectory, char *newDirectory);
int directoryChanger(char *actDirectory, char* path, char * correctedPath);
int commandParse(char *command, char **revisedCommand);
int executeCommand(char *command, char **args, char *directory, int procedure);
int executeSystemCommand(char * command, char **args, int procedure);
int reverseDirectoryChanger(char * homeDirectory, char *givenDirectory, char *revisedDirectory);
int main(){
    char *user = (char *)malloc(sizeof(char)*100);
    char *computer = (char *)malloc(sizeof(char)*200);
    char *directory = (char *)malloc(sizeof(char)*1000);
    char *command = (char *)malloc(sizeof(char)*1000);
    char **revisedCommand = (char **)malloc(sizeof(char*)*4);
    for(int i=0;i<4;i++){
        revisedCommand[i]= (char *)malloc(sizeof(char)*1000);
    }
    char homeDirectory[1000];
    char *currentDirectory = (char *)malloc(sizeof(char)*1000);
    getlogin_r(user, 100);
    gethostname(computer, 200);
    getcwd(currentDirectory, 1000);
    getcwd(homeDirectory, 1000);
    strcat(currentDirectory, "/");
    strcat(homeDirectory, "/");
    while(1){
        
        directoryChanger(homeDirectory, currentDirectory, directory);
        printf("<%s@%s:%s>", user, computer, directory);
        for(int i=0;i<1000;i++){
            command[i]='\0';
        }
        scanf("%[^\n]", command);
        getchar();
        char *token;
        token = strtok(command, ";");
        while(token!=NULL){
            for(int i=0;i<3;i++){
                strcpy(revisedCommand[i], "\0");
            }
            commandParse(token, revisedCommand);
            if(strcmp(revisedCommand[0], "exit")==0){
                exit(0);
            }
            if(revisedCommand[2][0]!='&'){
                if(strcmp(revisedCommand[0], "cd")==0 || strcmp(revisedCommand[0], "ls")==0 || strcmp(revisedCommand[0], "echo")==0 ||strcmp(revisedCommand[0], "pwd")==0 || strcmp(revisedCommand[0], "pinfo")==0){
                    if(strcmp(revisedCommand[0], "cd")!=0){
                        executeCommand(revisedCommand[0], revisedCommand, homeDirectory,  0);
                    }
                    else{
                        cd(revisedCommand[1], homeDirectory, currentDirectory);
                    }
                }
                else{
                    executeSystemCommand(revisedCommand[0], revisedCommand, 0);
                }
            }
            else{
                executeSystemCommand(revisedCommand[0], revisedCommand, 1);
            }
            for(int j=0;j<3;j++){
                for(int h=0;h<1000;h++){
                    revisedCommand[j][h]='\0';
                }
            }
            token = strtok(NULL, ";");
        }
        getcwd(currentDirectory, 1000); 
        strcat(currentDirectory, "/");          
    }
    return 0;
}
int commandParse(char *command, char **revisedCommand){
    int k=0;
    int echoAlert=0;
    int lsAlert=0;
    for(int i=0;i<3;i++){
        revisedCommand[i][0]='\0';
    }
    while(command[k]=='\t'||command[k]==' '){
        k++;
    }
    int n=0;
    while(command[k]!='\t'&&command[k]!=' '&&command[k]!='\0'){
        revisedCommand[0][n]=command[k];
        n++;
        k++;
    }
    if(strcmp(revisedCommand[0], "echo")==0){
        echoAlert=1;
    }
    if(strcmp(revisedCommand[0], "ls")==0){
        lsAlert=1;
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
                while(command[k]!='\t'&&command[k]!=' '&&command[k]!='\0'){
                    revisedCommand[1][m]=command[k];
                    m++;
                    k++;
                }
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
                while(command[k]!='\t'&&command[k]!=' '&&command[k]!='\0'){
                    revisedCommand[1][m]=command[k];
                    m++;
                    k++;
                }
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
                while(command[k]!='\t'&&command[k]!=' '&&command[k]!='\0'){
                    revisedCommand[1][m]=command[k];
                    m++;
                    k++;
                }
                while(command[k]=='\t'||command[k]==' '){
                    k++;
                }
            } 
        }
        if(lAlert==1&&aAlert==0){
            if(revisedCommand[1][0]=='\0'){
                strcpy(revisedCommand[1], "-l");
            }
            else{
                strcpy(revisedCommand[2], "-l");
            }
        }
        else if(lAlert==0 &&aAlert==1){
            if(revisedCommand[1][0]=='\0'){
                strcpy(revisedCommand[1], "-a");
            }
            else{
                strcpy(revisedCommand[2], "-a");
            }
        }
        else if(lAlert==0&&aAlert==0){
            if(revisedCommand[1][0]=='\0'){
                strcpy(revisedCommand[1], "\0");
            }
            else{
                strcpy(revisedCommand[2], "\0");
            }
        }
        else{
            if(revisedCommand[1][0]=='\0'){
                strcpy(revisedCommand[1], "-la");
            }
            else{
                strcpy(revisedCommand[2], "-la");
            }
        }
    }
    else{
        int m=0;
        while(k<strlen(command)){
            while(command[k]=='\t'||command[k]==' '){
                k++;
            }
            if((command[k-1]=='\t' || command[k-1]==' ' )&& command[k]=='&'){
                revisedCommand[2][0]='&';
                break;
            }
            if(command[k]!='\"'&& command[k]!='\''){
                revisedCommand[1][m] = command[k];
                m++;
            }
            k++;
        }
    }
    return 0;
}
int executeCommand(char *command, char **args, char *directory, int procedure){
    if(strcmp(command, "ls")!=0){
        char *path = (char *)malloc(sizeof(char)*1000);
        strcpy(path, directory);
        strcat(path, command);
        pid_t childPID = fork();
        if(childPID == 0){
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
            wait(NULL);
        }
    }
    else{
        char *path = (char *)malloc(sizeof(char)*1000);
        strcpy(path, directory);
        strcat(path, command);
        pid_t childPID = fork();
        if(childPID == 0){
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
            wait(NULL);
        }
    }
    return 0;
}
int executeSystemCommand(char * command, char **args, int procedure){
    if(procedure==0){
        pid_t childPID = fork();
        if(childPID == 0){
            if(args[1][0]!='\0'){
                char *arg[]={command, args[1], NULL};
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
            wait(NULL);
        }
    }
    else{
        int wstatus;
        pid_t fstGen = fork();
        if(fstGen==0){
            pid_t sndGen = fork();
            if(sndGen == 0){
                if(args[1][0]!='\0'){
                    char *arg[]={command, args[1], NULL};
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
            strcat(correctedPath, "\0");
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