#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<sys/sysmacros.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<errno.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<ctype.h>
int reverseDirectoryChanger(char * homeDirectory, char *givenDirectory, char *revisedDirectory);
int main(int argc, char **argv){
    struct dirent *dirStruct;
    struct stat tmpArg;
    struct passwd *pwd;
    struct group *grp;
    struct tm * t;
    static const char * lookup[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char homeDirectory[1000];
    strcpy(homeDirectory, argv[argc-1]);
    char currentDirectory[1000];
    getcwd(currentDirectory, 1000);
    if(argc==2){ 
        DIR *directory = opendir("."); 
        if (directory == NULL){ 
            printf("Could not open current directory\n"); 
            return 0; 
        } 
        while ((dirStruct = readdir(directory)) != NULL){
            if(strcmp(dirStruct->d_name, ".")!=0&&strcmp(dirStruct->d_name, "..")!=0&&dirStruct->d_name[0]!='.'){ 
                printf("%s\n", dirStruct->d_name);
            }
        }   
        closedir(directory);     
    }
    else if(argc==3){
        if(argv[1][0]=='-'){
            if(strcmp(argv[1], "-a")==0){
                DIR *directory = opendir("."); 
                if (directory == NULL){ 
                    printf("Could not open current directory" ); 
                    return 0; 
                } 
                while ((dirStruct = readdir(directory)) != NULL){
                    printf("%s\n", dirStruct->d_name);
                }   
                closedir(directory);
            }
            else if(strcmp(argv[1], "-l")==0){
                char filePath[1000];
                
                char revisedDirectory[1000];
                reverseDirectoryChanger(homeDirectory, currentDirectory, revisedDirectory);
                printf("%s is the revisedDirectory\n", revisedDirectory);

                DIR *directory = opendir(revisedDirectory); 
                if (directory == NULL){ 
                    printf("Could not open current directory" ); 
                    return 0; 
                } 
                while ((dirStruct = readdir(directory)) != NULL){
                    if(strcmp(dirStruct->d_name, ".")!=0&&strcmp(dirStruct->d_name, "..")!=0&&dirStruct->d_name[0]!='.'){ 
                        
                        strcpy(filePath, revisedDirectory);
                        strcat(filePath, "/");
                        strcat(filePath, dirStruct->d_name);
                        //printf("%s\n", filePath);
                        if(lstat(filePath, &tmpArg)<0){
                            perror("lstat");
                            exit(-1);
                        }
                        else{
                            switch (tmpArg.st_mode & S_IFMT) {
                            case S_IFBLK:
                                printf("b");
                                break;
                            case S_IFCHR:
                                printf("c");
                                break;
                            case S_IFDIR:
                                printf("d");
                                break;
                            case S_IFIFO:
                                printf("p");
                                break;
                            case S_IFLNK:
                                printf("l");
                                break;
                            case S_IFREG:
                                printf("-");
                                break;
                            case S_IFSOCK:
                                printf("s");
                                break;
                            default:
                                printf("?");
                                break;
                            }
                            if(tmpArg.st_mode & S_IRUSR){
                                printf("r");
                            }
                            else{
                                printf("-");
                            }
                            if(tmpArg.st_mode & S_IWUSR){
                                printf("w");
                            }
                            else{
                                printf("-");
                            }
                            if(tmpArg.st_mode & S_IXUSR){
                                printf("x");
                            }
                            else{
                                printf("-");
                            }
                            if(tmpArg.st_mode & S_IRGRP){
                                printf("r");
                            }
                            else{
                                printf("-");
                            }
                            if(tmpArg.st_mode & S_IWGRP){
                                printf("w");
                            }
                            else{
                                printf("-");
                            }
                            if(tmpArg.st_mode & S_IXGRP){
                                printf("x");
                            }
                            else{
                                printf("-");
                            }
                            if(tmpArg.st_mode & S_IROTH){
                                printf("r");
                            }
                            else{
                                printf("-");
                            }
                            if(tmpArg.st_mode & S_IWOTH){
                                printf("w");
                            }
                            else{
                                printf("-");
                            }
                            if(tmpArg.st_mode & S_IXOTH){
                                printf("x");
                            }
                            else{
                                printf("-");
                            }
                            printf(" %-3ld ", tmpArg.st_nlink);
                            pwd = getpwuid(tmpArg.st_uid);
                            printf("%s ", pwd->pw_name);
                            grp = getgrgid(tmpArg.st_gid);
                            printf("%s ", grp->gr_name);
                            printf("%ld\t", tmpArg.st_size);
                            t = localtime(&tmpArg.st_ctime);
                            printf("%s ", lookup[t->tm_mon]);
                            printf("%2.0d %2d:%2.0d ", t->tm_mday, t->tm_hour, t->tm_min);
                            printf("%s", dirStruct->d_name);
                            char linkname[1000];
                            ssize_t r = readlink(dirStruct->d_name, linkname, 1000);
                            if (r != -1) {
                                linkname[r] = '\0';
                                printf(" -> %s\n", linkname);
                            }
                            else{
                                printf("\n");
                            }
                        }
                    }
                }   
                closedir(directory);
            }
            else if(strcmp(argv[1], "-la")==0||strcmp(argv[1], "-al")==0){
                char filePath[1000];
                char revisedDirectory[1000];
                reverseDirectoryChanger(homeDirectory, currentDirectory, revisedDirectory);
                printf("%s is the revisedDirectory\n", revisedDirectory);
                DIR *directory = opendir(revisedDirectory); 
                if (directory == NULL){ 
                    printf("Could not open current directory" ); 
                    return 0; 
                } 
                while ((dirStruct = readdir(directory)) != NULL){
                    
                    strcpy(filePath, revisedDirectory);
                    strcat(filePath, "/");
                    strcat(filePath, dirStruct->d_name);
                    if(lstat(filePath, &tmpArg)<0){
                        perror("lstat");
                        exit(-1);
                    }
                    else{
                        switch (tmpArg.st_mode & S_IFMT) {
                        case S_IFBLK:
                            printf("b");
                            break;
                        case S_IFCHR:
                            printf("c");
                            break;
                        case S_IFDIR:
                            printf("d");
                            break;
                        case S_IFIFO:
                            printf("p");
                            break;
                        case S_IFLNK:
                            printf("l");
                            break;
                        case S_IFREG:
                            printf("-");
                            break;
                        case S_IFSOCK:
                            printf("s");
                            break;
                        default:
                            printf("?");
                            break;
                        }
                        if(tmpArg.st_mode & S_IRUSR){
                            printf("r");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IWUSR){
                            printf("w");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IXUSR){
                            printf("x");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IRGRP){
                            printf("r");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IWGRP){
                            printf("w");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IXGRP){
                            printf("x");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IROTH){
                            printf("r");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IWOTH){
                            printf("w");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IXOTH){
                            printf("x");
                        }
                        else{
                            printf("-");
                        }
                        printf(" %-3ld ", tmpArg.st_nlink);
                        pwd = getpwuid(tmpArg.st_uid);
                        printf("%s ", pwd->pw_name);
                        grp = getgrgid(tmpArg.st_gid);
                        printf("%s ", grp->gr_name);
                        printf("%ld\t", tmpArg.st_size);
                        t = localtime(&tmpArg.st_ctime);
                        printf("%s ", lookup[t->tm_mon]);
                        printf("%2.0d %2d:%2.0d ", t->tm_mday, t->tm_hour, t->tm_min);
                        printf("%s", dirStruct->d_name);
                        char linkname[1000];
                        ssize_t r = readlink(dirStruct->d_name, linkname, 1000);
                        if (r != -1) {
                            linkname[r] = '\0';
                            printf(" -> %s\n", linkname);
                        }
                        else{
                            printf("\n");
                        }
                    }
                }   
                closedir(directory);
            }
        }
        else{
            char revisedDirectory[1000];
            reverseDirectoryChanger(homeDirectory, argv[1], revisedDirectory);
                printf("%s is the revisedDirectory\n", revisedDirectory);

            DIR *directory = opendir(revisedDirectory); 
            if (directory == NULL){ 
                printf("Could not open current directory" ); 
                return 0; 
            } 
            while ((dirStruct = readdir(directory)) != NULL){ 
                if(strcmp(dirStruct->d_name, ".")!=0&&strcmp(dirStruct->d_name, "..")!=0&&dirStruct->d_name[0]!='.'){ 
                    printf("%s\n", dirStruct->d_name);
                }
            }   
            closedir(directory);     
            return 0;
        }
    }
    else if(argc==4){
        if(strcmp(argv[2], "-l")==0){
            char revDirectory[1000];
            reverseDirectoryChanger(homeDirectory, argv[1], revDirectory);
                printf("%s is the revDirectory\n", revDirectory);

            DIR *directory = opendir(revDirectory); 
            if (directory == NULL){ 
                printf("Could not open current directory\n" ); 
                return 0; 
            } 
            while ((dirStruct = readdir(directory)) != NULL){
                if(strcmp(dirStruct->d_name, ".")!=0&&strcmp(dirStruct->d_name, "..")!=0&&dirStruct->d_name[0]!='.'){ 
                    char filePath[1000];
                    strcpy(filePath, revDirectory);
                    strcat(filePath, "/");
                    strcat(filePath, dirStruct->d_name);
                    if(lstat(filePath, &tmpArg)<0){
                        perror("lstat");
                        exit(-1);
                    }
                    else{
                        switch (tmpArg.st_mode & S_IFMT) {
                        case S_IFBLK:
                            printf("b");
                            break;
                        case S_IFCHR:
                            printf("c");
                            break;
                        case S_IFDIR:
                            printf("d");
                            break;
                        case S_IFIFO:
                            printf("p");
                            break;
                        case S_IFLNK:
                            printf("l");
                            break;
                        case S_IFREG:
                            printf("-");
                            break;
                        case S_IFSOCK:
                            printf("s");
                            break;
                        default:
                            printf("?");
                            break;
                        }
                        if(tmpArg.st_mode & S_IRUSR){
                            printf("r");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IWUSR){
                            printf("w");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IXUSR){
                            printf("x");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IRGRP){
                            printf("r");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IWGRP){
                            printf("w");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IXGRP){
                            printf("x");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IROTH){
                            printf("r");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IWOTH){
                            printf("w");
                        }
                        else{
                            printf("-");
                        }
                        if(tmpArg.st_mode & S_IXOTH){
                            printf("x");
                        }
                        else{
                            printf("-");
                        }
                        printf(" %-3ld ", tmpArg.st_nlink);
                        pwd = getpwuid(tmpArg.st_uid);
                        printf("%s ", pwd->pw_name);
                        grp = getgrgid(tmpArg.st_gid);
                        printf("%s ", grp->gr_name);
                        printf("%ld\t", tmpArg.st_size);
                        t = localtime(&tmpArg.st_ctime);
                        printf("%s ", lookup[t->tm_mon]);
                        printf("%2.0d %2d:%2.0d ", t->tm_mday, t->tm_hour, t->tm_min);
                        printf("%s", dirStruct->d_name);
                        char linkname[1000];
                        char tempPath[1000];
                        strcpy(tempPath,argv[1]);
                        strcat(tempPath, dirStruct->d_name);
                        ssize_t r = readlink(tempPath, linkname, 1000);
                        if (r != -1) {
                            linkname[r] = '\0';
                            printf(" -> %s\n", linkname);
                        }
                        else{
                            printf("\n");
                        }
                    }
                }
            }   
            closedir(directory);
        }
        else if (strcmp(argv[2], "-a")==0){
            char revisedDirectory[1000];
            reverseDirectoryChanger(homeDirectory, argv[1], revisedDirectory);
                printf("%s is the revisedDirectory\n", revisedDirectory);

            DIR *directory = opendir(revisedDirectory); 
            if (directory == NULL){ 
                printf("Could not open current directory" ); 
                return 0; 
            } 
            while ((dirStruct = readdir(directory)) != NULL){ 
                printf("%s\n", dirStruct->d_name);
            }   
            closedir(directory);     
            return 0;
        }
        else{
            char revDirectory[1000];
            reverseDirectoryChanger(homeDirectory, argv[1], revDirectory);
                printf("%s is the revisedDirectory\n", revDirectory);

            DIR *directory = opendir(revDirectory); 
            if (directory == NULL){ 
                printf("Could not open current directory\n" ); 
                return 0; 
            } 
            while ((dirStruct = readdir(directory)) != NULL){
                char filePath[1000];
                strcpy(filePath, revDirectory);
                strcat(filePath, "/");
                strcat(filePath, dirStruct->d_name);
                if(lstat(filePath, &tmpArg)<0){
                    perror("lstat");
                    exit(-1);
                }
                else{
                    switch (tmpArg.st_mode & S_IFMT) {
                    case S_IFBLK:
                        printf("b");
                        break;
                    case S_IFCHR:
                        printf("c");
                        break;
                    case S_IFDIR:
                        printf("d");
                        break;
                    case S_IFIFO:
                        printf("p");
                        break;
                    case S_IFLNK:
                        printf("l");
                        break;
                    case S_IFREG:
                        printf("-");
                        break;
                    case S_IFSOCK:
                        printf("s");
                        break;
                    default:
                        printf("?");
                        break;
                    }
                    if(tmpArg.st_mode & S_IRUSR){
                        printf("r");
                    }
                    else{
                        printf("-");
                    }
                    if(tmpArg.st_mode & S_IWUSR){
                        printf("w");
                    }
                    else{
                        printf("-");
                    }
                    if(tmpArg.st_mode & S_IXUSR){
                        printf("x");
                    }
                    else{
                        printf("-");
                    }
                    if(tmpArg.st_mode & S_IRGRP){
                        printf("r");
                    }
                    else{
                        printf("-");
                    }
                    if(tmpArg.st_mode & S_IWGRP){
                        printf("w");
                    }
                    else{
                        printf("-");
                    }
                    if(tmpArg.st_mode & S_IXGRP){
                        printf("x");
                    }
                    else{
                        printf("-");
                    }
                    if(tmpArg.st_mode & S_IROTH){
                        printf("r");
                    }
                    else{
                        printf("-");
                    }
                    if(tmpArg.st_mode & S_IWOTH){
                        printf("w");
                    }
                    else{
                        printf("-");
                    }
                    if(tmpArg.st_mode & S_IXOTH){
                        printf("x");
                    }
                    else{
                        printf("-");
                    }
                    printf(" %-3ld ", tmpArg.st_nlink);
                    pwd = getpwuid(tmpArg.st_uid);
                    printf("%s ", pwd->pw_name);
                    grp = getgrgid(tmpArg.st_gid);
                    printf("%s ", grp->gr_name);
                    printf("%ld\t", tmpArg.st_size);
                    t = localtime(&tmpArg.st_ctime);
                    printf("%s ", lookup[t->tm_mon]);
                    printf("%2.0d %2d:%2.0d ", t->tm_mday, t->tm_hour, t->tm_min);
                    printf("%s", dirStruct->d_name);
                    char linkname[1000];
                    char tempPath[1000];
                    strcpy(tempPath,argv[1]);
                    strcat(tempPath, dirStruct->d_name);
                    ssize_t r = readlink(tempPath, linkname, 1000);
                    if (r != -1) {
                        linkname[r] = '\0';
                        printf(" -> %s\n", linkname);
                    }
                    else{
                        printf("\n");
                    }
                }
            }   
            closedir(directory);
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