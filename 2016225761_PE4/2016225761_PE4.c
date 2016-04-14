#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#define MAXPATH 1024
#define TRUE 1
#define FALSE 0

int checkIndexForFilename(char* filename);
int isDirectory(char* filename);
int isExist(char* filename);

int main(int argc , char* argv[])
{
    char filename[MAXPATH];
    char fullPath[MAXPATH];
    int startIdx , chk;
    struct stat st1 , st2 ;

    chdir("/home/pu0116/바탕화면/a");

    startIdx = checkIndexForFilename(argv[1]);

    strcpy(filename , &argv[1][startIdx+1]);

    if(argc != 3)
    {
        fprintf(stderr , "Usage : %s origin_filename new_filename\n" , argv[0]);
        exit(1);
    }
    else if(!isExist(argv[1]))
    {
        fprintf(stderr , "The file %s don't exist!!\n" , argv[1]);
        exit(1);
    }
    else if(isExist(argv[2]) && !isDirectory(argv[2]))
    {
        chk = checkIndexForFilename(argv[2]);

        if(strcmp(filename , &argv[2][chk + 1]) == 0)
        {
            if(startIdx == -1)
                lstat("." , &st1);
            else{
                strncpy(fullPath , argv[1] , startIdx ) ;
                fullPath[startIdx] = 0;
                lstat(fullPath , &st1);
            }

            if(chk == -1){
                lstat("." , &st2);
            }
            else{
                strncpy(fullPath , argv[2] , chk);
                fullPath[chk] = 0;
                lstat(fullPath , &st2);
            }

            if(st1.st_ino == st2.st_ino){
                fprintf(stderr , "%s and %s are the same!!\n" , argv[1] , argv[2]);
                exit(1);
            }

        }

        fprintf(stderr , "The file %s already exist!!\n" , argv[2]);
        exit(1);
    }

    chk = isDirectory(argv[2]);
    printf("[Debug] main , chk : %d\n" , chk);
//    chk = 0;
    if(chk == TRUE)
        //if It whom the argv[2] point is directory
    {
        strcpy(fullPath , argv[2]);

        if(fullPath[strlen(fullPath) - 1] != '/')
            strcat(fullPath , "/");

        strcat(fullPath , filename);
        link(argv[1] , fullPath);
    }
    else
    {
        //if It whom the argv[2] point is not directory
        strcpy(fullPath , argv[2]);
        strcpy(filename , argv[2]);

        startIdx = checkIndexForFilename(filename);
        if(startIdx != -1 )
            filename[startIdx] = 0;

        if(isDirectory(filename) == -1 && startIdx != -1)
        {
            fprintf(stderr , "The directory %s doesn't exist!!\n" , filename);
            exit(1);
        }
//        printf("[Debug] main , fullPath : %s\n " , fullPath);
        link(argv[1] , fullPath);
    }

    unlink(argv[1]);
    printf("success mv program !!\n");
    return 0;
}



int checkIndexForFilename(char* filename)
{
    int idx ;

    idx = strlen(filename);

    for(idx-- ; idx >= 0 ; idx--)
    {
        if(filename[idx] == '/')
            return idx;
    }

    return -1;
}

int isDirectory(char* filename)
{
    struct stat inode;
    if(lstat(filename , &inode) == -1)
        return -1;
    return S_ISDIR(inode.st_mode);
}

int isExist(char* filename)
{
    int descriptor ;

    descriptor = open(filename , O_RDONLY);
    if(descriptor == -1)
        return FALSE;
    else{
        close(descriptor);
        return TRUE;
    }
}

