#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define PATHLEN 1024
#define MAX_ITEMS 1024
void do_ls(char[] , int);
int dostat(char*);
int checkOption(int ac , char* av[]);
void show_file_info (char * , struct stat *);
void mode_to_letters(int mode , char str[]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

int main(int ac , char* av[])
{
    int i;
    int option = checkOption(ac , av);
    if(ac==1)
        do_ls("." , option != 0);
    else
    {
        int option = checkOption(ac , av);
        for(i=1; i<ac ; i++)
        {
            if(i == option)
                continue ;
            do_ls(av[i] , option != 0);
        }
    }

    return 0;
}


void do_ls(char dirname[] , int flag)
{
    DIR* dir_ptr;
    struct dirent * direntp ;
    char fullpath[PATHLEN];
    char directories[MAX_ITEMS][PATHLEN] ;
    int count , i ;

    puts("");
    puts(dirname);

    if((dir_ptr = opendir(dirname)) == NULL)
    {
        fprintf(stderr , "ls1 : cannot open %s\n" , dirname);
    }
    else
    {
        count = 0 ;
        while((direntp = readdir(dir_ptr)) != NULL)
        {
            strcpy(fullpath , dirname);
            strcat(fullpath , "/");
            strcat(fullpath , direntp->d_name);
            if(dostat(fullpath) && strcmp(direntp->d_name , ".") != 0 && strcmp(direntp->d_name , "..") != 0)
                strcpy(directories[count++] , fullpath);
        }
        closedir(dir_ptr);

        for(i=0 ; i < count ; i++){
            do_ls(directories[i] , flag);
        }
    }
}

int dostat(char *filename)
{
    struct stat info;

    if(lstat(filename , &info) == -1)
    {
        perror(filename);
    }
    else
    {
        show_file_info(filename , &info);
    }

   return S_ISDIR( info.st_mode );
}

int checkOption(int ac , char* av[])
{
    int i ;
    int result ;

    result = 0;
    for(i=0 ; i<ac ; i++)
    {
        if(strncmp(av[i] , "-R" ,2) == 0)
        {
            result = 1;
            break;
        }
    }

    return i ;
}

void show_file_info(char *filename , struct stat *info_p)
{
    char *uid_to_name() , *ctime() , *gid_to_name() , *filemode();
    void mode_to_letters();
    char modestr[11];

    mode_to_letters(info_p->st_mode , modestr);

    printf("%s " , modestr);
    printf("%4d " , (int) info_p->st_nlink);
    printf("%-8s " , uid_to_name(info_p->st_uid));
    printf("%-8s " , gid_to_name(info_p->st_gid));
    printf("%8ld " , (long)info_p->st_size);
    printf("%.12s " , 4+ctime(&info_p->st_mtime));
    printf("%s\n" , filename);
}

void mode_to_letters(int mode , char str[])
{
    strcpy(str , "----------");

    if(S_ISDIR(mode)) str[0] = 'd';
    if(S_ISCHR(mode)) str[0] = 'c';
    if(S_ISBLK(mode)) str[0] = 'b';

    if(mode & S_IRUSR) str[1] = 'r';
    if(mode & S_IWUSR) str[2] = 'w';
    if(mode & S_IXUSR) str[3] = 'x';

    if(mode & S_IRGRP) str[4] = 'r';
    if(mode & S_IWGRP) str[5] = 'w';
    if(mode & S_IXGRP) str[6] = 'x';

    if(mode & S_IROTH) str[7] = 'r';
    if(mode & S_IWOTH) str[8] = 'w';
    if(mode & S_IXOTH) str[9] = 'x';
}

#include <pwd.h>
char *uid_to_name(uid_t uid)
{
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];

    if((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr , "%d" , uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;
}

#include <grp.h>
char* gid_to_name(gid_t gid)
{
    struct group *getgrgid() , *grp_ptr;
    static char numstr[10];

    if((grp_ptr = getgrgid(gid)) == NULL)
    {
        sprintf(numstr , "%d" , gid);
        return numstr;
    }
    else
    {
        return grp_ptr->gr_name;
    }


}











































