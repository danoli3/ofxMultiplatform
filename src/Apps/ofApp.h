// ofApp.h
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#pragma once

#include "ofMain.h"

#include "ofxBaseApp.h" // <--- Add this

//--------------------------------------------------------------
class ofApp : public ofxBaseApp{  //public ofBaseApp{ // <--- change this to ofxBaseApp

	public:
    
    
        ofApp();  // <-- Add constructor to header and to source!
        ~ofApp();
        string getClassName() { return "ofApp"; } // <--- ADD THIS! WITH THIS CLASSNAME
    
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
};
