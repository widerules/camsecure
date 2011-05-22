//
//  RALPHUIView.m
//
//  Created by Ronald on 14/05/11.
//  by Ronald
//

#import "RALPHUIView.h"

#include "Tracker.h"
#import <AudioToolbox/AudioServices.h>

@implementation RALPHUIView
- (IBAction)AudioSelector:(id)sender 
{
	toggle = false;
	player = [[NSSound alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"breach1" ofType:@"mp3"] byReference:NO];
	worker = true; 



	[self performSelectorInBackground:@selector(ThreadRunnerVideo) withObject:nil];
	[self performSelectorInBackground:@selector(alert) withObject:nil];
	[self performSelectorInBackground:@selector(render) withObject:nil];
	[self performSelectorInBackground:@selector(webserver) withObject:nil];
	
}

- (IBAction)Convertor:(id)sender 
{
    
}

- (IBAction)VideoSelector:(id)sender 
{
    updateTracker(0);
	worker=false;
	closeall();
	
}

-(void)updateUI:(NSData*)img
{
	
	if(mImage)
	{
		[mImage release];
		mImage = nil;
	
	}
	mImage = [[NSImage alloc] initWithData:img];
	
	[image setImage:mImage];
	
}

-(void)webserver
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	WebserverCaller();
	[pool release];
	
	
}

-(void)render
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	

	while(worker==true)
	{
		if (getData()!=NULL)
		{
			SendData(getData());
			
			NSString *COLORSPACE;
			if (getData()->channel==1 )
			{
				COLORSPACE = NSDeviceWhiteColorSpace;
			}
			else {
				COLORSPACE = NSDeviceRGBColorSpace;
			}

			NSBitmapImageRep * BMP = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:NULL pixelsWide:200 pixelsHigh:200 bitsPerSample:getData()->depth samplesPerPixel:getData()->channel hasAlpha:NO isPlanar:NO colorSpaceName:COLORSPACE bytesPerRow:getData()->widthStep bitsPerPixel:0];
			int x,y;
			unsigned int colors[3];
			for (y=0; y<200; y++)
			{
				for (x=0; x<200; x++)
				{
					if (getData()->channel>1)
					{
						colors[2] = (unsigned int) getData()->mdata[(y*getData()->widthStep) + (x*3)];
						colors[1] = (unsigned int)  getData()->mdata[(y*getData()->widthStep) + (x*3)+1];
						colors[0] = (unsigned int)  getData()->mdata[(y*getData()->widthStep) + (x*3)+2];
					}
					else
					{
						colors[0] = (unsigned int)  getData()->mdata[(y*200) + x];
					}
					[BMP setPixel:colors atX:x y:y];
				}
			}
			NSData *mData = [BMP TIFFRepresentation];
			
			
				deleteafter();
			
			[self performSelectorOnMainThread:@selector(updateUI:) withObject:mData waitUntilDone:YES ];
			
		}
				
	}
	
	printf("\n Thread 1 ended");
	
	[pool release];
	
}



-(void)alert
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	int temp =0;
	while(worker==true)
	{
		int temp1 = getdisturbances();
		if (temp!=temp1)
		{
			if (![player isPlaying]) 
			{
				//[self performSelectorOnMainThread:@selector(updateUI) withObject:nil waitUntilDone:YES ];
				[player play];
			}
			
			temp = temp1;
		}
		
	}	
		printf("\n Thread 2 ended");
	[pool release];
}

-(void) ThreadRunnerVideo
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	updateTracker(1);
	StartTracking();
	
		printf("\n Thread 3 ended");
	[pool release];
	
	
}
@end
