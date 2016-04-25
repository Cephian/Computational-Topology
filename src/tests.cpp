
int main () {
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