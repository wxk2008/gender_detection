#include"faceProcessor.h"
#include "globalHeader.h"
    CvHaarClassifierCascade* faceCascade;
    CvHaarClassifierCascade* eyeCascade;
    CvHaarClassifierCascade* mouthCascade;

using namespace std;

// Perform face detection on the input image, using the given Haar Cascade.
// Returns a rectangle for the detected region in the given image.
CvRect detectFaceInImage(IplImage *inputImg, CvHaarClassifierCascade* cascade)
{
	// Smallest face size.
	CvSize minFeatureSize = cvSize(20, 20);
	// Only search for 1 face.
	int flags = CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH;
	// How detailed should the search be.
	float search_scale_factor = 1.1f;
	IplImage *detectImg;
	IplImage *greyImg = 0;
	CvMemStorage* storage;
	CvRect rc;
	double t;
	CvSeq* rects;
	CvSize size;
	int i, ms, nFaces;

	storage = cvCreateMemStorage(0);
	cvClearMemStorage( storage );
// printf("calling the eye detection functions\n");

	// If the image is color, use a greyscale copy of the image.
	detectImg = (IplImage*)inputImg;
	
	if (inputImg->nChannels > 1) {
		size = cvSize(inputImg->width, inputImg->height);
		greyImg = cvCreateImage(size, IPL_DEPTH_8U, 1 );
		cvCvtColor( inputImg, greyImg, CV_BGR2GRAY );
		detectImg = greyImg;	// Use the greyscale image.
	}

	// Detect all the faces in the greyscale image.
	t = (double)cvGetTickCount();
// 	printf("calling the eye detection functions\n");
	rects = cvHaarDetectObjects( detectImg, cascade, storage,
			search_scale_factor, 3, flags, minFeatureSize);
	t = (double)cvGetTickCount() - t;
	ms = cvRound( t / ((double)cvGetTickFrequency() * 1000.0) );
	nFaces = rects->total;
	//printf("Face Detection took %d ms and found %d objects\n", ms, nFaces);

	// Get the first detected face (the biggest).
	if (nFaces > 0)
		rc = *(CvRect*)cvGetSeqElem( rects, 0 );
	else
		rc = cvRect(-1,-1,-1,-1);	// Couldn't find the face.

	if (greyImg)
		cvReleaseImage( &greyImg );
	cvReleaseMemStorage( &storage );
	//cvReleaseHaarClassifierCascade( &cascade );

	return rc;	// Return the biggest face found, or (-1,-1,-1,-1).
}

void drawRectangle(int right_end,int left_end,int top_end,int down_end,CvScalar color,IplImage* target,int thickness)
{
CvPoint low_c,high_c;
high_c=cvPoint(left_end,down_end);
low_c=cvPoint(right_end,top_end);
cvRectangle(target,low_c,high_c,color,thickness);


}



void initializeFaceProcessor()
{
  
  char *faceCascadeFilename = "haarcascade_frontalface_alt.xml";
    char *eyeCascadeFilename = "haarcascade_mcs_eyepair_small.xml";
    char *mouthCascadeFilename = "haarcascade_mcs_mouth.xml";
    
    
    
    // Load the HaarCascade classifier for face detection.

    faceCascade = (CvHaarClassifierCascade*)cvLoad(faceCascadeFilename, 0, 0, 0);
    eyeCascade = (CvHaarClassifierCascade*)cvLoad(eyeCascadeFilename, 0, 0, 0);
    mouthCascade = (CvHaarClassifierCascade*)cvLoad(mouthCascadeFilename, 0, 0, 0);
    if( !faceCascade ) {
	    printf("Couldnt load Face detector '%s'\n", faceCascadeFilename);
	    exit(1);
    }
    if( !eyeCascade ) {
	    printf("Couldnt load Left Eye detector '%s'\n", eyeCascadeFilename);
	    exit(1);
    }
    if( !mouthCascade ) {
	    printf("Couldnt load Mouth detector '%s'\n", mouthCascadeFilename);
	    exit(1);
    }
  
  
  
  
}





void crop( IplImage* src,IplImage* dst,CvRect area ) 
{ 
   // Must have dimensions of output image 
  // IplImage* cropped = cvCreatImage( 1280,500, src->depth, src->nChannels ) 

   // Say what the source region is 
   cvSetImageROI( src,  area ); 

   // Do the copy 
   cvCopy( src, dst ); 
   cvResetImageROI( src ); 

} 

int processFace(IplImage* inputImage, IplImage* &faceImage, IplImage* &mouthImage, IplImage* &noseImage, IplImage* &eyeImage, bool cropped)
{
  
  
    CvSize sz;
    IplImage *showImg ;

    sz = cvGetSize( inputImage );
    
    
    
    IplImage* GaborImage;
    CvRect faceRect;
    CvRect eyeRect;
    CvRect mouthRect;
    CvRect noseRect;
    //bool findFaces=FALSE;
	bool findFaces = false;
    
    
      
    
    
    if (cropped)
    {
      showImg=cvCreateImage( sz, inputImage->depth, inputImage->nChannels);
      
      faceImage=cvCreateImage(cvSize(faceWidth,faceHeight),inputImage->depth,inputImage->nChannels);
      GaborImage=cvCreateImage( cvSize(faceWidth,faceHeight), inputImage->depth, 1);
      //findFaces=TRUE;
	  findFaces = true;
    //  cvCvtColor(inputImage,showImg,CV_RGB2GRAY);
      cvCopy(inputImage,showImg);
      cvResize(showImg,faceImage,CV_INTER_LINEAR);

      
    }
    else 
    {
      faceRect = detectFaceInImage(inputImage, faceCascade);
      if (faceRect.width > 0 )
      {
	showImg=cvCreateImage( sz, inputImage->depth, inputImage->nChannels);
	
	faceImage=cvCreateImage(cvSize(faceWidth,faceHeight),inputImage->depth,inputImage->nChannels);
	 GaborImage=cvCreateImage( cvSize(faceWidth,faceHeight), inputImage->depth, 1);
	findFaces=true;
	//cvCvtColor(inputImage,showImg,CV_RGB2GRAY);
	cvCopy(inputImage,showImg);
	cvSetImageROI(showImg,cvRect(faceRect.x+faceRect.width*0.1,faceRect.y,faceRect.width*0.8,faceRect.height));
	cvResize(showImg,faceImage,CV_INTER_LINEAR);
	
      }
    }
    
    if(findFaces)    
    {
      
      cvReleaseImage(&showImg);
      showImg=cvCreateImage(cvSize(faceWidth,faceHeight),inputImage->depth,inputImage->nChannels);
      cvCopy(faceImage,showImg);
      
      eyeImage=cvCreateImage(cvSize(eyeWidth,eyeHeight),inputImage->depth,1);
      

      //!Find Eyes     
  //    cout<< "finding eyes!\n";
      eyeRect = detectFaceInImage(showImg, eyeCascade);
	
      if (eyeRect.width<=0 || eyeRect.y>140)
      {
	cvReleaseImage(&eyeImage);
	cvReleaseImage(&GaborImage);
	cvReleaseImage(&showImg);
	return 0;
      }
      GaborImage=filterGabor(faceImage);
      int oldEyeY=eyeRect.y;
      int oldEyeH=eyeRect.height;
      eyeRect.y=eyeRect.y-eyeRect.height;
      eyeRect.height=eyeRect.height*1.8;
      cvSetImageROI(GaborImage,eyeRect);
      cvResize(GaborImage,eyeImage,CV_INTER_LINEAR);      
      cvResetImageROI(GaborImage);
       drawRectangle(eyeRect.x,eyeRect.x+eyeRect.width,eyeRect.y,eyeRect.y+eyeRect.height,cvScalar(0,0,0),  showImg,   -1  );

     //!Find Mouth
   //    cout<< "finding mouth!\n";
       mouthImage=cvCreateImage(cvSize(mouthWidth,mouthHeight),inputImage->depth,1);
      mouthRect = detectFaceInImage(showImg, mouthCascade);
      
      if (mouthRect.y<eyeRect.y+eyeRect.height || mouthRect.y<235)
      {
	cvReleaseImage(&eyeImage);
	cvReleaseImage(&GaborImage);
	cvReleaseImage(&mouthImage);
	cvReleaseImage(&showImg);
	return 0;
      }
      
      
      cvSetImageROI(GaborImage,cvRect(mouthRect.x,mouthRect.y+mouthRect.height*0.1,mouthRect.width,mouthRect.height*0.9));
      cvResize(GaborImage,mouthImage,CV_INTER_LINEAR);
      cvResetImageROI(GaborImage);
      
//       printf("eye height:%d\n",eyeRect.y);
//       printf("mouth height:%d\n",mouthRect.y);
//       cvNamedWindow("temp");
//       cvShowImage("temp",eyeImage);
//       cvWaitKey(0);
//       cvDestroyWindow("temp");
      
      
      //!Find Nose
    //  cout<< "finding nose!\n";
      noseImage=cvCreateImage(cvSize(noseWidth,noseHeight),inputImage->depth,1);
       
      
      
      int noseRectLeft=mouthRect.x+mouthRect.width/4;
      int noseRectRight=mouthRect.x+3*mouthRect.width/4;
      int noseRectTop=mouthRect.y;
      int noseRectBottom=(oldEyeY+oldEyeH);
      
      //cout << "Nose Left,Right,Top,Bottom:" << noseRectLeft<<" "<<noseRectRight<<" "<<noseRectTop<<" "<<noseRectBottom <<"\n";
      
    //  cout<<"Doing Nose\n";
//       cout<< "EYe Height:" << oldEyeY << "\n";
//       if (mouthRect.y-(oldEyeY+oldEyeH) <= 0)
//       {
// 	
// 	      cvNamedWindow("temp");
// 	      cvShowImage("temp",faceImage);
//       cvWaitKey(0);
//       cvShowImage("temp",mouthImage);
//       cvWaitKey(0);
//       cvShowImage("temp",eyeImage);
//       cvWaitKey(0);
//       cvDestroyWindow("temp");
// 	
// 	
// 	
//       }
      
      
      cvSetImageROI(GaborImage,cvRect(noseRectLeft,noseRectTop,mouthRect.width/2,mouthRect.y-(oldEyeY+oldEyeH)));
      
      
      cvResize(GaborImage,noseImage,CV_INTER_LINEAR);
       cvResetImageROI(GaborImage);
     // cout<<"Did Nose\n";
      
      cvReleaseImage(&showImg);
      cvReleaseImage(&GaborImage);
      return 1;
      
    }
    else 
    {
      return 0;
    }
    
}
