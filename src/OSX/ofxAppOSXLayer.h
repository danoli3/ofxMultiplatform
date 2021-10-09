//
// ofxAppOSXLayer
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.

#pragma once
#ifndef __ofxAppOSXLayer__
#define __ofxAppOSXLayer__

#include "ofMain.h"

#include "ofxAppManager.h"
#include "ofxMultiPlatformEvent.h"

//@protocol ofAppLayerOSXAppDelegate <NSObject>
//@optional
//- (void)launchWebView;
//@end


class ofxAppOSXLayer : public ofBaseApp {
	
public:
    ofxAppOSXLayer() {
        app = NULL;
    }
    void setup();
    void update();
    void draw();
    void exit();
    
    void launchWebView();
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);
    
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    
    void triggerEvent(ofxMultiPlatformEvent &e);

    ofxAppManager * app;

//    void setDelegate(id delegate);
//    void clearDelegate();
//    // variables
//    id delegate;
};

#endif /* defined(__ofxAppOSXLayer__) */


