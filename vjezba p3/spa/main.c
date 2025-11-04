#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct datum {
  int godina, mjesec, dan;
} DATUM;
typedef struct vrijeme {
  int sat, minut, sekund;
} VRIJEME;
typedef struct unos {
  char opis[100];
  DATUM datum;
  VRIJEME vrijeme;
  int vaznost;
  char naziv_fajla[50];
  char detaljan_opis[200];
} UNOS;

typedef struct kratak_unos {
  char opis[100];
  DATUM datum;
} SHORT_UNOS;

typedef struct kratak_unosV2 {
  char opis[100];
  DATUM datum;
  VRIJEME vrijeme;
  char detaljan_opis[200];
} SHORT_UNOS_vrijeme;

typedef struct node {
  UNOS unos;
  struct node *next;
} NODE; // implementacija stack-a

// komparatorska f-ja
int uporedi_unos(const UNOS *u1, const UNOS *u2) {
  if (u1->vaznost != u2->vaznost)
    return u1->vaznost - u2->vaznost; // u1>u2 vraca tacno, a u2>u1 vraca false
                                      // odnosno broj <0

  if (u1->datum.godina != u2->datum.godina)
    return u1->datum.godina - u2->datum.godina;
  if (u1->datum.mjesec != u2->datum.mjesec)
    return u1->datum.mjesec - u2->datum.mjesec;
  if (u1->datum.dan != u2->datum.dan)
    return u1->datum.dan - u2->datum.dan;

  if (u1->vrijeme.sat != u2->vrijeme.sat)
    return u1->vrijeme.sat - u2->vrijeme.sat;
  if (u1->vrijeme.minut != u2->vrijeme.minut)
    return u1->vrijeme.minut - u2->vrijeme.minut;
  if (u1->vrijeme.sekund != u2->vrijeme.sekund)
    return u1->vrijeme.sekund - u2->vrijeme.sekund;

  return 0; // jednaki su
}
// Sortiranje i ispis po vremenu za dati datum
void sortiraj_po_vremenu(NODE *tos, DATUM trazeni);
// Prikaz i sortiranje filtriranih unosa po datumu
void prikazi_sortirano(NODE *tos, DATUM low, DATUM high);
// Prikaz i sortiranje svih unosa po datumu
void sortiraj_prikaz_V1(FILE *fp, int brojac);
// Komparatorske funkcije
int compare(const void *a, const void *b);       // za datume
int compare_hours(const void *a, const void *b); // za vrijeme
// stavljanje na kraj reda
int put(NODE **front, NODE **rear, UNOS info) {
  NODE *q = (NODE *)calloc(1, sizeof(NODE));
  if (q == NULL)
    return 0;

  q->unos = info;

  if (*front == NULL) { // slucaj da je red prazan
    *front = *rear = q;
    return 1;
  }
  // ako novi ima veci prioritet od trenutnog
  if (uporedi_unos(&info, &(*front)->unos) > 0) {
    q->next = *front;
    *front = q;
    return 1;
  }
  // ako ne onda sortiramo, odnosno trazimo mjesto gdje cemo ubaciti element
  NODE *temp = *front;
  while (temp->next != NULL && uporedi_unos(&info, &temp->next->unos) <= 0)
    temp = temp->next;

  q->next = temp->next;
  temp->next = q;
  // slucaj da smo dodali na kraj
  if (q->next == NULL)
    *rear = q;

  return 1;
}
// uzimanje s pocetka reda
int get(NODE **front, NODE **rear, UNOS *info) {
  if (*front == NULL)
    return 0;
  NODE *q = *front;
  *info = q->unos;
  *front = q->next;
  if (*front == NULL)
    *rear = NULL;
  free(q);
  return 1;
}

int push(NODE **tos, UNOS info, int *brojac) {
  NODE *q = (NODE *)malloc(sizeof(NODE));
  if (q == NULL)
    return 0;
  q->unos = info;
  q->next = *tos;
  *tos = q;
  (*brojac)++;
  return 1;
}

int pop(NODE **tos, UNOS *info, int *brojac) {
  if (*tos == NULL)
    return 0;
  NODE *q = *tos;
  *tos = q->next;
  (*info) = q->unos;
  free(q);
  (*brojac)--;
  return 1;
}
int obrisired(const char *ime_fajla, int brojac) {
  FILE *fp1 = fopen(ime_fajla, "r");
  FILE *fp2 = fopen("temp.txt", "w");
  if (fp1 == NULL || fp2 == NULL) {
    if (fp1)
      fclose(fp1);
    if (fp2)
      fclose(fp2);
    return 0;
  }

  char temp_buffer[1000];
  int red = 0;

  // kopiranje svih linija, sem zadnje
  while (fgets(temp_buffer, sizeof(temp_buffer), fp1) != NULL) {
    if (red != brojac) {
      if (fputs(temp_buffer, fp2) == EOF) {
        fclose(fp1);
        fclose(fp2);
        return 0;
      }
    }
    red++;
  }

  fclose(fp1);
  fclose(fp2);

  if (remove(ime_fajla) != 0)
    return 0;
  if (rename("temp.txt", ime_fajla) != 0)
    return 0;

  return 1;
}

int is_valid_date(int dan, int mjesec, int godina, int sat, int minut,
                  int sekund) {
  if (godina <= 0)
    return 0;
  if (mjesec < 1 || mjesec > 12)
    return 0;
  if (dan < 1 || dan > 31)
    return 0;
  if (sat < 0 || sat > 23)
    return 0;
  if (minut < 0 || minut > 59)
    return 0;
  if (sekund < 0 || sekund > 59)
    return 0;
  return 1;
}

void ocisti_bafer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

int main(void) {
  NODE *tos = NULL; // za potrebe stacka
  int brojac = 0;
  // za potrebe prioritetnog reda
  NODE *front = NULL;
  NODE *rear = NULL;

  int izbor;
  do {
    printf("      -----MENI-----\n");
    printf("Izaberite jednu od opcija:\n"
           "1) Novi unos\n"
           "2) Brisanje poslednje dodatog unosa\n"
           "3) Prikazivanje 5 unosa sa najvecom vaznosti\n"
           "4) Kratak prikaz dnevnika\n"
           "5) Kraj programa!\n"
           "Vas izbor: ");
    scanf("%d", &izbor);
    if (izbor < 1 || izbor > 5) {
      printf("Neispravan izbor! unesite ponovo \n");
      continue;
    }
    switch (izbor) {
    case 1: {
      UNOS u1;
      FILE *fp1 = fopen("dnevnik.txt", "a");
      if (fp1 == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        fclose(fp1);
        break;
      }
      char filename[50];
      sprintf(filename, "unosi/unos%d.txt",
              brojac + 1); // jer se brojac tek naknadno u petlji poveca, pa
                           // da unos krece od 0 a ne od 1

      FILE *fp = fopen(filename, "w");
      if (fp == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        fclose(fp);
        break;
      }
      strcpy(u1.naziv_fajla, filename);
      printf("Unesi kratak opis! \n");
      ocisti_bafer();
      fgets(u1.opis, 100, stdin);
      u1.opis[strlen(u1.opis) - 1] =
          '\0'; // umjesto ocekivanog /n na kraju stavjamo null-term.
      do {
        printf("Unesi datum, po principu DAN/MJESEC/GODINA: ");
        scanf("%d/%d/%d", &u1.datum.dan, &u1.datum.mjesec, &u1.datum.godina);
        printf("Unesi vrijeme u principu SAT/MINUT/SEKUND: ");
        scanf("%d/%d/%d", &u1.vrijeme.sat, &u1.vrijeme.minut,
              &u1.vrijeme.sekund);
        if (!is_valid_date(u1.datum.dan, u1.datum.mjesec, u1.datum.godina,
                           u1.vrijeme.sat, u1.vrijeme.minut,
                           u1.vrijeme.sekund)) {
          printf("Neispravan datum ili vrijeme! Pokusajte ponovo.\n");
        }
      } while (!is_valid_date(u1.datum.dan, u1.datum.mjesec, u1.datum.godina,
                              u1.vrijeme.sat, u1.vrijeme.minut,
                              u1.vrijeme.sekund));
      printf("Unesi vaznost: \n");
      scanf("%d", &u1.vaznost);
      printf("Unesi detaljan opis u ovom unosu:");
      ocisti_bafer();
      fgets(u1.detaljan_opis, 200, stdin);
      u1.detaljan_opis[strlen(u1.detaljan_opis) - 1] = '\0';
      // prvo cemo upisati na stek, da bi povecali brojac a nakon toga cemo
      // upisati u datoteku push(*tos,, int *brojac)
      if (!push(&tos, u1, &brojac)) {
        printf("neuspesan push na stack");
        break;
      }
      fprintf(fp1,
              "Opis: %s ; Datum: %02d/%02d/%04d ; Vrijeme: %02d:%02d:%02d ; "
              "Vaznost: %d\n",
              u1.opis, u1.datum.dan, u1.datum.mjesec, u1.datum.godina,
              u1.vrijeme.sat, u1.vrijeme.minut, u1.vrijeme.sekund, u1.vaznost);
      fprintf(fp,
              "Opis: %s ; Datum: %02d/%02d/%04d ; Vrijeme: %02d:%02d:%02d ; "
              "Vaznost: %d\nDetaljan opis: %s\n",
              u1.opis, u1.datum.dan, u1.datum.mjesec, u1.datum.godina,
              u1.vrijeme.sat, u1.vrijeme.minut, u1.vrijeme.sekund, u1.vaznost,
              u1.detaljan_opis);
      fclose(fp);
      fclose(fp1);
      break;
    }
    case 2: {
      UNOS removed;
      if (!pop(&tos, &removed, &brojac)) {
        printf("neuspjesno skidanje! (stack greska)");
        break;
      }

      char filename1[sizeof(removed.naziv_fajla) + 1];
      strcpy(filename1, removed.naziv_fajla);
      if (remove(filename1) == 0) // remove vraca 0 ako jeste, 1 ako nije
        printf("Fajl uspjesno obrisan\n");
      else
        printf("Greska! fajl nije obrisan");
      if (obrisired("dnevnik.txt", brojac))
        printf("Red uspjesno obrisan sa dnevnika\n");
      else
        printf("Greska u brisanju reda sa dnevnika");
      break;
    }
    case 3: {
      printf("Prikaz pet unosa sa najvecom vaznosti: \n");
      // kopiramo elemente sa stacka u red da bi implementirali prioritetni red
      NODE *tempStackTOS = NULL;
      UNOS tempUnos;

      NODE *pom = tos;
      int tempBrojac = 0; // ne koristimo ga nigdje, da bi ispostovali
                          // prototip f-je
      while (pom != NULL) {
        push(&tempStackTOS, pom->unos, &tempBrojac);
        pom = pom->next;
      }

      while (tempStackTOS != NULL) {
        pop(&tempStackTOS, &tempUnos, &tempBrojac);
        put(&front, &rear,
            tempUnos); // tempUnos je varijabla u koj smo storovali podatak
      }
      int count = 0;
      while (front != NULL && count < 5) {
        UNOS info;
        get(&front, &rear, &info);
        printf("Opis: %s | Datum: %02d/%02d/%04d | Vrijeme: %02d:%02d:%02d | "
               "Vaznost: %d | Fajl: %s\n",
               info.opis, info.datum.dan, info.datum.mjesec, info.datum.godina,
               info.vrijeme.sat, info.vrijeme.minut, info.vrijeme.sekund,
               info.vaznost, info.naziv_fajla);
        count++;
      } // cistimo red (da se osiguramo ako se vise puta pozove case 3)
      while (front != NULL) {
        get(&front, &rear, &tempUnos);
      }

      break;
    }
    case 4: {
      printf("\nKratak prikaz dnevnika: \n");
      FILE *file;
      file = fopen("dnevnik.txt", "r");
      char opis_pom[50];
      DATUM datum_pom;
      DATUM datum_unos;
      sortiraj_prikaz_V1(file, brojac);
      rewind(file);
      printf(
          "\nDa li zelite da filtrirate po datumu prikaz? (1 - da, 0 - ne): ");
      int izbor3;
      do {
        scanf("%d", &izbor3);
      } while (izbor3 < 0 || izbor3 > 1);
      if (izbor3) {
        DATUM high, low;
        do {
          printf("Unesi donju granicu datuma u formatu: (DAN/MJESEC/GODINA): ");
          scanf("%d/%d/%d", &low.dan, &low.mjesec, &low.godina);
          if (!is_valid_date(low.dan, low.mjesec, low.godina, 0, 0, 0)) {
            printf("Neispravan datum! Pokusajte ponovo.\n");
          }
        } while (!is_valid_date(low.dan, low.mjesec, low.godina, 0, 0, 0));
        do {
          printf("Unesi gornju granicu datuma u formatu: (DAN/MJESEC/GODINA)");
          scanf("%d/%d/%d", &high.dan, &high.mjesec, &high.godina);
          if (!is_valid_date(high.dan, high.mjesec, high.godina, 0, 0, 0)) {
            printf("Neispravan datum! Pokusajte ponovo.\n");
          }
        } while (!is_valid_date(high.dan, high.mjesec, high.godina, 0, 0, 0));
        prikazi_sortirano(tos, low, high); // funkcija vrsi i ispis podataka
      }

      printf("\nDa li zelite procitati detaljan opis? Unesite 1 ako zelite "
             "procitati detaljno,"
             " ili 0 ako zelite nazad na pocetni meni\nVas izbor: ");
      int izbor2 = 0, brojac_pom = 1;
      do {
        scanf("%d", &izbor2);
        if (izbor2 < 0 || izbor2 > 1) {
          printf("\nPogresan unos - moguce unijeti 1 (detaljan prikaz),"
                 " ili 0 (povratak na meni)\n");
        }
      } while (izbor2 < 0 || izbor2 > 1);
      switch (izbor2) {
      case 1: {
        do {
          printf("Unesite zeljeni datum u formatu 'dan/mjesec/godina': ");
          scanf("%d/%d/%d", &datum_unos.dan, &datum_unos.mjesec,
                &datum_unos.godina);
          if (!is_valid_date(datum_unos.dan, datum_unos.mjesec,
                             datum_unos.godina, 0, 0, 0)) {
            printf("Neispravan datum! Pokusajte ponovo.\n");
          }
        } while (!is_valid_date(datum_unos.dan, datum_unos.mjesec,
                                datum_unos.godina, 0, 0, 0));

        rewind(file);
        sortiraj_po_vremenu(tos, datum_unos);
        break;
      }
      default:
        break;
      }
      fclose(file);
    }
    default:
      break;
    }
  } while (izbor != 5);

  printf("KRAJ PROGRAMA!\n");
  return 1;
}

// Komparator za sortiranje po datumu
int compare(const void *a, const void *b) {
  const SHORT_UNOS *u1 = (const SHORT_UNOS *)a;
  const SHORT_UNOS *u2 = (const SHORT_UNOS *)b;
  if (u1->datum.godina != u2->datum.godina)
    return u1->datum.godina - u2->datum.godina;
  if (u1->datum.mjesec != u2->datum.mjesec)
    return u1->datum.mjesec - u2->datum.mjesec;
  return u1->datum.dan - u2->datum.dan;
}

// Komparator za sortiranje po vremenu
int compare_hours(const void *a, const void *b) {
  const SHORT_UNOS_vrijeme *v1 = (const SHORT_UNOS_vrijeme *)a;
  const SHORT_UNOS_vrijeme *v2 = (const SHORT_UNOS_vrijeme *)b;
  if (v1->vrijeme.sat != v2->vrijeme.sat)
    return v1->vrijeme.sat - v2->vrijeme.sat;
  if (v1->vrijeme.minut != v2->vrijeme.minut)
    return v1->vrijeme.minut - v2->vrijeme.minut;
  return v1->vrijeme.sekund - v2->vrijeme.sekund;
}

// Prikaz i sortiranje svih unosa po datumu
// Prikaz i sortiranje svih unosa po datumu
void sortiraj_prikaz_V1(FILE *fp, int brojac) {
  DATUM datum_pom;
  char opis_pom[100];
  int brojac_pom = 0;
  SHORT_UNOS *shortunos_array =
      (SHORT_UNOS *)malloc(brojac * sizeof(SHORT_UNOS));
  if (!fp || !shortunos_array)
    return;

  while (fscanf(fp,
                "Opis: %[^;] ; Datum: %d/%d/%d ; Vrijeme: %*d:%*d:%*d ; "
                "Vaznost: %*d\n",
                opis_pom, &datum_pom.dan, &datum_pom.mjesec,
                &datum_pom.godina) > 0) {
    shortunos_array[brojac_pom].datum = datum_pom;
    strcpy(shortunos_array[brojac_pom].opis, opis_pom);
    brojac_pom++;
  }

  qsort(shortunos_array, brojac_pom, sizeof(SHORT_UNOS), compare);

  printf("\nUnosi sortirani po datumu:\n");
  for (int i = 0; i < brojac_pom; i++) {
    printf("Datum: %02d/%02d/%04d, %s\n", shortunos_array[i].datum.dan,
           shortunos_array[i].datum.mjesec, shortunos_array[i].datum.godina,
           shortunos_array[i].opis);
  }

  free(shortunos_array);
  rewind(fp);
}
void prikazi_sortirano(NODE *tos, DATUM low, DATUM high) {
  if (tos == NULL) {
    printf("Nema unosa!\n");
    return;
  }

  int count = 0;
  NODE *temp = tos;
  while (temp != NULL) {
    DATUM d = temp->unos.datum;
    int after_low =
        (d.godina > low.godina) ||
        (d.godina == low.godina && d.mjesec > low.mjesec) ||
        (d.godina == low.godina && d.mjesec == low.mjesec && d.dan >= low.dan);
    int before_high = (d.godina < high.godina) ||
                      (d.godina == high.godina && d.mjesec < high.mjesec) ||
                      (d.godina == high.godina && d.mjesec == high.mjesec &&
                       d.dan <= high.dan);

    if (after_low && before_high) {
      count++;
    }
    temp = temp->next;
  }

  if (count == 0) {
    printf("Nema unosa u traženom opsegu datuma!\n");
    return;
  }

  // Kopiram o u niz
  SHORT_UNOS *niz = (SHORT_UNOS *)malloc(count * sizeof(SHORT_UNOS));
  temp = tos;
  int index = 0;
  while (temp != NULL && index < count) {
    DATUM d = temp->unos.datum;
    int after_low =
        (d.godina > low.godina) ||
        (d.godina == low.godina && d.mjesec > low.mjesec) ||
        (d.godina == low.godina && d.mjesec == low.mjesec && d.dan >= low.dan);
    int before_high = (d.godina < high.godina) ||
                      (d.godina == high.godina && d.mjesec < high.mjesec) ||
                      (d.godina == high.godina && d.mjesec == high.mjesec &&
                       d.dan <= high.dan);

    if (after_low && before_high) {
      niz[index].datum = temp->unos.datum;
      strcpy(niz[index].opis, temp->unos.opis);
      index++;
    }
    temp = temp->next;
  }

  // Sortiraj i prikazi
  qsort(niz, count, sizeof(SHORT_UNOS), compare);
  printf("\nFiltrirani unosi sortirani po datumu:\n");
  for (int i = 0; i < count; i++) {
    printf("Datum: %02d/%02d/%04d, %s\n", niz[i].datum.dan, niz[i].datum.mjesec,
           niz[i].datum.godina, niz[i].opis);
  }

  free(niz);
}
void sortiraj_po_vremenu(NODE *tos, DATUM trazeni) {
  int brojac_pom = 0;
  SHORT_UNOS_vrijeme *niz = NULL;

  // Prvo prebrojimo koliko unosa ima za trazeni datum
  NODE *temp = tos;
  while (temp != NULL) {
    if (temp->unos.datum.dan == trazeni.dan &&
        temp->unos.datum.mjesec == trazeni.mjesec &&
        temp->unos.datum.godina == trazeni.godina) {
      brojac_pom++;
    }
    temp = temp->next;
  }

  if (brojac_pom == 0) {
    printf("Nema unosa za taj datum!\n");
    return;
  }

  niz = (SHORT_UNOS_vrijeme *)malloc(brojac_pom * sizeof(SHORT_UNOS_vrijeme));
  if (!niz)
    return;

  temp = tos;
  int index = 0;
  while (temp != NULL && index < brojac_pom) {
    if (temp->unos.datum.dan == trazeni.dan &&
        temp->unos.datum.mjesec == trazeni.mjesec &&
        temp->unos.datum.godina == trazeni.godina) {

      niz[index].datum = temp->unos.datum;
      niz[index].vrijeme = temp->unos.vrijeme;
      strcpy(niz[index].opis, temp->unos.opis);
      strcpy(niz[index].detaljan_opis, temp->unos.detaljan_opis);
      index++;
    }
    temp = temp->next;
  }

  qsort(niz, brojac_pom, sizeof(SHORT_UNOS_vrijeme), compare_hours);

  printf("\nUnosi za datum %02d/%02d/%04d sortirani po vremenu:\n", trazeni.dan,
         trazeni.mjesec, trazeni.godina);
  for (int i = 0; i < brojac_pom; i++) {
    printf("Vrijeme: %02d:%02d:%02d | Opis: %s | Detaljno: %s\n",
           niz[i].vrijeme.sat, niz[i].vrijeme.minut, niz[i].vrijeme.sekund,
           niz[i].opis, niz[i].detaljan_opis);
  }

  free(niz);
}
