//  Created by Daniel Rosser on 4/02/2024.
// ofxExampleStart.h
// Intro / splash scene — fades in, then auto-advances to the menu
// (or skip with click / space).
//--------------------------------------------------------------
#pragma once

#include "ofxBaseApp.h"
#include <string>

class ofxExampleStart : public ofxBaseApp {

public:
	ofxExampleStart();
	~ofxExampleStart() override;

	std::string getClassName() { return "ofxExampleStart"; }

	void setup() override;
	void update() override;
	void draw() override;

	void keyPressed(int key) override;
	void mousePressed(int x, int y, int button) override;
	void touchDown(int x, int y, int id) override;

	void onEnter() override;
	void onExit(float durationSec) override;

	bool hasEnded() const override { return finished; }
	std::string nextAppName() const override { return "ofxExampleMenu"; }

	float enterDuration() const override { return 0.4f; }
	float exitDuration() const override { return 0.3f; }

private:
	void markFinished();

	float holdSeconds = 2.5f;   // time after fade before auto-advance
	float age = 0.f;            // seconds since onEnter
	float contentFade = 0.f;    // 0..1 content opacity
	bool finished = false;
};
