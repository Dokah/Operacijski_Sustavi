#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;
int Pravo;
int Zastavica[2];
int i,j;

void kriticni_odsjecak(){
 Zastavica[i]=1;
 while(Zastavica[j]!=0){
  if(Pravo==j){
   Zastavica[i]=0;
   while(Pravo==j);
   Zastavica[i]=1;
  }
 }
}

void izadji_kriticni(){
 Pravo=j;
 Zastavica[i]=0;
}

void *Proces(void *arg){
 for(int x=1;x<=5;x++){
  kriticni_odsjecak();
  for(int k=1;k<=5;k++){
   cout<<"Dretva: "<<i<<", "<<"K.O. br: "<<x<<" ("<<k<<"/5)"<<endl;
   sleep(1);
  }
  izadji_kriticni();
 }
}

int main(){
 pthread_t thread_id[2];
 Pravo=0;
 for(i=0;i<2;i++){
  if(pthread_create(&thread_id[i],NULL,Proces,NULL)){
   cout<<"Greska pri stvaranju dretve!"<<endl;exit(1);
  }
 }

 for(int i=0;i<2;i++){
  pthread_join(thread_id[i],NULL);
 }
 return 0;
}
