// ofDefaultApp
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#include "ofxDefaultApp.h"

ofxDefaultApp::ofxDefaultApp()
: ofxBaseApp("ofxDefaultApp") {
}

ofxDefaultApp::~ofxDefaultApp() {
}

void ofxDefaultApp::setup() {
	ofSetBackgroundColor(20, 24, 20);
}

void ofxDefaultApp::onEnter() {
	ofLogNotice("ofxDefaultApp") << "onEnter";
}

void ofxDefaultApp::draw() {
	ofPushStyle();
	ofSetColor(200);
	ofDrawBitmapString("ofxDefaultApp  ·  stub scene", 40, 60);
	ofSetColor(140);
	ofDrawBitmapString("Press M for menu", 40, 88);
	ofPopStyle();
}

void ofxDefaultApp::keyPressed(int key) {
	if(key == 'm' || key == 'M') {
		requestAppChange("ofxExampleMenu");
	}
}
