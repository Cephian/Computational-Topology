#ifndef WITNESS_COMPLEX_H
#define WITNESS_COMPLEX_H

#include "../globals.h"
#include "../complex/filtered_complex.h"

filtered_complex* create_witness_complex(int v, int N, int dim, int cpx_dim, int n, real** data, real cutoff, int* landmarks);
filtered_complex* create_random_witness_complex(int v, int N, int dim, int cpx_dim, int n, real** data, real cutoff, int seed);
filtered_complex* create_maxmin_witness_complex(int v, int N, int dim, int cpx_dim, int n, real** data, real cutoff, int seed);
#endif