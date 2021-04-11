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

int main()
{
    char buff[MAX_BUFF], command[MAX_BUFF];
    fgets(buff,MAX_BUFF,stdin);
    size_t n = strlen(buff);
    if(buff[n-1] == '\n')
    {
        buff[n-1] = '\0';
        --n;
    }
    snprintf(command,sizeof(command),"cat %s",buff);
    FILE * fp = popen(command,"r");
    while((fgets(buff,MAX_BUFF,fp)) != NULL)
    {
        fputs(buff,stdout);
    }
    pclose(fp);

    return 0;
}