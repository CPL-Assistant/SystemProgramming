#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

#define oops(m,x) {\
    perror(m) ; \
    exit(x);\
}

FILE *popen2(const char *command , const char *mode);

int main(void)
{
    FILE* fp ;
    char buf[BUFSIZ];

    fp = popen2("who" , "r");
    while(fgets(buf , BUFSIZ , fp) != NULL)
        fputs(buf , stdout);

    fclose(fp);
    return 0;
}

FILE *popen2(const char *command , const char *mode)
{
    int pid , p[2] ;
    FILE* fp ;
    if(strcmp(mode , "r") != 0 && strcmp(mode , "w") != 0)
    {
        return NULL ;
    }

    if(pipe(p) == -1)
    {
        oops("can't make pipe" , 1);
    }

    if((pid = fork()) == -1){
        close(p[0]);
        close(p[1]);
        oops("cannot fork" , 2);
    }

    if(pid == 0)
    {
        if(strcmp(mode , "r") == 0)
        {
            close(p[0]);
            if(dup2(p[1] , 1) == -1)
                oops("cloud not redirect stdout" , 3);
            close(p[1]);
            execlp(command , command , NULL);
            oops(command , 4);
        }
        else
        {
            close(p[1]);
            if(dup2(p[0] , 0) == -1)
                oops("cloud not redirect stdin" , 5);
            close(p[0]);
            execlp(command , command , NULL);
            oops(command , 6);
        }
    }
    else if(pid > 0)
    {
        if(strcmp(mode , "r") == 0)
        {
            close(p[1]);

            if((fp = fdopen(p[0] , "r")) == NULL)
                oops("can't make file pointer about input pipe\n" , 7);

        }
        else
        {
            close(p[0]);

            if((fp = fdopen(p[1] , "w")) == NULL)
                oops("can't make file pointer about output pipe\n" , 8);
        }
    }

    return fp ;
}

