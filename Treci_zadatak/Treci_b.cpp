#include<iostream>
#include<ctime>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<ctime>
using namespace std;

int Id,SemId;

struct Zajednicko{
 int ULAZ;
 int IZLAZ;
 int UKUPNO;
 int M[5];
 int PISI;
 int PUN;
 int PRAZAN;
};

Zajednicko *ZajednickiProstor=new Zajednicko;

void SemGet(int n) {
   SemId = semget(IPC_PRIVATE, n, 0600);
   if (SemId == -1) {
      printf("Nema semafora!\n");
      exit(1);
   }
}

int SemSetVal(int SemNum, int SemVal) {
   return semctl(SemId, SemNum, SETVAL, &SemVal);
}

int SemOp(int SemNum, int SemOp) {
   struct sembuf SemBuf;

   SemBuf.sem_num = SemNum;
   SemBuf.sem_op  = SemOp;
   SemBuf.sem_flg = 0;
   return semop(SemId, & SemBuf, 1);
}

void SemRemove() {
   semctl(SemId, 0, IPC_RMID, 0);
}

void brisi(){
 cout<<"Brisem"<<endl;
 for(int z=1;z<100;z++){
 wait(NULL);
 }
 shmdt(ZajednickiProstor);
 shmctl(Id, IPC_RMID, NULL);
 SemRemove();
 exit(0);
}

void brisi(int sig){
 cout<<"Brisem"<<endl;
 for(int z=1;z<100;z++){
 wait(NULL);
 }
 shmdt(ZajednickiProstor);
 shmctl(Id, IPC_RMID, NULL);
 SemRemove();
 exit(0);
}

void proizvodac(int n,int m){
 int i=0;
 srand(time(NULL));
 for(i=0;i<n;i++){
  SemOp(ZajednickiProstor->PUN,-1);//ispitaj semafor PUN
  cout<<"Proso"<<endl;
  SemOp(ZajednickiProstor->PISI,-1);//ispitaj semafor PISI
  cout<<"Poslije semafori"<<endl;
  ZajednickiProstor->M[ZajednickiProstor->ULAZ] = rand()%1000;
  cout<<"Proizvodac "<<m<<" salje broj: "<<ZajednickiProstor->M[ZajednickiProstor->ULAZ]<<endl;
  ZajednickiProstor->ULAZ = (ZajednickiProstor->ULAZ + 1) % 5;
  SemOp(ZajednickiProstor->PISI,1);//postavi semafor PISI
  SemOp(ZajednickiProstor->PRAZAN,1);//postavi semafor PRAZAN
 }
}


void potrosac(int n){
 int i=0;
 int zbroj=0;
 do{
  SemOp(ZajednickiProstor->PRAZAN,-1);//Ispitaj semafor prazan
  cout<<"Potrosac prima  "<<ZajednickiProstor->M[ZajednickiProstor->IZLAZ]<<endl;
  zbroj=zbroj+ZajednickiProstor->M[ZajednickiProstor->IZLAZ];
  ZajednickiProstor->IZLAZ=(ZajednickiProstor->IZLAZ+1) % 5;
  SemOp(ZajednickiProstor->PUN,1);//postavi semafor PUN
  i=i+1;
 }while(i<ZajednickiProstor->UKUPNO);
 cout<<"Zbroj: "<<zbroj<<endl;
}

int main(int argc, char** argv){
sigset(SIGINT,brisi);

int n,m;

m=atoi(argv[1]);
n=atoi(argv[2]);

Id=shmget(IPC_PRIVATE,sizeof(ZajednickiProstor),0600);
if(Id == -1)exit(1);

ZajednickiProstor = (Zajednicko *) shmat(Id,NULL,0);

ZajednickiProstor->PUN=1;
ZajednickiProstor->PISI=2;
ZajednickiProstor->PRAZAN=0;
ZajednickiProstor->IZLAZ=0;
ZajednickiProstor->ULAZ=0;
ZajednickiProstor->UKUPNO=m*n;

SemGet(3);
SemSetVal(ZajednickiProstor->PUN,5);
SemSetVal(ZajednickiProstor->PISI,1);
SemSetVal(ZajednickiProstor->PRAZAN,0);

for(int i=0;i<m;i++){
 if(fork()==0){
  proizvodac(n,m);
  exit(0);
 }
}

if(fork()==0){
 potrosac(n);
 exit(0);
}

for(int i=0;i<=m;i++)wait(NULL);

brisi();
return 0;
}
