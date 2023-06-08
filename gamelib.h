#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Stato_giocatore {astronauta, impostore, morto, defenestrato};
enum Nome_giocatore {rosso, blu, nero, giallo, viola, arancione, verde, celeste, bianco, rosa};
enum Tipo_stanza {vuota, quest_semplice, quest_lunga, botola};

struct Stanza{
  struct Stanza *Avanti;
  struct Stanza *Destra;
  struct Stanza *Sinistra;
  struct Stanza *stanza_Precedente;
  enum Tipo_stanza tipo_stanza;
  int emergenza;
};

struct Giocatore{
  struct Stanza *posizione_Player;
  enum Stato_giocatore stato;
  enum Nome_giocatore nome;
};

void gioca(void);
void termina_gioco(void);
void menu(void);
void impostazioni(void);