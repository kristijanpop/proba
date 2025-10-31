#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct unos {
  char opis[100];
  char datum[50];
  char vrijeme[50];
  int vaznost;
  char naziv_fajla[50];
} UNOS;

typedef struct node {
  UNOS unos;
  struct node *next;
} NODE; // implementacija stack-a

int push(NODE **tos, UNOS info) {
  NODE *q = (NODE *)malloc(sizeof(NODE));
  if (q == NULL)
    return 0;
  q->unos = info;
  q->next = *tos;
  *tos = q;
  return 1;
}

int pop(NODE **tos, UNOS *info) {
  if (*tos == NULL)
    return 0;
  NODE *q = *tos;
  *info = q->unos;
  *tos = q->next;
  free(q);
  return 1;
}

int main(void) {
  NODE *tos = NULL;
  printf("-----MENI-----");
  printf("Izaberite jednu od opcija:\n1) Novi unos\n2)Brisanje poslednje "
         "dodatog unosa\n3) Prikazivanje 5 unosa sa najvecom vaznosti\n4) Kraj "
         "programa");
  int izbor;
  do {
    printf("-----MENI-----");
    printf(
        "Izaberite jednu od opcija:\n1) Novi unos\n2)Brisanje poslednje "
        "dodatog unosa\n3) Prikazivanje 5 unosa sa najvecom vaznosti\n4) Kraj "
        "programa");
    if(izbor <1 || izbor >4){
        printf("neispravan izbor! unesite ponovo \n");
        continue;
    }
    switch (izbor) {
        case 1:

    }
    
  }while(izbor!=4);
  printf("KRAJ PROGRAMA!\n");
  return 1;}