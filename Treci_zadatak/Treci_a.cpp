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

sem_t* Sem1;
sem_t* Sem2;

int Id,Id2,Id3;
int *ZajednickiProstor=new int [3];
sem_t* Semafor1;
sem_t* Semafor2;

void brisi(int sig){
 cout<<"Brisem"<<endl;
 for(int z=1;z<100;z++){
 wait(NULL);
 }
 shmdt(ZajednickiProstor);
 shmctl(Id, IPC_RMID, NULL);
 shmdt(Semafor1);
 shmctl(Id2, IPC_RMID, NULL);
 shmdt(Semafor2);
 shmctl(Id3, IPC_RMID, NULL);
 sem_destroy(Sem1);
 sem_destroy(Sem2);
 exit(0);
}

void brisi(){
 cout<<"Brisem"<<endl;
 for(int z=1;z<100;z++){
 wait(NULL);
 }
 shmdt(ZajednickiProstor);
 shmctl(Id, IPC_RMID, NULL);
 shmdt(Semafor1);
 shmctl(Id2, IPC_RMID, NULL);
 shmdt(Semafor2);
 shmctl(Id3, IPC_RMID, NULL);
 sem_destroy(Sem1);
 sem_destroy(Sem2);
 exit(0);
}

void generiraj(int n,int m){
 int brojac=0;
 srand(time(NULL));
 while(brojac<n){
  ZajednickiProstor[0]=rand()%1000;
  cout<<"Generiram broj..."<<ZajednickiProstor[0]<<endl;
  sem_post(Sem1);//postavi semafor 1
  brojac++;
  sem_wait(Sem2);//Ispitaj semafor 2
 }
 ZajednickiProstor[2]=brojac;
 ZajednickiProstor[1]=4;
 for(int i=1;i<m;i++)sem_post(Sem1);
}

void racunaj(int index,int n){
 int broj=0;
 int zbroj=0;
 while(ZajednickiProstor[1]!=4){
  sem_wait(Sem1);//Ispitaj semafor 1
  if(ZajednickiProstor[2]==n)exit(0);
  cout<<"Proces "<<index<<" Zapoceo s radom."<<endl;
  broj=ZajednickiProstor[0];
  cout<<"Proces "<<index<<" Preuzeo zadatak "<<broj<<endl;
  sem_post(Sem2);//Postavi semafor 2
  for (int j=0;j<broj;j++)zbroj=zbroj+j;
  cout<<"Proces "<<index<<" Zadatak: "<<broj<<" Zbroj je: "<<zbroj<<endl;
  zbroj=0;
 }
}

int main(int argc, char** argv){
 sigset(SIGINT,brisi);
 int n,m;
 m=atoi(argv[1]);
 n=atoi(argv[2]);

 /*if(atoi(argv[2]) > 1000000000){
  cout<<"Preveliki broj!"<<endl;
  exit(0);
 }*/

 Id=shmget(IPC_PRIVATE, sizeof(ZajednickiProstor)*atoi(argv[2]), 0600);
 if(Id == -1)exit(1);
 Id2=shmget(IPC_PRIVATE,sizeof(Semafor1),0600);
 if(Id2 == -1)exit(1);
 Id3=shmget(IPC_PRIVATE,sizeof(Semafor2),0600);
 if(Id3 == -1)exit(1);

 ZajednickiProstor = (int *) shmat(Id,NULL,0);
 Semafor1 = (sem_t *) shmat(Id2,NULL,0);
 Semafor2 = (sem_t *) shmat(Id3,NULL,0); 

 Sem1=Semafor1;
 Sem2=Semafor2;
 sem_init(Sem1,1,0);
 sem_init(Sem2,1,0);

 ZajednickiProstor[1]=0;
 ZajednickiProstor[2]=0;

 if(fork()==0){
  generiraj(n+1,m);
  exit(0);
 }

 for(int i=0;i<m;i++){
  if(fork()==0){
   racunaj(i+1,n+1);
   exit(0);
  }
 }

for(int i=0;i<=m;i++) wait(NULL);

 brisi();
 /*sem_destroy(Sem1);
 sem_destroy(Sem2);

 shmdt(ZajednickiProstor);
 shmctl(Id, IPC_RMID, NULL);
 shmdt(Semafor1);
 shmctl(Id2, IPC_RMID, NULL);
 shmdt(Semafor2);
 shmctl(Id3, IPC_RMID, NULL);

 cout<<"Obrisao sve."<<endl;*/
 return 0;
}
