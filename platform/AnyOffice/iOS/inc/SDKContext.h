//
//  SDKContext.h
//  anyofficesdk
//
//  Created by z00103873 on 14-7-9.
//  Copyright (c) 2014年 pangqi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "anyoffice_netstatus.h"
#import "svn_define.h"
#import "SDKContextOption.h"
@interface SDKContext : NSObject


+(SDKContext*) getInstance;

-(BOOL) init: (NSString*) workPath;
-(BOOL) init: (NSString *)userName andWorkPath:(NSString *) workPath;
-(BOOL) initWith: (SDKContextOption *)sdkContextOp;
-(BOOL) reset;

-(BOOL) uninit;
+(void)setLogCallback:(SVN_WriteLogCallback)callback;

-(NSString*) getVersion;

- (void)setChiErrorMsgByErrorID:(NSDictionary *)chDic;

- (void)setEnErrorMsgByErrorID:(NSDictionary *)enDic;

@end

