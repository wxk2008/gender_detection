#include "globalHeader.h"
#include "LBH-HF.h"
#define faceWidth 270
#define faceHeight 360
#define mouthWidth 57
#define mouthHeight 24
#define eyeWidth 67
#define eyeHeight 16
#define noseWidth 28
#define noseHeight 32

/*#define faceWidth 80
#define faceHeight 110
#define mouthWidth 57
#define mouthHeight 24
#define eyeWidth 67
#define eyeHeight 16
#define noseWidth 28
#define noseHeight 32*/

void initializeFaceProcessor();
int processFace(IplImage* inputImage, IplImage*& faceImage, IplImage*& mouthImage, IplImage*& noseImage, IplImage*& eyeImage, bool cropped);