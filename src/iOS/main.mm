// main.mm
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#include "ofMain.h"
#if defined(TARGET_OS_IOS)
#include "ofAppiOSWindow.h"
#include "ofxAppiOSLayer.h"
#include "iOSAppDelegate.h"
#include <stdio.h>

int main(){
    
    ofiOSWindowSettings settings;
    settings.enableMultiTouch = true;
    ofxiOSRendererType rendererType = ofxiOSRendererType::OFXIOS_RENDERER_ES2;
    settings.enableRetina = true; // enables retina resolution if the device supports it.
    settings.enableDepth = true; // enables depth buffer for 3d drawing.
    //settings.windowControllerType = ofxiOSWindowControllerType::CORE_ANIMATION; // old way
    settings.windowControllerType = ofxiOSWindowControllerType::GL_KIT; // Window Controller Type
    settings.colorType = ofxiOSRendererColorFormat::RGBA8888; // color format used default RGBA8888
    settings.depthType = ofxiOSRendererDepthFormat::DEPTH_NONE; // depth format (16/24) if depth enabled
    settings.stencilType = ofxiOSRendererStencilFormat::STENCIL_NONE; // stencil mode
    
    settings.enableHardwareOrientation = true; // enables native view orientation.
    settings.enableHardwareOrientationAnimation = true; // enables native orientation changes to be animated.
    settings.glesVersion = rendererType; // type of renderer to use, ES1, ES2, etc.
//    settings.setupOrientation = OF_ORIENTATION_90_LEFT;

    ofAppiOSWindow * window = (ofAppiOSWindow *)(ofCreateWindow(settings).get());
    bool bUseNative = true;
    if (bUseNative){
//        window->startAppWithDelegate("AppDelegate");
        window->startAppWithDelegate("iOSAppDelegate");
    } else {
        return ofRunApp(new ofxAppiOSLayer);
    }
}
#endif
