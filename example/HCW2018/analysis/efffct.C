

double efffct(double* x,double* p)
{
  if ( x[0] < 0.0001 ) { return 0.; }
  return p[0]*exp(p[1]*(x[0]+p[2]));
}
