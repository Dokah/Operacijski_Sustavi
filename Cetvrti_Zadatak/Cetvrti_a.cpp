#include<iostream>
#include<cstdlib>
#include<pthread.h>
using namespace std;

int N;
int *ZajednickiProstor=new int [N];

void *Upisi(void *arg){
 int broj;
 int index=*((int *)arg);
 cout<<"Dretva "<<index<<". "<<"unesite broj: "<<endl;
 cin>>broj;
 cout<<"Dretva "<<index<<". "<<"uneseni broj je: "<<broj<<endl;
 pthread_exit(NULL);
}

int main(int argc,char** argv){
 N=atoi(argv[1]);
 pthread_t thr_id[N];

 for(int i=0;i<N;i++){
  if (pthread_create(&thr_id[i], NULL, Upisi, &i) != 0) {
   printf("Greska pri stvaranju dretve!\n");
   exit(1);
  }
 }

 for(int i=0;i<N;i++)pthread_join(thr_id[i],NULL);

 return 0;
}
