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

    bool isAppRunning = false;

#ifdef TARGET_EMSCRIPTEN
	ofGLESWindowSettings settings;
	settings.glesVersion = 3;
	#elseifdef TARGET_ANDROID
	ofGLESWindowSettings settings;
	settings.glesVersion = 2;
#else
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setGLVersion(4, 1);
#endif

	settings.setSize(1280, 720);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

//-------------------- Android
#ifdef TARGET_ANDROID
	ofRunApp(window, std::make_shared<ofxAppAndroidLayer>());
	isAppRunning = true;
	ofRunMainLoop();
#endif

#ifdef TARGET_OSX
	ofRunApp(window, std::make_shared<ofxAppOSXLayer>());
	isAppRunning = true;
	ofRunMainLoop();
#endif

#ifdef TARGET_WIN32
	ofRunApp(window, std::make_shared<ofxAppWindowsLayer>());
	isAppRunning = true;
	ofRunMainLoop();
#endif

#ifdef TARGET_LINUX
	ofRunApp(window, std::make_shared<ofxAppLinuxLayer>());
	isAppRunning = true;
	ofRunApp(new ofxAppLinuxLayer());
#endif
	if (isAppRunning == false) {
		// --- Not Android, OSX or Windows?? Running standard...
		ofRunApp(window, std::make_shared<ofxAppWindowsLayer>());
		isAppRunning = true;
		ofRunMainLoop();
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
