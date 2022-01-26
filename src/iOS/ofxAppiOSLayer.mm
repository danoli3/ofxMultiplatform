// ofxAppiOSLayer
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------

#include "ofxAppiOSLayer.h"

//--------------------------------------------------------------
void ofxAppiOSLayer::setup(){
    ofSetOrientation(OF_ORIENTATION_DEFAULT);
    manager = new ofxAppManager();
    manager->setup();
}

//--------------------------------------------------------------
void ofxAppiOSLayer::update(){
    manager->update();
}

//--------------------------------------------------------------
void ofxAppiOSLayer::draw(){
	manager->draw();
}

//--------------------------------------------------------------
void ofxAppiOSLayer::exit(){
    
    if(manager) {
        manager->exit();
        delete manager;
        manager = NULL;
    }
}

//--------------------------------------------------------------
void ofxAppiOSLayer::touchDown(ofTouchEventArgs & touch){
    manager->touchDown(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppiOSLayer::touchMoved(ofTouchEventArgs & touch){
    manager->touchMoved(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppiOSLayer::touchUp(ofTouchEventArgs & touch){
    manager->touchUp(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppiOSLayer::touchDoubleTap(ofTouchEventArgs & touch){
    manager->touchDoubleTap(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppiOSLayer::touchCancelled(ofTouchEventArgs & touch){
    manager->touchCancelled(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppiOSLayer::lostFocus(){
    //manager->lostFocus();
}

//--------------------------------------------------------------
void ofxAppiOSLayer::gotFocus(){
    //manager->gotFocus();
}

//--------------------------------------------------------------
void ofxAppiOSLayer::gotMemoryWarning(){
    //manager->gotMemoryWarning();
}

//--------------------------------------------------------------
void ofxAppiOSLayer::deviceOrientationChanged(int newOrientation){
    //manager->deviceOrientationChanged(newOrientation);
}

void ofxAppiOSLayer::setDelegate(id delegate) {
    this->delegate = delegate;
}

void ofxAppiOSLayer::clearDelegate() {
    delegate = nil;
}

void ofxAppiOSLayer::gameControllerEvent(GameControllerEvent & event) {
    
}
