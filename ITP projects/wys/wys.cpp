#include "wys.h"

#include <iostream>
#include <limits.h>
#include <map>
#include <vector>

using std::pair;
using std::vector;

// w mapie trzymamy wektory posiadajace dokladnie n elementow
// ktore zawieraja sie w przedziale {0,1,...,k+1}
// wartosc w i-tym elemencie wektora odpowiada za
// liczbe klamstw Ali jezeli x = i
// w parze trzymamy odpowiednio
// indeks o jaki trzeba zadac nastepne pytanie
// oraz optymalna liczbe pytan jakie musimy zadac
// czyli nasza mapa to baza rozwiazan dla kazdej sytuacji
std::map<vector<int>, pair<int, int>> database;
int n, k;

vector<int> YesNo(vector<int> situation, bool yes, int ind) {  
    vector<int> newSit = situation;
    
    if (yes) {
        for (int j = ind; j < n; j++) {
            newSit[j] = std::min(newSit[j] + 1, k + 1);
        }
    } else {  // no
        for (int j = 0; j < ind; j++) {
            newSit[j] = std::min(newSit[j] + 1, k + 1);
        }
    }
    return newSit;
}

pair<int, int> searching(vector<int> situation) {
    // przechowujemy juz wynik dla tej sytuacji w bazie rozwiazan
    if (database.find(situation) != database.end()) return database.at(situation);

    int counter = 0, solution = INT_MAX, ind = -1, ans = -1;

    for (int i = 0; i < n; i++) {
        if (situation[i] != k + 1) counter++;
        if (situation[i] != k + 1 && ans == -1) ans = i;
    }

    if (counter == 1) {  // mamy tylko jedna mozliwa opcje
        database[situation] = {ans, 0};
        return {ans, 0};
    }

    if (counter == 0) {  // wynik niemozliwy do uzyskania
        database[situation] = {-1, INT_MAX};
        return {-1, INT_MAX};
    }

    for (int i = 1; i < n; i++) {
        vector<int> Y = YesNo(situation, true, i);
        vector<int> N = YesNo(situation, false, i);
        // unikamy zapetlen
        if (std::equal(situation.begin(), situation.end(), Y.begin()) ||
            std::equal(situation.begin(), situation.end(), N.begin())) continue;
        // minimax - chcemy najoptymalniej w najgorszej sytuacji
        // dodajemy jeden bo kazda odpowiedz ani, to nasz jeden ruch
        int res = std::max(searching(N).second + 1, searching(Y).second + 1);
        if (res < solution) {
            solution = res;
            ind = i;
        }
    }
    database[situation] = {ind, solution};
    return {ind, solution};
}

void play() {
    vector<int> v(n, 0);
    pair<int, int> q = searching(v);
    while (q.second) {
        // idziemy po najlepszej dla nas sciezce pytan
        bool Ala = mniejszaNiz(q.first + 1);
        v = YesNo(v, Ala, q.first);
        q = searching(v);
    }
    odpowiedz(q.first + 1);
    return;
}

int main() {
    int g;
    dajParametry(n, k, g);
    while (g--) {
        play();
    }
}