#import <Cordova/CDV.h>

@interface NetStatusManagerCordova : CDVPlugin

- (void) getNetStatus:(CDVInvokedUrlCommand*)command;
- (void) start:(CDVInvokedUrlCommand*)command;
@end