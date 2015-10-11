#import "SDKContextCordova.h"

#import <Svnsdk/SDKContext.h>
#import <SvnSdk/SecBrowHttpProtocol.h>


@implementation SDKContextCordova


- (void)pluginInitialize
{
    [NSURLProtocol registerClass:[SecBrowHttpProtocol class]];
}

- (void)init:(CDVInvokedUrlCommand*)command
{

    NSString* name = [[command arguments] objectAtIndex:0];
    
    NSString* workPath = [[command arguments] objectAtIndex:0];
    NSString* username = [[command arguments] objectAtIndex:1];
    
    BOOL res = FALSE;
    
    
    
//    if([username length] > 1)
//    {
//        res = [[SDKContext getInstance] init:username andWorkPath:workPath];
//    }
//    else
//    {
        res = [[SDKContext getInstance] init:workPath];
//    }
    
    NSLog(@"Cordova:SDKContext init.");
    CDVPluginResult* result;
    
    if(res)
    {
        result = [CDVPluginResult
                  resultWithStatus:CDVCommandStatus_OK
                  messageAsBool:YES];
        
 
    }
    else
    {
        result = [CDVPluginResult
                  resultWithStatus:CDVCommandStatus_ERROR
                  messageAsBool:NO];

    }
    
    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];


    

}

@end