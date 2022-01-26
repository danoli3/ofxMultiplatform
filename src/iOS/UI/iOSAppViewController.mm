// iOSAppViewController
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#import "iOSAppViewController.h"
#import "ofxAppiOSLayer.h"


@interface iOSAppViewController() <ofxAppiOSLayerAppDelegate, UIWebViewDelegate> {
    ofxAppiOSLayer * appLayer;
}
@end

@implementation iOSAppViewController

- (id)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app {
    appLayer = (ofxAppiOSLayer*)app;
    self = [super initWithFrame:frame app:app];
    appLayer->setDelegate(self);
    return self;
}

- (void) dealloc {
    
    if(appLayer) {
        appLayer->clearDelegate();
        appLayer = nil;
    }
    [super dealloc];
}

//-------------------------------------------------------------- iOS5 and earlier.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return (toInterfaceOrientation == UIInterfaceOrientationMaskLandscapeLeft);
}

//-------------------------------------------------------------- iOS6.

- (BOOL)shouldAutorotate {
    return YES;
}

@end
