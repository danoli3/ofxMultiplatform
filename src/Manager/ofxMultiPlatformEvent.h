// ofxMultiPlatformEvent
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#pragma once

#ifndef __ofxMultiPlatformEvent__
#define __ofxMultiPlatformEvent__

#include "ofMain.h"
#include <string>

// Packet ids for ofxAppEvent (scene manager)
namespace ofxAppEventID {
	const int ChangeApp = 1;   // message = scene / app id to load
	const int Platform  = 100; // reserved for platform-layer messages
}

// Packet ids for ofxMultiPlatformEvent (platform proxy layer)
namespace ofxMultiPlatformEventID {
	const int Generic = 0;
}

// Event for platform-specific proxy layers (iOS/Android/OSX shells)
//---------------------------------------------
class ofxMultiPlatformEvent : public ofEventArgs {

public:

	std::string message;
	int packetID;

	ofxMultiPlatformEvent() {
		packetID = 0;
		message = "";
	}

	static ofEvent <ofxMultiPlatformEvent> events;
};

// Event for scene / app manager (change scene, etc.)
//---------------------------------------------
class ofxAppEvent : public ofEventArgs {

public:

	std::string message;
	int packetID;

	ofxAppEvent() {
		packetID = 0;
		message = "";
	}

	static ofEvent <ofxAppEvent> events;
};

/// Request a scene change from anywhere (current scene, UI, etc.).
inline void ofxRequestAppChange(const std::string & appID) {
	ofxAppEvent e;
	e.packetID = ofxAppEventID::ChangeApp;
	e.message = appID;
	ofNotifyEvent(ofxAppEvent::events, e);
}

//---------------------------------------------
#endif /* defined(__ofxMultiPlatformEvent__) */
