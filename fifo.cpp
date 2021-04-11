#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;

#define MAX_BUFF 1024
#define FIFO1 "./fifo1"
#define FIFO2 "./fifo2"


void server(int readfd,int writefd)
{
    int fd;
    ssize_t n;
    char buff[MAX_BUFF+1];
    if((n = read(readfd,buff,MAX_BUFF)) == 0)
    {
        cout << "end-of-file while reading pathname" << endl;
    }
    buff[n] = '\0';
    //open file
    if((fd = open(buff,O_RDONLY)) < 0 )
    {
        //open file failed
        cout << "open file failed" << endl;
        snprintf(buff+n,sizeof(buff)-n,":can't open, %s\n",strerror(errno));
        n = strlen(buff);
        write(writefd,buff,n);
    }else{
        //open file success
        while((n = read(fd,buff,MAX_BUFF)) > 0){
            write(writefd,(const void *)buff,n);
        }
        close(fd);
    }
}
void client(int readfd,int writefd)
{
    size_t len;
    char buff[MAX_BUFF];
    cout << "please input file path:" << endl;
    fgets(buff,MAX_BUFF,stdin);
    len = strlen((const char *)buff);
    if(buff[len-1] == '\n')
    {
        --len;
    }
    write(writefd,(const void *)buff,len);
    ssize_t readlen;
    while((readlen = read(readfd,buff,MAX_BUFF)) > 0){
        write(STDOUT_FILENO,(const void *)buff,readlen);
    }
}


int main(int argc, char const *argv[])
{
    int readfd, writefd;
    pid_t childpid;
    if((mkfifo(FIFO1,0667) < 0 ) && errno != EEXIST)
    {
        printf("can't create %s\n",FIFO1);
    }
    if((mkfifo(FIFO2,0667) < 0 ) && errno != EEXIST)
    {
        unlink(FIFO1);
        printf("can't create %s\n",FIFO1);
    }
    if((childpid = fork()) == 0)
    {
        readfd = open(FIFO1,O_RDONLY,0);
        writefd = open(FIFO2,O_WRONLY,0);
        server(readfd,writefd);
        exit(0);
    }
    writefd = open(FIFO1,O_WRONLY,0);
    readfd = open(FIFO2,O_RDONLY,0);
    client(readfd,writefd);
    waitpid(childpid,NULL,0);
    close(readfd);
    close(writefd);
    unlink(FIFO1);
    unlink(FIFO2);
    return 0;
}
