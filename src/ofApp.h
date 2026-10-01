// ofApp.h
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#pragma once

#include "ofMain.h"
#include "ofxBaseApp.h"

//--------------------------------------------------------------
class ofApp : public ofxBaseApp {

public:
	ofApp();
	~ofApp() override;

	std::string getClassName() { return "ofApp"; }

	void setup() override;
	void update() override;
	void draw() override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;
	void windowResized(int w, int h) override;
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void dragEvent(ofDragInfo dragInfo) override;
	void gotMessage(ofMessage msg) override;

	void onEnter() override;

	void touchDown(int x, int y, int id) override {}
	void touchMoved(int x, int y, int id) override {}
	void touchUp(int x, int y, int id) override {}
	void touchDoubleTap(int x, int y, int id) override {}
	void touchCancelled(int x, int y, int id) override {}

	float enterDuration() const override { return 0.3f; }
	float exitDuration() const override { return 0.25f; }
};
