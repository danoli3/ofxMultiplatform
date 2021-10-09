// ofxBaseApp
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#pragma once

#include "ofMain.h"
#include "ofxMultiPlatformEvent.h"
#include "ofxAppGlobals.h"

// ----------------
// Implement your classes of this type to get access to touch events!
//

class ofxBaseApp : public ofBaseApp {
    
public:
    
    ofxBaseApp() {
        appName = "";
    }
    ofxBaseApp(string name = "") : appName(name) { }
    ~ofxBaseApp(){}
    
    virtual string getAppName();
    virtual void touchDown(int x, int y, int id){};
    virtual void touchMoved(int x, int y, int id){};
    virtual void touchUp(int x, int y, int id){};
    virtual void touchDoubleTap(int x, int y, int id){};
    virtual void touchCancelled(int x, int y, int id){};
    
//-------  Other ofBaseApp Virtual Functions which can be Implemented in inheriting classes
//    virtual void setup(ofEventArgs & args);
//    virtual void update(ofEventArgs & args);
//    virtual void draw(ofEventArgs & args);
//    virtual void exit(ofEventArgs & args);
//    virtual void windowResized(ofResizeEventArgs & resize);
//    virtual void keyPressed( ofKeyEventArgs & key );
//    virtual void keyReleased( ofKeyEventArgs & key );
//    virtual void mouseMoved( ofMouseEventArgs & mouse );
//    virtual void mouseDragged( ofMouseEventArgs & mouse );
//    virtual void mousePressed( ofMouseEventArgs & mouse );
//    virtual void mouseReleased(ofMouseEventArgs & mouse);
//    virtual void windowEntry(ofEntryEventArgs & entry);
//    virtual void dragged(ofDragInfo & drag);
//    virtual void messageReceived(ofMessage & message);
//    virtual void dragEvent(ofDragInfo dragInfo);
//    virtual void gotMessage(ofMessage msg);
//    int mouseX, mouseY;			// for processing heads
protected:
    string appName;
    virtual string getClassName();
};

inline string ofxBaseApp::getClassName() {
    ofLog(OF_LOG_WARNING, "ofxBaseApp::getClassName. Needs to be implemented in child!");
    return "";
}

inline string ofxBaseApp::getAppName() {
    return appName;
}
