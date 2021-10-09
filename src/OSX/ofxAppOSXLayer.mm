// ofxAppOSXLayer
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#include "ofxAppOSXLayer.h"
//--------------------------------------------------------------
void ofxAppOSXLayer::setup(){
    ofSetOrientation(OF_ORIENTATION_DEFAULT);
    ofAddListener(ofxMultiPlatformEvent::events, this, &ofxAppOSXLayer::triggerEvent);
    
    app = new ofxAppManager();
    app->setup();
}

//--------------------------------------------------------------
void ofxAppOSXLayer::update(){
    app->update();
}

//--------------------------------------------------------------
void ofxAppOSXLayer::draw(){
	app->draw();
}

//--------------------------------------------------------------
void ofxAppOSXLayer::exit(){
    app->exit();
    ofRemoveListener(ofxMultiPlatformEvent::events, this, &ofxAppOSXLayer::triggerEvent);
    
    if(app != NULL) {
        delete app;
        app = NULL;
    }
}

//--------------------------------------------------------------
void ofxAppOSXLayer::touchDown(ofTouchEventArgs & touch){
    app->touchDown(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppOSXLayer::touchMoved(ofTouchEventArgs & touch){
    app->touchMoved(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppOSXLayer::touchUp(ofTouchEventArgs & touch){
    app->touchUp(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppOSXLayer::touchDoubleTap(ofTouchEventArgs & touch){
    app->touchDoubleTap(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppOSXLayer::touchCancelled(ofTouchEventArgs & touch){
    app->touchCancelled(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppOSXLayer::lostFocus(){

}

//--------------------------------------------------------------
void ofxAppOSXLayer::gotFocus(){

}

//--------------------------------------------------------------
void ofxAppOSXLayer::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofxAppOSXLayer::deviceOrientationChanged(int newOrientation){

}

void ofxAppOSXLayer::triggerEvent(ofxMultiPlatformEvent &e) {
    // manage event for OSX.
}

//void ofxAppOSXLayer::setDelegate(id delegate) {
//    this->delegate = delegate;
//}
//
//void ofxAppOSXLayer::clearDelegate() {
//    delegate = nil;
//}


//--------------------------------------------------------------
void ofxAppOSXLayer::keyPressed(int key){
    if(app != NULL) {
        app->keyPressed(key);
    }

}

//--------------------------------------------------------------
void ofxAppOSXLayer::keyReleased(int key){
    if(app != NULL) {
        app->keyReleased(key);
    }
}

//--------------------------------------------------------------
void ofxAppOSXLayer::mouseMoved(int x, int y){
    if(app != NULL) {
        app->mouseMoved(x, y);
    }
}

//--------------------------------------------------------------
void ofxAppOSXLayer::mousePressed(int x, int y, int button){
    if(app != NULL) {
        app->mousePressed(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppOSXLayer::mouseDragged(int x, int y, int button){
    if(app != NULL) {
        app->mouseDragged(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppOSXLayer::mouseReleased(int x, int y, int button){
    if(app != NULL) {
        app->mouseReleased(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppOSXLayer::windowResized(int w, int h){
    if(app != NULL) {
        app->windowResized(w, h);
    }
}

//--------------------------------------------------------------
void ofxAppOSXLayer::gotMessage(ofMessage msg){
    if(app != NULL) {
        app->gotMessage(msg);
    }
}

//--------------------------------------------------------------
void ofxAppOSXLayer::dragEvent(ofDragInfo dragInfo){
    if(app != NULL) {
        app->dragEvent(dragInfo);
    }
}



