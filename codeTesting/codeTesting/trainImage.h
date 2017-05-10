#include "globalHeader.h"
struct trainImage
{
 IplImage* RawImage;
 IplImage* FaceImage;
 IplImage* NoseImage;
 IplImage* EyeImage;
 IplImage* MouthImage;
 double* LBPHF;
 int nonUniform;
 int complete;
 double* components;
};