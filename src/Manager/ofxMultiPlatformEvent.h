// ofxMultiPlatformEvent
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#pragma once

#ifndef __ofxMultiPlatformEvent__
#define __ofxMultiPlatformEvent__

#include "ofMain.h"

// This class defines event message structure to be sent back to the Proxy App Layer to trigger Platform specific functions
//---------------------------------------------
class ofxMultiPlatformEvent : public ofEventArgs {
    
public:
    
    string message;
    int packetID;
    
    ofxMultiPlatformEvent() {
        // init defaults
        packetID = 0;
        message = "";
    }
    
    static ofEvent <ofxMultiPlatformEvent> events;
};

// This class defines event message structure to be sent back to the Proxy App Layer to trigger Platform specific functions
//---------------------------------------------
class ofxAppEvent : public ofEventArgs {
    
public:
    
    string message;
    int packetID;
    
    ofxAppEvent() {
        // init defaults
        packetID = 0;
        message = "";
    }
    
    static ofEvent <ofxAppEvent> events;
};
//---------------------------------------------
#endif /* defined(__ofxMultiPlatformEvent__) */
