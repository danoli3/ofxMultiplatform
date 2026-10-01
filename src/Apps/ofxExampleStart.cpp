//  Created by Daniel Rosser on 4/02/2024.
// ofxExampleStart.cpp
//--------------------------------------------------------------
#include "ofxExampleStart.h"

//--------------------------------------------------------------
ofxExampleStart::ofxExampleStart()
: ofxBaseApp("ofxExampleStart") {
}

ofxExampleStart::~ofxExampleStart() {
}

//--------------------------------------------------------------
void ofxExampleStart::setup() {
	ofEnableAlphaBlending();
	ofSetBackgroundColor(12, 12, 16);
	age = 0.f;
	contentFade = 0.f;
	finished = false;
}

//--------------------------------------------------------------
void ofxExampleStart::onEnter() {
	age = 0.f;
	contentFade = 0.f;
	finished = false;
	ofLogNotice("ofxExampleStart") << "onEnter — intro";
}

//--------------------------------------------------------------
void ofxExampleStart::onExit(float durationSec) {
	ofLogNotice("ofxExampleStart") << "onExit (" << durationSec << "s)";
}

//--------------------------------------------------------------
void ofxExampleStart::update() {
	const float dt = ofGetLastFrameTime();
	age += dt;

	// Content fade over ~0.8s
	contentFade = ofClamp(age / 0.8f, 0.f, 1.f);

	if(!finished && age >= holdSeconds) {
		markFinished();
	}
}

//--------------------------------------------------------------
void ofxExampleStart::draw() {
	ofPushStyle();

	const int a = (int)(contentFade * 255);
	ofSetColor(220, 220, 230, a);
	ofDrawBitmapString("ofxMultiPlatform", ofGetWidth() * 0.5f - 70, ofGetHeight() * 0.45f);
	ofSetColor(160, 160, 180, a);
	ofDrawBitmapString("intro scene  ·  click or space to skip", ofGetWidth() * 0.5f - 140, ofGetHeight() * 0.45f + 28);

	// Progress bar toward auto-advance
	const float p = ofClamp(age / holdSeconds, 0.f, 1.f);
	const float barW = 200.f;
	const float barX = (ofGetWidth() - barW) * 0.5f;
	const float barY = ofGetHeight() * 0.45f + 60;
	ofSetColor(50, 50, 60, a);
	ofDrawRectangle(barX, barY, barW, 4);
	ofSetColor(120, 160, 255, a);
	ofDrawRectangle(barX, barY, barW * p, 4);

	ofPopStyle();
}

//--------------------------------------------------------------
void ofxExampleStart::markFinished() {
	if(finished) {
		return;
	}
	finished = true;
	ofLogNotice("ofxExampleStart") << "finished → menu";
}

//--------------------------------------------------------------
void ofxExampleStart::keyPressed(int key) {
	if(key == ' ' || key == OF_KEY_RETURN) {
		markFinished();
	}
}

void ofxExampleStart::mousePressed(int x, int y, int button) {
	(void)x;
	(void)y;
	(void)button;
	markFinished();
}

void ofxExampleStart::touchDown(int x, int y, int id) {
	(void)x;
	(void)y;
	(void)id;
	markFinished();
}
