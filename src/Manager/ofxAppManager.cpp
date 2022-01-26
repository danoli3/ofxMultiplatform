// ofxAppManager.h
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#include "ofxAppManager.h"

ofxAppManager::ofxAppManager() {
    app = NULL;
    bDebug = true;
    clickedTime = 0.0f;
}

ofxAppManager::~ofxAppManager() {
    killApp();
}

//--------------------------------------------------------------
void ofxAppManager::setup(){
    loadApp("ofApp");
}


//--------------------------------------------------------------
void ofxAppManager::loadApp(string appID) {
    if(app != NULL) {
        if(app->getAppName() == appID) {
            //
            return;
        }
    }
    
    killApp();
    
    //---------------------- Implement your own classes here - Make sure to implement killApp as well
    if(appID == "ofApp") {
        app = new ofApp();
    } else {
        ofLog(OF_LOG_ERROR, "ofxAppManager::loadApp. Not loading " + appID + " as no matching statement to load that type in loadApp");
    }
    if(app != NULL) {
        app->setup();
    }
}

void ofxAppManager::killApp() {
    if(app == NULL) {
        return;
    }
    
    string appID = app->getAppName();

    if(appID == "ofApp") {
        delete (ofApp *)app; // casting so we cleanup all memory for ofApp and parent ofBaseApp.
    } else {
        delete app; // this is still critical!! Leaking memory!! You must cast to correct object type!
        ofLogError("ofxAppManager::killApp()") << "Memory Leak! Must delete object of correct class type!";
    }
    app = NULL;
}

string ofxAppManager::getAppID() {
    if(app == NULL) {
        return "";
    }
    return app->getAppName();
}

//--------------------------------------------------------------
void ofxAppManager::update(){
    if(app != NULL) {
        app->update();
    }
}

//--------------------------------------------------------------
void ofxAppManager::draw(){
    if(app != NULL) {
        app->draw();
    }
    ofSetColor(255);
    ofDrawBitmapString(ofToString((int)ofGetFrameRate()) + " fps", ofGetWidth()-60, 20);
}

//--------------------------------------------------------------
void ofxAppManager::touchDown(int x, int y, int id) {
    if(app != NULL) {
        app->touchDown(x, y, id);
    }
}

//--------------------------------------------------------------
void ofxAppManager::touchMoved(int x, int y, int id) {
    if(app != NULL) {
        app->touchMoved(x, y, id);
    }
}

//--------------------------------------------------------------
void ofxAppManager::touchUp(int x, int y, int id) {
    if(app != NULL) {
        app->touchUp(x, y, id);
    }
}

//--------------------------------------------------------------
void ofxAppManager::touchDoubleTap(int x, int y, int id) {
    if(app != NULL) {
        app->touchDoubleTap(x, y, id);
    }
}

//--------------------------------------------------------------
void ofxAppManager::touchCancelled(int x, int y, int id) {
    if(app != NULL) {
        app->touchCancelled(x, y, id);
    }
}

//--------------------------------------------------------------
void ofxAppManager::keyPressed(int key){
    if(app != NULL) {
        app->keyPressed(key);
    }
    
    if(key == 'd' || key == 'D') {
        bDebug = !bDebug;
    }
}

//--------------------------------------------------------------
void ofxAppManager::keyReleased(int key){
    if(app != NULL) {
        app->keyReleased(key);
    }
}

//--------------------------------------------------------------
void ofxAppManager::mouseMoved(int x, int y){
    if(app != NULL) {
        app->mouseMoved(x, y);
    }
}

//--------------------------------------------------------------
void ofxAppManager::mousePressed(int x, int y, int button){
    if(app != NULL) {
        app->mousePressed(x, y, button);
        app->touchDown(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppManager::mouseDragged(int x, int y, int button){
    if(app != NULL) {
        app->mouseDragged(x, y, button);
        app->touchMoved(x, y, button);
    }
}

//--------------------------------------------------------------
void ofxAppManager::mouseReleased(int x, int y, int button){
    if(app != NULL) {
        app->mouseReleased(x, y, button);
        app->touchUp(x, y, button);
        float currentTime = ofGetElapsedTimef();
        if((currentTime - clickedTime) <= 0.24) {
            app->touchDoubleTap(x, y, button);
        }
        clickedTime = currentTime;
    }
}

//--------------------------------------------------------------
void ofxAppManager::windowResized(int w, int h){
    if(app != NULL) {
        app->windowResized(w, h);
    }
}

//--------------------------------------------------------------
void ofxAppManager::gotMessage(ofMessage msg){
    if(app != NULL) {
        app->gotMessage(msg);
    }
}

//--------------------------------------------------------------
void ofxAppManager::dragEvent(ofDragInfo dragInfo){ 
    if(app != NULL) {
        app->dragEvent(dragInfo);
    }
}

