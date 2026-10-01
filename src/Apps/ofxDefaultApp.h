// ofDefaultApp
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#pragma once

#include "ofxBaseApp.h"
#include <string>

/// Minimal stub scene — useful as a registry example / placeholder.
class ofxDefaultApp : public ofxBaseApp {

public:
	std::string getClassName() { return "ofxDefaultApp"; }

	ofxDefaultApp();
	~ofxDefaultApp() override;

	void setup() override;
	void draw() override;
	void keyPressed(int key) override;
	void onEnter() override;

	float enterDuration() const override { return 0.2f; }
	float exitDuration() const override { return 0.2f; }
};
