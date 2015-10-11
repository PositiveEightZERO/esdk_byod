#import <Cordova/CDV.h>

@interface FilePluginCordova : CDVPlugin

- (void) listFile:(CDVInvokedUrlCommand*)command;
- (void) readFile:(CDVInvokedUrlCommand*)command;
@end