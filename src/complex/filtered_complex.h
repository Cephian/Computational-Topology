#ifndef FILTERED_COMPLEX_H
#define FILTERED_COMPLEX_H

#include <vector>
#include <set>
#include "simplex.h"

// represents an R-persistence filtrated simplicial complex
struct filtered_complex {
  std::vector<std::set<simplex> > grade;
  std::vector<std::vector<std::vector<int> > > child_link;
  std::vector<std::vector<real> > critical_value;
  int size;

  filtered_complex(int dim);
  void print();
  void add(int dim, int* verts, double r);

};

#endif