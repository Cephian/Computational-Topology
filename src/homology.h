#ifndef HOMOLOGY_H
#define HOMOLOGY_H

#include "globals.h"
#include "complex/filtered_complex.h"

typedef std::pair<double,double> pdd;

struct filtered_simplex {
  const simplex* spx;
  real crit;

  filtered_simplex();
  filtered_simplex(real c, const simplex* s);
  bool operator< (const filtered_simplex& f) const;
};

//calculate barcode of a filtered complex
std::vector<pdd>* barcode(filtered_complex* fc);

#endif