#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <vector>
struct simplex {
  int dim,id;
  std::vector<int> verts;

  simplex(int d, int* v, int i);
  bool operator< (const simplex& s) const;
  simplex* child(int v);
  bool operator== (const simplex& fs) const;
  friend std::ostream& operator<<(std::ostream& o, const simplex& fs);
};
#endif