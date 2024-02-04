//
//  ofxExampleStart.hpp
//
//  Created by Daniel Rosser on 4/02/2024.
//
//

#ifndef ofxExampleStart_hpp
#define ofxExampleStart_hpp

#pragma once
#include "ofMain.h"

using std::string;
using std::vector;

#include "ofImage.h"

#include "ofxBaseApp.h" // <--- Add this
//--------------------------------------------------------------
class ofxExampleStart : public ofxBaseApp{  //public ofBaseApp{ // <--- change this to ofxBaseApp
    
public:
    
    
    ofxExampleStart();  // <-- Add constructor to header and to source!
    ~ofxExampleStart();
    string getClassName() { return "ofxExampleStart"; } // <--- ADD THIS! WITH THIS CLASSNAME
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    // --- Implement touch events!!!!
    void touchDown(int x, int y, int id){};
    void touchMoved(int x, int y, int id){};
    void touchUp(int x, int y, int id){};
    void touchDoubleTap(int x, int y, int id){};
    void touchCancelled(int x, int y, int id){};
    
    ofShader shader;
	ofFbo fboBlurOnePass;

    ofPlanePrimitive plane;
	ofShortImage * image;
    
    string fragShader;
    float beat = 0;
    
    virtual float endTime() {
        return 8; // MS
    }
    virtual bool hasEnded() {
        return (currentTime >= endTime());
    }
 
    
    float fadeInTotal = 10;
    float currentFadeIn = 0;
    bool isFadedIn = false;
  
    ofSoundPlayer sound;
};


#endif /* ofxExampleStart_hpp */
