#include "globalHeader.h"

#define PI 3.14159265
#include <complex>
#include "cvgabor.h"

typedef std::complex<double> dcmplx;
double dftH(int** histogram,int n,int u);
void getRN(int LBPLabel,int &r,int &n);
double* LBP_HF(IplImage* Image,int &nonUniform,int &complete);
IplImage* filterGabor(IplImage* input);