/* 3 March 2020 
Fatih Dursun Uzer
ilk once child process ların calismasini saglayan C programi */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 

int main()
{
    pid_t pid;
    for(int i=0;i<5;++i)
    {
        pid=fork();
    }
    int status;
    wait(&status);
    if(pid==0)//child process icerisinde
    {
        printf("B");
        fflush(stdout);
        sleep(1);  
    }
    else if(pid>0) //parent process icerisinde
    {
        printf("a");
        fflush(stdout);
        sleep(1);   
    }
}
