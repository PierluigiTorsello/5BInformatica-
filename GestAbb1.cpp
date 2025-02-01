/*GestAbb.CPP gestione Abbonati con uso di File ad Accesso Diretto */

//inclusione delle librerie che servono per l'esecuzione del progromma
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <conio.h>
#include <windows.h>
#include "miefunzioni.cpp"
using namespace std;
pthread_mutex_t mutex; //dichiarazione di un mutex

struct record      //dichiarazione del tracciato record
{ char  tiporec;
  int   Matricola;
  char  CogNome[35];
  char  Sesso;
  char  DataNasc[11];
  char  ComuneRes[25];
  char  Classif;
}abbonato;   //abbonato è una variabile strutturata  "Tracciato Record"

void CreaFile();    // creazione del file
void ApriFile();    // Apertura file
void Maschera();    // Maschera di input
void InsNom();      // aggiunta di un nominati
void inserimento(); // inserimento nuvi record
void Ricerca();     // Menu per la Ricrca 
void lettura();     // lettura da tastiera
void RicPosiz();    // lettura per PosRecione

int  ultmat=0,totrec=0,errore,PosRec,scelta;
char risp,Str[35];
char no_file[20]=" ";              //nome dei file suk Disco
FILE *archivio;                    //puntatore del file 
main()
{  impVideo();
   do 
   { errore=0;
     gotoxy(1,15);cout<<"Nome del File da utilizzare: ";
     cin>>no_file;
     if (strlen(no_file)<2) 
     { gotoxy(40,23);cout<<"Nome non accettabile!.. ";
       errore=1;
     }
   } while (errore|=0);      //nome del file errato
   gotoxy(40,23);cout<<"                        ";
   if (access(no_file,00)  )     //verifica l'esistenza del file
   { gotoxy(1,16);cout<<"Il file non esiste! Vuoi crearlo(S/N)?";
     cin>>risp;        
     if (upchar(risp)=='S')   CreaFile();
     else exit(1)  ;   //se non vuole creare il file, viene interrotta l'esexuazione
   }
     do{  ApriFile();
        gotoxy(21,2);cout<<"TotRec: "<<totrec<<"   ultMat:"<<ultmat;
    	CLEAR(0,6,79,20);
		cornice(20,6,53,16);
        gotoxy(24,7);cout<<"*** GESTIONE ABBONATI ***";
        gotoxy(22, 9);cout<<"1. Inserimento nuovo Record";
        gotoxy(22,10);cout<<"2. Ricerca Record Archiviato";
        gotoxy(22,11);cout<<"3. Cancellazione Fisica    ";
        gotoxy(22,12);cout<<"4. Stampa Riepilogativa    ";
        gotoxy(22,13);cout<<"5. Stampa Statistica       ";
        gotoxy(22,15);cout<<"7. esci";
        do
        { gotoxy(24,17);cout<<"Opzione scelta:     ";
          gotoxy(39,17);cin>>scelta;      //inserimento della scelta
          if((scelta<0)||(scelta>9))          //controllo se òa scelta e' inclusa nell'intervallo
          {  gotoxy(40,23);cout<<"errore";}
        }while ((scelta<0)||(scelta>9));
        gotoxy(40,23);cout<<"          ";
        if     (scelta==1)      InsNom();           // invece della struttura else if, si èio'usare lo swicth
        else if(scelta==2)      Ricerca();
//   else if(scelta==3)  cancfis();
//   else if(scelta==4)  StaRiep();
//   else if(scelta==5)  StaStat();
     fclose(archivio);  //chiusura file 
   }while(scelta!=7);
  return(0);
}

// **********************************  MENU DELLE POSSIBILI FORME DI RICERCA
void Ricerca()
 { 	CLEAR(30, 9,79,20);
	cornice(30, 9,63,17);
    gotoxy(34,10);cout<<"*** POSSIBILI RICERCHE ***";
    gotoxy(33,12);cout<<"1. Per Posizione   ";
    gotoxy(33,13);cout<<"2. Per Natricola   ";
    gotoxy(33,14);cout<<"3. Per Cognome/Nome";
    do
    { gotoxy(37,16);cout<<"Opzione scelta:     ";
      gotoxy(53,16);cin>>scelta;           //inserimento della scelta
      if((scelta<0)||(scelta>3))           //controllo che scelta sia inclusa nell'intervallo
      {  gotoxy(40,23);cout<<"errore";}
    }while ((scelta<0)||(scelta>3));
    gotoxy(40,23);cout<<"          ";
    if     (scelta==1)      RicPosiz();          
//    else if(scelta==2)      RicMatr();
 }
   

// **********************************  APERTURA DEL FILE
void ApriFile()
{   archivio = fopen(no_file, "r+");           //apertura del file
   fseek(archivio,0,2);                       //Posizionamento alla fine del file
   totrec=ftell(archivio)/sizeof(abbonato);   //calcolo del numero di record presenti nel file 
   if(totrec)                                 // Se ci sono record nel File
   { fseek(archivio,(sizeof(abbonato)*(totrec-1)),0);   // Ci Posizioniam all'ultimo record
     fread(&abbonato,sizeof(abbonato),1,archivio);      // per leggere ed acquisire l'ultima matricola 
     ultmat=abbonato.Matricola;                         
   }
}

// **********************************  INSERIMENTO NUOVO NOMINATIVO
void InsNom()
{ Maschera();
  inserimento();
  abbonato.tiporec='A';         // Imposto lo stato del come record attivo 
  abbonato.Matricola=ultmat;    // Imposto la Matricola come ultima Matricola
//  gotoxy(1,23);cout<<"ultima matricola"<<ultmat<<" ultimo Record"<<totrec;
  getch();
  fseek(archivio,(sizeof(abbonato)*totrec),0);    // Posizionamento alla fine File 
  fwrite(&abbonato,sizeof(abbonato),1,archivio);  // Scrittura del buovo record nel file
  totrec++;
  
}

// **********************************  LETTURA DEI DATI DI UN NOMINATIVO
void RicPosiz()              //lettura di un record in base alla posizione
{ CLEAR(30,11,69,20);
  cornice(30,11,69,18);
  do
  { gotoxy(32,12);cout<<"Pos.Record : ";
    gotoxy(44,12);cin>>PosRec;                 // Posizione del Record nel File
    if((PosRec<0)||(PosRec>totrec))            // controllo se la Posizione e' interna al File
    {  gotoxy(40,23);cout<<"errore";}
  }while (( PosRec<0)||(PosRec>totrec));
//  while (pthread_mutex_lock(&mutex)==0); //inizio sequenza critica 
  fseek(archivio,(sizeof(abbonato)*(PosRec-1)),0); // Posizionamento nel File
  fread(&abbonato,sizeof(abbonato),1,archivio);    // lettura del record dal file
//pthread_mutex_unlock(&mutex); //fine sequenza critica 
  if(abbonato.tiporec=='A')                        // Controllo se lo stato del record è attivo
  { Maschera();
    gotoxy(48,12);cout<<abbonato.Matricola;
    gotoxy(48,13);cout<<abbonato.CogNome;
    gotoxy(48,14);cout<<abbonato.Sesso;
    gotoxy(48,15);cout<<abbonato.DataNasc;
    gotoxy(48,16);cout<<abbonato.ComuneRes;
    gotoxy(48,17);cout<<abbonato.Classif;
    do
    { gotoxy(40,19);cout<<"Lett./Modif./Canc.:     ";
      gotoxy(60,19);cin>>risp;                         // Inserimento scelta
      if((upchar(risp)!='L')&& (upchar(risp)!='M') && (upchar(risp)!='C'))    //controllo se la scelta e' accettabile
      {  gotoxy(40,23);cout<<"errore";}
   }while ((upchar(risp)!='L')&& (upchar(risp)!='M') && (upchar(risp)!='C'));
   gotoxy(40,23);cout<<"          ";
   if ((upchar(risp)=='M') || (upchar(risp)=='C'))     // se richiesta Modifica  o Cancellazione
   {  if(upchar(risp)=='M')                            // se richiesta Modifica 
      {  lettura();
      } 
      else
      {  abbonato.tiporec='C';         //imposto lo stato del record Cancellato
      } 
     fseek(archivio,(sizeof(abbonato)*(PosRec-1)),0);   // Posizionamento nel File
     fwrite(&abbonato,sizeof(abbonato),1,archivio);     // Scrittura/Sovrapposizione del record nel file
   } 
  } 
  else
  {  gotoxy(35,14);cout<<"Record Cancellato ! ...";
     getch();
  }
}
 

// **********************************  CREAZIONE DEL FILE
void CreaFile()               
{archivio=fopen(no_file,"w"); // Apertura in creazione del File
 _fcloseall();                // Chiusura del File
}
// **********************************  Maschera di visualizzazione
void Maschera()
{ CLEAR(30,11,59,20);
  cornice(30,11,69,18);
  gotoxy(32,12);cout<<"Matricola    : ";
  gotoxy(32,13);cout<<"Cognome/Nome :            ";
  gotoxy(32,14);cout<<"Sesso        :            ";
  gotoxy(32,15);cout<<"Data Nascita :            ";
  gotoxy(32,16);cout<<"Comune Resid.:            ";
  gotoxy(32,17);cout<<"Classificaz. :            ";
}

// **********************************  Lettura dei dati da Tastiera
void inserimento()
{ ultmat++;
  gotoxy(48,12);cout<<ultmat<<" (Nuovo Record)";
  lettura(); //inserimento dei dati
}
// **********************************  Lettura dei dati da Tastiera
void lettura()
{   
    fflush(stdin);
    gotoxy(48,13);gets(Str);
    if (strlen(Str)!=0)  strcpy(abbonato.CogNome,Str);  // se si batte direttamente IBVIO, vale come conferma dati presenti 
    fflush(stdin);
    gotoxy(48,14);gets(Str);
    if (strlen(Str)!=0)  abbonato.Sesso=Str[0];          // se si batte direttamente IBVIO, vale come conferma dati presenti 
    fflush(stdin);
    gotoxy(48,15);gets(Str);
    if (strlen(Str)!=0)  strcpy(abbonato.DataNasc,Str);  // se si batte direttamente IBVIO, vale come conferma dati presenti 
    fflush(stdin);
    gotoxy(48,16);gets(Str);
    if (strlen(Str)!=0)  strcpy(abbonato.ComuneRes,Str);  // se si batte direttamente IBVIO, vale come conferma dati presenti 
    fflush(stdin);
    gotoxy(48,17);gets(Str);
    if (strlen(Str)!=0)  abbonato.Classif=Str[0];         // se si batte direttamente IBVIO, vale come conferma dati presenti 
    
}

