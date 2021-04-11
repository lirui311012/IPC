#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#define MAX_LEN 1024

int main()
{
    int file_fd = open("./test.txt",O_RDWR);
    printf("父进程打开文件text.txt fd = %d\n",file_fd);
    __pid_t pid = fork();
    if(pid == 0)
    {
        //子进程
        printf("子进程共享file_fd = %d\n",file_fd);
        lseek(file_fd,0,SEEK_END);
        char *data = "mmmmmmmm";
        ssize_t n = write(file_fd,data,strlen(data));
        if(n > 0)
        {
            printf("子进程写入数据成功\n");
        }
        else
        {
            printf("子进程写入数据失败\n");
        }
        exit(0);
    }
    else
    {
        sleep(1);
        //父进程
        printf("父进程file_fd = %d\n",file_fd);
        char buf[MAX_LEN] = {0};
        ssize_t readlen;
        while((readlen = read(file_fd,buf,MAX_LEN)) > 0){
            write(STDOUT_FILENO,(const void *)buf,readlen);
        }
        //printf("n = %ld\n父进程读取文件内容： %s\n",n,buf);
        //write(STDOUT_FILENO,buf,n);
        if(readlen == 0)
        {
            printf("father progress read data filed : %s\n",strerror(errno));
        }

    }
    printf("STDIN_FILENO = %d\n",STDIN_FILENO);
    printf("STDOUT_FILENO = %d\n",STDOUT_FILENO);
    printf("STDERR_FILENO = %d\n",STDERR_FILENO);
    waitpid(pid,NULL,0);
    close(file_fd);

    return 0;
}