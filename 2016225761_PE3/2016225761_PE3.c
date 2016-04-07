#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

typedef struct _nodeIno{
    ino_t inode ;
    struct _nodeIno* link;
}NodeIno;

ino_t get_inode(char*);
void printpathto();
void inum_to_name(ino_t , char * , int);

int main()
{
    printpathto();
    putchar('\n');
    return 0;
}

void printpathto()
//    prints path leading down to an object with this inode
//    kind of recursive
{
    ino_t this_inode;
    NodeIno* list , *node;
    char its_name[BUFSIZ];
    
    this_inode = get_inode(".") ;
    list = NULL;

    while(get_inode("..") != this_inode)
    {
        node = (NodeIno*)malloc(sizeof(NodeIno));
        node->link = list;
        node->inode = this_inode;
        list = node ;
        chdir("..");
        this_inode = get_inode(".");
    }

    for(node = list ; node != NULL ; node = list)
    {
        list = list -> link;
        inum_to_name(node->inode , its_name , BUFSIZ);
        printf("/%s" , its_name);
        chdir(its_name);
        free(node);

    }

    puts("");
}

void inum_to_name(ino_t inode_to_find , char *namebuf , int buflen)
{
    DIR *dir_ptr ;
    struct dirent *direntp;
    dir_ptr = opendir(".");
    if(dir_ptr == NULL)
    {
        perror(".");
        exit(1);
    }

    while((direntp = readdir(dir_ptr)) != NULL)
    {
        if(direntp->d_ino == inode_to_find)
        {
            strncpy(namebuf , direntp->d_name , buflen);
            namebuf[buflen-1] = '\0';
            closedir(dir_ptr);
            return ;
        }
    }

    fprintf(stderr , "error looking for inum %d\n" , (int)inode_to_find);
    exit(1);
}

ino_t get_inode(char* fname)
{
    struct stat info ;

    if(stat(fname , &info) == -1)
    {
        fprintf(stderr , "Cannot stat ");
        perror(fname);
        exit(1);
    }

    return info.st_ino;
}
