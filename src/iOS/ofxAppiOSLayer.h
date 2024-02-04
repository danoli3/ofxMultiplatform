// ofxAppiOSLayer
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#pragma once

#include "ofMain.h"

#if defined(TARGET_OS_IOS)

#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#include "ofxAppManager.h"

#include "ofxMultiPlatformEvent.h"

@protocol ofxAppiOSLayerAppDelegate <NSObject>
@optional
- (void)launchWebView;
@end


class ofxAppiOSLayer : public ofxiOSApp{
	
public:
    void setup();
    void update();
    void draw();
    void exit();
	
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);
    
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    
    void gameControllerEvent(GameControllerEvent & event);
    
    void triggerEvent(ofxMultiPlatformEvent &e);
    
    void setDelegate(id delegate);
    void clearDelegate();
    // variables
    id delegate;

    ofxAppManager* manager;
};

#endif
