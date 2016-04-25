#ifndef HOMOLOGY_H
#define HOMOLOGY_H

#include <utility>
#include "globals.h"
#include "complex/filtered_complex.h"

//calculate barcode of a filtered complex
std::vector<std::pair<real,real> >* barcode(filtered_complex* fc);

#endif