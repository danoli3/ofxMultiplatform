// iOSAppViewController
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#import "iOSAppGLKViewController.h"
#include "ofxAppiOSLayer.h"
#import "iOSAppDelegate.h"
//#import <GameController/GameController.h>
//#include "ofxCocosDenshion.h"
#include "GameControllerEvent.h"
#import <GameController/GameController.h>

@interface iOSAppGLKViewController() <ofxAppiOSLayerAppDelegate> {
    ofxAppiOSLayer * appLayer;
    BOOL hasRetrievedScores;
    //BOOL syncWithGameCenter;
    BOOL ShouldHideHomeIndicator;
    UIInterfaceOrientation currentInterfaceOrientation;
    UIInterfaceOrientation pendingInterfaceOrientation;
    BOOL bReadyToRotate;
    BOOL bFirstUpdate;
}
@end

@implementation iOSAppGLKViewController


-(id)init {
    self = [super init];
    return self;
}

- (NSURL*)applicationDirectory
{
    NSString* bundleID = [[NSBundle mainBundle] bundleIdentifier];
    NSFileManager*fm = [NSFileManager defaultManager];
    NSURL*    dirPath = nil;
    
    // Find the application support directory in the home directory.
    NSArray* appSupportDir = [fm URLsForDirectory:NSApplicationSupportDirectory
                                        inDomains:NSUserDomainMask];
    if ([appSupportDir count] > 0)
    {
        // Append the bundle ID to the URL for the
        // Application Support directory
        dirPath = [[appSupportDir objectAtIndex:0] URLByAppendingPathComponent:bundleID];
        
        // If the directory does not exist, this method creates it.
        // This method is only available in OS X v10.7 and iOS 5.0 or later.
        NSError*    theError = nil;
        if (![fm createDirectoryAtURL:dirPath withIntermediateDirectories:YES
                           attributes:nil error:&theError])
        {
            // Handle the error.
            
            return nil;
        }
    }
    
    return dirPath;
}

- (void)backupMyApplicationData {
    // Get the application's main data directory
    NSArray* theDirs = [[NSFileManager defaultManager] URLsForDirectory:NSApplicationSupportDirectory
                                                              inDomains:NSUserDomainMask];
    if ([theDirs count] > 0)
    {
        // Build a path to ~/Library/Application Support/<bundle_ID>/Data
        // where <bundleID> is the actual bundle ID of the application.
        NSURL* appSupportDir = (NSURL*)[theDirs objectAtIndex:0];
        NSString* appBundleID = [[NSBundle mainBundle] bundleIdentifier];
        NSURL* appDataDir = [[appSupportDir URLByAppendingPathComponent:appBundleID]
                             URLByAppendingPathComponent:@"Data"];
        
        // Copy the data to ~/Library/Application Support/<bundle_ID>/Data.backup
        NSURL* backupDir = [appDataDir URLByAppendingPathExtension:@"backup"];
        
        // Perform the copy asynchronously.
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            // It's good habit to alloc/init the file manager for move/copy operations,
            // just in case you decide to add a delegate later.
            NSFileManager* theFM = [[NSFileManager alloc] init];
            NSError* anError;
            
            // Just try to copy the directory.
            if (![theFM copyItemAtURL:appDataDir toURL:backupDir error:&anError]) {
                // If an error occurs, it's probably because a previous backup directory
                // already exists.  Delete the old directory and try again.
                if ([theFM removeItemAtURL:backupDir error:&anError]) {
                    // If the operation failed again, abort for real.
                    if (![theFM copyItemAtURL:appDataDir toURL:backupDir error:&anError]) {
                        // Report the error....
                    }
                }
            }
            
        });
    }
}


- (id)initWithCoder:(NSCoder *)aDecoder {
    
    [self applicationDirectory];
//    NSLog(applicationDir);
//    [self backupMyApplicationData];
    ofxiOSApp * oFApp;
    if(appLayer == nullptr) {
        oFApp = new ofxAppiOSLayer();
        appLayer = (ofxAppiOSLayer*)oFApp;
    }
    CGRect screenRect = CGRectZero;
    screenRect.size.width = [UIScreen mainScreen].bounds.size.width;
    screenRect.size.height = [UIScreen mainScreen].bounds.size.height;

    self = [super initWithFrame:screenRect app:oFApp sharegroup:nil];
    if(self != nil) {
        //[[GameCenterManager sharedManager] setDelegate:self];
        [self registerDefaultsFromSettingsBundle];
    }
   
    appLayer->setDelegate(self);
    
    currentInterfaceOrientation = pendingInterfaceOrientation = [[UIApplication sharedApplication] statusBarOrientation];
    if( [[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] == NSOrderedAscending ) {
        bReadyToRotate  = NO;
    }else{
        bReadyToRotate  = YES;
    }
    bFirstUpdate    = NO;
    
    return self;
}

- (UIRectEdge)preferredScreenEdgesDeferringSystemGestures {
    UIRectEdge edge = UIRectEdgeLeft | UIRectEdgeBottom | UIRectEdgeRight;
    return edge;
}


- (id)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app sharegroup:(EAGLSharegroup *)sharegroup{
    appLayer = (ofxAppiOSLayer*)app;
    self = [super initWithFrame:frame app:app sharegroup:(EAGLSharegroup *)sharegroup];
    appLayer->setDelegate(self);
    currentInterfaceOrientation = pendingInterfaceOrientation = [[UIApplication sharedApplication] statusBarOrientation];
    if( [[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] == NSOrderedAscending ) {
        bReadyToRotate  = NO;
    }else{
        bReadyToRotate  = YES;
    }
    bFirstUpdate    = NO;
    return self;
}

- (void) dealloc {
    
    if(appLayer) {
        appLayer->clearDelegate();
        appLayer = nil;
    }
//    [[GameCenterManager sharedManager] setDelegate:nil];
    #if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
    [[NSNotificationCenter defaultCenter] removeObserver: self];
    #endif
    [super dealloc];
}

- (BOOL) prefersHomeIndicatorAutoHidden
{
    return ShouldHideHomeIndicator;
}

- (void)viewWillAppear:(BOOL)animated
{
    
    UIGestureRecognizer* gr0 = self.view.window.gestureRecognizers[0];
    UIGestureRecognizer* gr1 = self.view.window.gestureRecognizers[1];
    
    if(gr0 != nil)
        gr0.delaysTouchesBegan = false;
    if(gr1 != nil)
        gr1.delaysTouchesBegan = false;
    
    [super viewWillAppear:animated];
}

- (void)viewDidLoad {
    
    [super viewDidLoad];
    
    #if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(controllerDidConnect) name:GCControllerDidConnectNotification object:nil];
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(controllerDidDisconnect) name:GCControllerDidDisconnectNotification object:nil];
//
    
    #endif
    [self controllerDidConnect]; // incase already connected
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
    // static dispatch_once_t onceToken;
    //dispatch_once(&onceToken, ^{
        ShouldHideHomeIndicator = YES;
    if (@available(iOS 11, *)){
        //UIView.animate(withDuration: 1, animations: {
            [self setNeedsUpdateOfHomeIndicatorAutoHidden];
    //     }
    
    //}
    }
    
    if( [[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] == NSOrderedAscending ) {
        bReadyToRotate  = YES;
        bFirstUpdate    = YES;
        [self rotateToInterfaceOrientation:pendingInterfaceOrientation animated:NO];
    }
    
    [UIViewController attemptRotationToDeviceOrientation];
    
    [self.navigationItem setHidesBackButton:YES animated:NO]; // hide back button for native view control

}
- (UIInterfaceOrientation)currentInterfaceOrientation {
    return currentInterfaceOrientation;
}
- (void)setCurrentInterfaceOrientation:(UIInterfaceOrientation) orient {
    currentInterfaceOrientation = pendingInterfaceOrientation = orient;
    [super setCurrentInterfaceOrientation:orient];
}
- (BOOL)isReadyToRotate {
    return YES;
}


- (void)firstFrameRun {
    NSLog(@"firstFrameRun");
    iOSAppDelegate* appDelegate = (iOSAppDelegate*)[[UIApplication sharedApplication] delegate];
//    [appDelegate closeSplashScreen];
}

//- (void)syncWithGameCenterScores {
//    syncWithGameCenter = YES;
//}

- (void)changeIdleTimer:(bool)value {
    NSLog(@"changeIdleTimer");
    [[UIApplication sharedApplication] setIdleTimerDisabled:(BOOL)value];
}

- (BOOL)canBecomeFirstResponder {
    return YES;
}

-(UIKeyCommand *)_keyCommandForEvent:(UIEvent *)event // UIPhysicalKeyboardEvent
{
    

    int theKeyCode = [[event valueForKey:@"_keyCode"] intValue];
    switch (theKeyCode) {
        case 80: // Left Arrow
        case 79: // Right Arrow
        case 4: // A
        case 7: // D
        case 26: // W
        case 1: // S
        case 42: // ESC
        case 43: // TAB
        case 40: // Enter / return
        case 44: // Space
        case 82: // Up Arrow
        case 81: // Down Arrow
        {
            // Do Keyevent Handling if it's one of the above keys only
            BOOL isTheKeyDown = [[event valueForKey:@"_isKeyDown"] boolValue];
            [self handleCommand:theKeyCode isKeyDown:isTheKeyDown];
            break;
        }
        default:
            // Nothing.
            break;
    }
    return nil;
}

- (void)handleCommand:(int)keyCode isKeyDown:(BOOL)isTheKeyDown
{
    int value = keyCode + 400; // (400 is magic keyCode padding as things are different).
    if(appLayer) {
        if(isTheKeyDown) {
            appLayer->keyPressed(value);
        } else {
            appLayer->keyReleased(value);
        }
    }
}

- (void)registerDefaultsFromSettingsBundle {
    // this function writes default settings as settings
    NSString *settingsBundle = [[NSBundle mainBundle] pathForResource:@"Settings" ofType:@"bundle"];
    if(!settingsBundle) {
        NSLog(@"Could not find Settings.bundle");
        return;
    }
    
    NSDictionary *settings = [NSDictionary dictionaryWithContentsOfFile:[settingsBundle stringByAppendingPathComponent:@"Root.plist"]];
    NSArray *preferences = [settings objectForKey:@"PreferenceSpecifiers"];
    
    NSMutableDictionary *defaultsToRegister = [[NSMutableDictionary alloc] initWithCapacity:[preferences count]];
    for(NSDictionary *prefSpecification in preferences) {
        NSString *key = [prefSpecification objectForKey:@"Key"];
        if(key) {
            [defaultsToRegister setObject:[prefSpecification objectForKey:@"DefaultValue"] forKey:key];
            NSLog(@"writing as default %@ to the key %@",[prefSpecification objectForKey:@"DefaultValue"],key);
        }
    }
    
    [[NSUserDefaults standardUserDefaults] registerDefaults:defaultsToRegister];
    

}

- (void)setMSAA:(bool)value{
     if(self.glView != nil) {
         [self.glView setMSAA:value];
     }
 }
- (void)setPreferredFPS:(int)fps {
    if(self.glView != nil) {
        self.preferredFramesPerSecond = fps;
    }
}

-(float)rotationForOrientation:(UIInterfaceOrientation)interfaceOrientation {
    if (interfaceOrientation == UIInterfaceOrientationPortrait) {
        return 0;           // 0 degrees.
    } else if (interfaceOrientation == UIInterfaceOrientationLandscapeRight) {
        return M_PI * 0.5;  // 90 degrees.
    } else if (interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown) {
        return M_PI;        // 180 degrees.
    } else if (interfaceOrientation == UIInterfaceOrientationLandscapeLeft) {
        return M_PI * 1.5;  // 270 degrees.
    } else {
        return 0;
    }
}

//-------------------------------------------------------------- orientation callbacks.
// http://developer.apple.com/library/ios/#featuredarticles/ViewControllerPGforiPhoneOS/RespondingtoDeviceOrientationChanges/RespondingtoDeviceOrientationChanges.html

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
                                duration:(NSTimeInterval)duration {
    
    // CALLBACK 1.
    // The window calls the root view controller’s willRotateToInterfaceOrientation:duration: method.
    // Container view controllers forward this message on to the currently displayed content view controllers.
    // You can override this method in your custom content view controllers to hide views or make other changes to your view layout before the interface is rotated.
    // Deprecated in iOS 8. See viewWillTransitionToSize below.
    [super willRotateToInterfaceOrientation:toInterfaceOrientation duration:duration];
}
- (void)deviceOrientationDidChange:(NSNotification *)notification { UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation]; NSLog(@"rotation %li", (long)orientation); }

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    
    // CALLBACK 2.
    // The window adjusts the bounds of the view controller’s view.
    // This causes the view to layout its subviews, triggering the view controller’s viewWillLayoutSubviews method.
    // When this method runs, you can query the app object’s statusBarOrientation property to determine the current user interface layout.
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
                                         duration:(NSTimeInterval)duration {
    
    // CALLBACK 3.
    // This method is called from within an animation block so that any property changes you make
    // are animated at the same time as other animations that comprise the rotation.
    // Deprecated in iOS 8. See viewWillTransitionToSize below.
    
    [super willAnimateRotationToInterfaceOrientation:toInterfaceOrientation duration:duration];

}

// iOS8+ version of willAnimateRotationToInterfaceOrientation
//NOTE: Only called if actually resizing and not masked
//http://stackoverflow.com/questions/25935006/ios8-interface-rotation-methods-not-called
//borg
#ifdef __IPHONE_8_0
- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
    
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
    
    CGPoint center;
    
    center.x = size.width * 0.5;
    center.y = size.height * 0.5;
    
    
   // if(true) {
        NSTimeInterval duration = 0.3;
        [self.glView.layer removeAllAnimations];
        [UIView animateWithDuration:duration animations:^{
            self.glView.center = center;
            self.glView.transform = CGAffineTransformMakeRotation(0);
            self.glView.frame = CGRectMake(0, 0, size.width,size.height);
        }];
//    } else {
//        self.glView.center = center;
//        self.glView.transform = CGAffineTransformMakeRotation(0);
//        self.glView.frame = CGRectMake(0, 0, size.width,size.height);
//    }
    
    [coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext> context) {
        
        // Place code here to perform animations during the rotation.
        // You can pass nil or leave this block empty if not necessary.
        
    } completion:^(id<UIViewControllerTransitionCoordinatorContext> context) {
        
        // Code here will execute after the rotation has finished.
        // Equivalent to placing it in the deprecated method -[didRotateFromInterfaceOrientation:]
        
    }];
}
#endif


- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    
    // CALLBACK 4.
    // This action marks the end of the rotation process.
    // You can use this method to show views, change the layout of views, or make other changes to your app.
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
}

//-------------------------------------------------------------- iOS5 and earlier.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    if(toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft)
        return YES;
    else if(toInterfaceOrientation == UIInterfaceOrientationLandscapeRight)
        return YES;
    else
        return NO;
}

- (BOOL)supportsOrientation:(UIInterfaceOrientation)orientation
{
    if(orientation == UIInterfaceOrientationLandscapeLeft)
        return YES;
    else if(orientation == UIInterfaceOrientationLandscapeRight)
        return YES;
    else
        return NO;
}

//-------------------------------------------------------------- iOS6.
#ifdef __IPHONE_6_0
#if __IPHONE_OS_VERSION_MAX_ALLOWED < 90000
- (NSUInteger)supportedInterfaceOrientations
#else
- (UIInterfaceOrientationMask)supportedInterfaceOrientations
#endif
{
    
    NSUInteger ret = 0;
    ret = ret | (1 << UIInterfaceOrientationLandscapeRight);
    ret = ret | (1 << UIInterfaceOrientationLandscapeLeft);
    return ret;
    
    //    switch (currentInterfaceOrientation) {
    //        case UIInterfaceOrientationPortrait:
    //            return UIInterfaceOrientationMaskPortrait;
    //            break;
    //        case UIInterfaceOrientationPortraitUpsideDown:
    //            return UIInterfaceOrientationMaskPortraitUpsideDown;
    //            break;
    //        case UIInterfaceOrientationLandscapeLeft:
    //            return UIInterfaceOrientationMaskLandscapeLeft;
    //            break;
    //        case UIInterfaceOrientationLandscapeRight:
    //            return UIInterfaceOrientationMaskLandscapeRight;
    //            break;
    //        default:
    //            break;
    //    }
    // defaults to orientations selected in the .plist file ('Supported Interface Orientations' in the XCode Project)
    return -1;
}

-(void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
animated:(BOOL)animated {

    if(bReadyToRotate == NO) {
        pendingInterfaceOrientation = interfaceOrientation;
        
        // we need to update the dimensions here, so if ofSetOrientation is called in setup,
        // then it will return the correct width and height
        CGSize screenSize = [UIScreen mainScreen].bounds.size;
        CGRect bounds = CGRectMake(0, 0, screenSize.width, screenSize.height);
        if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
            bounds.size.width   = screenSize.height;
            bounds.size.height  = screenSize.width;
        }
        self.glView.bounds = bounds;
        [self.glView updateDimensions];
        
        return;
    }
    
    
    if(currentInterfaceOrientation == interfaceOrientation && !bFirstUpdate) {
        return;
    }
    
    if(pendingInterfaceOrientation != interfaceOrientation) {
        CGSize screenSize = [UIScreen mainScreen].bounds.size;
        CGRect bounds = CGRectMake(0, 0, screenSize.width, screenSize.height);
        if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
            bounds.size.width   = screenSize.height;
            bounds.size.height  = screenSize.width;
        }
        self.glView.bounds = bounds;
        [self.glView updateDimensions];
    }
    
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    CGPoint center;
    CGRect bounds = CGRectMake(0, 0, screenSize.width, screenSize.height);
    
    if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
        center.x = screenSize.height * 0.5;
        center.y = screenSize.width * 0.5;
    } else {
        center.x = screenSize.width * 0.5;
        center.y = screenSize.height * 0.5;
    }
    
    // Is the iOS version less than 8?
    if( [[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] == NSOrderedAscending ) {
        if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
            bounds.size.width = screenSize.height;
            bounds.size.height = screenSize.width;
        }
    } else {
        // Fixes for iOS 8 Portrait to Landscape issues
        if((UIInterfaceOrientationIsPortrait(interfaceOrientation) && screenSize.width >= screenSize.height) ||
           (UIInterfaceOrientationIsLandscape(interfaceOrientation) && screenSize.height >= screenSize.width)) {
            bounds.size.width = screenSize.height;
            bounds.size.height = screenSize.width;
        } else {
            bounds.size.width = screenSize.width;
            bounds.size.height = screenSize.height;
        }
        //borg
        //NSLog(@"w %f h %f",[UIScreen mainScreen].bounds.size.width,[UIScreen mainScreen].bounds.size.height);
        //assumes Portrait orientation
        if(screenSize.width>screenSize.height){
            center.x = screenSize.height * 0.5;
            center.y = screenSize.width * 0.5;
        }else{
            center.x = screenSize.width * 0.5;
            center.y = screenSize.height * 0.5;
        }
        //NSLog(@"rotating to portrait %i, is portrait %i, currentInterfaceOrientation %i, bound: w %f h %f",UIInterfaceOrientationIsPortrait(interfaceOrientation),UIInterfaceOrientationIsPortrait(self.interfaceOrientation),UIInterfaceOrientationIsPortrait(currentInterfaceOrientation),bounds.size.width,bounds.size.height);
    }
    
    float rot1 = [self rotationForOrientation:currentInterfaceOrientation];
    float rot2 = [self rotationForOrientation:interfaceOrientation];
    float rot3 = rot2 - rot1;
    CGAffineTransform rotate = CGAffineTransformMakeRotation(rot3);
    rotate = CGAffineTransformConcat(rotate, self.glView.transform);
    
    if(animated) {
        NSTimeInterval duration = 0.3;
        if((UIInterfaceOrientationIsLandscape(currentInterfaceOrientation) && UIInterfaceOrientationIsLandscape(interfaceOrientation)) ||
           (UIInterfaceOrientationIsPortrait(currentInterfaceOrientation) && UIInterfaceOrientationIsPortrait(interfaceOrientation))) {
            duration = 0.6;
        }
        [self.glView.layer removeAllAnimations];
        [UIView animateWithDuration:duration animations:^{
            self.glView.center = center;
            self.glView.transform = rotate;
            self.glView.bounds = bounds;
        }];
    } else {
        self.glView.center = center;
        self.glView.transform = rotate;
        self.glView.bounds = bounds;
    }
    
    currentInterfaceOrientation = interfaceOrientation;
    bFirstUpdate = NO;
    
    [self.glView updateDimensions];
}
- (BOOL)shouldAutorotate {
    return YES;
}
#endif

#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
#pragma mark - Notification Center
-(void) controllerDidConnect
{
    [[GCController controllers] enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        GCController *controller=(GCController *)obj;
        NSLog(@"discover vendorName:%@ attrached:%d  gamepad:%@ playerIndex:%ld"
              ,controller.vendorName,controller.attachedToDevice,controller.gamepad,(long)controller.playerIndex);
        
        GCGamepad * pad = controller.gamepad;
        pad.buttonA.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::buttonA, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        pad.buttonB.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::buttonB, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        pad.buttonX.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::buttonX, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        pad.buttonY.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::buttonY, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        pad.dpad.up.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::upButton, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        pad.dpad.down.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::downButton, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        pad.dpad.left.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::leftButton, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        pad.dpad.right.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::rightButton, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        
        
        GCExtendedGamepad *extendPad = controller.extendedGamepad;
        extendPad.leftThumbstick.valueChangedHandler = ^(GCControllerDirectionPad *dpad, float xValue, float yValue)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::leftThumbstick, yValue, false, xValue);
                appLayer->gameControllerEvent(event);
            }
            //self.leftThumbStickButton.frame = CGRectMake(xValue*15+self.leftThumbStickPoint.x, self.leftThumbStickPoint.y-yValue*15,self.leftThumbStickButton.frame.size.width,self.leftThumbStickButton.frame.size.height);
            
        };
        extendPad.rightThumbstick.valueChangedHandler = ^(GCControllerDirectionPad *dpad, float xValue, float yValue)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::rightThumbstick, yValue, false, xValue);
                appLayer->gameControllerEvent(event);
            }
            //self.rightThumbStickButton.frame = CGRectMake(xValue*15+self.rightThumbStickPoint.x, self.rightThumbStickPoint.y-yValue*15,self.rightThumbStickButton.frame.size.width,self.rightThumbStickButton.frame.size.height);
            
        };
        extendPad.leftShoulder.valueChangedHandler= ^ (GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::l1Button, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        extendPad.rightShoulder.valueChangedHandler= ^ (GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::r1Button, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        extendPad.rightTrigger.valueChangedHandler = ^(GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::r2Button, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
        extendPad.leftTrigger.valueChangedHandler = ^(GCControllerButtonInput *button, float value, BOOL pressed)
        {
            if(appLayer){
                GameControllerEvent event(GameControllerEvent::Type::l2Button, value, pressed);
                appLayer->gameControllerEvent(event);
            }
        };
    }];
    
    
}
-(void) controllerDidDisconnect
{
    [GCController startWirelessControllerDiscoveryWithCompletionHandler:^{
        NSLog(@"startWirelessControllerDiscoveryWithCompletionHandler");
    }];
}
#endif

@end
