/*
 *  Tracker.c
 *  RALPH
 *
 *  Created by Ronald on 14/05/11.
 *  by Ronald
 *
 */

#include "Tracker.h"


#include <Opencv/cv.h>
#include <Opencv/highgui.h>


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vector.h>






using namespace std;

static int Controls=0;

static	int Disturbances = 0;

static	int counter=0;


static std::vector<DATA*> mStore;



DATA* getData()
{
	
		if (mStore.size()>0) {
			return mStore[0];
		}
	return NULL;
}

void deleteafter()
{
	
		if (mStore.size()>0) {
			free(mStore[0]->mdata);
			delete mStore[0];
			mStore.erase(mStore.begin());
		}
	
}

void pushtoStore(DATA* mdata)
{
	
	mStore.push_back(mdata);
	
}

int updateTracker(int data)
{
	Disturbances =0;
	
	Controls = data;
	
	return 0;
}

void StoreImage(IplImage* mImage,bool CG)
{
	CvSize imgSize;
	imgSize.width = 200;
	imgSize.height = 200;
	
	if (CG) 
	{
		//printf("\n %d",mStore.size());
		IplImage* motionHistory = cvCreateImage( imgSize, IPL_DEPTH_8U, 3);
		cvResize(mImage, motionHistory, CV_INTER_LINEAR);
		DATA* md = new DATA();
		md->mdata = (unsigned char*) malloc(motionHistory->imageSize);
		md->size = motionHistory->imageSize;
		memcpy(md->mdata,motionHistory->imageData,md->size);
		md->channel = motionHistory->nChannels;
		md->depth = motionHistory->depth;
		md->widthStep = motionHistory->widthStep;
		pushtoStore(md);
		
	//	char buf[100];
//		sprintf(buf,"/Users/Ronald/Desktop/SEC/%d.jpg",counter);
//		cvSaveImage(buf, mImage);
		
		

	}
	else
	{
		IplImage* motionHistory = cvCreateImage( imgSize, IPL_DEPTH_8U, 1);
		cvResize(mImage, motionHistory, CV_INTER_LINEAR);
		DATA* md = new DATA();
		md->mdata = (unsigned char*) malloc(motionHistory->imageSize);
		md->size = motionHistory->imageSize;
		memcpy(md->mdata,motionHistory->imageData,md->size);
			md->channel = motionHistory->nChannels;
		md->depth = motionHistory->depth;
			md->widthStep = motionHistory->widthStep;
		pushtoStore(md);

	}

		
}

int getdisturbances()
{
	
	return Disturbances;
	
}
int StartTracking()
{

	//char* outFilename = "/Users/Ronald/Desktop/SEC/";
	//CvVideoWriter* outputMovie = cvCreateVideoWriter(outFilename, 
//													 CV_FOURCC('F', 'L', 'V', 'I'), 29.97, cvSize(720, 576));
	
//	cvNamedWindow("SECURITY FEED", CV_WINDOW_AUTOSIZE);
	CvCapture *camfeed;
	
	camfeed = cvCaptureFromCAM( CV_CAP_ANY );
	
	if (!camfeed)
	{
		printf("\n CAMERA NOT ACCESSIBLE EXITING");
		return 0;
	}
	
	
	
	IplImage* frame=NULL;
	
	while (frame==NULL)
	{
		frame= cvQueryFrame( camfeed );
	}
	
	CvSize imgSize;
	imgSize.width = frame->width;
	imgSize.height = frame->height;
	
	IplImage* greyImage = cvCreateImage( imgSize, IPL_DEPTH_8U, 1);
	IplImage* colourImage;
	IplImage* movingAverage = cvCreateImage( imgSize, IPL_DEPTH_32F, 3);
	IplImage* difference;
	IplImage* temp;
	IplImage* mTempImage= cvCreateImage( imgSize, IPL_DEPTH_8U, 1);
	//IplImage* motionHistory = cvCreateImage( imgSize, IPL_DEPTH_8U, 3);
	CvRect bndRect = cvRect(0,0,0,0);
	CvPoint pt1, pt2;
	CvFont font;

	char wow[65];
	int avgX = 0;
	bool first = true;
	
	for(;Controls==1;)
	{
		bool enter =0;
		colourImage = cvQueryFrame(camfeed);
		
		if( !colourImage )
		{
			continue;
		}
		
		if(first)
		{
			difference = cvCloneImage(colourImage);
			temp = cvCloneImage(colourImage);
			cvConvertScale(colourImage, movingAverage, 1.0, 0.0);
			first = false;
		}
		else
		{
			cvRunningAvg(colourImage, movingAverage, 0.01, NULL);
		}
		
		cvConvertScale(movingAverage,temp, 1.0, 0.0);
		
		cvAbsDiff(colourImage,temp,difference);
		
		cvCvtColor(difference,greyImage,CV_RGB2GRAY);
		
		cvThreshold(greyImage, greyImage, 70, 255, CV_THRESH_BINARY);
		
		cvDilate(greyImage, greyImage, 0, 18);
		cvErode(greyImage, greyImage, 0, 10);
		
		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* contour = 0;
		cvFindContours( greyImage, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
		
		for( ; contour != 0; contour = contour->h_next )
		{
			enter=1;
			bndRect = cvBoundingRect(contour, 0);
			pt1.x = bndRect.x;
			pt1.y = bndRect.y;
			pt2.x = bndRect.x + bndRect.width;
			pt2.y = bndRect.y + bndRect.height;
			avgX = (pt1.x + pt2.x) / 2;
			
			CvPoint avg;
			avg.x = (pt1.x+pt2.x)/2;
			avg.y = (pt1.y+pt2.y)/2;
			cvRectangle(colourImage, pt1, pt2, CV_RGB(255,0,0), 0.5);
			//mTempImage =  cvCloneImage(colourImage);
			cvCvtColor(colourImage,mTempImage,CV_RGB2GRAY);
			
		//	colourImage = mTempImage;
		//	cvCircle(colourImage, avg, 5, CV_RGB(255,0,205), 2, 1, 1);
			Disturbances++;
			
		}
		
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.8, 0.8, 0, 2);
		sprintf(wow, "Security Breach %d",Disturbances);
		cvPutText(colourImage, wow, cvPoint(60, 60), &font, cvScalar(0, 0, 300));
		if (enter==1) {
			//cvWriteFrame(outputMovie, colourImage);
			counter++;
						
			
			StoreImage(colourImage,true);
			//cvShowImage("SECURITY FEED", colourImage);
		}
		else 
		{
			cvCvtColor(colourImage,mTempImage,CV_RGB2GRAY);
			StoreImage(mTempImage,false);
			//cvShowImage("SECURITY FEED", mTempImage);
			
		}

		//cvWaitKey(10);
		
	}
	
	
	cvReleaseImage(&temp);
	cvReleaseImage(&difference);
	cvReleaseImage(&greyImage);
	cvReleaseImage(&movingAverage);
//	cvReleaseVideoWriter(&outputMovie);
	
	cvReleaseCapture( &camfeed );
	cvReleaseImage(&mTempImage);
//	cvDestroyWindow( "SECURITY FEED" );
	
	
	return 0;


}



