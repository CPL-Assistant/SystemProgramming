#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define oops(m,x) {\
    perror(m) ; \
    exit(x);\
}

int main(int ac , char **av)
{
    int thepipe[2] , pid ;
    int index , line;

    if(ac < 3)
    {
        fprintf(stderr , "please give me atleast 3 2argument ex)./Program_name cmd1 cmd2 ...");
        exit(1);
    }

    line = atoi(av[ac-1]);
    index = ac - 2 ;
    if(line == 0)
    {
        if(strcmp(av[ac-1] , "0") != 0){
               index = ac - 1 ;
               line = -1 ;
        }

    }

    if(pipe(thepipe) == -1)
    {
        oops("Cannot get a pipe" , 1);
    }
    if((pid = fork()) == -1)
        oops("Cannot fork" , 2);

    if(pid > 0)
    {
        close(thepipe[1]);
        if(dup2(thepipe[0] , 0) == -1)
            oops("cloud not redirect stdin" , 3);
        close(thepipe[0]);

        if(line != -1)
            execlp(av[index] , av[index] , "-n" , av[ac-1] , NULL);
        else
            execlp(av[index] , av[index] , NULL);
        oops(av[index] , 4);
    }

    for(index-- ; index > 1 ; index--)
    {
        close(thepipe[0]);
        if(dup2(thepipe[1] , 1) == -1)
        {
            oops("could not redirect stdout" , 4);
        }

        if(pipe(thepipe) == -1)
        {
            oops("Cannot get a pipe" , 5);
        }
        if((pid = fork()) == -1)
            oops("Cannot fork" , 6);

        if(pid > 0)
        {
            close(thepipe[1]);
            if(dup2(thepipe[0] , 0) == -1)
                oops("cloud not redirect stdin" , 7);
            close(thepipe[0]);
            execlp(av[index] , av[index] , NULL);
            oops(av[index] , 8);
        }
    }
    close(thepipe[0]);
    if(dup2(thepipe[1] , 1) == -1)
    {
        oops("could not redirect stdout" , 4);
    }
    close(thepipe[1]);


    execlp(av[1] , av[1] , NULL);
    oops(av[1] , 9);

}

