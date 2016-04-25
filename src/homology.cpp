#include <cmath>
#include <algorithm>
#include <iostream>
#include "homology.h"

void list_xor(const std::vector<int>& a, std::vector<int>&b);

struct filtered_simplex {
  const simplex* spx;
  real crit;

  filtered_simplex();
  filtered_simplex(real c, const simplex* s);
  bool operator< (const filtered_simplex& f) const;
};

filtered_simplex::filtered_simplex() {}

filtered_simplex::filtered_simplex(real c, const simplex* s) {
  spx = s;
  crit = c;
}

bool filtered_simplex::operator< (const filtered_simplex& f) const {
  if(std::abs(crit - f.crit) < EPS)
    return *spx < *f.spx;
  return crit < f.crit;
}

void list_xor(const std::vector<int>& a, const std::vector<int>&b, std::vector<int>& c) {
  c.clear();
  int i=0,j=0;
  while(i < a.size() || j < b.size()) {
    if(i == a.size())
      c.push_back(b[j++]);
    else if(j == b.size())
      c.push_back(a[i++]);
    else if(a[i] == b[j]) {
      ++i;
      ++j;
    } else if(a[i] > b[j])
      c.push_back(a[i++]);
    else
      c.push_back(b[j++]);
  }
}

void prv(const std::vector<int>& v) {
  std::cout << "[";
  for(int i = 0; i < v.size(); ++i) {
    std::cout << v[i];
    if(i !=v.size()-1) std::cout << " ";
  }
  std::cout << "]";
}

std::vector<std::pair<real,real> >* barcode(filtered_complex* fc) {
  filtered_simplex* order = new filtered_simplex[fc->size+1];
  int sz = 0;
  for(int i = 0; i < fc->grade.size(); ++i)
    for(std::set<simplex>::iterator j = fc->grade[i].begin(); j != fc->grade[i].end(); ++j)
      order[sz++] = filtered_simplex(fc->critical_value[i][j->id], &(*j));
  simplex* nl = new simplex(-1,&sz,0);
  order[sz++] = filtered_simplex(0,nl);
  std::sort(order,order+sz);

  int** remap = new int*[fc->grade.size()];
  for(int i = 0; i < fc->grade.size(); ++i)
    remap[i] = new int[fc->grade[i].size()];
  for(int i = 1; i < sz; ++i)
    remap[order[i].spx->dim][order[i].spx->id] = i;

  std::vector<int>* R = new std::vector<int>[fc->size+1];
  std::vector<int> v0,w0;
  std::vector<int> *v=&v0, *w=&w0;
  std::vector<std::pair<real,real> >* answer = new std::vector<std::pair<real,real> >[fc->grade.size()-1];
  bool* birth = new bool[sz];
  real* death = new real[sz];
  for(int i = 1; i < sz; ++i) {
    v->clear();
    int dm = order[i].spx->dim;
    if(dm == 0)
      v->push_back(0);
    else {
      std::vector<int>& children = fc->child_link[dm][order[i].spx->id];
      for(int j = 0; j < children.size(); ++j)
        v->push_back(remap[dm-1][children[j]]);
    }
    std::sort(v->rbegin(),v->rend());
    while(!v->empty() && !R[(*v)[0]].empty()) {
      list_xor(R[(*v)[0]],*v,*w);
      swap(v,w);
    }
    if(v->empty()) {
      birth[i]=1;
      death[i]=INF;
    } else {
      birth[i]=0;
      int j = (*v)[0];
      R[j]=*v;
      death[j]=order[i].crit;
    }
  }
  delete nl;
  for(int i = 0; i < fc->grade.size(); ++i)
    delete[] remap[i];
  delete[] remap;
  delete[] R;

  for(int i = 1; i < sz; ++i) {
    if(!birth[i] || order[i].spx->dim == fc->grade.size()-1) continue;
    real a = order[i].crit,b=death[i];
    if(std::abs(a-b)<EPS) continue;
    answer[order[i].spx->dim].push_back(std::make_pair(a,b));
  }
  delete[] birth;
  delete[] death;
  delete[] order;
  return answer;
}