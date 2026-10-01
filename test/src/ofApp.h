#pragma once

#include "ofMain.h"
#include "ofxUnitTests.h"

/// Runs ofxMultiPlatform generic system tests and exits with fail count.
class ofApp : public ofxUnitTestsApp {
	void run() override;
};
