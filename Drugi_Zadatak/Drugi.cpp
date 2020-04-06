#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<time.h>
#include<cmath>
using namespace std;
int Id;
int *ZajednickiProstor=new int [1];
int pocetni=0;

void brisi(int sig){
 shmdt (ZajednickiProstor);
 shmctl(Id, IPC_RMID, NULL);
 exit(0);
}

void suma(int pocetni, int zavrsni){
 int sooma=0;
 for(int i=pocetni;i<=zavrsni;i++){
  sooma+=ZajednickiProstor[i];
 }
 cout<<sooma<<endl;
}

int main(int argc, char** argv){
 srand(time(NULL));
 double bp=((double)atoi(argv[1])/(double)atoi(argv[2]));
 int Broj_Procesa=ceil(bp);
 if(atoi(argv[1])>=10000){
  cout<<"Polje ne moze biti vece od 10000"<<endl;
  return 0;
 }

 Id=shmget( IPC_PRIVATE, sizeof(ZajednickiProstor)+atoi(argv[2]), 0600);
 if(Id == -1)exit(1);

 ZajednickiProstor = (int *)shmat(Id, NULL,0);
 for(int i=0;i<atoi(argv[1]);i++){
  ZajednickiProstor[i] = rand()%100;
  cout<<i<<". "<<ZajednickiProstor[i]<<endl;
 }
 cout<<"Broj procesa: "<<Broj_Procesa<<endl;
 sigset(SIGINT,brisi);

 for(int i=0;i<Broj_Procesa;i++){
  int zavrsni=pocetni+atoi(argv[2])-1;
  if (fork() == 0){
   if(zavrsni >= atoi(argv[1])){
    cout<<"Zbroj ("<<pocetni<<"-"<<atoi(argv[1])-1<<")"<<"= ";suma(pocetni,zavrsni);
   }
  else{
   cout<<"Zbroj ("<<pocetni<<"-"<<zavrsni<<")"<<"= ";suma(pocetni,zavrsni);
   }
   exit(0);
  }
 wait(NULL);
 pocetni=zavrsni+1;
 }
 brisi(0);
 return 0;
}
