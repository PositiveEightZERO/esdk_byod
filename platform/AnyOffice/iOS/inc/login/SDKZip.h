//
//  SDKZip.h
//  anyofficesdk
//
//  Created by ui1 on 14-8-15.
//  Copyright (c) 2014年 pangqi. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SDKZip : NSObject

-(BOOL) CreateZipFile2:(NSString*) zipFile;
-(BOOL) CreateZipFile2:(NSString*) zipFile Password:(NSString*) password;
-(BOOL) addFileToZip:(NSString*) file newname:(NSString*) newname;
-(BOOL) CloseZipFile2;

-(BOOL) UnzipOpenFile:(NSString*) zipFile;
-(BOOL) UnzipOpenFile:(NSString*) zipFile Password:(NSString*) password;
-(BOOL) UnzipFileTo:(NSString*) path overWrite:(BOOL) overwrite;
-(BOOL) UnzipCloseFile;

@end
