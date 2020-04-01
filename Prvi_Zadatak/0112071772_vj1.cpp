#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<ctime>
using namespace std;

int trenutni_index=0;

void prekidna_rutina(int broj){
 time_t curtime;
 time(&curtime);
 cout<<"Prekidna rutina pozvana u: "<<ctime(&curtime);
 cout<<"Razina prekida: "<<broj<<endl;
 cout<<"Pocela obrada prekida razine: "<<broj<<endl;
 for(int i=1;i<=5;i++){
  cout<<"Obrada prekida razine:"<<broj<<" "<<i<<"/5"<<endl;
  sleep(1);
  }
 cout<<"Zavrsila obrada prekida razine: "<<broj<<endl;
}

void ctrlc (int sig){
 int index=1;
 if(trenutni_index>=index)return;
 trenutni_index=index;
 prekidna_rutina(index);
}

void ctrll (int sig){
 int index=2;
 if(trenutni_index>=index)return;
 trenutni_index=index;
 prekidna_rutina(index);
}

void ctrlk (int sig){
 int index=3;
 if(trenutni_index>=index)return;
 trenutni_index=index;
 prekidna_rutina(index);
}

int main(){
 int brojac=1;
 sigset(SIGINT, ctrlc);
 sigset(SIGQUIT, ctrll);
 sigset(SIGTSTP, ctrlk);
 cout<<"Osnovni program"<<endl;
 while(brojac<=20){
  if(trenutni_index!=0)trenutni_index=0;
  cout<<"Glavni program "<<brojac<<"/20"<<endl;
  sleep(1);
  brojac++;
 }
 cout<<"Zavrsio glavni program"<<endl;
 return 0;
}
