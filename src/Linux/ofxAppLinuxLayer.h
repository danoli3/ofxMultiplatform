#pragma once

#ifndef __ofxAppLinuxLayer_h__
#define __ofxAppLinuxLayer_h__

#if defined(__linux__)

#include "ofMain.h"
#include "ofxAppManager.h"

#include "ofxMultiPlatformEvent.h"

class ofxAppLinuxLayer : public ofBaseApp {
	
public:
    ofxAppLinuxLayer() {
        manager = NULL;
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

    ofxAppManager * manager;

};

#endif

#endif
