//JULIAN POLAK 448462

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifndef WIERSZE
#define WIERSZE 22
#endif
#ifndef KOLUMNY
#define KOLUMNY 80
#endif

int X_kamery;
int Y_kamery;

//Lista kolumn w wierszu
struct TListaKolumn {
  int numer;
  int alive;
  int sasiedzi;
  struct TListaKolumn* next;
  struct TListaKolumn* prev;
};

//Lista wierszy
struct TListaWierszy {
  int numer;
  struct TListaWierszy* next;
  struct TListaWierszy* prev;
  struct TListaKolumn* kolumny;
};

/*
Sprawdza, czy istnieje kolumna o danym numerze
Podaj element od którego nale¿y zacz¹æ szukanie,
numer szukanego elementu i pusty element
*/
int czy_istnieje_kolumna(struct TListaKolumn* Ielement, int k,
                         struct TListaKolumn** dummy) {
  struct TListaKolumn* element = Ielement;
  while (element->numer <= k) {
    if (element->numer == k) {
      *dummy = element;
      return 1;
    }
    element = element->next;
  }
  *dummy = element->prev;
  return 0;
}

/*
Sprawdza, czy istnieje wiersz o danym numerze
Podaj element od którego nale¿y zacz¹æ szukanie,
numer szukanego elementu i pusty element
*/
int czy_istnieje_wiersz(struct TListaWierszy* Ielement, int k,
                        struct TListaWierszy** dummy){
  struct TListaWierszy* element = Ielement;
  while (element->numer <= k) {
    if (element->numer == k) {
      *dummy = element;
      return 1;
    }
    element = element->next;
  }
  *dummy = element->prev;
  return 0;
}

//Wypisuje tablioê
void wypisz_tab(struct TListaWierszy* plansza) {
  struct TListaWierszy* dummy = plansza;
  struct TListaWierszy* ost_wiersz = plansza;
  for (int i = Y_kamery; i < Y_kamery + WIERSZE; i++) {
    if (czy_istnieje_wiersz(ost_wiersz, i, &dummy)) {
      struct TListaKolumn* ost_kol = dummy->kolumny;
      struct TListaKolumn* dummy2 = ost_kol;
      for (int j = X_kamery; j < X_kamery + KOLUMNY; j++) {
        if (czy_istnieje_kolumna(ost_kol, j, &dummy2)) {
          printf("%c", '0');
        }
        else
          printf("%c", '.');
        ost_kol = dummy2;
      }
    }
    else {
      for (int j = 0; j < KOLUMNY; j++) {
        printf("%c", '.');
      }
    }
    printf("%c", '\n');
    ost_wiersz = dummy;
  }
  for (int j = 0; j < KOLUMNY; j++) {
        printf("%c", '=');
      }
printf("%c", '\n');
}

//Usuwa dany wiersz
void usun_wiersz(struct TListaWierszy* element) {
  if (element->prev != NULL)
    element->prev->next = element->next;

  if (element->next != NULL)
    element->next->prev = element->prev;

  while (element->kolumny->numer != INT_MAX) {
    struct TListaKolumn* temp = element->kolumny;
    element->kolumny = element->kolumny->next;
    free(temp);
  }
  free(element->kolumny);
  free(element);
}

/*Dodaje wiersz
Podaj poprzedni element*/
void dodaj_wiersz(struct TListaWierszy** element) {

  struct TListaWierszy* nowy = malloc(sizeof(struct TListaWierszy));
  nowy->prev = *element;
  nowy->next = (*element)->next;
  (*element)->next = nowy;
  nowy->next->prev = nowy;
  nowy->kolumny = malloc(sizeof(struct TListaKolumn));

  nowy->kolumny->prev = NULL;
  nowy->kolumny->next = malloc(sizeof(struct TListaKolumn));
  nowy->kolumny->sasiedzi = 0;
  nowy->kolumny->numer = INT_MIN;
  nowy->kolumny->alive = 0;


  nowy->kolumny->next->prev = nowy->kolumny;
  nowy->kolumny->next->next = NULL;
  nowy->kolumny->next->sasiedzi = 0;
  nowy->kolumny->next->numer = INT_MAX;
  nowy->kolumny->next->alive = 0;

}
//Usuwa dan¹ kolumnê
void usun_kolumne(struct TListaKolumn* element) {
  if (element->prev != NULL)
    element->prev->next = element->next;

  if (element->next != NULL)
    element->next->prev = element->prev;

  free(element);
}

/*Dodaje kolumnê
Podaj poprzedni element*/
void dodaj_kolumne(struct TListaKolumn** element) {
  struct TListaKolumn* nowa = malloc(sizeof(struct TListaKolumn));
  nowa->prev = *element;
  nowa->next = (*element)->next;
  (*element)->next = nowa;
  if (nowa->next != NULL)
    nowa->next->prev = nowa;
}

//Usuwa martwe komórki
void usun_martwe(struct TListaWierszy* plansza) {
  (plansza) = (plansza)->next;
  while ((plansza)->numer != INT_MAX) {
    struct TListaKolumn* kol = (plansza)->kolumny->next;
    while (kol->numer != INT_MAX) {
      struct TListaKolumn* kol_temp = kol;
      if ((kol->sasiedzi == 3 || kol->sasiedzi == 2)
        && kol->alive) {
        kol->sasiedzi = 0;
        kol->alive = 1;
        kol = kol->next;
      }
      else if (kol->sasiedzi == 3 && kol->alive == 0) {
        kol->sasiedzi = 0;
        kol->alive = 1;
        kol = kol->next;
      }
      else {
        kol = kol->next;
        usun_kolumne(kol_temp);
      }
    }
    plansza = (plansza)->next;
  }
}

//Usuwa wiersze niezawieraj¹ce ¿adnych elementów
void usun_puste_wiersze(struct TListaWierszy* plansza) {

  plansza = (plansza)->next;
  while ((plansza)->numer != INT_MAX) {
    struct TListaWierszy* temp = (plansza)->next;
    if ((plansza)->kolumny->next->numer == INT_MAX) {
      usun_wiersz((plansza));
    }
    plansza = temp;
  }
}

//Tworzenie nowej generacji
void utworz_nowa_generacje(struct TListaWierszy* lista_wierszy) {
  struct TListaWierszy* elem = lista_wierszy->next;
  if (elem->numer != INT_MAX) {
    struct TListaWierszy* elem_nast = elem->next;
    
    //PrzejdŸ przez wszystkie wiersze
    while (elem->numer != INT_MAX) {
      int dodano_wiersz = 0;
      if (elem_nast->numer == elem->numer + 1) {
      }
      else {
        dodaj_wiersz(&elem);
        elem->next->numer = elem->numer + 1;
        elem_nast = elem->next;
        dodano_wiersz = 1;
      }
      
      struct TListaKolumn* kol = elem->kolumny->next;
      struct TListaKolumn* kol_nast = elem_nast->kolumny->next;
      struct TListaKolumn* ost_kol = kol_nast;
      struct TListaKolumn* dummy = ost_kol;
      struct TListaKolumn* nowa = dummy;
      while (kol->numer != INT_MAX) {
        if (kol->alive) {
          int k = kol->numer;
          //SprawdŸ czy w tym samym wierszu
          //istniej¹ komórki s¹siednie
          if(kol->prev->numer != k-1)
          {
          
            dodaj_kolumne(&(kol->prev));
            kol->prev->numer = k-1;
            kol->prev->sasiedzi = 1;
            kol->prev->alive=0;
          }
          else {
            kol->prev->sasiedzi += 1;
          }

          if(kol->next->numer != k+1)
          {
            dodaj_kolumne(&kol);
            kol->next->numer = k+1;
            kol->next->sasiedzi = 1;
            kol->next->alive=0;
          }else {
            kol->next->sasiedzi += 1;
          }

          if(elem_nast->numer !=INT_MAX){
            //SprawdŸ, czy istniej¹ komórki
            //s¹siednie w rzêdzie ni¿ej
            if (czy_istnieje_kolumna(ost_kol, k - 1, &dummy)) {
              dummy->sasiedzi += 1;
              nowa = dummy;
            }
            else{
              dodaj_kolumne(&dummy);
              nowa = dummy->next;

              nowa->alive = 0;
              nowa->numer = k - 1;
              nowa->sasiedzi = 1;
            }

            if (nowa->next->numer == k) {
              nowa->next->sasiedzi += 1;
            }
            else {
              dodaj_kolumne(&nowa);
              nowa->next->alive = 0;
              nowa->next->numer = k;
              nowa->next->sasiedzi = 1;
            }

            nowa = nowa->next;

            if (nowa->next->numer == k + 1) {
              nowa->next->sasiedzi += 1;
            }
            else {
              dodaj_kolumne(&nowa);
              nowa->next->alive = 0;
              nowa->next->numer = k + 1;
              nowa->next->sasiedzi = 1;
            }
            ost_kol = nowa;
          }
        }
        kol = kol->next;
      }
      elem = elem->next;
      elem_nast = elem_nast->next;
      if (dodano_wiersz) {
        elem = elem->next;
        elem_nast = elem_nast->next;
      }
    }
  }

  if (elem->numer != INT_MIN) {
    elem = elem->prev;
    struct TListaWierszy* elem_nast = elem->prev;
    //PrzejdŸ przez wszystkie wiersze
    while (elem->numer != INT_MIN) {
      int dodano_wiersz = 0;
      if (elem_nast->numer == elem->numer - 1) {
      }
      else {
        dodaj_wiersz(&(elem->prev));
        elem->prev->numer = elem->numer - 1;

        elem_nast = elem->prev;

        dodano_wiersz = 1;
      }
      struct TListaKolumn* kol = elem->kolumny->next;
      struct TListaKolumn* kol_nast = elem_nast->kolumny->next;
      struct TListaKolumn* ost_kol = kol_nast;
      struct TListaKolumn* dummy = ost_kol;
      struct TListaKolumn* nowa = dummy;
      while (kol->numer != INT_MAX) {
        if (kol->alive) {
          int k = kol->numer;
          //SprawdŸ, czy istniej¹ komórki
          //s¹siaduj¹ce w rzêdzie wy¿ej
          if (czy_istnieje_kolumna(ost_kol, k - 1, &dummy)) {
            dummy->sasiedzi += 1;
            nowa = dummy;
          }
          else
          {
            dodaj_kolumne(&dummy);
            nowa = dummy->next;

            nowa->alive = 0;
            nowa->numer = k - 1;
            nowa->sasiedzi = 1;
          }

          if (nowa->next->numer == k) {
            nowa->next->sasiedzi += 1;
          }
          else {
            dodaj_kolumne(&nowa);
            nowa->next->alive = 0;
            nowa->next->numer = k;
            nowa->next->sasiedzi = 1;
          }

          nowa = nowa->next;

          if (nowa->next->numer == k + 1) {
            nowa->next->sasiedzi += 1;
          }
          else {
            dodaj_kolumne(&nowa);
            nowa->next->alive = 0;
            nowa->next->numer = k + 1;
            nowa->next->sasiedzi = 1;
          }
          ost_kol = nowa;
        }
        kol = kol->next;
      }
      elem = elem->prev;
      elem_nast = elem_nast->prev;
      if (dodano_wiersz) {
        elem = elem->prev;
        elem_nast = elem_nast->prev;
      }
    }
  }
  struct TListaWierszy* temp = lista_wierszy;
  struct TListaWierszy* temp2 = lista_wierszy;
  usun_martwe(temp);
  usun_puste_wiersze(temp2);
}

//Tworzy pust¹ planszê
void stworz_pusta(struct TListaWierszy* plansza) {
  plansza->numer = INT_MIN;
  plansza->prev = NULL;
  plansza->next = malloc(sizeof(struct TListaWierszy));
  plansza->kolumny = malloc(sizeof(struct TListaKolumn));

  plansza->kolumny->numer = INT_MIN;
  plansza->kolumny->prev = NULL;
  plansza->kolumny->next = malloc(sizeof(struct TListaKolumn));
  plansza->kolumny->alive = 0;
  plansza->kolumny->sasiedzi = 0;

  plansza->kolumny->next->numer = INT_MAX;
  plansza->kolumny->next->prev = plansza->kolumny;
  plansza->kolumny->next->next = NULL;
  plansza->kolumny->next->alive = 0;
  plansza->kolumny->next->sasiedzi = 0;


  plansza->next->numer = INT_MAX;
  plansza->next->prev = plansza;
  plansza->next->kolumny = malloc(sizeof(struct TListaKolumn));
  plansza->next->next = NULL;

  plansza->next->kolumny->numer = INT_MIN;
  plansza->next->kolumny->prev = NULL;
  plansza->next->kolumny->next = malloc(sizeof(struct TListaKolumn));
  plansza->next->kolumny->alive = 0;
  plansza->next->kolumny->sasiedzi = 0;

  plansza->next->kolumny->next->numer = INT_MAX;
  plansza->next->kolumny->next->prev = plansza->kolumny;
  plansza->next->kolumny->next->next = NULL;
  plansza->next->kolumny->next->alive = 0;
  plansza->next->kolumny->next->sasiedzi = 0;
}

//Oblicza k-t¹ generacjê
void graj(struct TListaWierszy* plansza, int k) {
  for (int i = 1; i <= k; i++) {
    utworz_nowa_generacje(plansza);
  }
}

//Czyszczenie tablicy
void czysc_tab(struct TListaWierszy* plansza) {
  while (plansza->numer != INT_MAX)
  {
    struct TListaWierszy* temp = plansza;
    while (temp->kolumny->numer != INT_MAX) {
      struct TListaKolumn* temp2 = temp->kolumny;
      temp->kolumny = temp->kolumny->next;
      free(temp2);
    }
    free(temp->kolumny);
    plansza = plansza->next;
    free(temp);
  }
  free(plansza->kolumny->next);
  free(plansza->kolumny);
  free(plansza);
}

//Wykonuje zrzut wszystkich danych
void zrob_zrzut(struct TListaWierszy* plansza){
  plansza = plansza->next;
  while(plansza->numer != INT_MAX){
    printf("%c", '/');
    printf("%d", plansza->numer);
      struct TListaKolumn* kol = plansza->kolumny->next;
      while(kol->numer != INT_MAX){
        printf("%c", ' ');
        printf("%d", kol->numer);
        kol = kol->next;
      }
    plansza = plansza->next;
    printf("%c", '\n');
  }
  printf("%c", '/');
  printf("%c", '\n');
}

//Wczytywanie danych
void czytaj_dane(struct TListaWierszy* plansza) {
  struct TListaWierszy* plansza_pocz = plansza;
  char a;
  scanf("%c", &a);

  char b;
  scanf("%c", &b);
  while(b != '\n'){
    ungetc(b, stdin);
    int d;
    scanf("%d", &d);

    dodaj_wiersz(&plansza);
    plansza = plansza->next;
    plansza->numer = d;

    char c;
    scanf("%c", &c);
    struct TListaKolumn* temp = plansza->kolumny;
    while(c != '\n'){
      int e;
      scanf("%d", &e);
      dodaj_kolumne(&(temp));
      temp = temp->next;

      temp->numer = e;
      temp->alive = 1;
      temp->sasiedzi = 0;
      scanf("%c", &c);
    }
    scanf("%c", &a);
    scanf("%c", &b);
  }

  wypisz_tab(plansza_pocz);
  int gra = 1;
  while(gra){
    char k;
    int l;
    scanf("%c", &k);
    if(k == '\n'){
      graj(plansza_pocz, 1);
      wypisz_tab(plansza_pocz);
    }else if (k == '.'){
    gra = 0;
    }else{
      ungetc(k, stdin);
      scanf("%d", &l);
      scanf("%c", &k);
      if(k == '\n'){
        if(l == 0){
          zrob_zrzut(plansza_pocz);
          wypisz_tab(plansza_pocz);
        }
        else{
          graj(plansza_pocz, l);
          wypisz_tab(plansza_pocz);
        }
      }
      else{
        int p;
        scanf("%d", &p);
        Y_kamery = l;
        X_kamery = p;
        scanf("%c", &k);
        wypisz_tab(plansza_pocz);
      }
    }
  }
}

int main() {
  X_kamery = 1;
  Y_kamery = 1;
  struct TListaWierszy* plansza = malloc(sizeof(struct TListaWierszy));
  stworz_pusta(plansza);
  czytaj_dane(plansza);
  czysc_tab(plansza);
  return 0;
}