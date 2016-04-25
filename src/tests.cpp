#include <iostream>
#include "construction/witness_complex.h"
#include "homology.h"
#include "globals.h"
#include "complex/filtered_complex.h"

int main () {
  int N, dim;
  std::cin >> N >> dim;
  real** data = new real*[N];
  for(int i = 0; i < N; ++i) {
    data[i] = new real[dim];
    for(int j = 0; j < dim; ++j)
      std::cin >> data[i][j];
  }
  filtered_complex* fc = create_random_witness_complex(2,N,dim,4,12,data,5000,5671);
  fc->print();
  barcode(fc);
}