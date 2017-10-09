////////////////////////////////////////////////////////////////////////
//
// hello-world.cpp
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file, inverts it, and displays the result. 
//
////////////////////////////////////////////////////////////////////////

#if 00

#include "stdafx.h"
//#include <opencv/cv.h>
//#include <opencv/highgui.h>

typedef struct rowRGBi 
{
    // weights for constructing grayscale
    static const int wRed = 40;
    static const int wGrn = 35;
    static const int wBlu = 25;
    static const int wTot = (wRed + wGrn + wBlu);

    float  red;
    float  grn;
    float  blu;
    float  gry;
    int    len;
    uchar *pix;

    rowRGBi(uchar *px, int length) {
        pix = px;
        red = grn = blu = gry = 0.0F;
        for(int len = length; --len >= 0; ) {
            red += *px++;
            grn += *px++;
            blu += *px++;
        }
        gry = (red*wRed + grn*wGrn + blu*wBlu) / (length*wTot);
        red /= length;
        grn /= length;
        blu /= length;
    };
} rowRGBi;




struct lessRowRGBi
{
  inline bool operator()(const rowRGBi *rP, const rowRGBi *rQ) const
  {
      if (rP->gry != rQ->gry)
          return rP->gry - rQ->gry < 0;
      if (rP->red != rQ->red)
          return rP->red - rQ->red < 0;
      if (rP->grn != rQ->grn    )
          return rP->grn - rQ->grn < 0;
      return rP->blu - rQ->blu < 0;
  }
};

typedef std::map<const rowRGBi*, int, lessRowRGBi> RowRGBiMap;

/** Differences for row of interleaved uchar RGB */
void rowDifRGBi(int *pdR, int *pdG, int *pdB, uchar *rowA, uchar *rowB, int chan, int len)
{
    *pdR = 0; *pdG = 0; *pdB = 0;
    for (uchar *pA = rowA, *pB = rowB; --len >= 0; pA += chan, pB += chan) {
       *pdR += pA++ - pB++;
       *pdG += pA++ - pB++;
       *pdB += pA++ - pB++;
    }
}

int wtdRowDifRGBi(uchar *rowA, uchar *rowB, int chan, int len)
{
    int r, g, b;
    rowDifRGBi(&r, &g, &b, rowA, rowB, chan, len);
    return ((r + g)*35 + b*30)/100;
}



/** Differences for row of interleaved uchar RGB */
void absRowDifRGBi(int *pdR, int *pdG, int *pdB, uchar *rowA, uchar *rowB, int chan, int len)
{
    *pdR = 0; *pdG = 0; *pdB = 0;
    int difR, difG, difB, extra = chan - 3;
    for (uchar *pA = rowA, *pB = rowB; --len >= 0; pA += extra, pB += extra) {
       difR = *pA++ - *pB++;
       *pdR += (difR < 0 ? -difR : difR);
       difG = *pA++ - *pB++;
       *pdG += (difG < 0 ? -difG : difG);
       difB = *pA++ - *pB++;
       *pdB += (difB < 0 ? -difB : difB);
    }
}

float wtdAbsRowDifRGBi(uchar *rowA, uchar *rowB, int chan, int len)
{
    int r, g, b;
    absRowDifRGBi(&r, &g, &b, rowA, rowB, chan, len);
    return ((r + g)*35 + b*30)/100.0F;
}

float rowRunDifRGBi(int difNow, int & difPrv, int t0, int t1, int t2, float f0, float f1, float f2)
{
    float distance = 0;
    int  difNowAbs, difNowPrv, difNowPrvAbs;
    difNowAbs    = difNow < 0 ? -difNow : difNow;
    difNowPrv    = difNow - difPrv;
    difNowPrvAbs = difNowPrv < 0 ? -difNowPrv : difNowPrv;
    if (difNowPrvAbs <= t0)
        distance += difNowAbs * f0;
    else if (difNowPrvAbs <= t1)
        distance += difNowAbs * f1;
    else if (difNowPrvAbs <= t2)
        distance += difNowAbs * f2;
    else
        distance += difNowAbs;

    difPrv = difNow;
    return distance;
}

/** Differences for row of interleaved uchar RGB */
void absRowRunDifRGBi(float *pdR, float *pdG, float *pdB, uchar *rowA, uchar *rowB, int chan, int len)
{
    int t0 = 3, t1 = 7, t2 = 15;
    float f0 = 0.2F, f1 = 0.45F, f2 = 0.75F;
    *pdR = 0; *pdG = 0; *pdB = 0;
    int difR, difG, difB, extra = chan - 3;
    int prvR = 0, prvG = 0, prvB = 0;
    for (uchar *pA = rowA, *pB = rowB; --len >= 0; pA += extra, pB += extra) {
       difR = *pA++ - *pB++;
       *pdR += rowRunDifRGBi(difR, prvR, t0, t1, t2, f0, f1, f2);
       difG = *pA++ - *pB++;
       *pdG += rowRunDifRGBi(difG, prvG, t0, t1, t2, f0, f1, f2);
       difB = *pA++ - *pB++;
       *pdB += rowRunDifRGBi(difB, prvB, t0, t1, t2, f0, f1, f2);
    }
}

float wtdAbsRowRunDifRGBi(uchar *rowA, uchar *rowB, int chan, int len)
{
    float r, g, b;
    absRowRunDifRGBi(&r, &g, &b, rowA, rowB, chan, len);
    return ((r + g)*35 + b*30)/100;
}

int qsort_int( const void *arg1, const void *arg2 )
{
   return *(int*)arg1 - *(int*)arg2;    // compare values of 2 pointers to int
}
        
int qsort_s_arrayVal( void *arr, const void *arg1, const void *arg2 )
{
   return ((int*)arr)[*(int*)arg1] - ((int*)arr)[*(int*)arg2];    // compare values of 2 pointers to int
}


void showImage(const char *name, IplImage *img, int xpos, int ypos)
{
  cvNamedWindow(name, CV_WINDOW_AUTOSIZE); 
  cvMoveWindow( name, xpos, ypos);
  cvShowImage(  name, img );
}


IplImage *cloneByRowIndex(IplImage *orig, int rowIdx[]) 
{
    // Copy the shuffled image using the supplied row order

    // IplImage *copy = cvCreateImage( cvSize( orig->width, orig->height ), IPL_DEPTH_8U, orig->nChannels );
    // uchar *copyData = (uchar *)copy->imageData;

    IplImage *copy = cvCloneImage(orig);
    uchar *copyData = (uchar *)copy->imageData;
    uchar *origData = (uchar *)orig->imageData;
    int nChannels = orig->nChannels, step = orig->widthStep;
    for(int id, i = 0; i < orig->height; i++) {
        id = rowIdx[i];
        for(int j = 0; j < orig->width; j++) 
            for(int k = 0; k < nChannels; k++)
                copyData[i*step + j*nChannels + k] = origData[id*step + j*nChannels + k];
    }
    return copy;
}

#include <hash_set>
typedef float (*rowDistRGBi)(uchar *srcA, uchar *srcB, int chan, int wide);


int idxMinDifs(int idx[], int idx0, uchar *src, int wide, int tall, int step, int chan, rowDistRGBi rowDist)
{
    int idxMinMax = 0;
    float abdMinMax = 0;
    idx[0] = idx0;
    std::hash_set<int> used;
    used.insert(idx0);
    //printf("\nMinimum difference indices, etc:\n");
    for (int j = 0; j < tall-1; j++) {
        float abdMin = (float)INT_MAX;
        int   idxMin;
        for (int k = 0; k < tall; k++) {
            if (used.find(k) == used.end()) {
              //float abd = wtdAbsRowDifRGBi(src + idx[j]*step, src + k*step, chan, wide);
                float abd = rowDist(src + idx[j]*step, src + k*step, chan, wide);
                if (abdMin > abd) {
                    abdMin = abd;
                    idxMin = k;
                }
            }
        }
        used.insert(idxMin);
        idx[j+1] = idxMin;
        if (abdMinMax < abdMin) {
            abdMinMax = abdMin;
            idxMinMax = idxMin;
        }

        //printf("%3d  %3d  %7d\n", j, idxMin, abdMin);
    }
    return idxMinMax;
}


/**
 * idx[] must hold height*2 entries, 
 */
int idxMinDifsB(int idx2[], int rowGray[], int idx0, uchar *src, int wide, int tall, int step, int chan, rowDistRGBi rowDist)
{
    for (int j = 0; j < tall; j++) {
        idx2[j] = 0;
        idx2[j+tall] = 0;
    }

    float abdMinMax = 0;
    int   idxMinMax = 0, iNow = tall, iDec = iNow, iInc = iNow;
    idx2[iNow] = idx0;

    std::hash_set<int> used;
    used.insert(idx0);
    //printf("\nMinimum difference indices, etc:\n");
    for (int j = 0; j < tall-1; j++) {
        float abdDecMin = (float)INT_MAX, abdIncMin = (float)INT_MAX;
        int   idxDecMin, idxIncMin;
        float abdDec, abdInc;
        for (int k = 0; k < tall; k++) {
            if (used.find(k) == used.end()) {
                abdDec = rowDist(src + idx2[iDec]*step, src + k*step, chan, wide);
                if (abdDecMin > abdDec) {
                    abdDecMin = abdDec;
                    idxDecMin = k;
                }
                if (iDec == iInc)
                    abdInc = abdDec;
                else
                    abdInc = rowDist(src + idx2[iInc]*step, src + k*step, chan, wide);
                if (abdIncMin > abdInc) {
                    abdIncMin = abdInc;
                    idxIncMin = k;
                }
            }
        }
        printf("%3d  %3d  %3d  %g  %g   size: %d\n"
            , j, idxDecMin, idxIncMin, abdDecMin, abdIncMin, used.size());
        if (abdDecMin < abdIncMin) {     
            used.insert(idxDecMin);
            --iDec;
            iNow = iDec;
            idx2[iNow] = idxDecMin;
            if (abdMinMax < abdDecMin) {
                abdMinMax = abdDecMin;
                idxMinMax = iNow;
            }
        } else {
            used.insert(idxIncMin);
            ++iInc;
            iNow = iInc;
            idx2[iNow] = idxIncMin;
            if (abdMinMax < abdIncMin) {
                abdMinMax = abdIncMin;
                idxMinMax = iNow;
            }
        }

        //printf("%3d  %3d  %7d\n", j, idxMin, abdMin);
    }
    return iDec;
}
        

/**
 * idx[] must hold height*2 entries, 
 */
int idxMinDifsA(int idx2[], int rowGray[], int idx0, uchar *src, int wide, int tall, int step, int chan, rowDistRGBi rowDist)
{
    for (int j = 0; j < tall; j++) {
        idx2[j] = 0;
        idx2[j+tall] = 0;
    }

    float abdMinMax = 0;
    int   idxMinMax = 0, iNow = tall, iDec = iNow, iInc = iNow;
    bool  bGetNewDecMin = true, bGetNewIncMin = false;
    idx2[iNow] = idx0;

    std::hash_set<int> left;
    int k = 0;
    while (k < idx0)
        left.insert(k++);
    while (++k < tall)
        left.insert(k);

    float abdDecMin = (float)INT_MAX, abdIncMin = (float)INT_MAX;
    int   idxDecMin =        INT_MAX, idxIncMin =        INT_MAX;
    float abdDec, abdInc;
    for (int j = 0; j < tall-1; j++) {
        for (std::hash_set<int>::iterator it = left.begin(); it != left.end(); ++it) {
            k = *it;
            if (bGetNewDecMin || 1) {                 // need a new dec row
                abdDec = rowDist(src + idx2[iDec]*step, src + k*step, chan, wide);
                if (abdDecMin > abdDec) {
                    abdDecMin = abdDec;
                    idxDecMin = k;
                }
            } 
            if (bGetNewIncMin || 1) {
                abdInc = rowDist(src + idx2[iInc]*step, src + k*step, chan, wide);
                if (abdIncMin > abdInc) {
                    abdIncMin = abdInc;
                    idxIncMin = k;
                }
            }
        }
        if (idxDecMin == idxIncMin) {
            printf("idxDecMin == idxIncMin (%d) at shuffled row %d\n", idxDecMin, j);
        }
        printf("%3d  %3d  %3d  %g  %g   left.size: %d\n"
            , j, idxDecMin, idxIncMin, abdDecMin, abdIncMin, left.size());
        if (abdDecMin < abdIncMin) {
            iNow = --iDec;
            idx2[iNow] = idxDecMin;
            left.erase(idxDecMin);
            bGetNewDecMin = true;
            abdDecMin     = (float)INT_MAX;
            if (idxDecMin == idxIncMin) {
                bGetNewIncMin = true; 
                abdIncMin     = (float)INT_MAX;
            }
            if (abdMinMax < abdDecMin) {
                abdMinMax = abdDecMin;
                idxMinMax = iNow;
            }
        } else {
            iNow = ++iInc;
            idx2[iNow] = idxIncMin;
            left.erase(idxIncMin);
            bGetNewIncMin = true;
            abdIncMin     = (float)INT_MAX;
            if (idxDecMin == idxIncMin) {
                bGetNewDecMin = true; 
                abdDecMin     = (float)INT_MAX;
            }
            if (abdMinMax < abdIncMin) {
                abdMinMax = abdIncMin;
                idxMinMax = iNow;
            }
        }

    }
    return iDec;
}



int test_ocvRandomRows(IplImage *orig)
{

    // get the original image & data
    int height      = orig->height;
    int width       = orig->width;
    int step        = orig->widthStep;
    int channels    = orig->nChannels;
    uchar *origData = (uchar *)orig->imageData;
    printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

  showImage("origWin", orig, 50, 50);

  // randomly shuffle the rows of the image into another
  IplImage *shuf = cvCloneImage(orig);
  uchar *shufData = (uchar *)shuf->imageData;
  unsigned int seed = (unsigned int) time(NULL);
  int id, *idx = new int[height], *rdx = new int[height];
  for (id = 0; id < height; id++)
      idx[id] = id;
  randomShuffle(idx, height, seed);
  printf("\nShuffle indices:\n");
  for(int i = 0; i < height; i++) {
      id = idx[i];
      printf("%3d  %3d\n", i, id);
      for(int j = 0; j < width; j++) 
          for(int k = 0; k < channels; k++)
              shufData[i*step + j*channels + k] = origData[id*step + j*channels + k];
  }

  // create shuffled-row window
  showImage("shufWin", shuf, 50, 400);

  // grayscale copy of shuffled-row image
  IplImage *gray = cvCreateImage( cvSize( width, height ), IPL_DEPTH_8U, 1 );
  cvCvtColor( shuf, gray, CV_RGB2GRAY );
  uchar *grayData = (uchar *)gray->imageData;
  // create gray window


  std::map<int, int> gray2row;
  printf("\nGray shuffled row averages:\n");
  int *rowGray = new int[height];
  for(int i = 0; i < height; i++) {
      int g = 0;
      for(int j = 0; j < width; j++) 
          g += grayData[i*gray->widthStep + j];
      rowGray[i] = g / width;
      gray2row.insert(std::pair<int,int>(rowGray[i], i));
      printf("%3d  %3d    %3d\n", i, rowGray[i], *gray2row.find(rowGray[i]));
  }
  printf("\nGray shuffled row averages -> map sort:\n");
  std::map<int, int>::iterator  it = gray2row.begin();
  std::map<int, int>::iterator end = gray2row.end();
  for(int j = 0; it != end ; ++it, j++ ) {
      printf("    %3d  %4d  %4d\n", j, it->first, it->second);
  }
  
  //qsort(rowGray, height, sizeof(int), qsort_int);
  //for (int j = 0; j < height; j++)
  //    printf("sort %7d  %3d\n", j, rowGray[j]);

  int *rowIdx = new int[height];
  for (int j = 0; j < height; j++)
      rowIdx[j] = j;
  qsort_s(rowIdx, height, sizeof(int), qsort_s_arrayVal, rowGray);
  for (int j = 0; j < height; j++)
      printf("arrValSort %7d  %3d  %3d\n", j, rowGray[j], rowIdx[j]);

  // average-intensity sorted grayscale copy of shuffled-row image
  int border = 40, grayWide = width+border, grayWideStep = border + gray->widthStep;
  IplImage *graySort = cvCreateImage( cvSize( grayWide, height ), IPL_DEPTH_8U, 1 );
  uchar *graySortData = (uchar *)graySort->imageData;
  for(int i = 0; i < height; i++) {
      int iDst = height - 1 - i;
      int iSrc = rowIdx[i];
      int j = 0;
      for( ; j < width; j++) 
          graySortData[iDst*grayWideStep + j] = grayData[iSrc*gray->widthStep + j];
      for( ; j < grayWide; j++) 
          graySortData[iDst*grayWideStep + j] = rowGray[iSrc];
  }

  showImage("grayShuf", gray,      50, 750);
  showImage("graySort", graySort, 500, 750);
 
  // Try to guess the original row ordering (by row distances), then reorder
  int idxMinMax1 = idxMinDifs(rdx, rowIdx[height-1], shufData, width, height, step, channels, wtdAbsRowRunDifRGBi);
  IplImage *rest1 = cloneByRowIndex(shuf, rdx);
  showImage("rest1", rest1,  50, 1100);

  // Try to guess the original row ordering (by row distances), then reorder
  int idxMinMax2 = idxMinDifs(rdx, idxMinMax1, shufData, width, height, step, channels, wtdAbsRowDifRGBi);
  IplImage *rest2 = cloneByRowIndex(shuf, rdx);
  showImage("rest2", rest2, 500, 1100);
  
  int *rdx2 = new int[height*2];
  int  iDec = idxMinDifsB(rdx2, rowGray, idxMinMax1, shufData, width, height, step, channels, wtdAbsRowDifRGBi);

  IplImage *rest3 = cvCreateImage(cvSize(orig->width, orig->height), IPL_DEPTH_8U, orig->nChannels);
  uchar *copyData = (uchar *)rest3->imageData;
  int nChannels = orig->nChannels;
  for(int iDst, iSrc, i = 0; i < height; i++) {
      iDst = height - 1 - i;
      iSrc = rdx2[i + iDec];
      for(int j = 0; j < orig->width; j++) 
          for(int k = 0; k < nChannels; k++)
              copyData[iDst*step + j*nChannels + k] = shufData[iSrc*step + j*nChannels + k];
  }
  showImage("rest3", rest3, 500, 50);


  // wait for keyboard input
  cvWaitKey(0);

  // release the images
  cvReleaseImage(&orig);
  cvReleaseImage(&shuf);
  cvReleaseImage(&gray); 
  cvReleaseImage(&graySort);
  //cvReleaseImage(&rest1);
  //cvReleaseImage(&rest2);
  return 0;
}


int test_ocvRandomRows(const char *imgFileName)
{
    if(imgFileName == NULL){
        printf("Usage: main <image-file-name>\n\7");
        exit(0);
    }

    // load an image  
    IplImage *orig = orig=cvLoadImage(imgFileName, CV_LOAD_IMAGE_COLOR );
    if(!orig){
        printf("Could not load image file: %s\n",imgFileName);
        return -1;
    }
    return test_ocvRandomRows(orig);
}


int test_ocvRandomRows(int nCols, int nRows)    // width, height
{
    // synthesize an image  
    IplImage *orig = cvCreateImage( cvSize( nCols, nRows ), IPL_DEPTH_8U, 3 );
    if(!orig){
        printf("Could not create image file: %d x %d\n", nCols, nRows);
        return -1;
    }
    uchar *origData = (uchar *)orig->imageData;
    int    origStep = orig->widthStep;
    uchar *pRGB = NULL, rgb = 0;
    for(int i = 0; i < nRows; i++) {
        pRGB = &origData[i*origStep];
        rgb  = (i * 255) / nRows;
        for(int j = 0; j < nCols; j++) {
            *pRGB++ = rgb;
            *pRGB++ = rgb;
            *pRGB++ = rgb;
        }
    }

    return test_ocvRandomRows(orig);
}

#endif // 00