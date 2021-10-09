// iOSAppDelegate
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#import "iOSAppDelegate.h"
#import "iOSAppViewController.h"
#import "ofxAppiOSLayer.h"

@implementation iOSAppDelegate

@synthesize navigationController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    [super applicationDidFinishLaunching:application];
    self.navigationController = [[[UINavigationController alloc] init] autorelease];
    [self.window setRootViewController:self.navigationController];
    
    CGRect screenRect = CGRectZero;
    screenRect.size.width = [UIScreen mainScreen].bounds.size.height;
    screenRect.size.height = [UIScreen mainScreen].bounds.size.width;
    UIViewController * viewController = [[[iOSAppViewController alloc] initWithFrame:screenRect
                                                                                app:new ofxAppiOSLayer()] autorelease];
    [self.navigationController pushViewController:viewController animated:YES];
    [self.navigationController setNavigationBarHidden:YES];
    
    return YES;
}

- (void) dealloc {
    self.navigationController = nil;
    [super dealloc];
}

//-------------------------------------------------------------------------------------------
//#ifdef __IPHONE_6_0
//-(NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window {
//    return UIInterfaceOrientationMaskLandscape;
//}
//#endif

@end
