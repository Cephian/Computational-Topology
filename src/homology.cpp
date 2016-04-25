#include <cmath>
#include <algorithm>
#include "homology.h"
#include "globals.h"

using namespace std;

filtered_simplex::filtered_simplex() {}

filtered_simplex::filtered_simplex(real c, const simplex* s) {
  spx = s;
  crit = c;
}

bool filtered_simplex::operator< (const filtered_simplex& f) const {
  if(abs(crit - f.crit) < EPS)
    return *spx < *f.spx;
  return crit < f.crit;
}

//calculate barcode of a filtered complex
std::vector<pdd>* barcode(filtered_complex* fc) {
  filtered_simplex* order = new filtered_simplex[fc->size+1];
  int sz = 0;
  for(int i = 0; i < fc->grade.size(); ++i)
    for(std::set<simplex>::iterator j = fc->grade[i].begin(); j != fc->grade[i].end(); ++j)
      order[sz++] = filtered_simplex(fc->critical_value[i][j->id], &(*j));
  simplex* nl = new simplex(-1,&sz,0);
  order[sz++] = filtered_simplex(0,nl);
  sort(order,order+sz);

  int** remap = new int*[fc->grade.size()];
  for(int i = 0; i < fc->grade.size(); ++i)
    remap[i] = new int[fc->grade[i].size()];
  for(int i = 1; i < sz; ++i)
    remap[order[i].spx->dim][order[i].spx->id] = i;

  std::vector<int>* R = new vector<int>[fc->size+1];
  std::vector<int> v;
  for(int i = 1; i < sz; ++i) {
    v.clear();
    int dm = order[i].spx->dim;
    if(dm == 0)
      v.push_back(0);
    else {
      std::vector<int>& children = fc->child_link[dm][order[i].spx->id];
      for(int j = 0; j < children.size(); ++j)
        v.push_back(remap[dm-1][children[j]]);
    }
    std::sort(v.begin(),v.end());
  }

  delete nl;
  for(int i = 0; i < fc->grade.size(); ++i)
    delete[] remap[i];
  delete[] remap;
  delete[] order;
  delete[] R;
  vector<pdd>* ans = new vector<pdd>();
  return ans;
}