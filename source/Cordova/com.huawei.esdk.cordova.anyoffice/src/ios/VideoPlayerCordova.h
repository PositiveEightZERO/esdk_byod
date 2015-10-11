//
//  CDVVideo.h
//  
//
//  Created by Peter Robinett on 2012-10-15.
//
//

#import <Cordova/CDV.h>

@interface VideoPlayerCordova : CDVPlugin {

}


- (void) play:(CDVInvokedUrlCommand*)command;
- (void) close:(CDVInvokedUrlCommand*)command;

@end