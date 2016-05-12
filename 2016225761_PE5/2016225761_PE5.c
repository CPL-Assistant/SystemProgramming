#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <stdlib.h>
int get_response(char* question);
void set_crmode();
int tty_mode(int how);

int main(void)
{
    void f(int);
    int i ;
    signal(SIGINT , f);
    for(i=0 ; i<10 ; i++)
    {
        printf("hello\n");
        sleep(1);
    }

    return 0;
}

void f(int signum)
{
    int response;
    tty_mode(0);
    set_crmode();
    response = get_response("interrupted! OK to quit(y/n)? ");
    tty_mode(1);

    if(response == 0)
        exit(0);
}

void set_crmode()
{
    struct termios ttystate ;
    tcgetattr(0 , &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0 , TCSANOW , &ttystate);
}

int tty_mode(int how)
{
    static struct termios original_mode;

    if(how == 0)
    {
        tcgetattr(0 , &original_mode);
    }
    else
    {
        return tcsetattr(0, TCSANOW, &original_mode);
    }
}

int get_response(char* question)
{
    printf("%s", question);
    while(1)
    {
        switch(getchar())
        {
        case 'y':
        case 'Y':
            return 0;
        case 'n':
        case 'N':
        case EOF:
            return 1;
        }
    }
}
