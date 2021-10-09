// ofxAppManager.h
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#pragma once

#ifndef __ofxAppManager_h__
#define __ofxAppManager_h__

#include "ofMain.h"
#include "ofxBaseApp.h"
#include "ofxAppGlobals.h"
#include "ofxMultiPlatformEvent.h"

//-------------------------
// --- Include for Default oF Example App 
#include "ofApp.h"


//-------------------------

#include "ofxDefaultApp.h"

//---------

class ofxAppManager : public ofBaseApp {

public:
    
    ofxAppManager();
    ~ofxAppManager();
    
    void setup();
    
    
    void loadApp(string appID);
    void killApp();
    string getAppID();
    
    void update();
    void draw();
    
    void touchDown(int x, int y, int id);
    void touchMoved(int x, int y, int id);
    void touchUp(int x, int y, int id);
    void touchDoubleTap(int x, int y, int id);
    void touchCancelled(int x, int y, int id);
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    
    bool bDebug;
    float clickedTime;

    ofxBaseApp * app;
};

#endif
