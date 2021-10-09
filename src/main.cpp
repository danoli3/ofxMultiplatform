// main.cpp
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#include "ofMain.h"

#if defined(TARGET_ANDROID) || defined(__ANDROID__)
#ifndef TARGET_ANDROID
#define TARGET_ANDROID
#endif
#include "ofxAppAndroidLayer.h"
#endif

#if defined(TARGET_OSX)
#include "ofxAppOSXLayer.h"
#endif

#if defined(TARGET_WINDOWS) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
#ifndef TARGET_WINDOWS
#define TARGET_WINDOWS
#endif
#include "ofxAppWindowsLayer.h"
#endif

#if defined(TARGET_LINUX) || defined(__linux) || defined(__unix) || defined(__posix)
#ifndef TARGET_LINUX
#define TARGET_LINUX
#endif
#include "ofxAppLinuxLayer.h"
#endif

#include "ofxAppManager.h"


//========================================================================
int main( ){

    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(1024, 768, OF_WINDOW);
    
    bool isAppRunning = false;

//-------------------- Android
#ifdef TARGET_ANDROID
	ofRunApp(new ofxAppAndroidLayer());
    isAppRunning = true;
#endif
    
#ifdef TARGET_OSX
    ofRunApp(new ofxAppOSXLayer());
    isAppRunning = true;
#endif
   
#ifdef TARGET_WIN32
    ofRunApp(new ofxAppWindowsLayer());
    isAppRunning = true;
#endif
    
#ifdef TARGET_LINUX
    ofRunApp(new ofxAppLinuxLayer());
    isAppRunning = true;
#endif
    if(isAppRunning == false) {
        // --- Not Android, OSX or Windows?? Running standard...
        ofRunApp(new ofxAppManager());
    }
	return 0;
}

#ifdef TARGET_ANDROID
#include <jni.h>

//========================================================================
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
		main();
	}
}
#endif
