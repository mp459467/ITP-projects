#include <bits/stdc++.h>
#include "prev.h"
using namespace std;

struct tree {
    int ind;
    shared_ptr<tree> left, right;
    tree(int index) {
        ind = index;
        left = right = nullptr;
    }
};

std::vector<shared_ptr<tree>> v;
int counter = 0;

void pushBack(int y) {
    long int p = INT_MIN, k = INT_MAX, mid;
    shared_ptr<tree> drzewko = make_shared<tree>(counter);
    shared_ptr<tree> x = drzewko;
    shared_ptr<tree> poprzednie;
    if(counter) {
        poprzednie = v[counter - 1];
    }
    while(p != k) {
        mid = (p + k) / 2;
        if(p < 0) mid -= 1;
        if(mid < y) {
            if(counter) {
                if(poprzednie) drzewko->left = poprzednie->left;
                if(poprzednie) poprzednie = poprzednie->right;
            }
            drzewko->right = make_shared<tree>(counter);
            drzewko = drzewko->right;
            p = mid + 1;
        }
        else {
            if(counter) {
                if(poprzednie) drzewko->right = poprzednie->right;
                if(poprzednie) poprzednie = poprzednie->left;
            }
            drzewko->left = make_shared<tree>(counter);
            drzewko = drzewko->left;
            k = mid;
        }
    }
    v.push_back(x);
    counter++;
}

void init(const vector<int> &x) {
    for(int y : x) pushBack(y);
}

int prevInRange(int i, int lo, int hi) {
    long int p = INT_MIN, k = INT_MAX, mid;
    int wyn = -1;
    shared_ptr<tree> lowest = v[i]; // idziemy lowestem do wartosci lo
    shared_ptr<tree> highest = v[i]; // idziemy highestem do wartosci hi
    while(p != k && lowest) {
        if(p >= lo && k <= hi) { // mamy to co chielismy
            wyn = max(wyn, lowest->ind);
            break;
        }
        if(k < lo || p > hi) break; // nie dostaniemy czego chcemy
        mid = (p + k) / 2;
        if(p < 0) mid -= 1; // aby rowniez bralo podloge dla ujemnych
        if(mid < lo) {
            lowest = lowest->right;
            p = mid + 1;
        }
        else {
            /* za kazdym razem jak idziemy do lewego syna sprawdzamy,
            czy dla prawego syna przedzial sie zgadza i poprawiamy wynik,
            w ten sposob sprawdzamy indeksy w przedziale wartosci */
            if(mid + 1 >= lo && k <= hi && lowest->right) wyn = max(wyn, lowest->right->ind);
            lowest = lowest->left; 
            k = mid;
        }
    }
    if(lowest && p==k) wyn = max(wyn,lowest->ind);
    p = INT_MIN;
    k = INT_MAX;
    // analogicznie dla drugiego krancu przedzialu
    while(p != k && highest) {
        if(p >= lo && k <= hi) {
            wyn = max(wyn, highest->ind);
            break;
        }
        if(k < lo || p > hi) break;
        mid = (p + k) / 2;
        if(p < 0) mid -= 1;
        if(mid < hi) {
            if(p >= lo && mid <= hi && highest->left) wyn = max(wyn, highest->left->ind);
            highest = highest->right;
            p = mid + 1;
        }
        else {
            highest = highest->left;
            k = mid;
        }
    }
    if(highest && p == k) wyn = max(wyn, highest->ind);
    return wyn;
}

void done() {
    counter = 0;
    v.clear();
}