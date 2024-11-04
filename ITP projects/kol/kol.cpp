#include <cstdlib>
#include <cstdio>
#include "kol.h"

using namespace std;

namespace {

typedef struct lista {
  interesant *head, *tail;
} lista;

lista create_list() {
  lista res;
  res.head = (interesant*)malloc(sizeof(interesant));
  res.tail = (interesant*)malloc(sizeof(interesant));
  res.head->l1 = NULL;
  res.head->l2 = res.tail;
  res.tail->l1 = NULL;
  res.tail->l2 = res.head;
  return res;
}

bool isempty(const lista l) {
  return l.head->l2 == l.tail;
}

interesant *front(const lista l) {
  return l.head->l2;
}

interesant *back(const lista l) {
  return l.tail->l2;
}

void link(interesant *el, interesant *old, interesant *nw) {
  if(el) {
    if(el->l1 == old) {
      el->l1 = nw;
    }
    else {
      el->l2 = nw;
    }
  }
}

void remove_interesant(interesant *el) {
  link(el->l1, el, el->l2);
  link(el->l2, el, el->l1);
  el->l1 = NULL; // numerek musi zostac
  el->l2 = NULL;
}

void push_back(lista &l, int x) {
  interesant *el = (interesant*)malloc(sizeof(interesant));
  el->val = x;
  el->l1 = l.tail;
  el->l2 = l.tail->l2;
  link(l.tail->l2, l.tail, el);
  l.tail->l2 = el;
}

void pop_front(lista &l) {
  if(!isempty(l))
    remove_interesant(l.head->l2);
}

void reverse(lista &l) {
  interesant *tmp = l.head;
  l.head = l.tail;
  l.tail = tmp;
}

void append(lista &l1, lista &l2) {
  interesant *b = l1.tail;
  interesant *f = l2.head;
  link(b->l2, b, f->l2);
  link(f->l2, f, b->l2);
  b->l2 = f;
  f->l2 = b;
  l1.tail = l2.tail;
  l2.head = f;
  l2.tail = b;
}

}

vector<lista> kolejki;

int akt_numerek;


void otwarcie_urzedu(int m) {
  kolejki.resize(m);
  for(int i = 0; i < m; i++)
    kolejki[i] = create_list();
  akt_numerek = 0;
}

interesant *nowy_interesant(int k) {
  push_back(kolejki[k], akt_numerek++);
  return back(kolejki[k]);
}

int numerek(interesant *i) {
  return i->val;
}

interesant *obsluz(int k) {
  if(isempty(kolejki[k]))
    return NULL;
  interesant *i = front(kolejki[k]);
  pop_front(kolejki[k]);
  return i;
}

void zmiana_okienka(interesant *i, int k) {
  remove_interesant(i);
  i->l1 = kolejki[k].tail;
  i->l2 = kolejki[k].tail->l2;
  link(kolejki[k].tail->l2, kolejki[k].tail, i);
  kolejki[k].tail->l2 = i;
}

void zamkniecie_okienka(int k1, int k2) {
  append(kolejki[k2], kolejki[k1]); 
}

vector<interesant *> fast_track(interesant *i1, interesant *i2) {
  vector<interesant *> wyn;
  vector<interesant *> wyn2;
  wyn.push_back(i1);
  if(i1 == i2) {
    remove_interesant(i1);
    return wyn;
  }
  wyn2.push_back(i1);
  interesant *glowny;
  interesant *sasiad;
  interesant *temp;
  glowny = i1;
  sasiad = i1->l1;
  temp = sasiad;
  while(sasiad && sasiad != i2) {
    wyn.push_back(sasiad);
    if(glowny == sasiad->l2)
      sasiad = sasiad->l1;
    else
      sasiad = sasiad->l2;
    glowny = temp;
    temp = sasiad;
  }

  if(sasiad == i2) {
    wyn.push_back(i2);
    for(interesant *el : wyn)
      remove_interesant(el);
    wyn2.clear();
    return wyn;
  }
  glowny = i1;
  sasiad = i1->l2;
  temp = sasiad;
  while(sasiad != i2) {
    wyn2.push_back(sasiad);
    if(glowny == sasiad->l2)
      sasiad = sasiad->l1;
    else
      sasiad = sasiad->l2;
    glowny = temp;
    temp = sasiad;
  }
  wyn2.push_back(i2);
  for(interesant *el : wyn2)
    remove_interesant(el);
  wyn.clear();
  return wyn2;
}

void naczelnik(int k) {
  reverse(kolejki[k]);
}

vector<interesant *> zamkniecie_urzedu() {
  vector<interesant *> wyn;
  for(int k = 0; (size_t)k < kolejki.size(); k++) {
    while(!isempty(kolejki[k])){
      interesant *x = obsluz(k);
      wyn.push_back(x);
    }
    free(kolejki[k].head);
    free(kolejki[k].tail);
  }
  return wyn;
}