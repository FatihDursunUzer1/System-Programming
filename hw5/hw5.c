
/*Fatih Dursun Üzer
18120205041
Bilgisayar Mühendisliği 2.Sınıf 
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    int pipefds[2];
    pipe2(pipefds, O_CLOEXEC);
    if(!fork())
    {
        int pipefds1[2];
        /* Call with the O_CLOEXEC flag to prevent any commands from blocking */
        pipe2(pipefds1, O_CLOEXEC);
        if(!fork())
        {
            int pipefds2[2];
            pipe2(pipefds2,O_CLOEXEC);
            if (!fork()){/* Child */
                /* Make the stdout of the process, the write end */
                dup2(pipefds2[1], 1); //ls den aldıgı cıktıyı pipe a yazar.
                /* Exec! Don’t forget the cast*/
                execlp("ls", "ls", "-1", (char *)NULL);
                exit(-1);
            }
        dup2(pipefds2[0], 0);// pipe dan gelen bilgiyi cut komutunun okumasını saglar.
        dup2(pipefds1[1],1);//cut komutundan aldıgı cıktıyı pipe a yazar.
        execlp("cut", "cut", "-f1", "-d.",(char *)NULL);
        exit(-1);
        } 
        dup2(pipefds1[0], 0);//pipe dan aldıgı bilgiyi uniq komutunun okumasını sağlar.
        dup2(pipefds[1],1);// uniq komutununun ciktisini pipe a yazar.
        execlp("uniq","uniq",(char *)NULL);
        exit(-1);  
    }
    dup2(pipefds[0], 0);//pipe dan gelen bilgiyi tee komutunun okumasını saglar.
    // Same deal here
    execlp("tee","tee","dirents",(char *)NULL);
    exit(-1);
    return 0;
}