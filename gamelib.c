#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gamelib.h"

/* ELENCO VARIBILI GLOBALI */
static int n; // numero giocatori inseriti
static struct Giocatore *giocatori; //punta all' array dei giocatori che vogliono giocare
static struct Giocatore *giocatoriscorr;//puntatore che scorre l'array giocatori
static unsigned short quest_da_finire; //numero di quest rimaste
static struct Stanza *stanza_inizio; //Prima stanza della partita
static struct Stanza *lista_stanze; //Lista di tutte le stanze inserite in ordine di creazione
static struct Stanza *ultima_stanza = NULL;//ultima stanza creata
static int alive;//inidica il numero degli astronauti rimasti
static int numero_stanze= 0;
static int nimposstori;
static int fine_gioco = 1;
static int giaimpostato = 0;
static int giocare = 0;

/*ELENCO DICHIARAZIONI FUNZIONI*/
static void menu_impostazioni(void);
static void n_giocatori(void);
static void n_quest(void);
static void stampa_giocatori(void);
static void stampa_stanza(struct Giocatore*);
static int inizia_gioco(void);
static void det_stanza_inizio(struct Stanza*);
static void randomize(int arr[], int n);//funzione che serve a determinare i turni dei giocatori
static void stampanome(struct Giocatore*);
static void menu_turno(struct Giocatore*);//stampa il menu relativo ai turni
static struct Stanza* avanza(struct Stanza*);//funzione che serve a far muovere i giocatori
static void esegui_quest(struct Stanza*);
static void chiamata_emergenza(struct Stanza*);
static void sabotaggio(struct Stanza*);
static void usa_botola(struct Giocatore*);
static void stampa_lista(void);//provvisioria
static void uccidi_astronauta(struct Giocatore*);
static struct Stanza *cancellaStanza(struct Stanza*);
static struct Stanza *nextStanza(struct Stanza*);

/*-----INIZIO FUNZIONI-----*/
void impostazioni(){
  if(giaimpostato){
    termina_gioco();
  }
  int continua_imp = 1;
  int scelta_imp;
  do{
    menu_impostazioni();
    scanf("%d", &scelta_imp);
    switch (scelta_imp) {
      case 1:
              n_giocatori();
              break;
      case 2:
              n_quest();
              break;
      case 3:
              stampa_giocatori();
              break;
      case 4:
              continua_imp = inizia_gioco();
              break;
      default:
              printf("Scelta sbagliata\n");
    }
  }while(continua_imp);
  menu();
}

static int inizia_gioco(){
  if(giocatori != NULL && quest_da_finire != 0){
    giocare = 1;
  }else if(giocatori == NULL || quest_da_finire == 0){
    giocare = 0;
  }
  return 0;
}

static void menu_impostazioni(){
printf("--------------------------------------------------------\n|\t\t   MENU' IMPOSTAZIONI\t\t  |\n--------------------------------------------------------\n");
  printf("\tChe cosa vuoi fare?\n");
  printf("\t\t1. Inserire il numero dei GIOCATORI\n");
  printf("\t\t2. Inserire il numero delle QUEST\n");
  printf("\t\t3. Stampare la LISTA dei  giocatori\n");
  printf("\t\t4. Inizia a Giocare\n");
}

/*----- CREA UN ARRAY CHE CONTIENE I GIOCATORI E LE INFO -----*/
static void n_giocatori(){
  /*----CREAZIONE PRIMA STANZA-----*/
  stanza_inizio = (struct Stanza*) malloc(sizeof(struct Stanza));
  lista_stanze = (struct Stanza*) malloc(sizeof(struct Stanza));
  stanza_inizio->Avanti = NULL;
  stanza_inizio->Destra = NULL;
  stanza_inizio->Sinistra = NULL;
  stanza_inizio->stanza_Precedente = NULL;
  stanza_inizio->tipo_stanza = 0;
  stanza_inizio->emergenza = 1;
  lista_stanze = stanza_inizio;

  /*---- DEFINIZIONE NUMERO GIOCATORI----*/
  while(n < 4 || n > 10){
    printf("Quante persone vogliono giocare?\nTi ricordo che si può giocare da minimo 4 persone ad un massimo di 10\n");
    scanf("%d", &n);
    if(n < 4 || n > 10){
      printf("ATTENZIONE!! il numero di giocatori inserito non è corretto!\n\n");
    }
  }
  giocatori = (struct Giocatore*) calloc(n, sizeof(struct Giocatore));//Creo una zona di memoria dinamica delle dimensione del numero di giocaori * la dimensione della struct Giocatore
  giocatoriscorr = giocatori;

  int nomi[n];

  for(int i = 0; i < n; i ++){
    nomi[i] = i;
  }
  randomize(nomi, n);

  for(int i = 0; i < n; i++){
    switch(nomi[i]){
      case 0:
              giocatoriscorr->nome = (char)rosso;
              break;
      case 1:
              giocatoriscorr->nome = (char)blu;
              break;
      case 2:
              giocatoriscorr->nome = (char)nero;
              break;
      case 3:
              giocatoriscorr->nome = (char)giallo;
              break;
      case 4:
              giocatoriscorr->nome = (char)viola;
              break;
      case 5:
              giocatoriscorr->nome = (char)arancione;
              break;
      case 6:
              giocatoriscorr->nome = (char)verde;
              break;
      case 7:
              giocatoriscorr->nome = (char)celeste;
              break;
      case 8:
              giocatoriscorr->nome = (char)bianco;
              break;
      case 9:
              giocatoriscorr->nome = (char)rosa;
              break;
    }
    giocatoriscorr->posizione_Player = stanza_inizio;
    giocatoriscorr += sizeof(struct Giocatore);
  }
  /*----- DEFINIZIONE IMPOSTORI E ASTRONAUTI-----*/
  /*----- IMPOSOTORI CON MENO DI 7 PARTECIPANTI-----*/
  int probImp = 1+ rand()%100;
  giocatoriscorr = giocatori;
  if(n < 7){
    if(probImp < 70){
      printf("ci sarà UN SOLO impostore\n");
      nimposstori = 1;
      alive = n - 1;
      while (getchar() != '\n');
      int imp = rand()%n;
      for(int i = 0; i < n; i++){
        if(i != imp){
          giocatoriscorr->stato = 0;
          giocatoriscorr += sizeof(struct Giocatore);
        }else{
          giocatoriscorr->stato = 1;
          giocatoriscorr += sizeof(struct Giocatore);
        }
      }
    }else if(probImp < 90 && probImp >=71){
      printf("ci saranno DUE impostori\n");
      nimposstori = 2;
      alive = n - 2;
      while (getchar() != '\n');
      int continua = 1;
      int imp, imp2;
      do{
         imp = rand()%n;
         imp2 = rand()*rand()%n;
        if(imp != imp2){
          if(imp >= 0 && imp2 >= 0){
            continua = 0;
          }
        }
      }while(continua);
      for(int i = 0; i < n; i++){
        if(i != imp && i != imp2){
          giocatoriscorr->stato = 0;
          giocatoriscorr += sizeof(struct Giocatore);
        }else if(i == imp || i == imp2){
          giocatoriscorr->stato = 1;
          giocatoriscorr += sizeof(struct Giocatore);
        }
      }
    }else if(probImp < 100 && probImp >= 91){
      printf("ci saranno TRE impostori\n");
      nimposstori = 3;
      alive = n - 3;
      while (getchar() != '\n');
      int continua = 1;
      int imp, imp2, imp3;
      do{
         imp = rand()%n;
         imp2 = rand()*rand()%n;
         imp3 = rand()*rand()*rand()%n;
        if(imp != imp2 && imp != imp3 && imp2 != imp3){
          if(imp >= 0 && imp2 >= 0 && imp3 >= 0){
            continua = 0;
          }
        }
      }while(continua);
      for(int i = 0; i < n; i++){
        if(i != imp && i != imp2 && i != imp3){
          giocatoriscorr->stato = 0;
          giocatoriscorr += sizeof(struct Giocatore);
        }else if(i == imp || i == imp2 || i == imp3){
          giocatoriscorr->stato = 1;
          giocatoriscorr += sizeof(struct Giocatore);
        }
      }
    }
      while (getchar() != '\n');
  }
  /*----IMPOSTORI CON MENO DI 9 PERTECIPANTI----*/
  else if(n < 9){
    if(probImp < 40){
      printf("ci sarà UN SOLO impostore\n");
      nimposstori = 1;
      alive = n - 1;
      while (getchar() != '\n');
      int imp = rand()%(n-1);
      for(int i = 0; i < n; i++){
        if(i != imp){
          giocatoriscorr->stato = 0;
          giocatoriscorr += sizeof(struct Giocatore);
        }else{
          giocatoriscorr->stato = 1;
          giocatoriscorr += sizeof(struct Giocatore);
        }
      }
    }else if(probImp < 80 && probImp >= 41){
      printf("ci saranno DUE impostori\n");
      nimposstori = 2;
      alive = n - 2;
      while (getchar() != '\n');
      int continua = 1;
      int imp, imp2;
      do{
        imp = rand()%n;
        imp2 = rand()*rand()%(n-1);
        if(imp != imp2){
          if(imp >= 0 && imp2 >= 0){
            continua = 0;
          }
        }
      }while(continua);
      for(int i = 0; i < n; i++){
        if(i != imp && i != imp2){
          giocatoriscorr->stato = 0;
          giocatoriscorr += sizeof(struct Giocatore);
        }else if(i == imp || i == imp2){
          giocatoriscorr->stato = 1;
          giocatoriscorr += sizeof(struct Giocatore);
        }
      }
    }else if(probImp < 100 && probImp >= 81){
      printf("ci saranno TRE impostori\n");
      nimposstori = 3;
      alive = n - 3;
      while (getchar() != '\n');
      int continua = 1;
      int imp, imp2, imp3;
      do{
        imp = rand()%(n-1);
        imp2 = rand()*rand()%(n-1);
        imp3 = rand()*rand()*rand()%(n-1);
        if(imp != imp2 && imp2 != imp3 && imp2 != imp3){
          if(imp >= 0 && imp2 >= 0 && imp3 >= 0){
            continua = 0;
          }
        }
      }while(continua);
      for(int i = 0; i < n; i++){
        if(i != imp && i != imp2 && i != imp3){
          giocatoriscorr->stato = 0;
          giocatoriscorr += sizeof(struct Giocatore);
        }else if(i == imp || i == imp2 || i == imp3){
          giocatoriscorr->stato = 1;
          giocatoriscorr += sizeof(struct Giocatore);
        }
      }
    }
      while (getchar() != '\n');
  }
  /*---- IMPOSTORI CON MENO DI 10 PARTECIPANTI----*/
  else if(n < 11){
    if(probImp < 10){
      printf("ci sarà UN SOLO impostore\n");
      alive = n - 1;
      nimposstori = 1;
      int imp = rand()%(n);
      for(int i = 0; i < n; i++){
        if(i != imp){
          giocatoriscorr->stato = 0;
          giocatoriscorr += sizeof(struct Giocatore);
        }else{
          giocatoriscorr->stato = 1;
          giocatoriscorr += sizeof(struct Giocatore);
        }
      }
    }else if(probImp < 60 && probImp >= 11){
      printf("ci saranno DUE impostori\n");
      alive = n - 2;
      nimposstori = 2;
      while (getchar() != '\n');
      int continua = 1;
      int imp, imp2;
      do{
        imp = rand()%(n);
        imp2 = rand()*rand()%(n);
        if(imp != imp2){
          if(imp >= 0 && imp2 >= 0){
          continua = 0;
          }
        }
      }while(continua);
      for(int i = 0; i < n; i++){
        if(i != imp && i != imp2){
          giocatoriscorr->stato = 0;
          giocatoriscorr += sizeof(struct Giocatore);
        }else if(i == imp || i == imp2){
          giocatoriscorr->stato = 1;
          giocatoriscorr += sizeof(struct Giocatore);
        }
      }
    }else if(probImp < 100 && probImp >= 61){
      printf("ci saranno TRE impostori\n");
      alive = n - 3;
      nimposstori = 3;
      while (getchar() != '\n');
      int continua = 1;
      int imp, imp2, imp3;
      do{
        imp = rand()%(n);
        imp2 = rand()*rand()%(n);
        imp3 = rand()*rand()*rand()%(n);
        if(imp != imp2 && imp2 != imp3 && imp2 != imp3){
            if(imp >= 0 && imp2 >= 0 && imp3 >= 0){
          continua = 0;
          }
        }
      }while(continua);
      for(int i = 0; i < n; i++){
        if(i != imp && i != imp2 && i != imp3){
          giocatoriscorr->stato = 0;
          giocatoriscorr += sizeof(struct Giocatore);
        }else if(i == imp || i == imp2 || i == imp3){
          giocatoriscorr->stato = 1;
          giocatoriscorr += sizeof(struct Giocatore);
        }
      }
    }
      while (getchar() != '\n');
  }
  giaimpostato = 1;
}

/*---- STAMPA LA LISTA DEI GIOCATORI E LE INFORMAZIONI RELATIVE-----*/
static void stampa_giocatori(){
  struct Giocatore *ptrStampa = giocatori;
  for(int i = 0; i < n; i++){
    printf("Giocatore: ");
    stampanome(ptrStampa);
    printf("\n");
    printf("Stato Giocaotre: ");
      switch(ptrStampa->stato){
        case 0:
                printf("ASTRONAUTA\n");
                break;
        case 1:
                printf("IMPOSTORE\n");
                break;
        case 2:
                printf("MORTO\n");
                break;
        case 3:
                printf("DEFENESTRATO\n");
                break;
      }
    printf("Posizione Giocatore: %p\n", ptrStampa->posizione_Player);
    printf("\n\n");

    ptrStampa += sizeof(struct Giocatore);
  }
  while (getchar() != '\n');
}

static void det_stanza_inizio(struct Stanza *ptr){
    int probStanza = 1+rand()%100;
    if (probStanza < 25 && probStanza >=0){
      ptr->tipo_stanza = 3;
    }else if(probStanza < 40 && probStanza >= 26){
      ptr->tipo_stanza = 2;
    }else if(probStanza < 71 && probStanza >= 41){
      ptr->tipo_stanza = 1;
    }else if(probStanza < 100 && probStanza >= 71){
      ptr->tipo_stanza = 0;
    }
}

void gioca(){
  if(!giocare){
    printf("ATTENZIONE!! Non hai inserito il numero dei giocatori che prenderanno parte alla partita!\nOppure ti sei scordato il numero di quest?\nIn ogni caso verrai rimandato al menu delle IMPOSTAZIONI\n\n");
    while (getchar() != '\n');
    impostazioni();
  }else{
    printf("Benvenuto nell' Astronave Skelt.\nCi sono un po' di lavori da fare per arrivare a destinazione...\n");
    printf("Iniza il gioco\n");
    while (getchar() != '\n');
    int turno = 1;
    int turni[n];
    //inizializzazione array turni giocatori da 1 a n-1
    for(int i = 0; i < n; i++){
      turni[i]= i;
    }
    //inizio turno
    if(fine_gioco){
      while(fine_gioco){
        stampa_lista();
        printf("TURNO num: %d\n", turno);
        randomize(turni, n);//crea i turni dei giocatori mescolando l' array turni
        for(int i = 0; i < n; i++){
          giocatoriscorr = giocatori;//sono all' inizio dell 'array giocatori
          giocatoriscorr += turni[i]*sizeof(struct Giocatore);
          printf("+--------------------------------------------------------+\n");
          printf("\t\tGiocatore: ");
          stampanome(giocatoriscorr);
          printf("\n");
          printf("+--------------------------------------------------------+\n");
          menu_turno(giocatoriscorr);
        }
        turno++;
      }
    }
  }
}

void termina_gioco(){
  giocatoriscorr = NULL;
  quest_da_finire = 0;
  printf("Sto deallocando giocatori\n");
  free(giocatori);//viene cancellata la memoria dinamica riservata all' array giocatori

  printf("Sto deallocando la mappa\n");
  cancellaStanza(stanza_inizio);//viene cancellata tutta la mappa creata durante la partita
  printf("Ho deallocato tutte le stanze create!\n");

  printf("Sto deallocando la lista delle stanze\n");
  if(lista_stanze->stanza_Precedente == NULL){
    free(ultima_stanza);
  }else{
  struct Stanza *ptrScan = lista_stanze;
  while(ptrScan->stanza_Precedente != NULL){
    struct Stanza *cancellare = ptrScan;
    ptrScan = ptrScan->stanza_Precedente;
    printf("stanza %p\n", ptrScan);
    free(cancellare);
  }
}
  printf("Ho deallocato la lista delle stanze\n");
}

static struct Stanza *cancellaStanza(struct Stanza *ptr){//funzione ricorsiva usata per cancellare tutte le stanze create durante la partita
  while(nextStanza(ptr) != ptr){
    if((ptr->Avanti) != NULL){
      ptr->Avanti = cancellaStanza(ptr->Avanti);
    }
    if((ptr->Sinistra) != NULL){
      ptr->Sinistra = cancellaStanza(ptr->Sinistra);
    }
  if((ptr->Destra) != NULL){
    ptr->Destra = cancellaStanza(ptr->Destra);
    }
  }
  free(ptr);
  return NULL;
}
static struct Stanza *nextStanza(struct Stanza *ptr){//funione utilizzata per arrivare alla fine dell' albero delle stanze che si crea durante la partita
  if(ptr->Avanti != NULL){
    ptr = ptr->Avanti;
  }else if(ptr->Sinistra != NULL){
    ptr = ptr->Sinistra;
  }else if(ptr->Destra != NULL){
    ptr = ptr->Destra;
  }
  return ptr;
}

static void n_quest(){
  printf("Inserisci il numero di QUEST che gli astronauti dovranno fare,\nse gli astronauti completano le quest vincono la partita!\n");
  scanf("%hd", &quest_da_finire);
  printf("Le Quest da completare sono %hd\n", quest_da_finire);
  while (getchar() != '\n');
}

static void randomize(int arr[], int n){
  for(int i= n-1; i > 0; i--){
    int j = rand()%(i+1);
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

static void stampanome(struct Giocatore *ptr){
  switch(ptr->nome){
    case 0:
            printf("Rosso");
            break;
    case 1:
            printf("Blu");
            break;
    case 2:
            printf("Nero");
            break;
    case 3:
            printf("Giallo");
            break;
    case 4:
            printf("Viola");
            break;
    case 5:
            printf("Arancione");
            break;
    case 6:
            printf("Verde");
            break;
    case 7:
            printf("Celeste");
            break;
    case 8:
            printf("Bianco");
            break;
    case 9:
            printf("Rosa");
            break;
  }
}

static void stampa_stanza(struct Giocatore *ptr){
  switch(ptr->posizione_Player->tipo_stanza){
    case 0:
            printf("Vuota\n");
            break;
    case 1:
            printf("Quest semplice\n");
            break;
    case 2:
            printf("Quest lunga\n");
            break;
    case 3:
            if(ptr->stato == 1){
            printf("Botola\n");
          }else{
            printf("Vuota\n");
          }
            break;
  }
}

static void menu_turno(struct Giocatore *ptr){
  int x = 0;//1 se c'e qualcino nella stessa stanza 0 altrimenti;
  int morti = 0;//1 se ci sono morti nella stessa stanza
  if(fine_gioco){
  printf("Ti trovi nella stanza di tipo: ");
  stampa_stanza(ptr);
  while (getchar() != '\n');
  struct Giocatore *stessaStanza = giocatori;
  printf("Nella tua stessa stanza ci sono: ");
  for(int i = 0; i < n; i++){
    if(ptr->posizione_Player == stessaStanza->posizione_Player && ptr->nome != stessaStanza->nome){
      stampanome(stessaStanza);
      printf(", ");
      if(stessaStanza->stato == astronauta){
        x = 1;
      }
      if(stessaStanza->stato == morto){
        morti = 1;
      }
    }
    stessaStanza += sizeof(struct Giocatore);
  }
  printf("\n");
  while (getchar() != '\n');
  int scelta = 0;
  int giocato = 1;
  do{
    if(ptr->stato == 0){
      printf("Sei un ASTRONAUTA, cosa vuoi fare?\n");
      printf("\t1.Avanza\n");
      if(ptr->posizione_Player->tipo_stanza == quest_semplice || ptr->posizione_Player->tipo_stanza == quest_lunga){
      printf("\t2.Esegui Quest\n");
      }
      if(morti && ptr->posizione_Player->emergenza == 1){
        printf("\t3.Chiama Emergenza\n");
      }
      scanf("%d", &scelta);
    }else if(ptr->stato == 1){
      printf("Sei un IMPOSTORE, cosa vuoi fare?\n");
      printf("\t1.Avanza\n");
      if(x){
        printf("\t2.Uccidi Astronauta\n");
      }
      if(morti && ptr->posizione_Player->emergenza == 1){
      printf("\t3.Chiama Emergenza\n");
    }
      if(ptr->posizione_Player->tipo_stanza == botola){
        printf("\t4.Usa Botola\n");
        printf("La nuova stanza dopo la botola è: ");
        stampa_stanza(ptr);
      }
      if(ptr->posizione_Player->tipo_stanza == quest_lunga || ptr->posizione_Player->tipo_stanza == quest_semplice){
        printf("\t5.Sabotaggio\n");
        printf("La stanza è passata ad un nuovo tipo: ");
        stampa_stanza(ptr);
      }
      scanf("%d", &scelta);
    }else if(ptr->stato == morto){
      printf("Sei MORTO, cosa vuoi fare?\n");
      while (getchar() != '\n');
      printf("Niente, i morti non possono fare nulla!\n");
      break;
    }else if(ptr->stato == defenestrato){
      printf("Sei stato DEFENESTRATO, continua a nuotare nello spazio!\n");
    }

    switch(scelta){
      case 1:
            ptr->posizione_Player =(struct Stanza*)avanza(ptr->posizione_Player);
            printf("Ora ti trovi in una stanza di tipo: ");
            stampa_stanza(ptr);
            break;
      case 2:
            if(ptr->stato == 0 || ptr->stato == 2 || ptr->stato == 3){
              esegui_quest(ptr->posizione_Player);
              printf("numero quest rimaste: %d\n", quest_da_finire);
            }else if(ptr->stato == 1){
              uccidi_astronauta(ptr);
            }
            break;
      case 3:
            chiamata_emergenza(ptr->posizione_Player);
            break;
      case 4:
            printf("posizione prima della botola: %p\n", ptr->posizione_Player);
            usa_botola(ptr);
            printf("posizione dopo la botola: %p\n", ptr->posizione_Player);
            printf("Tipo nuova stanza: ");
            stampa_stanza(ptr);
            break;
      case 5:
            sabotaggio(ptr->posizione_Player);
            printf("stanza di tipo: ");
            stampa_stanza(ptr);
            printf("\n");
            break;
      default:
            printf("scelta sbagliata!");
            break;
    }
    if(quest_da_finire == 0){
      printf("##     ## ####  ######  ########  #######  ########  ##    ## \n");
      printf("##     ##  ##  ##    ##    ##    ##     ## ##     ##  ##  ##  \n");
      printf("##     ##  ##  ##          ##    ##     ## ##     ##   ####   \n");
      printf("##     ##  ##  ##          ##    ##     ## ########     ##    \n");
      printf(" ##   ##   ##  ##          ##    ##     ## ##   ##      ##    \n");
      printf("  ## ##    ##  ##    ##    ##    ##     ## ##    ##     ##    \n");
      printf("   ###    ####  ######     ##     #######  ##     ##    ##    \n\n\n");
      printf("COMPLIMENTI!! Gli astonauti hanno completato le quest\nL'astronave è arrivata a destinazione!!\n\n");
      while (getchar() != '\n');
      printf("Gli impostori hanno perso\n\n");
      printf("La Partita è finita\n\n");
      while (getchar() != '\n');
      termina_gioco();
      fine_gioco = 0;
      break;
    }else if(alive == 0){
        printf("  __ _  __ _ _ __ ___   ___    _____   _____ _ __  \n");
        printf(" / _` |/ _` | '_ ` _\\ / _\\  / _ \\\\/ / _ \\'__| \n");
        printf("| (_| | (_| | | | | | |  __/ | (_) \\V /  __/ |    \n");
        printf(" \\__, |\\__,_|_| |_| |_|\\___|  \\___/\\_/\\___|_| \n");
        printf("  __/ |                                            \n");
        printf(" |___/                                             \n\n\n");
        printf("Gli IMPOSTORI hanno eliminato tutti gli astronauti nella nave\n\n");
        while (getchar() != '\n');
        printf("La partita termina qua!\n\n");
        while (getchar() != '\n');
        termina_gioco();
        fine_gioco = 0;
        break;
      }else if(nimposstori == 0){
        printf("##     ## ####  ######  ########  #######  ########  ##    ## \n");
        printf("##     ##  ##  ##    ##    ##    ##     ## ##     ##  ##  ##  \n");
        printf("##     ##  ##  ##          ##    ##     ## ##     ##   ####   \n");
        printf("##     ##  ##  ##          ##    ##     ## ########     ##    \n");
        printf(" ##   ##   ##  ##          ##    ##     ## ##   ##      ##    \n");
        printf("  ## ##    ##  ##    ##    ##    ##     ## ##    ##     ##    \n");
        printf("   ###    ####  ######     ##     #######  ##     ##    ##    \n\n\n");
        printf("Gli astronauti hanno scoperto tutti gli impostori!!\nPossono arrivare tranquillamente a destinazione!!\n\n");
        while (getchar() != '\n');
        printf("La partita finisce qua!!\n\n");
        while (getchar() != '\n');
        termina_gioco();
        fine_gioco = 0;
        break;
      }
      giocato--;
    }while(giocato);
  }
}

static struct Stanza* avanza(struct Stanza *ptr){
  int scelta = 0;
  struct Stanza *nuovaStanza = NULL;
  printf("Stai lasciando questa stanza, ti puoi muovere in 3 direzioni\nRicorda bene una cosa, NON si può tornare indietro.\nQuale scegli?\n");
  printf("\t1. Avanti\n\t2. Sinistra\n\t3. Destra\n\t4. Rimani Fermo\n");
  scanf("%d", &scelta);
  switch(scelta){
    case 1:
          if(ptr->Avanti == NULL){
            nuovaStanza = malloc(sizeof(struct Stanza));
            numero_stanze++;
            nuovaStanza->stanza_Precedente = ptr;
            nuovaStanza->emergenza = 1;
            ptr->Avanti = nuovaStanza;
            ptr= nuovaStanza;
            det_stanza_inizio(nuovaStanza);
            /*lista dinamica delle stanze create*/
            struct Stanza *snew = (struct Stanza*) malloc(sizeof(struct Stanza));
            snew = nuovaStanza;
            //verifico se il primo elemento delle lista non ha altri nodi attaccati
            if(lista_stanze->stanza_Precedente == NULL){
              lista_stanze->stanza_Precedente = snew;
              ultima_stanza = snew;
              ultima_stanza->stanza_Precedente = NULL;
            }else if(ultima_stanza->stanza_Precedente == NULL){
              ultima_stanza->stanza_Precedente = snew;
              ultima_stanza = snew;
              ultima_stanza->stanza_Precedente = NULL;
            }
            return ptr;
          }else{
            ptr = ptr->Avanti;
            return ptr;
          }
          break;
    case 2:
          if(ptr->Sinistra == NULL){
            nuovaStanza = malloc(sizeof(struct Stanza));
            numero_stanze++;
            ptr->Sinistra = nuovaStanza;
            nuovaStanza->stanza_Precedente = ptr;
            nuovaStanza->emergenza = 1;
            ptr = nuovaStanza;
            det_stanza_inizio(nuovaStanza);
            struct Stanza *snew = (struct Stanza*) malloc(sizeof(struct Stanza));
            snew = nuovaStanza;
            //verifico se il primo elemento delle lista non ha altri nodi attaccati
            if(lista_stanze->stanza_Precedente == NULL){
              lista_stanze->stanza_Precedente = snew;
              ultima_stanza = snew;
              ultima_stanza->stanza_Precedente = NULL;
            }else if(ultima_stanza->stanza_Precedente == NULL){
              ultima_stanza->stanza_Precedente = snew;
              ultima_stanza = snew;
              ultima_stanza->stanza_Precedente = NULL;
            }
            return ptr;
          }else{
            ptr = ptr->Sinistra;
            return ptr;
          }
          break;
    case 3:
          if(ptr->Destra == NULL){
            nuovaStanza = malloc(sizeof(struct Stanza));
            numero_stanze++;
            ptr->Destra = nuovaStanza;
            nuovaStanza->stanza_Precedente = ptr;
            nuovaStanza->emergenza = 1;
            ptr = nuovaStanza;
            det_stanza_inizio(nuovaStanza);
            struct Stanza *snew = (struct Stanza*) malloc(sizeof(struct Stanza));
            snew = nuovaStanza;
            //verifico se il primo elemento delle lista non ha altri nodi attaccati
            if(lista_stanze->stanza_Precedente == NULL){
              lista_stanze->stanza_Precedente = snew;
              ultima_stanza = snew;
              ultima_stanza->stanza_Precedente = NULL;
            }else if(ultima_stanza->stanza_Precedente == NULL){
              ultima_stanza->stanza_Precedente = snew;
              ultima_stanza = snew;
              ultima_stanza->stanza_Precedente = NULL;
            }
          }else{
            ptr = ptr->Destra;
            return ptr;
          }
          break;
    case 4:
          printf("Ok, non ti muoverai da questa stanza, ma avrai sprecato un turno!\n");
          break;
    default: printf("scelta sbagliata\n");
             break;
  }
  return ptr;
}

static void esegui_quest(struct Stanza *ptr){
  if(ptr->tipo_stanza == quest_semplice){
    quest_da_finire--;
    ptr->tipo_stanza = 0;
  }else if(ptr->tipo_stanza == quest_lunga){
    quest_da_finire -= 2;
    ptr->tipo_stanza = 0;
  }
  if(quest_da_finire > 65533){//faccio il controllo se il numero delle quest va sotto lo 0, ma essendo unsigned short il valore che prendera sara 65.535, quindi controllo se è maggiore di 65.533(per sicurezza), in caso positivo assegno 0
    quest_da_finire = 0;
  }
}

static void chiamata_emergenza(struct Stanza *ptr){
  struct Giocatore *scorrimento = giocatori;
  struct Giocatore *pscan;
  int emeImp = 0;//indica la probabilità che un impostore venga defenestrato
  int emeAstro = 0;//inidica la probabilità che un astronauta venga defenestrato
  int Defenestrato = 1;//se 0 indica che un player è già stato DEFENESTRATO
  for(int i = 0; i < n; i++){
    if(scorrimento->posizione_Player == ptr){//controllo se il giocaotore è nella stessa stanza dell' emergenza
      if(scorrimento->stato == astronauta){//calcolo la probabilità nel caso si astronauta
        pscan = giocatori;
        emeAstro = 30;
        for(int j = 0; j < n; j++){
          if(pscan->posizione_Player == ptr && pscan->stato == impostore && pscan->nome != scorrimento->nome){
            /*---GUARDO SE CI SONO IMPOSOTORI NELLA STANZA NEL CASO IL GIOCATORE PRESO IN CONSIDERAZIONE SIA UNA ASTRONAUTA---*/
            emeAstro += 20;
          }else if(pscan->posizione_Player == ptr && pscan->stato == astronauta && pscan->nome != scorrimento->nome){
            /*---GUARDO SE CI SONO ASTRONAUTI NELLA STANZA NEL CASO IL GIOCATORE PRESO IN CONSIDERAZIONE SIA UNA ASTRONAUTA---*/
            emeAstro -=30;
          }
          if(emeAstro < 0){
            emeAstro = 0;
          }else if(emeAstro > 100){
            emeAstro = 100;
          }
          pscan += sizeof(struct Giocatore);
        }
      }else if(scorrimento->stato == impostore){//calcolo la probabilità nel caso sia impostore
        pscan = giocatori;
        emeImp = 30;
        for(int j = 0; j < n; j++){
          if(pscan->posizione_Player == ptr && pscan->stato == astronauta && pscan->nome != scorrimento->nome){
            emeImp += 20;
          }else if(pscan->posizione_Player == ptr && pscan->stato == impostore && pscan->nome != scorrimento->nome){
            emeImp -= 30;

          }
          pscan += sizeof(struct Giocatore);
          if(emeImp < 0){
            emeImp = 0;
          }else if(emeImp > 100){
            emeImp = 100;
          }
        }
      }
      /*---ESTRAGGO UN NUMERO RANDOM E VEDO SE DEVE ESSERE DEFENESTRATO----*/
      if(Defenestrato){
        int r = rand()%100;
        if(scorrimento->stato == astronauta){
          if(r < emeAstro){
            scorrimento->stato = defenestrato;
            Defenestrato--;
            alive--;
            printf("Gli astonauti hanno deciso di defenestrare:");
            stampanome(scorrimento);
            printf(" %d\n", r);
            while (getchar() != '\n');
          }else{
            printf("Gli atronauti hanno deciso di skippare questa emergenza\nNesuno sarà defenestrato\n\n");
            while (getchar() != '\n');
          }
        }else if(scorrimento->stato == impostore){
          if(r < emeImp){
            scorrimento->stato = defenestrato;
            Defenestrato--;
            nimposstori--;
            printf("Gli astonauti hanno deciso di defenestrare:");
            stampanome(scorrimento);
            printf(" %d\n",r);
            while (getchar() != '\n');
          }else{
            printf("Gli atronauti hanno deciso di skippare questa emergenza\nNesuno sarà defenestrato\n\n");
            while (getchar() != '\n');
          }
        }
        ptr->emergenza = 0;
      }else{
        break;
      }
    }
    scorrimento += sizeof(struct Giocatore);
  }
}

static void sabotaggio(struct Stanza *ptr){
  if(ptr->tipo_stanza == quest_lunga || ptr->tipo_stanza == quest_semplice){
    ptr->tipo_stanza = vuota;
  }
  printf("\n");
}

static void usa_botola(struct Giocatore *ptr){
  int x = 1;//indica se è avvenuto un passaggio tramite la botola;
  int continua = 1;//variabile di controllo
  int trovata = 0;
  struct Stanza *scorrimento = lista_stanze;
  do{
  while(scorrimento->stanza_Precedente != NULL){
    if(scorrimento->tipo_stanza == botola && scorrimento != ptr->posizione_Player){
      trovata = 1;
      printf("trovata %p\n", scorrimento);
      int r = rand()%100;
      if(r > 50 && r < 100){
        ptr->posizione_Player = scorrimento;
        x = 0;//indico che è stata cambiata stanza;
        if(!x){
          trovata= 0;
        }
        break;
      }
    }
    scorrimento = scorrimento->stanza_Precedente;
  }
}while(trovata);
  while(continua){
    scorrimento = lista_stanze;
    if(x){
      while(scorrimento->stanza_Precedente != NULL){
        int r = rand()%100;
        if(r > 50 && r < 100){
          ptr->posizione_Player = scorrimento;
          continua = 0;
          break;
        }
        scorrimento = scorrimento->stanza_Precedente;
      }
    }else{
      continua = 0;
    }
  }
}

static void stampa_lista(){
  int x = 0;//variabile per sapere che numero si stanza è DA CANCELLARE
  struct Stanza* scorrimento = lista_stanze;
  printf("Ecco la lista delle stanze create durante la partita\n");
  if(scorrimento == NULL){
    printf("Lista Vuota\n");
  }else{
    do{
      printf("Stanza numero %d di tipo %d all' inidirizzo %p\n",x++ ,scorrimento->tipo_stanza, scorrimento);
      scorrimento = scorrimento->stanza_Precedente;
    }while(scorrimento != NULL);
  }
}

static void uccidi_astronauta(struct Giocatore *ptr){
  int mortox = 0;//indica il giocatore che muore
  int giocpresenti = 0;//indica il numero di giocatori presenti nella stessa stanza
  int probmorte = 0;//indica la probabilità che l'impostore venga defenestrato
  int giocprima = 0;//indica il numero di persone presenti nella stanza precedente a quella dove si trova l'impostore
  struct Giocatore *scorrimento = giocatori;//serve a scorrere l'array dei giocatori
  struct Stanza *stanzaprima = ptr->posizione_Player->stanza_Precedente;//punta alla stanza precedente rispetto a quella in cui si trova l'impostore
  for(int i = 0; i < n; i++){
    if(ptr->posizione_Player == scorrimento->posizione_Player && scorrimento->stato == astronauta){
      printf("%d. ", scorrimento->nome);
      stampanome(scorrimento);
      printf("\n");
      giocpresenti++;
    }
    scorrimento += sizeof(struct Giocatore);
  }
  scanf("%d", &mortox);
  scorrimento = giocatori;
  while(scorrimento->nome != mortox){
    scorrimento += sizeof(struct Giocatore);
  }
    scorrimento->stato = morto;
    printf("Hai ucciso: ");
    stampanome(scorrimento);
    printf("\n");
    alive--;
    giocpresenti--;
    scorrimento = giocatori;
    for(int i = 0; i < n; i++){
      if(stanzaprima == scorrimento->posizione_Player){
        if(scorrimento->stato == astronauta){
        giocprima++;
      }
    }
      scorrimento += sizeof(struct Giocatore);
    }
    while (getchar() != '\n');
    switch(giocpresenti){
      case 0:
            if(giocprima){
              probmorte = (giocprima * 20);
              if (probmorte > 100){
                probmorte = 100;
              }
            }
            int r = rand()%100;
            if(r <= probmorte){
              ptr->stato = defenestrato;
              printf("Gli astronauti presenti nella stanza precedente ti hanno scoperto!\nHanno deciso che non farai più parte della astronave!\nLa tua partita finisce qua!\n\n");
              while (getchar() != '\n');
              nimposstori--;
            }
            break;
      case 1 ... 10 :
                    if(giocprima){
                      probmorte = giocprima * 20;
                      }
                      if(probmorte > 100){
                        probmorte = 100;
                    }else{
                      probmorte = (giocpresenti * 50);
                      if(probmorte > 100){
                        probmorte = 100;
                      }
                    }
                    int nr = rand()%100;
                    if(nr <= probmorte){
                      ptr->stato = defenestrato;
                      printf("Gli astronauti ti hanno visto uccidere un loro compagno\nHanno deciso di buttarti fuori dalla nave!\nBuon viaggio nello spazio!!\n\n");
                      nimposstori--;
                    }
            break;
    }
}