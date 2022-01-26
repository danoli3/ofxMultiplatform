// iOSAppDelegate
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#import "iOSAppDelegate.h"
#import "iOSAppViewController.h"
#import "ofxAppiOSLayer.h"
#import "iOSAppViewController.h"
#import "iOSAppGLKViewController.h"

@implementation iOSAppDelegate  {
    BOOL isBackgrounded;
}

@synthesize navigationController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    [super applicationDidFinishLaunching:application];
    
    isBackgrounded = NO;
    
    [iOSAppViewController class];
    [iOSAppGLKViewController class];
    
    self.navigationController = [[[UINavigationController alloc] init] autorelease];
    [self.window setRootViewController:self.navigationController];
    [self.window makeKeyAndVisible];
    
    UIStoryboard * storyboard = [UIStoryboard storyboardWithName:@"Storyboard" bundle:nil];
    [self.navigationController pushViewController:[storyboard instantiateInitialViewController] animated:YES];
    
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    NSNotificationCenter * center = [NSNotificationCenter defaultCenter];
    
    [center addObserver:self
               selector:@selector(receivedRotate:)
                   name:UIDeviceOrientationDidChangeNotification
                 object:nil];
    
    UIInterfaceOrientation iOrient  = [[UIApplication sharedApplication] statusBarOrientation];
    // is the os version less than 6.0?
    if( [[[UIDevice currentDevice] systemVersion] compare:@"6.0" options:NSNumericSearch] == NSOrderedAscending ) {
        iOrient = UIInterfaceOrientationPortrait;
        
        UIDeviceOrientation deviceOrientation = [[UIDevice currentDevice] orientation];
        switch (deviceOrientation) {
            case UIDeviceOrientationPortrait:
                iOrient = UIInterfaceOrientationPortrait;
                break;
            case UIDeviceOrientationPortraitUpsideDown:
                iOrient = UIInterfaceOrientationPortraitUpsideDown;
                break;
            case UIDeviceOrientationLandscapeLeft:
                iOrient = UIInterfaceOrientationLandscapeRight;
                break;
            case UIDeviceOrientationLandscapeRight:
                iOrient = UIInterfaceOrientationLandscapeLeft;
                break;
            default:
                iOrient = UIInterfaceOrientationPortrait;
                break;
        }
    }
    BOOL bDoesHWOrientation = YES;
    BOOL bIsPortrait = UIInterfaceOrientationIsPortrait( iOrient );
    
    CGRect frame = [[UIScreen mainScreen] bounds];
    
    if( (!bIsPortrait && bDoesHWOrientation)) {
        float tWidth    = frame.size.width;
        float tHeight   = frame.size.height;
        frame.size.width    = tHeight;
        frame.size.height   = tWidth;
    }
    
    ofOrientation defaultOrient = ofGetOrientation();
    
    ofOrientation requested = ofGetOrientation();
    UIInterfaceOrientation interfaceOrientation = UIInterfaceOrientationPortrait;
    switch (requested) {
        case OF_ORIENTATION_DEFAULT:
            interfaceOrientation = UIInterfaceOrientationPortrait;
            break;
        case OF_ORIENTATION_180:
            interfaceOrientation = UIInterfaceOrientationPortraitUpsideDown;
            break;
        case OF_ORIENTATION_90_RIGHT:
            interfaceOrientation = UIInterfaceOrientationLandscapeLeft;
            break;
        case OF_ORIENTATION_90_LEFT:
            interfaceOrientation = UIInterfaceOrientationLandscapeRight;
            break;
    }
    
    if(!bDoesHWOrientation) {
        if([self.uiViewController respondsToSelector:@selector(rotateToInterfaceOrientation:animated:)]) {
            [self.uiViewController rotateToInterfaceOrientation:UIInterfaceOrientationPortrait animated:false];
        }
    } else {
        
        if([self.uiViewController respondsToSelector:@selector(rotateToInterfaceOrientation:animated:)]) {
            [self.uiViewController rotateToInterfaceOrientation:interfaceOrientation animated:false];
        }
        ofSetOrientation(requested);
    }
    
    return YES;
}


-(void)applicationWillResignActive:(UIApplication *)application {
    [super applicationWillResignActive:application];
    if(ofxiOSGetOFWindow()->getWindowControllerType() == CORE_ANIMATION)
        [ofxiOSGetGLView() stopAnimation];
    isBackgrounded = NO;

}

-(void)applicationDidEnterBackground:(UIApplication *)application {
    isBackgrounded = YES;
}

-(void)applicationWillEnterForeground:(UIApplication *)application {
    isBackgrounded = NO;
}

-(void)applicationDidBecomeActive:(UIApplication *)application {
    isBackgrounded = NO;
    [super applicationDidBecomeActive:application];
    if(ofxiOSGetOFWindow()->getWindowControllerType() == CORE_ANIMATION)
        [ofxiOSGetGLView() startAnimation];
    
}

-(void)applicationWillTerminate:(UIApplication *)application {
    [super applicationWillTerminate:application];
    
    if(ofxiOSGetOFWindow()->getWindowControllerType() == CORE_ANIMATION)
        [ofxiOSGetGLView() stopAnimation];
    
    // stop listening for orientation change notifications
    [[NSNotificationCenter defaultCenter] removeObserver: self];
    [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
    
}

- (void)applicationSignificantTimeChange:(UIApplication *)application {
    
}

- (void)receivedRotate:(NSNotification*)notification {
    UIDeviceOrientation deviceOrientation = [[UIDevice currentDevice] orientation];
    ofLogVerbose("ofxiOSAppDelegate") << "device orientation changed to " << deviceOrientation;
    if( [[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] == NSOrderedAscending ) {
        //iOS7-
        if(deviceOrientation != UIDeviceOrientationUnknown && deviceOrientation != UIDeviceOrientationFaceUp && deviceOrientation != UIDeviceOrientationFaceDown ) {
            if([self.uiViewController respondsToSelector:@selector(isReadyToRotate)]) {
                if([self.uiViewController isReadyToRotate]) {
                    ofxiOSAlerts.deviceOrientationChanged( deviceOrientation );
                } else {
                    ofLogVerbose("ofxiOSAppDelegate") << "receivedRotate however isReadyToRotate = NO ";
                }
            } else {
                ofxiOSAlerts.deviceOrientationChanged( deviceOrientation );
            }
        }
    }else {
        ofxiOSAlerts.deviceOrientationChanged( deviceOrientation );
    }
}

- (void)closeSplashScreen {
    NSLog(@"Closing Launch Loading Screen");
    [UIView animateWithDuration:0.33 delay:0.0
                        options:UIViewAnimationOptionAllowUserInteraction
                     animations:^{
                         [[self window] viewWithTag:10].alpha = 0.0;
                     }
                     completion:^(BOOL fin) {
                         if (fin) {
                             UIView * view = [[self window] viewWithTag:10];
                             NSArray *subViewArray = [view subviews];
                             for (id obj in subViewArray)
                             {
                                 if([obj isKindOfClass:[UIImageView class]]) {
                                     UIImageView * imageView = (UIImageView*)obj;
                                     if(imageView.image != nil) {
                                         imageView.image = nil;
                                     }
                                     [imageView removeFromSuperview];
                                 }
                             }
                              [[[self window] viewWithTag:10] removeFromSuperview];
                         }
                     }];

// Instant close
//    [[del window] addSubview:cover.view];
//    NSArray *subViewArray = [self.window subviews];
//    for (id obj in subViewArray)
//    {
//        [obj removeFromSuperview];
//    }
}

- (void) dealloc {
    self.navigationController = nil;
    [super dealloc];
}

//-------------------------------------------------------------------------------------------
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
#ifdef __IPHONE_6_0
#ifdef __IPHONE_9_0
- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
#else
- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
#endif
{
    if ([UIDevice currentDevice].systemVersion.floatValue < 8.f) {
        
        return  UIInterfaceOrientationLandscapeLeft | UIInterfaceOrientationLandscapeRight;
    }
    else
        return UIInterfaceOrientationMaskLandscape;
}
#endif

#endif


@end
