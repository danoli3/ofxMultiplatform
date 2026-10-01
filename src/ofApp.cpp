// ofApp
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp()
: ofxBaseApp("ofApp") {
}

ofApp::~ofApp() {
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(8, 12, 20);
}

void ofApp::onEnter() {
	ofLogNotice("ofApp") << "onEnter — main scene (M = menu)";
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofPushStyle();
	ofSetColor(210);
	ofDrawBitmapString("ofApp  ·  main scene", 40, 60);
	ofSetColor(140);
	ofDrawBitmapString("Press M for scene menu", 40, 88);
	ofDrawBitmapString("Press I for intro", 40, 108);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if(key == 'm' || key == 'M') {
		requestAppChange("ofxExampleMenu");
	} else if(key == 'i' || key == 'I') {
		requestAppChange("ofxExampleStart");
	}
}

void ofApp::keyReleased(int key) { (void)key; }
void ofApp::mouseMoved(int x, int y) { (void)x; (void)y; }
void ofApp::mouseDragged(int x, int y, int button) { (void)x; (void)y; (void)button; }
void ofApp::mousePressed(int x, int y, int button) { (void)x; (void)y; (void)button; }
void ofApp::mouseReleased(int x, int y, int button) { (void)x; (void)y; (void)button; }
void ofApp::mouseEntered(int x, int y) { (void)x; (void)y; }
void ofApp::mouseExited(int x, int y) { (void)x; (void)y; }
void ofApp::windowResized(int w, int h) { (void)w; (void)h; }
void ofApp::gotMessage(ofMessage msg) { (void)msg; }
void ofApp::dragEvent(ofDragInfo dragInfo) { (void)dragInfo; }
