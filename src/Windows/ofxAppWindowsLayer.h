#pragma once

#include "ofMain.h"

#if defined(_WIN64) || defined(_WIN32)

	#include "ofBaseApp.h"
	#include "ofxAppManager.h"
	#include "ofxMultiPlatformEvent.h"

class ofxAppWindowsLayer : public ofBaseApp {

public:
	ofxAppWindowsLayer() {
		manager = nullptr;
	}
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void touchDown(ofTouchEventArgs & touch);
	void touchMoved(ofTouchEventArgs & touch);
	void touchUp(ofTouchEventArgs & touch);
	void touchDoubleTap(ofTouchEventArgs & touch);
	void touchCancelled(ofTouchEventArgs & touch);

	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);

	void gameControllerEvent(GameControllerEvent & event);

	void triggerEvent(ofxMultiPlatformEvent & e);

	ofxAppManager * manager;
};

#endif
