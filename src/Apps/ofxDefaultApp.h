// ofDefaultApp
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#pragma once

#include "ofxBaseApp.h"


///------------------------------------------
// Example Application on how to use
class ofxDefaultApp : public ofxBaseApp {
    
public:
    
    string getClassName() { return "ofxDefaultApp"; }
    
    ofxDefaultApp();
    ~ofxDefaultApp();
};
