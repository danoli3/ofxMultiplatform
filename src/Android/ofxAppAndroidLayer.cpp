#include "ofxAppAndroidLayer.h"

#if defined(__ANDROID__)
//--------------------------------------------------------------
void ofxAppAndroidLayer::setup(){
	manager = new ofxAppManager();
	manager->setup();

	JNIEnv *env = ofGetJNIEnv();
	jclass localClass = env->FindClass("cc/openframeworks/ofxMultiplatform/OFActivity");
	javaClass = (jclass)env->NewGlobalRef(localClass);
	if(javaClass == NULL) {
		ofLog() << "javaClass not found!" << endl;
	}

	javaObject = ofGetOFActivityObject();
	javaObject = (jobject)env->NewGlobalRef(javaObject);
	if(javaObject == NULL) {
		ofLog() << "javaObject not found!" << endl;
	}
}

void ofxAppAndroidLayer::exit(){
	manager->exit();
	if(manager){
		delete manager;
		manager = NULL;
	}
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::update(){
	manager->update();
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::draw(){
	if(bCanReadFromDataFolder == false) {
        int timeSinceStart = ofGetSeconds();
        if(timeSinceStart % 2) {
            ofSetColor(255, 0, 0);
        } else {
            ofSetColor(0, 0, 0);
        }
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        ofDrawBitmapStringHighlight("Critical Error: Could not load Data Folder...", ofPoint(ofGetWidth()/2-150, ofGetHeight()/2-40), ofColor::black, ofColor::red);
        return;
    }
	manager->draw();
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::keyPressed  (int key){
	manager->keyPressed(key);
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::keyReleased(int key){
	manager->keyReleased(key);
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::windowResized(int w, int h){
	//manager->windowResized(w,h);
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::touchDown(int x, int y, int id){
	manager->touchDown(x, y, id);
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::touchMoved(int x, int y, int id){
	manager->touchMoved(x, y, id);
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::touchUp(int x, int y, int id){
	manager->touchUp(x, y, id);
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::touchDoubleTap(int x, int y, int id){
	manager->touchDoubleTap(x, y, id);
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::touchCancelled(int x, int y, int id){
	manager->touchCancelled(x, y, id);
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::swipe(ofxAndroidSwipeDir swipeDir, int id){
	//
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::pause(){
	//
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::stop(){
	//
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::resume(){
	//
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::reloadTextures(){
	//
}

//--------------------------------------------------------------
bool ofxAppAndroidLayer::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::okPressed(){
	//
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::cancelPressed(){
	//
}

#endif
