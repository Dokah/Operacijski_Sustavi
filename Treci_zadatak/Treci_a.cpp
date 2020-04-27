#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
using namespace std;
int dijeljeni_broj=0;
int brojac=0;
void generiraj(int n){
 while(brojac<n){
  int k=rand();
  //spremi ga u dijeljenu varijablu
  //javiti procesima da je stavljeni novi rand broj || koristiti semafor
  //čekaj da neki proces pročita slučajni broj || koristiti semafor
 }
 brojac++;
}

void racunaj(){
 int broj=0;
 int zbroj=0;
 while(dijeljeni_broj){ //dok ima zadataka
  //čekaj da proces generiraj stavi sljedeći broj; || rabiti semafor
  //broj=dijeljena varijabla
  //javi procesu generiraj da je dijeljena varijabla pročitana || rabiti semafor
  //for (int j=0;j<generirani_broj_iz varijable;j++)zbroj=zbroj+j;
  cout<<"Zbroj je: "<<zbroj<<endl;
 }
}

int main(){
 int n,m;
 generiraj(n);
 for(int i=0;i<m;i++){
  racunaj(); 
 }
 //cekaj na zavrsetak svih procesa
 return 0;
}
