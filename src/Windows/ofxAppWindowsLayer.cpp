#include "ofxAppWindowsLayer.h"

#if defined(_WIN64) || defined(_WIN32)

//--------------------------------------------------------------
void ofxAppWindowsLayer::setup() {

	ofAddListener(ofxMultiPlatformEvent::events, this, &ofxAppWindowsLayer::triggerEvent);

	ofSetOrientation(OF_ORIENTATION_DEFAULT);

	ofLog(OF_LOG_NOTICE, "ofxAppWindowsLayer:setup ");
	ofLog(OF_LOG_NOTICE, "ofGetScreenWidth: " + ofToString(ofGetScreenWidth()));
	ofLog(OF_LOG_NOTICE, "ofGetScreenHeight: " + ofToString(ofGetScreenHeight()));
	ofLog(OF_LOG_NOTICE, "ofGetWidth: " + ofToString(ofGetWidth()));
	ofLog(OF_LOG_NOTICE, "ofGetHeight: " + ofToString(ofGetHeight()));

	manager = new ofxAppManager();
	manager->setup();
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::update() {
	if (manager != nullptr)
		manager->update();
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::draw() {
	if (manager != nullptr)
		manager->draw();
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::exit() {
	if (manager != nullptr)
		manager->exit();
	ofRemoveListener(ofxMultiPlatformEvent::events, this, &ofxAppWindowsLayer::triggerEvent);

	if (manager != nullptr) {
		delete manager;
		manager = nullptr;
	}
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::touchDown(ofTouchEventArgs & touch) {
	if (manager != nullptr)
		manager->touchDown(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::touchMoved(ofTouchEventArgs & touch) {
	if (manager != nullptr)
		manager->touchMoved(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::touchUp(ofTouchEventArgs & touch) {
	if (manager != nullptr)
		manager->touchUp(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::touchDoubleTap(ofTouchEventArgs & touch) {
	if (manager != nullptr)
		manager->touchDoubleTap(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::touchCancelled(ofTouchEventArgs & touch) {
	if (manager != nullptr)
		manager->touchCancelled(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::lostFocus() {
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::gotFocus() {
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::gotMemoryWarning() {
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::deviceOrientationChanged(int newOrientation) {
}

void ofxAppWindowsLayer::triggerEvent(ofxMultiPlatformEvent & e) {
	// manage event for Windows.
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::keyPressed(int key) {
	if (manager != nullptr) {
		manager->keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::keyReleased(int key) {
	if (manager != nullptr) {
		manager->keyReleased(key);
	}
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::mouseMoved(int x, int y) {
	if (manager != nullptr) {
		manager->mouseMoved(x, y);
	}
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::mousePressed(int x, int y, int button) {
	if (manager != nullptr) {
		manager->mousePressed(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::mouseDragged(int x, int y, int button) {
	if (manager != nullptr) {
		manager->mouseDragged(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::mouseReleased(int x, int y, int button) {
	if (manager != nullptr) {
		manager->mouseReleased(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::windowResized(int w, int h) {
	if (manager != nullptr) {
		manager->windowResized(w, h);
	}
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::gotMessage(ofMessage msg) {
	if (manager != nullptr) {
		manager->gotMessage(msg);
	}
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::dragEvent(ofDragInfo dragInfo) {
	if (manager != nullptr) {
		manager->dragEvent(dragInfo);
	}
}
#endif
