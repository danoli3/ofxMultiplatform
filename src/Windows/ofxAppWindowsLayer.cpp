#include "ofxAppWindowsLayer.h"

//--------------------------------------------------------------
void ofxAppWindowsLayer::setup(){
    ofSetOrientation(OF_ORIENTATION_DEFAULT);
    ofAddListener(ofAppEvent::events, this, &ofxAppWindowsLayer::triggerEvent);
    
    app = new ofMainApp();
    app->setup();
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::update(){
    app->update();
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::draw(){
	app->draw();
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::exit(){
    app->exit();
    ofRemoveListener(ofAppEvent::events, this, &ofxAppWindowsLayer::triggerEvent);
    
    if(app != NULL) {
        delete app;
        app = NULL;
    }
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::touchDown(ofTouchEventArgs & touch){
    app->touchDown(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::touchMoved(ofTouchEventArgs & touch){
    app->touchMoved(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::touchUp(ofTouchEventArgs & touch){
    app->touchUp(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::touchDoubleTap(ofTouchEventArgs & touch){
    app->touchDoubleTap(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::touchCancelled(ofTouchEventArgs & touch){
    app->touchCancelled(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::lostFocus(){

}

//--------------------------------------------------------------
void ofxAppWindowsLayer::gotFocus(){

}

//--------------------------------------------------------------
void ofxAppWindowsLayer::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofxAppWindowsLayer::deviceOrientationChanged(int newOrientation){

}

void ofxAppWindowsLayer::triggerEvent(ofAppEvent &e) {
    // manage event for Windows.
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::keyPressed(int key){
    if(app != NULL) {
        app->keyPressed(key);
    }

}

//--------------------------------------------------------------
void ofxAppWindowsLayer::keyReleased(int key){
    if(app != NULL) {
        app->keyReleased(key);
    }
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::mouseMoved(int x, int y){
    if(app != NULL) {
        app->mouseMoved(x, y);
    }
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::mousePressed(int x, int y, int button){
    if(app != NULL) {
        app->mousePressed(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::mouseDragged(int x, int y, int button){
    if(app != NULL) {
        app->mouseDragged(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::mouseReleased(int x, int y, int button){
    if(app != NULL) {
        app->mouseReleased(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::windowResized(int w, int h){
    if(app != NULL) {
        app->windowResized(w, h);
    }
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::gotMessage(ofMessage msg){
    if(app != NULL) {
        app->gotMessage(msg);
    }
}

//--------------------------------------------------------------
void ofxAppWindowsLayer::dragEvent(ofDragInfo dragInfo){
    if(app != NULL) {
        app->dragEvent(dragInfo);
    }
}


