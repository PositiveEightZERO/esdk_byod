//
//  com_huaweil_svn_sdk_CustomCopyAndPast.h
//  com.huaweil.svn.sdk.CustomCopyAndPast
//
//  Created by kf2 on 13-11-25.
//  Copyright (c) 2013年 anyoffice. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface CustomCopyAndPast : NSObject

+(CustomCopyAndPast *)sharedSingleton;
@property (nonatomic, readwrite, retain) NSMutableString *strCopy;

+(void)SVN_API_Clipboard_FrontToBack;
+(void)SVN_API_Clipboard_BackToFront;
@end
