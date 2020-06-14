#include<iostream>
#include<cstdlib>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<sys/types.h>
using namespace std;

int br[2];
int vrsta;
int ceka[2];
int siti;

pthread_mutex_t m;
pthread_cond_t red, menza;

void udji(int vrsta){
 pthread_mutex_lock(&m);
 ceka[vrsta]++;
 while(br[1-vrsta]>0 || ((siti>=N)&&(ceka[1-vrsta]>0))){
  pthread_cond_wait(&red, &m);
 }
 br[vrsta]++;
 ceka[vrsta]--;
 if(ceka[1-vrsta]>0) siti++;
 pthread_mutex_unlock(&m);
}

void izadji(int vrsta){
 pthread_mutex_lock(&m);
 br[vrsta]--;
 if(br[vrsta] == 0){
  siti=0;
  pthread_cond_broadcast(&red);
 }
 pthread_mutex_unlock(&m);
}

void programer(int vrsta){
 usleep(rand()%100);
 udji(vrsta);
 usleep(rand()%100);
 izadji(vrsta);
}

int main(int argc,char** argv){
 sigset(SIGINT,brisi);
 N=atoi(argv[1]); //najvise N programera moze uci u restac
 M=atoi(argv[2]); //Broj programera svake vrste
 //svaki proces predastavlja jednog programera

 pthread_mutex_init(&m, NULL);
 pthread_cond_init(&red, NULL);
 pthread_cond_init(&menza, NULL);

 for(int i=1;i<=M*2;i++){
  //stvori programera odnosno procesa
  if(fork()==0){
   programer(i%2);
   exit(0);
  }
 }

 brisi();

 return 0;
}
