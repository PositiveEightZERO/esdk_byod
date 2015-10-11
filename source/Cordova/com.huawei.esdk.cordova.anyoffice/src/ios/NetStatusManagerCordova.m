#import "NetStatusManagerCordova.h"
#import <SvnSdk/NetStatusManager.h>

@interface NetStatusManagerCordova()<NetChangeCallbackDelegate>
{
    NSString* callbackId;
}
@end

@implementation NetStatusManagerCordova

- (void)getNetStatus:(CDVInvokedUrlCommand*)command
{

    NSString* callbackID= [command callbackId];
    
    int res = [[NetStatusManager getInstance] getNetStatus];


    CDVPluginResult* result = [CDVPluginResult
                               resultWithStatus:CDVCommandStatus_OK
                               messageAsInt:res];

    [self.commandDelegate sendPluginResult:result callbackId:callbackID];
}


- (void) start:(CDVInvokedUrlCommand*)command
{
    callbackId = [command callbackId];
    [[NetStatusManager getInstance] initWithDelegate:self];
    
    NSLog(@"Cordova NetStatusManager init finished.");
}


-(void)onNetChangedWithOldStatus:(NET_STATUS_EN)oldStatus newStatus:(NET_STATUS_EN)newStatus errCode:(int)error
{
    NSLog(@"Cordova onNetChangedWithOldStatus:%d newStatus:%d errorCode;%d", oldStatus,
          newStatus, error);
    
    NSMutableDictionary *obj = [[NSMutableDictionary alloc] init];
    [obj setValue:[NSNumber numberWithInt:oldStatus] forKey:@"oldStatus"];
    [obj setValue:[NSNumber numberWithInt:newStatus] forKey:@"newStatus"];
    [obj setValue:[NSNumber numberWithInt:error] forKey:@"errorCode"];


    
    CDVPluginResult* result = [CDVPluginResult
                               resultWithStatus:CDVCommandStatus_OK
                               messageAsDictionary:obj];
    [result setKeepCallback:[NSNumber numberWithBool:YES]];
    [self.commandDelegate sendPluginResult:result callbackId:callbackId];
}

@end