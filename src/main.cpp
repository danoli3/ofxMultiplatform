// main.cpp
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#include "ofMain.h"

// iOS has its own entry point in src/iOS/main.mm.
#if !defined(TARGET_OF_IOS)

// Platform selection relies on ofConstants.h, which defines exactly one
// TARGET_* (Android is not TARGET_LINUX, Emscripten is not TARGET_LINUX).
#if defined(TARGET_ANDROID)
#include "ofxAppAndroidLayer.h"
#elif defined(TARGET_OSX)
#include "ofxAppOSXLayer.h"
#elif defined(TARGET_WIN32)
#include "ofxAppWindowsLayer.h"
#elif defined(TARGET_LINUX)
#include "ofxAppLinuxLayer.h"
#endif

#include "ofxAppManager.h"


//========================================================================
int main( ){

#if defined(TARGET_OPENGLES)
	// Android, Emscripten, Linux ARM
	ofGLESWindowSettings settings;
#if defined(TARGET_EMSCRIPTEN)
	settings.glesVersion = 3;
#else
	settings.glesVersion = 2;
#endif
#else
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLFWWindowSettings settings;
	settings.setGLVersion(4, 1);
	settings.transparent = true;
#endif

	settings.setSize(1280, 720);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

#if defined(TARGET_ANDROID)
	ofRunApp(window, std::make_shared<ofxAppAndroidLayer>());
#elif defined(TARGET_OSX)
	ofRunApp(window, std::make_shared<ofxAppOSXLayer>());
#elif defined(TARGET_WIN32)
	ofRunApp(window, std::make_shared<ofxAppWindowsLayer>());
#elif defined(TARGET_LINUX)
	ofRunApp(window, std::make_shared<ofxAppLinuxLayer>());
#else
	// No platform layer (e.g. Emscripten): run the manager directly.
	ofRunApp(window, std::make_shared<ofxAppManager>());
#endif
	return ofRunMainLoop();
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

#endif // !TARGET_OF_IOS
