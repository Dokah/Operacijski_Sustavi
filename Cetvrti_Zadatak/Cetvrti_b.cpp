#include<iostream>
#include<cstdlib>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;

struct s{
 int N;
 int br[2];
 int ceka[2];
 int siti;
 pthread_mutex_t m;
 pthread_cond_t uv[2];
};

int Id;
s *ZajednickiProstor=new s;

pthread_mutexattr_t monitor;
pthread_condattr_t uvjet[2];

void brisi(int sig){
 cout<<"Brisem..."<<endl;
 for(int z=1;z<100;z++)wait(NULL);
 shmdt(ZajednickiProstor);
 shmctl(Id, IPC_RMID, NULL);
 pthread_mutexattr_destroy(& monitor);
 pthread_condattr_destroy(&uvjet[0]);
 pthread_condattr_destroy(&uvjet[1]);
 exit(0);
}

void brisi(){
 cout<<"Brisem..."<<endl;
 for(int z=1;z<100;z++)wait(NULL);
 shmdt(ZajednickiProstor);
 shmctl(Id, IPC_RMID, NULL);
 pthread_mutexattr_destroy(& monitor);
 pthread_condattr_destroy(&uvjet[0]);
 pthread_condattr_destroy(&uvjet[1]);
 exit(0);
}

void udji(int vrstai){
 pthread_mutex_lock(&ZajednickiProstor->m);
 ZajednickiProstor->ceka[vrstai]++;
 //cout<<"U red cekanja"<<endl;//ispisi red cekanja i red u restoranu ((u red cekanja))
 while(ZajednickiProstor->br[1-vrstai]>0 || ((ZajednickiProstor->siti>=ZajednickiProstor->N)&&(ZajednickiProstor->ceka[1-vrstai]>0))){
  cout<<"U red cekanja"<<endl;
  pthread_cond_wait(&ZajednickiProstor->uv[vrstai], &ZajednickiProstor->m);
 }
 ZajednickiProstor->br[vrstai]++;
 ZajednickiProstor->ceka[vrstai]--;
 if(ZajednickiProstor->ceka[1-vrstai]>0) ZajednickiProstor->siti++;
 cout<<"Siti: "<<ZajednickiProstor->siti<<endl;
 cout<<"U restoran"<<endl;//ispisi red cekanja i red u restoranu ((u restoran))
 pthread_mutex_unlock(&ZajednickiProstor->m);
}

void izadji(int vrstao){
 pthread_mutex_lock(&ZajednickiProstor->m);
 ZajednickiProstor->br[vrstao]--;
 if(ZajednickiProstor->br[vrstao] == 0){
  ZajednickiProstor->siti=0;
  pthread_cond_broadcast(&ZajednickiProstor->uv[1-vrstao]);
 }
 cout<<"Iz restorana"<<endl;//ipisi red cekana i red iz restorana ((iz restorana))
 pthread_mutex_unlock(&ZajednickiProstor->m);
}

void programer(int vrstap){
 usleep(rand()%100);
 udji(vrstap);
 usleep(rand()%100);
 izadji(vrstap);
}

int main(int argc,char** argv){
 sigset(SIGINT,brisi);
 int N=atoi(argv[1]); //najvise N programera moze uci u restac
 int M=atoi(argv[2]); //Broj programera svake vrste

 Id=shmget(IPC_PRIVATE, sizeof(ZajednickiProstor), 0600);
 if(Id == -1)exit(1);

 ZajednickiProstor = (s *) shmat(Id,NULL,0);

 ZajednickiProstor->N=N;

 pthread_mutexattr_init(&monitor);
 pthread_mutexattr_setpshared(&monitor, PTHREAD_PROCESS_SHARED);

 pthread_condattr_init(&uvjet[0]);
 pthread_condattr_init(&uvjet[1]);
 pthread_condattr_setpshared(&uvjet[0], PTHREAD_PROCESS_SHARED);
 pthread_condattr_setpshared(&uvjet[1], PTHREAD_PROCESS_SHARED);

 pthread_mutex_init(&ZajednickiProstor->m,&monitor);
 pthread_cond_init(&ZajednickiProstor->uv[0],&uvjet[0]);
 pthread_cond_init(&ZajednickiProstor->uv[1],&uvjet[1]);

 for(int i=1;i<=M*2;i++){
  if(fork()==0){
   programer(i%2);
   exit(0);
  }
 }

 for(int i=0;i<=M*2;i++)wait(NULL);

 brisi();

 return 0;
}
