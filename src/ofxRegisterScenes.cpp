// ofxRegisterScenes.cpp — main app scene registration
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
//--------------------------------------------------------------
#include "ofxSceneRegistry.h"

#include "ofApp.h"
#include "ofxDefaultApp.h"
#include "ofxExampleStart.h"
#include "ofxExampleMenu.h"

void ofxRegisterProjectScenes() {
	ofxRegisterScene<ofApp>("ofApp");
	ofxRegisterScene<ofxExampleStart>("ofxExampleStart");
	ofxRegisterScene<ofxExampleMenu>("ofxExampleMenu");
	ofxRegisterScene<ofxDefaultApp>("ofxDefaultApp");
}
