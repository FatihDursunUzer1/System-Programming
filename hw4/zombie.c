/*Fatih Dursun Üzer
18120205041 
Bilgisayar Mühendisliği 

srand fonksiyonunu kullandığımda 1 de her seferinde kazanıyor 
2 de kazanma oranı %60 lara düşüyor 3 ve sonrasında kazanma oranı %50'nin baya altında oluyor
bu yüzden srand fonksiyonu kullandığımda optimal sayı 1 oluyor.
srand fonksiyonunu kullanmadan 1-2-3 de %100 kazanıyor 4de %30-%60 arası kazanma oranı var 
5 ve sonrasında ise hiç kazanamıyor. srand fonksiyonunu kullanmadığımda optimal sayı 3 oluyor.*/


#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define milisecond 2000
/*number of zombies in the game: 
you should synchronize threads editing this variable*/
int zombieCounter;
int killedZombieCounter;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;

/* Keeps track of number of zombies entered.*/
void zombieEntered(){
    zombieCounter++;
}
/* Keeps track of number of zombies killed.*/
void zombieKilled(){
        zombieCounter--;
        killedZombieCounter++;
}
/* Returns true if number of zombies in the room are 
greater than or equal to 100.*/
int tooManyZombiesInTheRoom(){
    if(getInTheRoomCount()>=100){
        return 1;
    }
    return 0;
}

/*Returns true if more than 100 zombies have been killed.*/
int killed100Zombies(){
    if(getKilledCount()>100){
        return 1;
    }
    return 0;
}

/* Returns true if there is at least one zombies in the room.*/
int zombiesExist(){
    if(getInTheRoomCount()>=1){
        return 1;
    }
    return 0;
}
/*Returns the count of number of zombies killed.*/
int getKilledCount(){
    return killedZombieCounter;
}

/* Returns the count of number of zombies in the room.*/
int getInTheRoomCount(){
    return zombieCounter;
}
/*doorman thread*/
void *doorMan(void *param){
    while(!(killed100Zombies() || tooManyZombiesInTheRoom())){
        //srand(time(0));
        int randomNumber=rand()% 2;
        if(randomNumber){
            int s=pthread_mutex_lock(&m);
            if (s != 0) perror("pthread_mutex_lock");
            zombieEntered();
            s=pthread_mutex_unlock(&m);
            if (s != 0) perror("pthread_mutex_unlock");
        }
        usleep(milisecond);
    }
        pthread_exit(NULL);
}

/*slayer thread*/
void *slayer(void* param){
    while(!(killed100Zombies() || tooManyZombiesInTheRoom())){
        if(zombiesExist()){ 
            int s=pthread_mutex_lock(&m);
            if (s != 0) perror("pthread_mutex_lock");
            zombieKilled();
            s=pthread_mutex_unlock(&m);
             if (s != 0) perror("pthread_mutex_unlocklock");
        }
        usleep(milisecond);
    }
    pthread_exit(NULL);
}
/*simulator main thread*/
int main(int argc, char **argv){
    if(argc==2){
        const char* arg=argv[1];
        int numberOfDoorMen=atoi(arg);
        if(numberOfDoorMen){
            int playCount=0;
            int win=0;
            int lose=0;
            while(playCount<10)
            {
                pthread_t tSlayer;
                pthread_t tDoorMan[numberOfDoorMen];
                pthread_t pthreadCreate;
                pthreadCreate=pthread_create(&tSlayer,NULL,slayer,NULL);
                if(pthreadCreate!=0){
                    perror("Slayer could not be created \n");
                    return -1;
                } 
                for(int i=0;i<numberOfDoorMen;++i){
                    pthreadCreate=pthread_create(&tDoorMan[i],NULL,doorMan,NULL);
                    if(pthreadCreate!=0){
                    perror("DoorMan could not be created \n");
                    return -1;
                    }
                }
                pthread_join(tSlayer,NULL);
                for(int i=0;i<numberOfDoorMen;++i)
                   pthread_join(tDoorMan[i],NULL);
                if(tooManyZombiesInTheRoom()){
                    lose++;
                        printf("You failed!.You LOSE! \n");
                }
                else if(killed100Zombies()){
                    win++;
                        printf("You SUCCEED. you WIN \n");
                }
            printf(" Remaining zombie(s) : %d  \n  Killed zombie(s): %d \n",zombieCounter,killedZombieCounter);
            zombieCounter=0;
            killedZombieCounter=0;
            playCount++;
            }
            printf("win : %d , lose : %d",win,lose);
        }
    }
    else
        printf("Ornek kullanim bicimi: ./zombie 5 \n 5--->kapıdaki adam sayisi.");
        return 0;
}
