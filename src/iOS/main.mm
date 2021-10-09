// main.mm
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#include "ofMain.h"
#include "ofAppiOSWindow.h"
#include "ofxAppiOSLayer.h"

#include <stdio.h>

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
FILE* fopen$UNIX2003(const char* filename, const char* mode);
size_t fwrite$UNIX2003(const void* ptr, size_t size, size_t nitems, FILE* stream);

FILE* fopen$UNIX2003(const char* filename, const char* mode) {
    return fopen(filename, mode);
}

size_t fwrite$UNIX2003(const void* ptr, size_t size, size_t nitems, FILE* stream) {
    return fwrite(ptr, size, nitems, stream);
}
#endif

int main(){
	ofAppiOSWindow * window = new ofAppiOSWindow();
    window->enableRendererES2();
    window->enableRetina();
    
	ofSetupOpenGL(window, 1024, 768, OF_FULLSCREEN);
    window->startAppWithDelegate("iOSAppDelegate");
}
