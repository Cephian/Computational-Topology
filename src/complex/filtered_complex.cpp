#include <vector>
#include <iostream>
#include <set>
#include "../globals.h"
#include "filtered_complex.h"

filtered_complex::filtered_complex(int dim) {
  while(grade.size() <= dim) {
    grade.push_back(std::set<simplex>());
    child_link.push_back(std::vector<std::vector<int> >());
    critical_value.push_back(std::vector<real>());
  }
  size=0;
}

void filtered_complex::print() {
  std::cout << "--- " << size << " total simplices ---\n";
  for(int i = 0; i < grade.size(); ++i)
    std::cout << "dim " << i << ": " << grade[i].size() << " simplices\n";
}

void filtered_complex::add(int dim, int* verts, real r) {
  simplex* fs = new simplex(dim,verts,grade[dim].size());
  if(grade[dim].count(*fs)) return;
  std::vector<int> children;
  children.reserve(dim+1);
  if(dim > 0) {
    for(int i = 0; i <= dim; ++i) {
      simplex* child = fs->child(i);

      std::set<simplex>::iterator it = grade[child->dim].find(*child);
      if(it == grade[child->dim].end()) {
        delete child;
        delete fs;
        return;
      }
      children.push_back(it->id);
      if(critical_value[dim-1][it->id] > r)
        r = critical_value[dim-1][it->id];
      delete child;
    }
  }
  child_link[dim].push_back(children);
  critical_value[dim].push_back(r);
  ++size;
  grade[dim].insert(*fs);
  delete fs;
}