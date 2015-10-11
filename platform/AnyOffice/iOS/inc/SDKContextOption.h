//
//  SXDKContextOption.h
//  anyofficesdk
//
//  Created by kf1 on 15-4-3.
//  Copyright (c) 2015年 pangqi. All rights reserved.
//
#import <Foundation/Foundation.h>

@interface SDKContextOption : NSObject
-(BOOL) addCertFromFile :(NSString *) fileName;
-(BOOL) addCert: (NSString *) certContent;
-(id)initWithUserName:(NSString *)userName workpath:(NSString *)workpath sdkeyAuth:(BOOL) sdkeyAuth;
@property NSString * userName;
@property NSString * workPath;
@property BOOL sdkeyAuth;
@property NSMutableArray *certs;

@end
