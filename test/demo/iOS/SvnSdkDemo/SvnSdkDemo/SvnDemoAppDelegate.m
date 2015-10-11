//
//  SvnDemoAppDelegate.m
//  SvnSdkDemo
//
//  Created by l00174413 on 13-6-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import "SvnDemoAppDelegate.h"
#import "SvnSdk/SvnHttpURLProtocol.h"
#import "SvnDemoViewController.h"

#import "SvnSdk/svn_define.h"
#import "SvnSdk/customCopyAndPast.h"

#import "SvnSdk/SecBrowHttpProtocol.h"

#import "Svnsdk/SSOUrlSchema.h"

#import "UIHelper.h"

NSURL* urlURL;
#define anyofficeScheme @"anyoffice"



@implementation SvnDemoAppDelegate

@synthesize window = _window;


- (void)customizeAppearance
{
    // Create resizable images
    UIColor * navigationBarColor = [UIColor colorWithRed:(246/255.f) green:(246/255.f) blue:(246/255.f) alpha:1.0f];
    
    
    
    
    UIImage *gradientImage44 = [[UIImage imageWithColor:navigationBarColor size:CGSizeMake(1, 44)]
                                resizableImageWithCapInsets:UIEdgeInsetsMake(0, 0, 0, 0)];
    UIImage *gradientImage32 = [[UIImage imageWithColor:navigationBarColor size:CGSizeMake(1, 32)]
                                resizableImageWithCapInsets:UIEdgeInsetsMake(0, 0, 0, 0)];
    
    // Set the background image for *all* UINavigationBars
    [[UINavigationBar appearance] setBackgroundImage:gradientImage44
                                       forBarMetrics:UIBarMetricsDefault];
    [[UINavigationBar appearance] setBackgroundImage:gradientImage32
                                       forBarMetrics:UIBarMetricsLandscapePhone];
    
    // Customize the title text for *all* UINavigationBars
    [[UINavigationBar appearance] setTitleTextAttributes:
     [NSDictionary dictionaryWithObjectsAndKeys:
      [UIColor colorWithRed:51.0/255.0 green:51.0/255.0 blue:51.0/255.0 alpha:1.0],
      UITextAttributeTextColor,
      [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.8],
      UITextAttributeTextShadowColor,
      [NSValue valueWithUIOffset:UIOffsetMake(0, 0)],
      UITextAttributeTextShadowOffset,
      [UIFont fontWithName:@"Arial" size:14.0],
      UITextAttributeFont,
      nil]];
    
    UIColor * selectedColor = [UIColor colorWithRed:(0/255.f) green:(153/255.f) blue:(255/255.f) alpha:1.0f];
    UIColor * normalColor = [UIColor colorWithRed:(255/255.f) green:(255/255.f) blue:(255/255.f) alpha:1.0f];
    
    //iOS6
    [[UISegmentedControl appearance] setBackgroundImage:[UIImage imageWithColor:selectedColor size:CGSizeMake(1, 29)]
                                               forState:UIControlStateSelected
                                             barMetrics:UIBarMetricsDefault];
    
    [[UISegmentedControl appearance] setBackgroundImage:[UIImage imageWithColor:normalColor size:CGSizeMake(1, 29)]
                                               forState:UIControlStateNormal
                                             barMetrics:UIBarMetricsDefault];
    
    [[UISegmentedControl appearance] setDividerImage:[UIImage imageWithColor:selectedColor size:CGSizeMake(1, 29)]
                                 forLeftSegmentState:UIControlStateNormal
                                   rightSegmentState:UIControlStateSelected
                                          barMetrics:UIBarMetricsDefault];
    
    [[UISegmentedControl appearance] setTitleTextAttributes:@{
                                                              UITextAttributeTextColor: selectedColor,
                                                              UITextAttributeFont: [UIFont systemFontOfSize:14],
                                                              UITextAttributeTextShadowOffset: [NSValue valueWithUIOffset:UIOffsetMake(0, 0)] }
                                                   forState:UIControlStateNormal];
    
    [[UISegmentedControl appearance] setTitleTextAttributes:@{
                                                              UITextAttributeTextColor: normalColor,
                                                              UITextAttributeFont: [UIFont systemFontOfSize:14],
                                                              UITextAttributeTextShadowOffset: [NSValue valueWithUIOffset:UIOffsetMake(0, 0)]}
                                                   forState:UIControlStateSelected];
    
    
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{

    // Override point for customization after application launch.
    
    [self customizeAppearance];
    
    //[[UIBarButtonItem appearance] setBackgroundImage:[UIImage imageNamed:@"clear"] forState:UIControlStateNormal barMetrics:UIBarMetricsDefault] ;
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
    [CustomCopyAndPast SVN_API_Clipboard_FrontToBack];
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
    [CustomCopyAndPast SVN_API_Clipboard_BackToFront];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
}


- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    //exampleapp://auth?source=Huawei&user_name=xxx&password=xxx&SvnServer=xxx&SvnServerBackup=xxx&SrcAppScheme=anyoffice%3A%2F%2F
    
//    if (!url)
//    {
//        return NO;
//    }
//    
//    SSOUrlSchema *schema = [[SSOUrlSchema alloc] initWithUrlSchema:url.absoluteString];
//    NSLog(@"schema:%@, %@, %@", [schema getServerAddress], [schema getUsername], [schema getPassword]);

    return YES;
}

@end
