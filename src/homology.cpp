#include <iostream>
#include <set>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;
typedef double real;
typedef pair<double,double> pdd;
const real INF = 1e30;
const real EPS = 1e-10;

//represents a simplex
struct simplex {
  int dim,id;
  vector<int> verts;

  simplex(int d, int* v, int i) {
    dim = d;
    verts.reserve(d+1);
    for(int j = 0; j <= d; ++j)
      verts.push_back(v[j]);
    id = i;
  }

  bool operator< (const simplex& s) const {
    if(dim == s.dim) {
      for(int i = 0; i <= dim; ++i) {
        if(verts[i] == s.verts[i]) continue;
        return verts[i] < s.verts[i];
      }
      return 0;
    }
    return dim < s.dim;
  }

  simplex* child(int v) {
    int* cverts = new int[dim];

    for(int i = 0; i < dim; ++i)
      cverts[i] = verts[i + (i >= v)];

    simplex* cd = new simplex(dim-1,cverts,0);
    delete[] cverts;
    return cd;
  }

  bool operator== (const simplex& fs) const {
    if(dim != fs.dim) return 0;
    for(int i = 0; i <= dim; ++i)
      if(verts[i] != fs.verts[i]) return 0;
    return 1;
  }

  friend ostream& operator<<(ostream& o, const simplex& fs) {
    o << "{";
    for(int i = 0; i <= fs.dim; ++i) {
      o << fs.verts[i];
      if(i != fs.dim) o << ",";
    }
    o << "}";
    return o;
  }
};

//represents a filtered simplicial complex
struct filtered_complex {
  vector<set<simplex> > grade;
  vector<vector<vector<int> > > child_link;
  vector<vector<real> > critical_value;
  int size;

  filtered_complex(int dim) {
    while(grade.size() <= dim) {
      grade.push_back(set<simplex>());
      child_link.push_back(vector<vector<int> >());
      critical_value.push_back(vector<real>());
    }
    size=0;
  }

  void print() {
    cout << "--- " << size << " total ---\n";
    for(int i = 0; i < grade.size(); ++i)
      cout << "dim " << i << ": " << grade[i].size() << " simplices\n";
  }

  void add(int dim, int* verts, double r) {
    simplex* fs = new simplex(dim,verts,grade[dim].size());
    if(grade[dim].count(*fs)) return;
    vector<int> children;
    children.reserve(dim+1);
    if(dim > 0) {
      for(int i = 0; i <= dim; ++i) {
        simplex* child = fs->child(i);

        set<simplex>::iterator it = grade[child->dim].find(*child);
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
};

// choose random landmarks indices
int* get_random_landmarks(int size, int amt) {
  int* arr = new int[size];
  int* res = new int[amt];
  for(int i = 0; i < size; ++i)
    arr[i] = i;
  srand(time(0));
  // srand(2016);
  for(int i = 0; i < amt; ++i) {
    swap(arr[i],arr[i+rand()%(size-i)]);
    res[i] = arr[i];
  }
  delete[] arr;
  return res;
}

// euclidean distance
real dist(real* a, real* b, int dim) {
  double dot = 0;
  for(int i = 0; i < dim; ++i)
    dot += (a[i]-b[i])*(a[i]-b[i]);
  return sqrt(dot);
}

struct filtered_simplex {
  const simplex* spx;
  real crit;

  filtered_simplex() {

  }

  filtered_simplex(real c, const simplex* s) {
    spx = s;
    crit = c;
  }

  bool operator< (const filtered_simplex& f) const {
    if(abs(crit - f.crit) < EPS)
      return *spx < *f.spx;
    return crit < f.crit;
  }
};

//calculate barcode of a filtered complex
vector<pdd>* barcode(filtered_complex* fc) {

  filtered_simplex* order = new filtered_simplex[fc->size+1];
  int sz = 0;
  for(int i = 0; i < fc->grade.size(); ++i)
    for(set<simplex>::iterator j = fc->grade[i].begin(); j != fc->grade[i].end(); ++j)
      order[sz++] = filtered_simplex(fc->critical_value[i][j->id], &(*j));
  simplex* nl = new simplex(-1,&sz,0);
  order[sz++] = filtered_simplex(0,nl);
  sort(order,order+sz);

  int** remap = new int*[fc->grade.size()];
  for(int i = 0; i < fc->grade.size(); ++i)
    remap[i] = new int[fc->grade[i].size()];
  for(int i = 1; i < sz; ++i)
    remap[order[i].spx->dim][order[i].spx->id] = i;

  vector<int>* R = new vector<int>[fc->size+1];
  vector<int> v;
  for(int i = 1; i < sz; ++i) {
    v.clear();
    int dm = order[i].spx->dim;
    if(dm == 0)
      v.push_back(0);
    else {
      vector<int>& children = fc->child_link[dm][order[i].spx->id];
      for(int j = 0; j < children.size(); ++j)
        v.push_back(remap[dm-1][children[j]]);
    }
    sort(v.begin(),v.end());
  }
  cout << "adventure" << endl;

  delete nl;
  for(int i = 0; i < fc->grade.size(); ++i)
    delete[] remap[i];
  delete[] remap;
  delete[] order;
  delete[] mat;
  vector<pdd>* ans = new vector<pdd>();
  return ans;
}

//add all tgt_dim dimensional simplices to the complex with proper critical times
void fill_simplices(filtered_complex* fc, int* chosen, int& tgt_dim, int cur_dim, int i, double& cutoff) {
  if(cur_dim == tgt_dim) {
    fc->add(tgt_dim,chosen,0);
    return;
  }
  ++cur_dim;
  for(; i < fc->grade[0].size(); ++i) {
    chosen[cur_dim] = i;
    fill_simplices(fc,chosen,tgt_dim,cur_dim, i+1,cutoff);
  }
}

//construct a witness complex with random landmarks
filtered_complex* create_random_witness_complex(int v, int N, int dim, int cpx_dim, int n, real** data, double cutoff) {
  int* landmarks = get_random_landmarks(N,n);
  real** D = new real*[n];
  real* m = new real[N];
  for(int i = 0; i < n; ++i) {
    D[i] = new real[N];
    for(int j = 0; j < N; ++j) {
      D[i][j] = dist(data[landmarks[i]],data[j],dim);
    }
  }

  real* tmp = new real[v+1];
  for(int j = 0; j < N; ++j) {
    for(int i = 0; i <= v; ++i)
      tmp[i] = INF;

    for(int i = 0; i < n; ++i) {
      int pl = v+1;
      real t = D[i][j];
      while(pl > 0 && tmp[pl-1] > t)
        --pl;

      while(pl <= v) {
        real q = t;
        t = tmp[pl];
        tmp[pl++] = q;
      }

    }
    m[j] = tmp[v];
  }
  delete[] tmp;
  int* spx = new int[2];
  filtered_complex* fc = new filtered_complex(cpx_dim);
  for(int i = 0; i < n; ++i)
    fc->add(0,&i,0);
  for(int a = n-1; a != -1; --a) {
    for(int b = n-1; b != a; --b) {
      real r = INF;
      int q = -1;
      for(int i = 0; i < N; ++i) {
        real r2 = max(real(0),max(D[a][i],D[b][i])-m[i]);
        if(r2 < r) {
          q = i;
          r = r2;
        }
      }
      if(r > cutoff) continue;
      spx[0]=a;
      spx[1]=b;
      fc->add(1,spx,r);
    }
  }
  delete[] spx;
  for(int dm = 2; dm <= cpx_dim; ++dm) {
    spx = new int[dm+1];
    fill_simplices(fc,spx,dm,-1,0,cutoff);
  }
  return fc;
}

int main (){
  int N, dim;
  cin >> N >> dim;
  real** data = new real*[N];
  for(int i = 0; i < N; ++i) {
    data[i] = new real[dim];
    for(int j = 0; j < dim; ++j)
      cin >> data[i][j];
  }
  filtered_complex* fc = create_random_witness_complex(2,N,dim,4,min(12,N),data,5000);
  fc->print();
  barcode(fc);
  // create_random_landmark_complex(2,N,dim,4,10,data);
  // filtered_complex c(2,N,dim,30,data,0,100);
}