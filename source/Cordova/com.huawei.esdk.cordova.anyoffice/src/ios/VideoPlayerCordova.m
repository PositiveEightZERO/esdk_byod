//
//  CDVVideo.m
//  
//
//  Created by Peter Robinett on 2012-10-15.
//
//

#import "VideoPlayerCordova.h"
#import "MediaPlayer/MPMoviePlayerViewController.h"
#import "MediaPlayer/MPMoviePlayerController.h"

#import <Cordova/CDV.h>

#import <SvnSdk/SecBrowHttpProtocol.h>
#import <SvnSdk/SvnHttpURLProtocol.h>
#import "GUIPlayerView.h"

@implementation VideoPlayerCordova


- (void)pluginInitialize
{

    [NSURLProtocol registerClass:[SecBrowHttpProtocol class]];
    //[NSURLProtocol registerClass:[SvnHttpURLProtocol class]];
     NSLog(@"VideoPlayerCordova registerClass");
    
    
}

- (void)play:(CDVInvokedUrlCommand*)command
{
    NSString *movie = [[command arguments] objectAtIndex:0];
    
    if([movie hasPrefix:@"http://"] || [movie hasPrefix:@"https://"] ) {
        movie = [NSString stringWithFormat:@"media%@", movie];
        
    }
    
    
    NSDictionary *positon = (NSDictionary *)[command.arguments objectAtIndex:1];
    
    float height = ([positon objectForKey:@"height"]) ? [[positon objectForKey:@"height"] floatValue] : self.webView.bounds.size.height/2;
    float width = ([positon objectForKey:@"width"]) ? [[positon objectForKey:@"width"] floatValue] : self.webView.bounds.size.width;
    
    
    float left = ([positon objectForKey:@"left"]) ? [[positon objectForKey:@"left"] floatValue] : 0;
    float top = ([positon objectForKey:@"top"]) ? [[positon objectForKey:@"top"] floatValue] : 0;

    
    
    left += self.webView.bounds.origin.x;
    top += self.webView.bounds.origin.y;
    
    
    CGRect rect = CGRectMake(left, top, width, height);

    NSLog(@"player pos:%@", NSStringFromCGRect(rect));
    
    [self addPlayerFor:movie atPosition:rect andOptions:nil];
 
}

- (void)MovieDidFinish:(NSNotification *)notification {
    
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                  name:MPMoviePlayerPlaybackDidFinishNotification
                                                object:nil];
    //[self writeJavascript:[NSString stringWithFormat:@"CDVVideo.finished(\"%@\");", movie]];
  
}

- (void)addPlayerFor:(NSString *)url atPosition:(CGRect)position andOptions:(NSDictionary *)options
{

    // defaults
    GUIPlayerView *playerView = [[GUIPlayerView alloc] initWithFrame:position];
    [playerView setDelegate:self];
    
    
    //UIWindow* currentWindow = [UIApplication sharedApplication].keyWindow;
    //[currentWindow addSubview:playerView];
    [[[self viewController] view] addSubview:playerView];
    //[self.webView.superview addSubview:playerView];
    
    NSURL *URL = [NSURL URLWithString:url];
    [playerView setVideoURL:URL];
    [playerView prepareAndPlayAutomatically:YES];
    
}

//- (IBAction)removePlayer:(UIButton *)sender {
//    [copyrightLabel setHidden:YES];
//    
//    [playerView clean];
//    
//    [addPlayerButton setEnabled:YES];
//    [removePlayerButton setEnabled:NO];
//}

@end
