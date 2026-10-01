// ofxExampleMenu.cpp
//--------------------------------------------------------------
#include "ofxExampleMenu.h"
#include "ofxSceneRegistry.h"

//--------------------------------------------------------------
ofxExampleMenu::ofxExampleMenu()
: ofxBaseApp("ofxExampleMenu") {
}

ofxExampleMenu::~ofxExampleMenu() {
}

//--------------------------------------------------------------
void ofxExampleMenu::setup() {
	ofSetBackgroundColor(18, 18, 24);
	items.clear();
	items.push_back({ "Start / intro (ofxExampleStart)", "ofxExampleStart", {} });
	items.push_back({ "Main empty app (ofApp)", "ofApp", {} });
	items.push_back({ "Default stub (ofxDefaultApp)", "ofxDefaultApp", {} });
	layoutButtons();
}

//--------------------------------------------------------------
void ofxExampleMenu::onEnter() {
	ofLogNotice("ofxExampleMenu") << "onEnter — pick a scene";
	layoutButtons();
}

//--------------------------------------------------------------
void ofxExampleMenu::layoutButtons() {
	const float w = ofGetWidth();
	const float margin = 40.f;
	const float btnH = 52.f;
	const float gap = 14.f;
	const float btnW = ofClamp(w - margin * 2.f, 200.f, 520.f);
	const float startY = ofGetHeight() * 0.35f;

	for(size_t i = 0; i < items.size(); ++i) {
		const float x = (w - btnW) * 0.5f;
		const float y = startY + (float)i * (btnH + gap);
		items[i].bounds.set(x, y, btnW, btnH);
	}
}

//--------------------------------------------------------------
void ofxExampleMenu::update() {
	// hover for mouse
	hoverIndex = -1;
	const int mx = ofGetMouseX();
	const int my = ofGetMouseY();
	for(size_t i = 0; i < items.size(); ++i) {
		if(items[i].bounds.inside(mx, my)) {
			hoverIndex = (int)i;
			break;
		}
	}
}

//--------------------------------------------------------------
void ofxExampleMenu::draw() {
	ofPushStyle();
	ofSetColor(230);
	ofDrawBitmapStringHighlight("ofxMultiPlatform  ·  scene menu", 40, 48);
	ofSetColor(160);
	ofDrawBitmapString("Click / tap a row, or press 1–3.  D toggles debug HUD.", 40, 72);

	for(size_t i = 0; i < items.size(); ++i) {
		const bool hot = ((int)i == hoverIndex);
		ofSetColor(hot ? ofColor(70, 90, 160) : ofColor(40, 42, 55));
		ofDrawRectRounded(items[i].bounds, 8);
		ofSetColor(hot ? ofColor(255) : ofColor(210));
		const std::string line = ofToString((int)i + 1) + "   " + items[i].label;
		ofDrawBitmapString(line, items[i].bounds.x + 18, items[i].bounds.y + items[i].bounds.height * 0.58f);
	}

	ofSetColor(120);
	ofDrawBitmapString("Registered scenes:", 40, ofGetHeight() - 60);
	float y = ofGetHeight() - 42;
	for(const auto & id : ofxSceneRegistry::ids()) {
		ofDrawBitmapString("  · " + id, 40, y);
		y += 14;
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxExampleMenu::hit(int x, int y) {
	for(const auto & item : items) {
		if(item.bounds.inside(x, y)) {
			ofLogNotice("ofxExampleMenu") << "→ " << item.sceneId;
			requestAppChange(item.sceneId);
			return;
		}
	}
}

//--------------------------------------------------------------
void ofxExampleMenu::mousePressed(int x, int y, int button) {
	(void)button;
	hit(x, y);
}

void ofxExampleMenu::touchDown(int x, int y, int id) {
	(void)id;
	hit(x, y);
}

//--------------------------------------------------------------
void ofxExampleMenu::keyPressed(int key) {
	if(key >= '1' && key <= '9') {
		const size_t idx = (size_t)(key - '1');
		if(idx < items.size()) {
			requestAppChange(items[idx].sceneId);
		}
	}
}

//--------------------------------------------------------------
void ofxExampleMenu::windowResized(int w, int h) {
	(void)w;
	(void)h;
	layoutButtons();
}
