#include "ofxAppLinuxLayer.h"

//--------------------------------------------------------------
void ofxAppLinuxLayer::setup(){
    ofSetOrientation(OF_ORIENTATION_DEFAULT);
    ofAddListener(ofAppEvent::events, this, &ofxAppLinuxLayer::triggerEvent);
    
    manager = new ofxAppManager();
    manager->setup();
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::update(){
    manager->update();
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::draw(){
	manager->draw();
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::exit(){
    manager->exit();
    ofRemoveListener(ofAppEvent::events, this, &ofxAppLinuxLayer::triggerEvent);
    
    if(manager != NULL) {
        delete manager;
        manager = NULL;
    }
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::touchDown(ofTouchEventArgs & touch){
    manager->touchDown(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::touchMoved(ofTouchEventArgs & touch){
    manager->touchMoved(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::touchUp(ofTouchEventArgs & touch){
    manager->touchUp(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::touchDoubleTap(ofTouchEventArgs & touch){
    manager->touchDoubleTap(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::touchCancelled(ofTouchEventArgs & touch){
    manager->touchCancelled(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::lostFocus(){

}

//--------------------------------------------------------------
void ofxAppLinuxLayer::gotFocus(){

}

//--------------------------------------------------------------
void ofxAppLinuxLayer::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofxAppLinuxLayer::deviceOrientationChanged(int newOrientation){

}

void ofxAppLinuxLayer::triggerEvent(ofAppEvent &e) {
    // manage event for Linux.
}



//--------------------------------------------------------------
void ofxAppLinuxLayer::keyPressed(int key){
    if(manager != NULL) {
        manager->keyPressed(key);
    }

}

//--------------------------------------------------------------
void ofxAppLinuxLayer::keyReleased(int key){
    if(manager != NULL) {
        manager->keyReleased(key);
    }
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::mouseMoved(int x, int y){
    if(manager != NULL) {
        manager->mouseMoved(x, y);
    }
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::mousePressed(int x, int y, int button){
    if(manager != NULL) {
        manager->mousePressed(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::mouseDragged(int x, int y, int button){
    if(manager != NULL) {
        manager->mouseDragged(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::mouseReleased(int x, int y, int button){
    if(manager != NULL) {
        manager->mouseReleased(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::windowResized(int w, int h){
    if(manager != NULL) {
        manager->windowResized(w, h);
    }
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::gotMessage(ofMessage msg){
    if(manager != NULL) {
        manager->gotMessage(msg);
    }
}

//--------------------------------------------------------------
void ofxAppLinuxLayer::dragEvent(ofDragInfo dragInfo){
    if(manager != NULL) {
        manager->dragEvent(dragInfo);
    }
}


