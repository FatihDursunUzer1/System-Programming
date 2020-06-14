/* 
Fatih Dursun Uzer
3 March 2020
aBc şeklinde parent processi önceden yazdıran ve daha sonra child lardan ilk oluşanı seçip yazdıran
ardından ikinci childı yazdıran C programı */


#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <stdlib.h>

 int main()
{  
  pid_t pid;//1.child process olusturmak icin degisken
  pid_t pid1; //2.child process olusturmak icin degisken
  int status; //statuyu tutan degisken
  for(int i=0;i<10;++i)
    {
      wait(&status);
      pid=fork();
      if(pid==0)//1.child process icinde
        {     
             printf("B");
             sleep(1);
             fflush(stdout);
             exit(1);
        }
      else if(pid>0) //parent process icerisinde
            {  
                pid1=fork(); //2.child process olusturma
                if(pid1==0) //2.child process icinde
                {
                    sleep(1);
                    printf("c");
                    fflush(stdout);
                    exit(1);
                }
                else if(pid1>0)
                {
                    printf("a");
                    fflush(stdout);
                    sleep(2);
                }
            }
    }
}