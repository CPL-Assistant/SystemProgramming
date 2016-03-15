#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include "utmplib.h"

//#define SHOWHOST

void showtime(long);
void show_info(struct utmp* utbufp);

int main(int argc , char* argvp[])
{
    struct utmp *utbufp ;
    struct utmp* utmp_next();

    if(utmp_open(UTMP_FILE) == -1){
        perror(UTMP_FILE);
        exit(1);
    }

    while((utbufp = utmp_next()) != ((struct utmp*)NULL))
    {
        show_info(utbufp);
    }

    utmp_close();

    return 0;
}

void showtime(long timeval)
{
    char* cp ;
    cp = ctime(&timeval);

    printf("%12.12s", cp+4);
}

void show_info(struct utmp* utbufp)
{
    if(utbufp->ut_type != USER_PROCESS)
    {
//        printf("%d %d\n" , utbufp->ut_type , USER_PROCESS);
        return 0;
    }

    printf("%-8.8s", utbufp->ut_name);
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    showtime(utbufp->ut_time);

#ifdef SHOWHOST
    if(utbufp->ut_host[0] != '\0')
        printf("(%s)" , utbufp->ut_host);
#endif
    printf("\n");
}
