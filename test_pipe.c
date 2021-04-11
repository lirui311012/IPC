#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


#define MAX_BUFF 1024


int main(int argc, char **argv)
{
    int fd[2],n;
    char c;
    pid_t childpid;
    pipe(fd);
    if((childpid = fork()) == 0)
    {
        sleep(3);
        if((n = read(fd[0],&c,1)) != 1)
        {
            printf("child read failed errinfo : %s\n",strerror(errno));
        }

        printf("child read %c\n",c);
        write(fd[0],&c,1);
        exit(0);
    }
    write(fd[1],"p",1);
    if((n = read(fd[1],&c,1)) != 1)
    {
        printf("parent read failed errinfo : %s\n",strerror(errno));
    }
    printf("parent read %c\n",c);

    return 0;
}