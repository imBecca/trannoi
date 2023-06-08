



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main(void){
  srand(time(0));
  int scelta = 0;
  int continua = 1;

  printf("--------------------------------------------------------\n|\t\t      BENVENUTO SU ....\t\t      |\n--------------------------------------------------------\n");
  printf("d888888P                                              oo \n");
  printf("   88                                                    \n");
  printf("   88    88d888b. .d8888b. 88d888b. 88d888b. .d8888b. dP  \n");
  printf("   88    88'  `88 88'  `88 88'  `88 88'  `88 88'  `88 88 \n");
  printf("   88    88       88.  .88 88    88 88    88 88.  .88 88 \n");
  printf("   dP    dP       `88888P8 dP    dP dP    dP `88888P' dP \n");
  printf("\n");
  while (getchar() != '\n');
  printf("Un gruppo di astronauti sta viaggiando nello spazio,\nma non sanno che tra di loro si nasconde un gruppo di impostori.\n");
  while (getchar() != '\n');
  printf("Riusciranno a completare tutte le task prima di essere decimati?\nArriveranno a destinazione sani e salvi?\n");
  while (getchar() != '\n');

  do{
    menu();
    scanf("%d", &scelta);

    switch(scelta){
      case 1:
                impostazioni();
                break;
      case 2:
                gioca();
                break;
      case 3:
                termina_gioco();
                return 0;
                break;
      default:
                printf("La tua scelta non è corretta!\n");
    }

  }while(continua);
  return 0;
}

void menu(){
  printf("--------------------------------------------------------\n|\t\t      MENU' DI GIOCO\t\t      |\n--------------------------------------------------------\n");
  printf("\tCosa vuoi fare?\n");
  printf("\t\t 1. imposta gioco\n");
  printf("\t\t 2. gioca\n");
  printf("\t\t 3. termina gioco\n");
}