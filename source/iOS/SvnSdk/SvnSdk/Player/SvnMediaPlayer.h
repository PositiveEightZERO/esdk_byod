//
//  SvnMediaPlayer.h
//  SvnSdk
//
//  Created by l00174413 on 8/4/15.
//  Copyright (c) 2015 huawei. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SvnMediaPlayer : NSObject

+(BOOL) playMediaFile:(NSString*) path frame:(CGRect) frame;

@end



@interface NSBundle (SvnSdkLibrary)
+ (NSBundle*)myLibraryResourcesBundle;
@end
