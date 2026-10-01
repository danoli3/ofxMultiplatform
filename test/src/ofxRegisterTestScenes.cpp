// Test-app scene registration (replaces main ofxRegisterScenes.cpp)
#include "ofxSceneRegistry.h"
#include "TestScenes.h"

void ofxRegisterProjectScenes() {
	ofxRegisterScene<TestSceneA>("TestSceneA");
	ofxRegisterScene<TestSceneB>("TestSceneB");
	ofxRegisterScene<TestSceneEvent>("TestSceneEvent");
}
