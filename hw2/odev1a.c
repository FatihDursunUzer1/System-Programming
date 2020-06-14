/* 3 March 2020
   Fatih Dursun Uzer
   İlk önce parent processin calis daha sonra child processin calismasini saglayan C programi
*/
#include <sys/wait.h> 
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
int main()
{
    int status;
    pid_t pid=fork();
    for(int i=0;i<10;++i)
    { 
        wait(&status);
        pid=fork();
        if(pid==0)//child process icerisinde
        {
            printf("B");
            fflush(stdout);
            exit(1);
        }
        else if(pid>0)//parent process icerisinde
        {
            printf("a");
            fflush(stdout);
        }
    }

}