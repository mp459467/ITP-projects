#include <stdbool.h>
#include <math.h>
#include "ary.h"

bool rowne(double a, double b)
{
  return fabs(a - b) < 1e-10;
}

bool iszero(double x)
{
  return rowne(x, 0.0);
}

bool wieksze_rowne(double a, double b)
{
  return iszero(a - b) || a > b;
}

bool pusty(wartosc a)
{
  return isnan(a.pocz) && isnan(a.kon);
}

bool czy_przedzial(wartosc a) 
{
  return wieksze_rowne(a.kon, a.pocz);
}

bool przedzialy(wartosc a, wartosc b)
{
  return czy_przedzial(a) && czy_przedzial(b);
}

bool anty_przedzialy(wartosc a, wartosc b)
{
  return !pusty(a) && !pusty(b) && !czy_przedzial(a) && !czy_przedzial(b);
}

void sortowanie(double t[], int n)
{
  for(int i = 0; i < n; i++)
  {
    for(int j = n - 1; j > 0; j--)
    {
      if(wieksze_rowne(t[j - 1], t[j]))
      {
        double temp = t[j];
        t[j] = t[j - 1];
        t[j - 1] = temp;
      }
    }
  } 
}

wartosc wartosc_dokladnosc(double x, double p)
{
  wartosc c;
  c.pocz = x - fabs(p * x / 100.0);
  c.kon = x + fabs(p * x / 100.0);
  return c;
}

wartosc wartosc_od_do(double x, double y)
{
  wartosc c;
  c.pocz = x;
  c.kon = y;
  return c;
}

wartosc wartosc_dokladna(double x)
{
  wartosc c;
  c.pocz = x;
  c.kon = x;
  return c;
}

bool in_wartosc(wartosc w, double x)
{
  if(czy_przedzial(w))
  {
    return wieksze_rowne(x, w.pocz) && wieksze_rowne(w.kon, x);

  }
  else
  {
    return wieksze_rowne(x, w.pocz) || wieksze_rowne(w.kon, x);
  }
}

double min_wartosc(wartosc w)
{
  if(pusty(w))
  {
    return NAN;  
  }
  else if(czy_przedzial(w))
  {
    return w.pocz;
  }
  else
  {
    return -HUGE_VAL;
  }
}

double max_wartosc(wartosc w)
{
  if(pusty(w))
  {
    return NAN;  
  }
  else if(czy_przedzial(w))
  {
    return w.kon;
  }
  else
  {
    return HUGE_VAL;
  }
}

double sr_wartosc(wartosc w)
{
  return (min_wartosc(w) + max_wartosc(w)) / 2.0;
}

wartosc plus(wartosc a, wartosc b)
{
  wartosc c;
  if(przedzialy(a, b))
  {
    c.pocz = a.pocz + b.pocz;
    c.kon = a.kon + b.kon;
  }
  else if(anty_przedzialy(a, b))
  {
    c.pocz = -HUGE_VAL;
    c.kon = HUGE_VAL;
  }
  else // jeden antyprzedzial, drugi przedzial
  {   
    c.pocz = a.pocz + b.pocz;
    c.kon = a.kon + b.kon;
    if(czy_przedzial(c)) // wynikiem takiego dodawania jest antyprzedzial, wiec jesli pocz >= kon, to wynikiem jest R
    {
      c.pocz = -HUGE_VAL;
      c.kon = HUGE_VAL;
    }
  }
  if(iszero(c.pocz)) // nie chcemy zeby wyszlo nam -0.0
  {
    c.pocz = 0.0;
  }
  if(iszero(c.kon))
  {
    c.kon = 0.0;
  }
  return c;
}

wartosc przeciwny(wartosc x)
{
  wartosc c;
  c.pocz = -x.kon;
  c.kon = -x.pocz;
  return c;
}

wartosc minus(wartosc a, wartosc b)
{
  return plus(a, przeciwny(b));
}

wartosc razy(wartosc a, wartosc b)
{
  if(pusty(a) || (iszero(a.pocz) && iszero(a.kon)))
  {
    return a;
  }
  else if(pusty(b) || (iszero(b.pocz) && iszero(b.kon)))
  {
    return b;
  }
  wartosc c;
  // tworze i sortuje tabelke, wszystkich mozliwych mnozen
  double t[] = {a.pocz * b.pocz, a.pocz * b.kon, a.kon * b.pocz, a.kon * b.kon};
  for(int i = 0; i < 4; i++)
  {
    if(isnan(t[i]) || iszero(t[i])) // zabezpieczenie przed -0.0 i przypadkiem inf * 0 = nan
    {
      t[i] = 0.0;
    }
  }
  int n = sizeof(t) / sizeof(t[0]);
  sortowanie(t, n);
  if(przedzialy(a, b))
  {
    c.pocz = t[0]; //skrajne wartosci
    c.kon = t[3];
  }
  /* dowolny antyprzedzial mnozony przec cos co zawiera 0, daje wynik R, */
  else if((!czy_przedzial(a) && in_wartosc(b, 0.0)) || (!czy_przedzial(b) && in_wartosc(a, 0.0)))
  {
    c.pocz = -HUGE_VAL;
    c.kon = HUGE_VAL;
  }
  else // gdy conajmniej jeden jest antyprzedzialem z wylaczeniem przypadku powyzej
  {
    c.pocz = t[2]; // wartosci nieskrajne
    c.kon = t[1];
    if(rowne(t[1], t[2]))
    { // jesli sa rowne no to mamy R
      c.pocz = -HUGE_VAL;
      c.kon = HUGE_VAL;
    }
  }
  return c;
}

wartosc odwrotny(wartosc x)
{
  wartosc c;
  c.pocz = 1.0 / x.kon;
  c.kon = 1.0 / x.pocz;
    if(iszero(x.pocz) && iszero(x.kon))
    { // nie mozna dzielic przez wartosc dokladnie rowna zero
      c.pocz = NAN;
      c.kon = NAN;
    }
    else if(iszero(x.pocz))
    {
      c.kon = HUGE_VAL;
    }
    else if(iszero(x.kon))
    {
      c.pocz = -HUGE_VAL;
    }
    else if(isinf(x.pocz) && isinf(x.kon))
    { // odwrotnosc R to R
      c.pocz = -HUGE_VAL;
      c.kon = HUGE_VAL;
    }
  return c;
}

wartosc podzielic(wartosc a, wartosc b)
{
  return razy(a, odwrotny(b));
}