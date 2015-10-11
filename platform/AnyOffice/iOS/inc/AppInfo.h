//
//  AppInfo.h
//  anyofficesdk
//
//  Created by ljj on 14-9-12.
//  Copyright (c) 2014年 pangqi. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AppInfo : NSObject <NSCopying> {
    
}

@property(nonatomic, strong) NSString* appVersion;
@property(nonatomic, strong) NSString* identifier;
@property(nonatomic, strong) NSString* name;
@property(nonatomic, strong) NSString* appSize;
@property(nonatomic, strong) NSString* packageURL;
@property(nonatomic, strong) NSString* iconURL;
@property(nonatomic, strong) NSString* description;
@property(nonatomic, assign) BOOL      isBetaVersion;
@property(nonatomic,assign)long packageSize;

@end
