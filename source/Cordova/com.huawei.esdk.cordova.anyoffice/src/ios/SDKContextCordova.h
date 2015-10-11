#import <Cordova/CDV.h>

@interface SDKContextCordova : CDVPlugin
- (void)pluginInitialize;
- (void) init:(CDVInvokedUrlCommand*)command;

@end