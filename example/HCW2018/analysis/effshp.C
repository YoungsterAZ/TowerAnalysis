
#include <TMath.h>

double effshp(double* x,double* p) {
  return p[0]*TMath::Erf(x[0]-p[1]);
}
