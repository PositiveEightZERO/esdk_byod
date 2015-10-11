//
//  UserInfo.h
//  anyofficesdk
//
//  Created by z00103873 on 14-7-4.
//  Copyright (c) 2014年 pangqi. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AnyOfficeUserInfo : NSObject

@property(strong, nonatomic)NSString* domain;
@property(strong, nonatomic)NSString* userName;
@property(strong, nonatomic)NSString* password;

-(id)initWithDomain:(NSString *)domain username:(NSString *)user password:(NSString*) pass;

@end
