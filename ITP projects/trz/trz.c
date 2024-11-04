#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    if(a > b) {
        return a;
    }
    return b;
}

int min(int a, int b) {
    if(a > b) {
        return b;
    }
    return a;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void zwezanie(int a, int b, int c, int *x, int *y, int odl[], int nr[], int* wyn) {
    *wyn = min(*wyn, max(odl[b] - odl[a], odl[c] - odl[b]));
    int p = b;
    /* jesli w przedziale pomiedzy b..c znajduje sie motel o
     nr a, to zawezamy przedzial do b..a..c i zamieniamy a z b, aktualizujemy wynik, i tak dalej
    */
    for(int i = p + 1; i < c; i++) {
        if(nr[a] == nr[i]) {
            a = i;
            swap(&a, &b);
            *wyn = min(*wyn, max(odl[b] - odl[a], odl[c] - odl[b]));
        }
    }
    *x = b - 1; // moze byc tak ze zostaniemy z z przedzialem a...abc
    *y = b;
}

void wyrownanie(int a, int b, int c, int *y, int odl[], int nr[], int* wyn) {
    *wyn = min(*wyn, max(odl[b] - odl[a], odl[c] - odl[b]));
    int p = b + 1;
    /* dla przedzialu  a..b....c w ktorym odl pomiedzy b i c jest wieksza niz pomiedzy a i b, szukamy takich moteli pomiedzy b i c,
    aby moc wyrownac odleglosc pomiedzy a, b i c
    */
    while(p < c) {
        if(nr[p] != nr[a] && nr[p] != nr[c]) {
            if(min(odl[p] - odl[a], odl[c] - odl[p]) <= *wyn) {
                *y = p;
                *wyn = min(*wyn, max(odl[p] - odl[a], odl[c] - odl[p]));
            }
        }
        p++;
    }
}

int najblizsza(int a, int b, int c, int nr[], int odl[], int n) {
    int i = c + 1;
    int wyn = odl[n - 1];
    zwezanie(a, b, c, &a, &b, odl, nr, &wyn);
    wyrownanie(a, b, c, &b, odl, nr, &wyn);
    while(i < n) { // gdy znajdziemy nowy rozny to przesuwamy wszystkie 3 indeksy, zwezamy i wyrownujemy
        if(nr[i] != nr[b] && nr[i] != nr[c]) {
            swap(&a, &c);
            swap(&b, &a);
            c = i;
            zwezanie(a, b, c, &a, &b, odl, nr, &wyn);
            wyrownanie(a, b, c, &b, odl, nr, &wyn);
        }
        i++;
    }
    return wyn;
}

int najdalsza(int a, int b, int c, int nr[], int odl[], int n) {
    int wyn = 0;
    int i = 2;
    int d = n - 1;
    int e = -1;
    int f = -1; 
    while(i < n && e < 0) {
        if(nr[n - i] != nr[d] ) {
            e = n - i;
        }
        i++;
    }
    while(i <= n && f < 0) {
        if(nr[n - i] != nr[d] && nr[n - i] != nr[e]) {
            f = n - i;
        }
        i++;
    } // d, e, f - ostatnie 3 rozne motele o roznych nr
    int t[] = {a, b, c, d, e, f};
    int pocz;
    int kon;
    int indeks;
    for(int x = 0; x < 3; x++) {
        for(int y = 5; y > 2; y--) {
            if(t[x] < t[y]) {
                pocz = t[x];
                kon = t[y];
            }
            else {
                kon = t[x];
                pocz = t[y];
            }
            indeks = pocz;
            while(indeks < kon) {
                if(nr[indeks] != nr[kon] && nr[indeks] != nr[pocz] && nr[pocz] != nr[kon]) {
                    wyn = max(min(odl[indeks] - odl[pocz], odl[kon] - odl[indeks]), wyn);
                }
                indeks++;
            }  
        }
    }
    return wyn;
}

int main() {   
    int n;
    scanf("%d", &n);
    int *odl = (int *)malloc(sizeof(int) * (size_t)n);
    int *nr = (int *)malloc(sizeof(int) * (size_t)n);
    for(int v = 0; v < n; v++) {
        int x;
        int y;
        scanf("%d %d", &x, &y);
        nr[v] = x;
        odl[v] = y;
    }
    int i = 0;
    int a = 0;
    int b = -1;
    int c = -1; // indeksy moteli
    while(i < n && b < 0) {
        if(nr[i] != nr[a] ) {
            b = i;
        }
        i++;
    }
    while(i < n && c < 0) {
        if(nr[i] != nr[a] && nr[i] != nr[b]) {
            c = i;
        }
        i++;
    } // mamy pierwsze trzy hotele o roznych nr
    if(c == -1) { // bajtek sie pomylil
        printf("%d %d\n", 0, 0);
    }
    else {
        printf("%d %d\n", najblizsza(a, b, c, nr, odl, n), najdalsza(a, b, c, nr, odl, n));
    }
    free(odl);
    free(nr);
    return 0;
}