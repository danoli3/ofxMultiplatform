// ofxExampleMenu.h
// Demo hub scene — pick next scene, shows requestAppChange + registry ids.
//--------------------------------------------------------------
#pragma once

#include "ofxBaseApp.h"
#include <string>
#include <vector>

struct ofxMenuItem {
	std::string label;
	std::string sceneId;
	ofRectangle bounds;
};

class ofxExampleMenu : public ofxBaseApp {

public:
	ofxExampleMenu();
	~ofxExampleMenu() override;

	std::string getClassName() { return "ofxExampleMenu"; }

	void setup() override;
	void update() override;
	void draw() override;

	void keyPressed(int key) override;
	void mousePressed(int x, int y, int button) override;
	void touchDown(int x, int y, int id) override;
	void windowResized(int w, int h) override;

	void onEnter() override;
	float enterDuration() const override { return 0.25f; }
	float exitDuration() const override { return 0.25f; }

private:
	void layoutButtons();
	void hit(int x, int y);

	std::vector<ofxMenuItem> items;
	int hoverIndex = -1;
};
