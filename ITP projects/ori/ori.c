#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct pkt pkt;
struct pkt {
    double x;
    double y;
    int akt; // 0 - nieaktywny punkt, 1 - aktywny
};

typedef struct figura figura;
struct figura {
    char rodzaj; // w ten sposob rozrozniamy kartke pomiedzy Z, P a K
    pkt p1; 
    pkt p2; // jesli to kolo to p2 = (r,r)
    int nr; // zmienna potrzebna wylacznie dla typu Z, ktora wskazuje na poprzednia figure jaka skladamy
};


typedef struct pytanie pytanie;
struct pytanie {
    int nr;
    pkt P;
};

typedef struct prosta prosta;
struct prosta {
    pkt P1;
    pkt P2;
};

int pow2(int n) {
    return 1 << n;
}

bool rowne(double a, double b) {
    return fabs(a - b) < 1e-10;
}

bool wieksze_rowne(double a, double b) {
    return rowne(a, b) || a > b;
}

pkt odwroc_pkt_wzg_prostej(prosta z, pkt q) {
    pkt wyn;
    pkt przeciecie;
    if(rowne(z.P1.x, z.P2.x)) { // pionowa
        wyn.x = 2 * z.P1.x - q.x;
        wyn.y = q.y;
        return wyn;
    }
    if(rowne(z.P1.y, z.P2.y)) { // pozioma
        wyn.x = q.x;
        wyn.y = 2 * z.P1.y - q.y;
        return wyn;
    }
    double a = (z.P1.y - z.P2.y) / (z.P1.x - z.P2.x); 
    double b = z.P1.y - (a * z.P1.x); // y = ax + b
    double aprost = -1 / a; 
    double bprost = q.y - (aprost * q.x); ; // b dla prostej prostopadlej
    przeciecie.x = (bprost - b) / (a - aprost);
    przeciecie.y = a * przeciecie.x + b;
    wyn.x = 2 * przeciecie.x - q.x;
    wyn.y = 2 * przeciecie.y - q.y;
    return wyn;
}

bool czy_nalezy_do(figura z, pkt q) { 
    if(z.rodzaj == 'K') { // sprawdzamy czy promien jest wiekszy rowny odl pomiedzy srodkiem a pkt
        double odleglosc = sqrt(pow(z.p1.x - q.x, 2)  + pow(z.p1.y - q.y, 2));
        return wieksze_rowne(z.p2.x, odleglosc); 
    }
    else { //rodzaj = P
        return wieksze_rowne(q.x, z.p1.x) && wieksze_rowne(z.p2.x, q.x)
            && wieksze_rowne(q.y, z.p1.y) && wieksze_rowne(z.p2.y, q.y);
    }
}

bool czy_po_prawej(prosta z, pkt q) { // () := Prawa patrzac w kierunku od P1 do P2
    if(rowne(z.P1.x, z.P2.x)) { // pionowa
        if(z.P1.y > z.P2.y)
            return q.x < z.P1.x; // () jest po lewej
        return z.P1.x < q.x; // () jest po prawej
    }
    double a = (z.P1.y - z.P2.y) / (z.P1.x - z.P2.x); 
    double b = z.P1.y - (a * z.P1.x); // y = ax + b
    if(z.P1.x < z.P2.x)
        return q.y < a * q.x + b;  // () jest nad
    return q.y > a * q.x + b ; // () jest pod
}

int zlozenie(figura z, figura t[], pkt punkty[]) {
    /* zamiast skladac figure, odbijamy punkty wzgledem prostych, i robimy to w odwrotnej kolejnosci
     (od ostatniego zlozenia, do pierwszego),
    tym sposobem uzyskujemy zbior punktow na plaszczyznie, sprawdzamy ktore znajduja sie w naszej figurze, i ta liczba
    odzwierciedla ilosc warstw w punkcie wbicia szpilki
    */
    int rozmiar = 1;
    punkty[0].akt = 1;
    while(z.rodzaj == 'Z') {
        prosta kr;
        kr.P1.x = z.p1.x;
        kr.P1.y = z.p1.y;
        kr.P2.x = z.p2.x;
        kr.P2.y = z.p2.y;
        for(int i = 0; i < rozmiar; i++) {
            pkt odwrocony = odwroc_pkt_wzg_prostej(kr, punkty[i]);
            odwrocony.akt = 1;
            /*jesli punkt jest po prawej to nie bedziemy go liczyc (bo idziemy od konca), jesli po lewej,
             dodajemy do zbioru punktow jego odbicie wzg prostej, a jesli jest na prostej
            punkt zostaje jak jest bo nie chcemy liczyc go dwa razy*/
            if(czy_po_prawej(kr, punkty[i])) {
                punkty[i].akt = 0; //dezaktywujemy punkty bo nie chcemy ich liczyc
                odwrocony.akt = 0;
            }
            if(((rowne(punkty[i].x, odwrocony.x) && rowne(punkty[i].y, odwrocony.y)) || punkty[i].akt == 0)) 
                odwrocony.akt = 0;
            punkty[i + rozmiar] = odwrocony;
        }
        rozmiar *= 2;
        z = t[z.nr]; // po opisie (Z) kartki przekierowujemy na opis na ktory wskazuje jej nr
    }
    int wyn = 0;
    for(int i = 0; i < rozmiar; i++) {
        if(punkty[i].akt == 1 && czy_nalezy_do(z, punkty[i]))
            wyn++;
    }
    return wyn;
}

int main() {   
    int n;
    int q;
    scanf("%d %d", &n, &q);
    figura *figury = (figura *)malloc(sizeof(figura) * (size_t)n);
    pytanie *pytanka = (pytanie *)malloc(sizeof(pytanie) * (size_t)q);
    for(int i = 0; i < n; i++) { //wczytywanie figur
        figura fig;
        char jaki;
        double a, b, c, d;
        int z;
        scanf(" %c", &jaki);
        if(jaki == 'K') {
            scanf("%lf %lf %lf", &a, &b, &c);
            fig.p2.y = c;
            fig.nr = 0;
        }
        if(jaki == 'P') {
            scanf("%lf %lf %lf %lf", &a, &b, &c, &d);
            fig.p2.y = d;
            fig.nr = 0;
        }
        if(jaki == 'Z') {
            scanf("%d %lf %lf %lf %lf", &z, &a, &b, &c, &d);
            fig.nr = z - 1;
            fig.p2.y = d;
        }
        fig.rodzaj = jaki;
        fig.p1.x = a;
        fig.p1.y = b;
        fig.p2.x = c;
        figury[i] = fig;
    }
    for(int i = 0; i < q; i++) { //wczytywanie pytan
        int nr;
        double x, y;
        pytanie pyt;
        scanf("%d %lf %lf", &nr, &x, &y);  
        pyt.nr = nr - 1;
        pyt.P.x = x;
        pyt.P.y = y;
        pytanka[i] = pyt;
    }
    for(int j = 0; j < q; j++) {
        figura fig = figury[pytanka[j].nr];
        if(fig.rodzaj != 'Z') { // dla pytania o zwykla kartke poprostu sprawdzamy czy punkt nalezy do figury
            if(czy_nalezy_do(fig, pytanka[j].P))
                printf("%d\n", 1);
            else
                printf("%d\n", 0);
        }
        else {
            int size = pow2(n);
            pkt pusty;
            pusty.x = 0;
            pusty.y = 0;
            pusty.akt = 0;
            pkt* zbior = (pkt *)malloc(sizeof(pkt) * (size_t)size);
            for(int i = 1; i < size; i++)
                zbior[i] = pusty; // wypelniamy tablice nieaktywnymi punktami (smieci)
            zbior[0] = pytanka[j].P;
            printf("%d\n", zlozenie(fig, figury, zbior));
            free(zbior);
        }
    }
    free(figury);
    free(pytanka);
    return 0;
}

