//
//  SvnMediaPlayer.m
//  SvnSdk
//
//  Created by l00174413 on 8/4/15.
//  Copyright (c) 2015 huawei. All rights reserved.
//

#import "SvnMediaPlayer.h"

//#import "SvnSdk/SvnFileManager.h"
//#import "SvnSdk/SecBrowHttpProtocol.h"

#import "SvnFileManager.h"
#import "SecBrowHttpProtocol.h"


#import "SvnMediaPlayerView.h"
//#import "SvnMediaFileURLProtocol.h"











@implementation SvnMediaPlayer

+ (BOOL) playMediaFile:(NSString*) path frame:(CGRect) frame
{
    if([[[UIDevice currentDevice] systemVersion] floatValue] < 7.0)
    {
        return NO;
    }
    
    
    if(!([path hasPrefix:@"/"] || [path hasPrefix:@"http://"] || [path hasPrefix:@"file:///"]))
    {
        return NO;
    }
    
//    BOOL isDirectory = NO;
//    
//    BOOL isExist = [[SvnFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDirectory];
//    
//    if(!isExist || isDirectory)
//    {
//        return NO;
//    }
    
    [NSURLProtocol registerClass:[SecBrowHttpProtocol class]];
    
    //[NSURLProtocol registerClass:[SvnMediaFileURLProtocol class]];
    
    if([path hasPrefix:@"/"])
    {
        path = [NSString stringWithFormat:@"file://%@", path];
    }
    
    NSString *srcFilePath = [NSString stringWithFormat:@"media%@", path];
    
    
    //CGRect frame = self.view.frame;
    
    //frame.origin.y += 64;
    
    //frame.size.height -= 64;
    
    // defaults
    SvnMediaPlayerView *playerView = [[SvnMediaPlayerView alloc] initWithFrame:frame];
   
    
    //UIWindow* currentWindow = [UIApplication sharedApplication].keyWindow;
    //[currentWindow addSubview:playerView];
    //[self.view addSubview:playerView];
    //[self.webView.superview addSubview:playerView];
    
    UIWindow* currentWindow = [UIApplication sharedApplication].keyWindow;
    [currentWindow addSubview:playerView];
    

    NSURL *URL = [NSURL URLWithString:[srcFilePath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    
    //NSURL *URL = [NSURL fileURLWithPath:[srcFilePath substringFromIndex:12]];
    
    
    
    NSLog(@"to playMediaFile:%@", URL);
    
    NSLog(@"url scheme:%@", URL.scheme);
    
    NSLog(@"url absoluteString:%@", URL.absoluteString);
    
    [playerView setVideoURL:URL];
    [playerView prepareAndPlayAutomatically:YES];
    
    return YES;
}


@end


@implementation NSBundle (SvnSdkLibrary)

+ (NSBundle*)myLibraryResourcesBundle {
    static dispatch_once_t onceToken;
    static NSBundle *myLibraryResourcesBundle = nil;
    dispatch_once(&onceToken, ^{
        myLibraryResourcesBundle = [NSBundle bundleWithURL:[[NSBundle mainBundle] URLForResource:@"SvnSdkResources" withExtension:@"bundle"]];
    });
    return myLibraryResourcesBundle;
}

@end
