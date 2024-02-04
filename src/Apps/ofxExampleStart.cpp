//
//  ofxExampleStart.cpp
//
//  Created by Daniel Rosser on 4/02/2024.
//
//

#define STRINGIFY(A) #A

#include "ofxExampleStart.h"
//--------------------------------------------------------------
ofxExampleStart::ofxExampleStart() : ofxBaseApp(getClassName()) { // <--- Class name is passed into parent ofxBaseApp.
    
}

//--------------------------------------------------------------
ofxExampleStart::~ofxExampleStart() {
    
}

//--------------------------------------------------------------
void ofxExampleStart::setup(){
   ofEnableAlphaBlending();
    ofEnableAntiAliasing();
}

//--------------------------------------------------------------
void ofxExampleStart::update(){
    
    float timeFromLast = ofGetLastFrameTime();
    
    currentTime += timeFromLast;
    
    
    
    if(isFadedIn == false) {
        currentFadeIn += timeFromLast;
        if(currentFadeIn >= fadeInTotal) {
            isFadedIn = true;
			ofLog(OF_LOG_NOTICE) << "FadedIn";
        }
    }


}

//--------------------------------------------------------------
void ofxExampleStart::draw(){
      
    if(isFadedIn == false) {
        ofPushStyle();
        int alpha = int(255 * (1 -(float)currentFadeIn/fadeInTotal));
        if(alpha <= 0) {
            alpha = 0;
        } else if (alpha >= 255) {
            alpha = 255;
        }

        ofSetColor(0, 0, 0, alpha);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofPopStyle();
    }   
}

//--------------------------------------------------------------
void ofxExampleStart::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofxExampleStart::keyReleased(int key){
   
}

//--------------------------------------------------------------
void ofxExampleStart::mouseMoved(int x, int y ){
    mouseX = x;
    mouseY = y;
}

//--------------------------------------------------------------
void ofxExampleStart::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofxExampleStart::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofxExampleStart::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofxExampleStart::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofxExampleStart::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofxExampleStart::dragEvent(ofDragInfo dragInfo){
    
}

