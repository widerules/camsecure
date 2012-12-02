//
//  RALPHUIView.h
//
//  Created by Ronald on 14/05/11.
//  by Ronald
//

#import <Cocoa/Cocoa.h>

@interface RALPHUIView : NSObject {
    IBOutlet id SetButton;
	 IBOutlet id image;
	bool worker;
	NSSound *player;
	NSImage* mImage;
	bool toggle;
}
- (IBAction)AudioSelector:(id)sender;
- (IBAction)Convertor:(id)sender;
- (IBAction)VideoSelector:(id)sender;
@end
