#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;
int Pravo;
int Zastavica[2];

void ulaz_kriticni_odsjecak(int i, int j){
 Zastavica[i]=1;
 while(Zastavica[j]!=0){
  if(Pravo==j){
   Zastavica[i]=0;
   while(Pravo==j);
   Zastavica[i]=1;
  }
 }
}

void izadji_kriticni(int i, int j){
 Pravo=j;
 Zastavica[i]=0;
}

void *Proces(void *arg){
 int i= *((int *)arg);
 int j=i+1;
 if(j>1)j=0;
 for(int x=1;x<=5;x++){
  ulaz_kriticni_odsjecak(i,j);
  for(int k=1;k<=5;k++){
   cout<<"Dretva: "<<i+1<<", "<<"K.O. br: "<<x<<" ("<<k<<"/5)"<<endl;
   sleep(1);
  }
  izadji_kriticni(i,j);
 }
}

int main(){
 Pravo=0;
 int id[2];
 pthread_t thread_id[2];

 for(int zz=0;zz<2;zz++){
  Zastavica[zz]=0;
  id[zz]=zz;
 }

 if(pthread_create(&thread_id[0],NULL,Proces,&id[0])){
  cout<<"Greska pri stvaranju dretve!"<<endl;exit(1);
 }

  if(pthread_create(&thread_id[1],NULL,Proces,&id[1])){
  cout<<"Greska pri stvaranju dretve!"<<endl;exit(1);
 }

 for(int tr=0;tr<2;tr++){
  pthread_join(thread_id[tr],NULL);
 }
 return 0;
}
