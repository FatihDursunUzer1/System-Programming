/*Fatih Dursun Üzer 
18120205041
İstanbul Medeniyet Universitesi Bilgisayar Muhendisligi 2.Sinif
mymalloc.c
HW3
*/

#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <unistd.h>

/*Bir blogu bolerek kullanicinin istedigi byte kadar alani bolen fonksiyon */ 
Block * split(Block* b,size_t size)
{
    int oldsize=b->info.size;
    b->info.size=size;
    Block *q=(void *)b+(2*sizeof(Block))+b->info.size;
    q->info.size=oldsize-size-(2*sizeof(Block));
    q->info.isfree=1;
    Block * bTag=(void *)q-sizeof(Block);
    bTag->info.size=b->info.size;
    bTag->info.isfree=b->info.isfree;
    Block *qTag=(void *)q+q->info.size+sizeof(Block);
    qTag->info.isfree=1;
    qTag->info.size=q->info.size;
    Block *temp=free_list;
    printf(" %d kadar alan ayrilmistir. Alan ayrılan blok'un yeni size ı %d \n",b->info.size,q->info.size);

    return q;
}

/*Kullanıcının istedigi kadar alanı eger heap icinde varsa, belirtilen stratejiye 
gore bloklardan alan ve o alanı free_listten cıkaran fonksiyon */
void * mymalloc(size_t size)
{
    static int firstcall=1;
    if(firstcall)
    {
        heap_start=sbrk((sizeof(Block)*2)+1024);
        heap_start->info.size=1024;
        heap_start->info.isfree=1;
        end=(void*)heap_start+heap_start->info.size+sizeof(Block);
        end->info.size=1024;
        end->info.isfree=1;
        free_list=heap_start;
        firstcall=0;
        printf("heap_start ve  end  %d -  %d \n",heap_start,end);
    }
    while(size%16 !=0) size++;
    printf("%d kadar alan ayirmaya calisiliyor. \n",size);
    Block *p;
   if(strategy==bestFit){
    p=bestFitAlgorithm(free_list,size);
   }
   else if(strategy==worstFit){
       p=worstFitAlgorithm(free_list);
   }
   else if(strategy==firstFit){
       p=firstFitAlgorithm(free_list,size);
   }
    Block *q;
    if(p==NULL)
    {
        return NULL;
    }
    if (p == end || size+(2*sizeof(Block)) > p->info.size){
        fprintf(stderr,"yeterli alan bulunamadi. \n");
        return NULL;
    }
    else if(size+(2*sizeof(Block)) < p->info.size){
        printf("Istediginiz kadar alan %d size lı bloktan alinacaktir",p->info.size);
        q=split(p, size);
        if(p==free_list){
            free_list=q;
        }
        /*Kullaniciya verilmeyen kismi, verilen kisim yerine free_liste ekleme */
        Block *temp=free_list;
        while(temp!=NULL)
        { 
            if (temp->next==p){
                    Block *b=((Block *)temp->next)->next;
                    Block *btag=(void *)temp->next+((Block *)temp->next)->info.size+sizeof(Block);
                    if(b!=NULL)
                    {
                        Block *nBtag=(void *)b+b->info.size+sizeof(Block);
                        nBtag->next=q;
                    }
                    temp->next=q;
                    break;   
            }
            temp=temp->next;
        }
    }
    else if(size==p->info.size){ /*kullanici alanin tamamini isterse */
        if(p==free_list){
           free_list=NULL;
        }
        /* kullanicin aldigi alani free_listten cikarma */
        else{
            Block *temp=free_list;
            while(temp!=NULL)
            {
                if(temp->next!=NULL && temp->next==p)
                {
                    Block *b=((Block *)temp->next)->next;
                    Block *btag=(void *)temp->next+((Block *)temp->next)->info.size+sizeof(Block);
                    if(b!=NULL)
                    {
                        Block *nBtag=(void *)b+b->info.size+sizeof(Block);
                        nBtag->next=temp;
                        temp->next=b;
                    }
                    else 
                    {
                        temp->next=NULL;
                    }
                    break;
                }
                temp=temp->next;
            }
        }
    }
    p->info.isfree = 0; 
    printf(" %d sizelık bir blok ayrilmistir. \n",p->info.size);
    return (void *)p+sizeof(Block);
}

/*Strateji 1.
firstFit
ilk uygun alanı bulup kullanilmasi icin return eden fonksiyon */
Block *firstFitAlgorithm(Block * p,size_t size)
{
    /* p nin sonuna ve p den sonraki bloğa yeni tagler eklenecegi icin 
    .Bu taglere yer kalmasi icin p->info.size 'ın size + 2*sizeof(Block) dan buyuk olmasi gerekir */
    if(p==NULL)
    return NULL;
    while(p<end && (size+(2*sizeof(Block)>p->info.size))){ 
        p=p->next;
    }
    return p;
}

/*Strateji 2
worstFit
Uygun olan en buyuk alani kullanilmasi icin return eden fonksiyon */
Block *worstFitAlgorithm(Block *p)
{
    Block * max=free_list;
    while(p<end && (p !=NULL)){
       if((p->info.size > max->info.size)){
            max=p;
       }
        p=p->next;
        printf("max - p(free)  %d  -  %d \n",max,p);
    }
    return max;
}

/*Strateji 3
bestFit
Kapasite olarak en uygun alanı bularak kullanilmasi icin return eder */
Block *bestFitAlgorithm(Block *p,size_t size)
{
    Block *min=free_list;
    while(p<end && (p!=NULL)){
       if(p->info.size<min && size+(2*sizeof(Block))<=p->info.size){
            min=p;
        }
        p=p->next;
    }
    return min;
}
/*Kullanicinin almis oldugu bir alani tekrardan free etmesini saglayan fonksiyon*/
void *myfree(void *p) 
{
    if(p==NULL)
    return NULL;
    Block *b = (Block*)(p-sizeof(Block));/*block header*/
    printf("B Nin adresi %d \n",b);
     if(b->info.isfree==1)
    {
        fprintf(stderr,"Bos duruma getirmek istediginiz alan zaten bos durumda ");
        return NULL;
    }
    b->info.isfree=1;
    Block *next=(void *)b+b->info.size+(2*sizeof(Block));
    if(next!=NULL)
    { 
        /*Kullanicini free ettigi alanın sonrasındaki alan da free ise bu iki alanı birlestirir */
        if(next->info.isfree)
        {
            b->info.size+=next->info.size+(2*sizeof(Block));
            Block *bTag=(void *)b+b->info.size+sizeof(Block);
            bTag->info.size=b->info.size;
            if(free_list==next)
            {
                free_list=NULL;
            }
            else{
                Block *temp=free_list;
                while(temp->next!=NULL){
                    if((temp->next==next)){
                        Block *tNext=((Block *)temp->next)->next;
                        Block *btag=(void *)temp->next+((Block *)temp->next)->info.size+sizeof(Block);
                        if(tNext!=NULL){
                            Block *nBtag=(void *)tNext+tNext->info.size+sizeof(Block);
                            nBtag->next=temp;
                            temp->next=tNext;
                        }
                        else{
                            temp->next=NULL;
                        }
                        break;
                    }
                    temp=temp->next;
                }
            }
            if(free_list==NULL){
                free_list=b;
                printf("b size %d \n",b->info.size);
                printf("free_list size %d \n",free_list->info.size);
            }
            else{
                Block *temp=free_list;
                while(temp->next!=NULL){
                    temp=temp->next;
                }
                temp->next=b;
            }
        }
    }
     }
 	/*Eger sonrasındaki alanda free blok yoksa free_liste eklenir.*/
        Block *temp=free_list;
        while(temp->next!=NULL)
        {
            if(temp==b)
            {
                printf("zaten free_liste eklenmis durumda");
                break;
            }
            temp=temp->next;
        }
        if(temp!=b){
            temp->next=b;
        }
    return (void *)b;
}

/*Test Fonksiyonu. Stratejileri test eden fonksiyon */
int test(int fitStrategy)
{
    strategy=fitStrategy;
    void *a=mymalloc(100);
    void *b=mymalloc(16);
    myfree(a);
    void *c=mymalloc(25);
    void *d=mymalloc(150);
    myfree(d);

    while(free_list!=NULL){
        printf("free_list size %d \n",free_list->info.size);
        free_list=free_list->next;
    }
}

int main()
{
    test(0);
    test(1);
    test(2);
    return 0;
}

