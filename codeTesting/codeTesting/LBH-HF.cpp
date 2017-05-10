#include "LBH-HF.h"


void getRN(int LBPLabel,int &r,int &n)
{
  r=-1;
  n=1;
  switch(LBPLabel)
  {
    case 0:
      n=0;
      r=0;
      break;
    case 1:
      r=0;
      n=1;
      break;
    case 2:
      r=1;
      n=1;
      break;
    case 3:
      r=0;
      n=2;
      break;
    case 4:
      r=2;
      n=1;
      break;
    case 6:
      r=1;
      n=2;
      break;
    case 7:
      r=0;
      n=3;
      break;
    case 8:
      r=3;
      n=1;
      break;
    case 12:
      r=2;
      n=2;
      break;
    case 14:
      r=1;
      n=3;
      break;
    case 15:
      r=0;
      n=4;
      break;
    case 16:
      r=4;
      n=1;
      break;
    case 24:
      r=3;
      n=2;
      break;
    case 28:
      r=2;
      n=3;
      break;
    case 30:
      r=1;
      n=4;
      break;
    case 31:
      r=0;
      n=5;
      break;
    case 32:
      r=5;
      n=1;
      break;
    case 48:
      r=4;
      n=2;
      break;
    case 56:
      r=3;
      n=3;
      break;
    case 60:
      r=2;
      n=4;
      break;  
 
    case 62:
      r=1;
      n=5;
      break;
    case 63:
      r=0;
      n=6;
      break;
    case 64:
      r=6;
      n=1;
      break;
    case 96:
      r=5;
      n=2;
      break;
    case 112:
      r=4;
      n=3;
      break;
    case 120:
      r=3;
      n=4;
      break;
    case 124:
      r=2;
      n=5;
      break;
    case 126:
      r=1;
      n=6;
      break;
    case 127:
      r=0;
      n=7;
      break;
    case 128:
      r=7;
      n=1;
      break;
    case 129:
      r=7;
      n=2;
      break;
    case 131:
      r=7;
      n=3;
      break;
    case 135:
      r=7;
      n=4;
      break;
    case 143:
      r=7;
      n=5;
      break;
    case 159:
      r=7;
      n=6;
      break;
    case 191:
      r=7;
      n=7;
      break;
    case 192:
      r=6;
      n=2;
      break;
    case 193:
      r=6;
      n=3;
      break;
    case 195:
      r=6;
      n=4;
      break;
    case 199:
      r=6;
      n=5;
      break;
    case 207:
      r=6;
      n=6;
      break;
    case 223:
      r=6;
      n=7;
      break;
    case 224:
      r=5;
      n=3;
      break;
    case 225:
      r=5;
      n=4;
      break;
    case 227:
      r=5;
      n=5;
      break;
    case 231:
      r=5;
      n=6;
      break;
    case 239:
      r=5;
      n=7;
      break;
    case 240:
      r=4;
      n=4;
      break;
      
    case 241:
      r=4;
      n=5;
      break;
    case 243:
      r=4;
      n=6;
      break;
    case 247:
      r=4;
      n=7;
      break;
    case 248:
      r=3;
      n=5;
      break;
    case 249:
      r=3;
      n=6;
      break;
    case 251:
      r=3;
      n=7;
      break;
    case 252:
      r=2;
      n=6;
      break;
    case 253:
      r=2;
      n=7;
      break;
    case 254:
      r=1;
      n=7;
      break;
    case 255:
      r=0;
      n=8;
      break;
  }
}


double dftH(int** histogram,int n,int u)
{
  std::complex<double> sum (0.0,0.0);

// std::complex<double> i (0.0,-1.0);
 for (int r=0;r<8;r++)
 {
   std::complex<double> exp1 (0,-2*PI*(double)u*r/8);
   sum=sum+(dcmplx)(histogram[n][r])*(exp(exp1));
  }
  std::complex<double> cc (conj(sum));
  sum=sum*cc;
  sum=sqrt(sum);
  return (double)(abs(sum));
  
}

double* LBP_HF(IplImage* InImage,int &nonUniform,int &complete)
{
  nonUniform=0;
  complete=0;
  
  double* values=new double[9];
  CvSize sz=cvGetSize(InImage);
 
  IplImage* Image=cvCreateImage(sz,InImage->depth,1);
//  printf("scn:%d",InImage->nChannels);
  if(InImage->nChannels>1)  
    cvCvtColor(InImage,Image,CV_RGB2GRAY);
  else
    cvCopy(InImage,Image);
  int r;
  int n;
  int** histogram=new int*[8];
    for (int i=0;i<8;i++)
      histogram[i]=new int[8];
  
  for (int i=0;i<8;i++)
    for (int j=0;j<8;j++)
      histogram[i][j]=0;
    
//  printf("height:%d\n",sz.width);
  for (int i=1;i<sz.height-1;i++)
  {
    for (int j=1;j<sz.width-1;j++)
    {
  //    printf("i,j:%d,%d\n",i,j);
      values [0]=cvGetReal2D(Image,i,j);
      values [1]=cvGetReal2D(Image,i+1,j);
      values [2]=cvGetReal2D(Image,i+1,j-1);
      values [3]=cvGetReal2D(Image,i,j-1);
      values [4]=cvGetReal2D(Image,i-1,j-1);
      values [5]=cvGetReal2D(Image,i-1,j);
      values [6]=cvGetReal2D(Image,i-1,j+1);
      values [7]=cvGetReal2D(Image,i,j+1);
      values [8]=cvGetReal2D(Image,i+1,j+1);
      
            
      int LBPLabel=0;
      for (int a=1;a<9;a++)
	LBPLabel+=(values[0]>=values[a])*pow(2.0,a-1);
      
      getRN(LBPLabel,r,n);
      if (r==-1)
	nonUniform+=1;
      else if (n==8)
	complete+=1;
      else
	histogram[n][r]=histogram[n][r]+1;
      
    }     
  }
 // printf("calculated histogram!\n");
  delete[] values;
  
  
  double* H=new double[5*7];
  int index=0;
  
  for (n=1;n<8;n++)
  {
    for (int u=0;u<5;u++)
    {
      H[index]=dftH(histogram,n,u);
      index++;
    }
    
  }
 // printf("calculated DFT!\n");
  
  for (int i=0;i<7;i++)
      delete[] histogram[i];
    delete[] histogram;
  
  return H;
}






IplImage* filterGabor(IplImage* input)
{
    IplImage* temp=input;
  CvSize sz=cvGetSize(temp);
  IplImage* pic1=cvCreateImage(sz,temp->depth,1);
  IplImage* res=cvCreateImage(sz,temp->depth,1);
  IplImage* final=cvCreateImage(sz,temp->depth,1);
  //cvSetImageCOI(temp, 1);
  //cvCopy(temp, pic1); 
  cvCvtColor(temp,pic1,CV_RGB2GRAY);
    
    
    //Gabor Test
    
  
 // cvNamedWindow("Gabor Kernel", 1);
      double Sigma = 2*PI;
      double F = sqrt(2.0);
      CvGabor *gabor1 = new CvGabor; 
      double total=0;
   for (double dPhi=0;dPhi<2*PI;dPhi=dPhi+PI/4)
   {
     for (int iNu=0;iNu<6;iNu++)
     {
	gabor1->Init(dPhi, iNu, Sigma, F);
	gabor1->conv_img(pic1,res,CV_GABOR_REAL); 
	cvAddWeighted(final,(total-1)/total,res,1/total,0,final);
	total++;
     }
      
   }
   delete gabor1;
   cvReleaseImage(&pic1);
   cvReleaseImage(&res);
   
   
   return final;
  
  
}