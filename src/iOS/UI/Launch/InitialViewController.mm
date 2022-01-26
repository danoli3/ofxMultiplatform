//
//  InitialViewController.m
//  Super Hexagon
//
//  Created by Daniel Rosser on 17/10/2015.
//
//

#import "InitialViewController.h"
#import "iOSAppGLKViewController.h"
#import "iOSAppViewController.h"
#import "iOSAppDelegate.h"
#include "ofxiOSViewController.h"
#include "ofxiOSExternalDisplay.h"
#include "ofxiOSExtras.h"
#include "ofxiOSAlerts.h"
#include "ofxiOSEAGLView.h"
#include "ofAppiOSWindow.h"
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofLog.h"

#define IS_OS_7_OR_LATER    ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
#define IS_OS_8_OR_LATER    ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)

@interface InitialViewController () {
    IBOutlet UIImageView * logoImage;
}

@end

@implementation InitialViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    NSBundle* bundle = [NSBundle mainBundle];
    NSString *imagePath = [bundle pathForResource:@"loading" ofType:@"png"];
    UIImage* image = [UIImage imageWithContentsOfFile:imagePath];
    logoImage.image = image;
    
}

-(void)dealloc {
    [super dealloc];
    if(logoImage != nil)
        [logoImage release];
    logoImage = nil;
}


- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
//    static dispatch_once_t onceToken;
//    dispatch_once(&onceToken, ^{
//        [self performSegueWithIdentifier:@"show_launch" sender:nil];
//    });
    
    UIStoryboard * storyboard = [UIStoryboard storyboardWithName:@"Storyboard" bundle:nil];
    iOSAppDelegate *del = (iOSAppDelegate *)[UIApplication sharedApplication].delegate;
    
    
    
    // Launch Cover Splash First
//    LaunchSplashViewController * cover = [storyboard instantiateViewControllerWithIdentifier:@"LaunchSplashViewController"];
//    cover.view.tag = 10;
//    if(IS_OS_8_OR_LATER) {
//        // nothing it is fine
//    } else {
//        cover.view.transform = CGAffineTransformMakeRotation(M_PI_2);
//    }
    
//    [[del window] addSubview:cover.view];
    
        // Load Game
        switch(ofxiOSGetOFWindow()->getWindowControllerType()) {
            case METAL_KIT:
                NSLog(@"No MetalKit yet supported for openFrameworks: Falling back to GLKit");
            case GL_KIT: {
                iOSAppGLKViewController * game = [storyboard instantiateViewControllerWithIdentifier:@"iOSAppGLKViewController"];
                [del.navigationController pushViewController:game animated:NO];
                break;
            }
            case CORE_ANIMATION:
            default: {
                iOSAppViewController * game = [storyboard instantiateViewControllerWithIdentifier:@"iOSAppViewController"];
                [del.navigationController pushViewController:game animated:NO];
                break;
            }
    
        }
//
//    iOSAppViewController * game = [storyboard instantiateViewControllerWithIdentifier:@"iOSAppViewController"];
//    [del.navigationController pushViewController:game animated:NO];
////
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
