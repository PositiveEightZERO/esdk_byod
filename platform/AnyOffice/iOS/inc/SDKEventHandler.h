//
//  SDKEventHandler.h
//  SDKtest
//
//  Created by yzy on 15-3-10.
//  Copyright (c) 2015年 mail_user. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface SDKEventHandler : NSObject
- (void)applicationDidEnterBackground:(UIApplication *)application;
- (void)applicationDidBecomeActive:(UIApplication *)application;
- (void)applicationWillEnterForeground:(UIApplication *)application;

@end
