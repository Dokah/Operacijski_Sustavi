#include<iostream>
#include<cstdlib>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<sys/types.h>
using namespace std;

pthread_mutex_t m;
pthread_cond_t red;

int N,br;
pthread_t thr_id[15];

void brisi(){
 cout<<"Brisem"<<endl;
 pthread_mutex_destroy(&m);
 pthread_cond_destroy(&red);
}

void brisi(int sig){
 cout<<"Brisem"<<endl;
 for(int i=0;i<N;i++)pthread_join(thr_id[i],NULL);
 pthread_mutex_destroy(&m);
 pthread_cond_destroy(&red);
}

void *barijera(void *arg){
 int broj;
 int index=*((int *)arg);
 pthread_mutex_lock(&m);
 cout<<"Dretva "<<index<<". "<<"unesite broj: "<<endl;
 cin>>broj;
 br++;
 if(br<N){
  pthread_cond_wait(&red, &m);
 }
 else{
  br=0;
  pthread_cond_broadcast(&red);
 }
 cout<<"Dretva "<<index<<". "<<"uneseni broj je: "<<broj<<endl;
 usleep(500000);
 pthread_mutex_unlock(&m);
 pthread_exit(NULL);
}

int main(int argc,char** argv){
 sigset(SIGINT,brisi);
 N=atoi(argv[1]);

 pthread_mutex_init(&m, NULL);
 pthread_cond_init(&red, NULL);

 for(int i=0;i<N;i++){
  if (pthread_create(&thr_id[i], NULL, barijera, &i) != 0) {
   printf("Greska pri stvaranju dretve!\n");
   exit(1);
  }
 }

 for(int i=0;i<N;i++)pthread_join(thr_id[i],NULL);
 brisi();

 return 0;
}
