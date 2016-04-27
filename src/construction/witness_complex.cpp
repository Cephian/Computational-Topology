#include <random>
#include <iostream>
#include "witness_complex.h"

real dist(real* a, real* b, int dim);
void fill_simplices(filtered_complex* fc, int* chosen, int& tgt_dim, int cur_dim, int i, double& cutoff);

// euclidean distance
real dist(real* a, real* b, int dim) {
  real dot = 0;
  for(int i = 0; i < dim; ++i)
    dot += (a[i]-b[i])*(a[i]-b[i]);
  return sqrt(dot);
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

/* ------- construct a witness complex from a given data set of points and landmarks -------
 * v -> an edge (a,b) exists at time T iff max(D[a,x], D[b,x]) is within T of the vth closest landmark to some witness x. Usually set to 0,1,2.
 * N -> number of points in the dataset
 * dim -> dimension of the dataset
 * cpx_dim -> cutoff dimension of the filtered_complex. we can only compute up to (cpx_dim-1)-dimensional homology.
 * n -> number of landmark points to choose
 * data -> the data itself. each row is a vector.
 * cutoff -> no simplices that appear strictly after time T=cutoff will be added to the complex
 * landmarks -> the indices of the landmark set
 */
filtered_complex* create_witness_complex(int v, int N, int dim, int cpx_dim, int n, real** data, real cutoff, int* landmarks) {
  if(cpx_dim > dim)
  cpx_dim = dim;
  // cout << "of dimension " << cpx_dim << endl;
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
        real r2 = D[a][i];
        if(D[b][i] > r2) r2 = D[b][i];
        r2 -= m[i];
        if(r2 < r) {
          q = i;
          r = r2;
          if(r <= 0) break;
        }
      }
      if(r > cutoff) continue;
      spx[0]=a;
      spx[1]=b;
      if(r < 0) r = 0;
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

// construct a witness complex with random landmarks
// all arguments are as detailed in the create_witness_complex method
filtered_complex* create_random_witness_complex(int v, int N, int dim, int cpx_dim, int n, real** data, real cutoff, int seed) {
  if(n > N) n = N;
  int* arr = new int[N];
  int* landmarks = new int[n];
  for(int i = 0; i < N; ++i)
    arr[i] = i;
  // srand(time(0));
  srand(seed);
  for(int i = 0; i < n; ++i) {
    int j =  i+rand()%(N-i);
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
    landmarks[i] = arr[i];
  }
  delete[] arr;
  filtered_complex* fc = create_witness_complex(v,N,dim,cpx_dim,n,data,cutoff,landmarks);
  delete[] landmarks;
  return fc;
}


filtered_complex* create_maxmin_witness_complex(int v, int N, int dim, int cpx_dim, int n, real** data, real cutoff, int seed) {
  if(N < n) n = N;
  int* landmarks = new int[n];
  srand(seed);
  landmarks[0] = rand()%n;
  for(int i = 1; i < n; ++i) {
    int best = -1;
    real best_dist = -1;
    for(int j = 0; j < N; ++j) {
      real mnd = INF;
      for(int k = 0; k < i; ++k) {
        if(j == landmarks[k]) goto skp;
        real ds = dist(data[landmarks[k]],data[j],dim);
        if(ds < mnd) mnd = ds;
      }
      if(mnd > best_dist) {
        best_dist = mnd;
        best = j;
      }
      skp:;
    }
    landmarks[i]=best;
  }
  filtered_complex* fc = create_witness_complex(v,N,dim,cpx_dim,n,data,cutoff,landmarks);
  delete[] landmarks;
  return fc;
}
