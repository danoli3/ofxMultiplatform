#include "ofxAppAndroidLayer.h"

#if defined(__ANDROID__)
//--------------------------------------------------------------
void ofxAppAndroidLayer::setup(){
	manager = new ofxAppManager();
	manager->setup();

	javaClass = nullptr;
	javaObject = nullptr;

	JNIEnv *env = ofGetJNIEnv();
	if(!env) {
		ofLogError("ofxAppAndroidLayer") << "JNIEnv missing";
		return;
	}

	// Project Generator copies OFActivity.java in this package. The Java
	// class name is not the applicationId.
	jclass localClass = env->FindClass("cc/openframeworks/android/OFActivity");
	if(!localClass) {
		if(env->ExceptionCheck()) {
			env->ExceptionClear();
		}
		ofLogError("ofxAppAndroidLayer") << "cc.openframeworks.android.OFActivity not found";
	} else {
		javaClass = (jclass)env->NewGlobalRef(localClass);
		env->DeleteLocalRef(localClass);
	}

	jobject activity = ofGetOFActivityObject();
	if(activity) {
		javaObject = (jobject)env->NewGlobalRef(activity);
	} else {
		ofLogError("ofxAppAndroidLayer") << "OF activity object not found";
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
	if(manager) {
		manager->windowResized(w, h);
	}
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
	if(manager) {
		manager->pause();
	}
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::stop(){
	//
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::resume(){
	if(manager) {
		manager->resume();
	}
}

//--------------------------------------------------------------
void ofxAppAndroidLayer::reloadTextures(){
	//
}

//--------------------------------------------------------------
bool ofxAppAndroidLayer::backPressed(){
	if(!manager) {
		return false;
	}
	return manager->backPressed();
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
