// iOSAppViewController
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#import <UIKit/UIKit.h>

#pragma once

//#import "GameCenterManager.h"
#include <TargetConditionals.h>
#import <GLKit/GLKit.h>


#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
#import "ofxiOSGLKViewController.h"
@interface iOSAppGLKViewController : ofxiOSGLKViewController<UIActionSheetDelegate //,
    //GameCenterManagerDelegate
>
#else 
#import "ofxtvOSGLKViewController.h"
@interface iOSAppGLKViewController : ofxtvOSGLKViewController<GameCenterManagerDelegate>
#endif

- (UIInterfaceOrientation)currentInterfaceOrientation;
- (void)setCurrentInterfaceOrientation:(UIInterfaceOrientation) orient;
- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
                            animated:(BOOL)animated;
- (BOOL)isReadyToRotate;
- (void)setPreferredFPS:(int)fps;
- (void)setMSAA:(bool)value;

@end

