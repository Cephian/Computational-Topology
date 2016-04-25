#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "construction/witness_complex.h"
#include "homology.h"
#include "globals.h"
#include "complex/filtered_complex.h"

int main (int argc, char *argv[]) {

  if(argc >= 1 && !strcmp(argv[1], "help")) {
      std::cout << argv[0] << " <input> <output> [-l <landmarks>] [-d <dimension>][-r <cutoff>] [-s <seed>] [-v <reach>] [-m]\n";
      std::cout << "<input>: the input file to process\n";
      std::cout << "-l <landmarks>: the number of landmark points\n";
      std::cout << "-d <dimension>: max dimensional homology to compute\n";
      std::cout << "-r <cutoff>: ignore simplices of introduction time higher than <cutoff>\n";
      std::cout << "-v <reach>: ranked reach of witness points\n";
      std::cout << "-m: use minmax instead of witness complex\n";
      return 0;
    };

  if(argc < 2) {
    std::cout << "Not enough arguments! Consult " << argv[0] << " help for usage.\n";
    return 0;
  }

  std::ifstream ifile(argv[1]);
  if(!ifile.is_open()) {
    std::cout << "\"" << argv[1] << "\"" << " is not a valid file path\n";
    return 0;
  }
  std::ofstream ofile(argv[2]);
  if(!ofile.is_open()) {
    std::cout << "\"" << argv[2] << "\"" << " is not a valid file path\n";
    return 0;
  }

  int n=25;
  int seed = time(0);
  int cpx_dim = 2;
  int v = 1;
  bool minmax = 0;
  real cutoff = INF;

  for(int i = 3; i < argc; ++i) {
    if(argv[i][0] == '-' && strcmp(argv[i],"-m") && i == argc-1) {
      std::cout << "Error: required value after flag " << argv[i] << "\n";
      return 0;
    }
    if(!strcmp(argv[i],"-l")) {
      n = atoi(argv[++i]);
    } else if(!strcmp(argv[i],"-d")) {
      cpx_dim = atoi(argv[++i]);
    } else if(!strcmp(argv[i],"-r")) {
      cutoff =atoi(argv[++i]);
    } else if(!strcmp(argv[i],"-s")) {
      seed = atoi(argv[++i]);
    } else if(!strcmp(argv[i],"-v")) {
      v = atoi(argv[++i]);
    } else if(!strcmp(argv[i],"-m")) {
      minmax = 1;
    }
  }

  std::cout << "Reading from " << argv[1] << "" << std::endl;
  int N, dim;
  ifile >> N >> dim;
  real** data = new real*[N];
  for(int i = 0; i < N; ++i) {
    data[i] = new real[dim];
    for(int j = 0; j < dim; ++j)
      ifile >> data[i][j];
  }
  ifile.close();

  std::cout << "Constructing complex..." << std::endl;

  filtered_complex* fc;
  if(minmax) fc = create_maxmin_witness_complex(1,N,dim,cpx_dim+1,n,data,cutoff,seed);
  else fc = create_random_witness_complex(1,N,dim,cpx_dim+1,n,data,cutoff,seed);

  std::cout << "\n";
  fc->print();
  std::cout << std::endl;
  std::cout << "Computing homology..." << std::endl;
  std::vector<std::pair<double,double> >* bars = barcode(fc);
  std::cout << "Writing to " << argv[2] << std::endl;

  ofile << fc->grade.size()-1 << "\n";
  for(int i = 0; i < fc->grade.size()-1; ++i) {
    ofile << bars[i].size() << "\n";
    for(int j = 0; j < bars[i].size(); ++j) {
      ofile << bars[i][j].first << " " << bars[i][j].second << "\n";
    }
  }
  ofile.close();
}