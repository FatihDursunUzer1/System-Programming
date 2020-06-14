/* 3 March 2020
Fatih Dursun Uzer
 Parent process in belli bir childi bekleyip diger childi beklememesini gosteren C programi */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <stdlib.h>

int main()
{  
    int pid;
    int pid1;
   for(int i=0;i<5;++i)
   {
       pid=fork();
   }
    int status;
    if(pid>0)
    { 
        pid1=fork();
        waitpid(pid1,&status,0);
        if(pid1==0) //2.child process icerisinde
        {
            printf("c");
            fflush(stdout);
            sleep(1);
        }
        else if(pid1>0) //parent process icerisinde
        {
            printf("a");
            fflush(stdout);
            sleep(1);
        }
    }
    else if(pid==0)//1.child icerisinde
    {
        printf("B");
        sleep(2);
    }
    return 0;
}