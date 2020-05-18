#include<iostream>
#include<ctime>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<semaphore.h>
using namespace std;

int Id;
semt Sem1;
semt Sem2;
int *ZajednickiProstor=new int [1];
bool dijeljeni_broj=true;
int brojac=0;

void brisi(int sig){
 cout<<"Brisem"<<endl;
 for(int z=1;z<100;z++){
 wait(NULL);
 }
 sem_destroy(1);
 sem_destroy(2);
 shmdt(ZajednickiProstor);
 shmctl(Id, IPC_RMID, NULL);
 exit(0);
}

void generiraj(int n){
 srand(time(NULL));
 while(brojac<n){
  sem_wait(2);//Ispitaj semafor 2
  ZajednickiProstor[brojac]=rand();
  sem_post(1);//postavi semafor 1
  brojac++;
 }
 dijeljeni_broj=false;
}

void racunaj(){
 int broj=0;
 int zbroj=0;
 while(dijeljeni_broj){
  sem_wait(1);//Ispitaj semafor 1
  broj=ZajednickiProstor[dijeljeni_broj];
  for (int j=0;j<broj;j++)zbroj=zbroj+j;
  cout<<"Zbroj je: "<<zbroj<<endl;
  sem_post(2);//Postavi semafor 2
 }
}

int main(int argc, char** argv){
 sigset(SIGINT,brisi);
 int n,m;
 m=atoi(argv[1]);
 n=atoi(argv[2]);

 if(atoi(argv[2]) > 1000000000){
  cout<<"Preveliki broj!"<<endl;
  exit(0);
 }

 sem_init(Sem1,1,0);
 sem_init(Sem2,1,0);

 Id=shmget(IPC_PRIVATE, sizeof(ZajednickiProstor)*atoi(argv[2]), 0600);
 if(Id == -1)exit(1);

 ZajednickiProstor = (int *) shmat(Id,NULL,0);
 if(fork()==0){
  generiraj(n);
  exit(0);
 }

 for(int i=0;i<m;i++){
   if(fork()==0){
    racunaj();
    exit(0);
   }
 }
 wait(NULL);

 sem_destroy(1);
 sem_destroy(2);

 return 0;
}
