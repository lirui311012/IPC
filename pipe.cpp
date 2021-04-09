#include <iostream>
#include <vector>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;


#define MAX_BUFF 1024

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

int main(int argc, char **argv)
{
    cout << "hello world" << endl;
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    vector<int> vec(arr, arr + (sizeof(arr) / sizeof(arr[0])));
    for (int i = 0; i < vec.size(); ++i)
    {
        cout << vec[i] << " ";
    }
    cout << endl;
    
    int pipe1[2],pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    __pid_t pid = fork();
    if (pid == 0)
    {
        //son
        cout << "sub" << endl;
        close(pipe1[1]);
        close(pipe2[0]);
        server(pipe1[0],pipe2[1]);
        exit(0);
    }
    //father
    cout << "father" << endl;
    close(pipe1[0]);
    close(pipe2[1]);
    client(pipe2[0],pipe1[1]);    
    waitpid(pid,NULL,0);

    return 0;
}