#include <iostream>
#include "simplex.h"

//simplex constructor
simplex::simplex(int d, int* v, int i) {
  dim = d;
  verts.reserve(d+1);
  for(int j = 0; j <= d; ++j)
    verts.push_back(v[j]);
  id = i;
}

bool simplex::operator< (const simplex& s) const {
  if(dim == s.dim) {
    for(int i = 0; i <= dim; ++i) {
      if(verts[i] == s.verts[i]) continue;
      return verts[i] < s.verts[i];
    }
    return 0;
  }
  return dim < s.dim;
}

simplex* simplex::child(int v) {
  int* cverts = new int[dim];

  for(int i = 0; i < dim; ++i)
    cverts[i] = verts[i + (i >= v)];

  simplex* cd = new simplex(dim-1,cverts,0);
  delete[] cverts;
  return cd;
}

bool simplex::operator== (const simplex& fs) const {
  if(dim != fs.dim) return 0;
  for(int i = 0; i <= dim; ++i)
    if(verts[i] != fs.verts[i]) return 0;
  return 1;
}

std::ostream& operator<<(std::ostream& o, const simplex& fs) {
  o << "{";
  for(int i = 0; i <= fs.dim; ++i) {
    o << fs.verts[i];
    if(i != fs.dim) o << ",";
  }
  o << "}";
  return o;
}