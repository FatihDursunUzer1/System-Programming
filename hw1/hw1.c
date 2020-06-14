/*Fatih Dursun Üzer  18120205041
15 February 2020 / 15 Şubat 2020
İki tane 10 elemanlı a ve b arraylerı için kullanıcıdan dataları okuyup, bu iki arrayi karşılaştırarak, a'nın kaç elemanın b'nin elemanlarından büyük olduğunu bulan C programı. Macro kullanımı yapılmıştır
*/

#include <stdio.h>
#define compare(x,y) x>y? 1:0 

int hesapla(int a[],int b[],int sizea,int sizeb)
{
/* a dizisinin i. indexi b dizisinin tüm elemanlarından büyük olup olmadığını tutan dizi. Eğer büyükse i.index 1 değilse i.index 0 değerini alır*/
  int c[sizea]; 
 for(int i=0;i<sizea;++i)
 {
    c[i]=1; // a dizisinin her bir elemanının b dizisinin tüm elemanlarından büyük olduğunu varsayıldı.
 }

 for(int i=0;i<sizea;++i)
 {
  for(int j=0;j<sizeb;++j)
  {
   if(!(compare(a[i],b[j]))) //a dizisinin i.indexi b dizisinin tüm elemanlarından büyük değilse c dizisinde i.indexi sıfırla
   {
    c[i]=0; 
    break;
   }
  }
 }
 int toplam=0;
 for(int i=0;i<sizea;++i) 
  {
    toplam+=c[i];
  }
 return toplam;
}

int main()
{
int a[10];
int b[10];

for(int i=0;i<10;++i)
{
 scanf("%d",&a[i]);
}

for(int i=0;i<10;++i)
{
scanf("%d",&b[i]);
}

printf("%d",hesapla(a,b,10,10));


}
