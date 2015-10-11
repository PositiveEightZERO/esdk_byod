//
//  SvnFileManager.m
//  iDeskAPI
//
//  Created by yemingxing on 6/13/14.
//  Copyright (c) 2014 www.huawei.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SvnFileManager.h"

#import "SvnFileHandle.h"

//#import "ACCForIOS.h"
#import "SvnFileURLProtocol.h"

#import <Foundation/Foundation.h>

#import "svn_define.h"
#import "svn_api.h"
#import "svn_file_api.h"
#import "svn_file_api_ex.h"


//#define MDM_LAST_LOGON_ACCOUNT_KEY     @"com.huawei.lastLogonAccount"
//
//static NSString* mdmFileContainer = @"com.huawei.idesk.file_container";
//
//static NSString* __defaultUser = nil;

static SvnFileManager* __sharedManager = nil;

@interface SvnFileManager()
{
    //BOOL bInited;
}

-(BOOL)_copyFile:(NSString*)source toFile:(NSString*)destination error:(NSError**)error;

-(BOOL)_copyPath:(NSString*)source toPath:(NSString*)destination error:(NSError**)error;

@end

@implementation SvnFileManager



+(instancetype) defaultManager {
    if (!__sharedManager) {
        __sharedManager = [[SvnFileManager alloc] init];
        if (__sharedManager) {
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                if ([NSURLProtocol registerClass:[SvnFileURLProtocol class]]) {
                    NSLog(@"Succeed to register idesk file protocol.");
                } else {
                    NSLog(@"Failed to register idesk file protocol.");
                }
            });
        }
        
    }
    return __sharedManager;
}

//+ (void) setDefaultUser:(NSString*) userAccount
//{
//    if (!userAccount) {
//        return;
//    }
////    [ACCForIOS startAccServer:userAccount];
//    
//    
//    IDeskByodManager *byodManager = [[IDeskByodManager alloc] init];
//    byodManager.username = userAccount;
//    [byodManager appInforReport];
//    
//    
//    if ([__defaultUser compare:userAccount options:NSCaseInsensitiveSearch]) {
//        __defaultUser = userAccount;
//        [__sharedManager.vpnfile FileEncCleanEnv];
//        [__sharedManager release];
//        
//        __sharedManager = nil;
//    }
//    __defaultUser = userAccount;
//
//}
//
//
//
//+ (NSString*) defaultUser {
//    NSString* clientUser = __defaultUser;
//    if (!clientUser) {
//        clientUser = [IDeskMDMIdentity currentIdentity].userAccount;
//    }
//   
//    return clientUser;
//}

//- (id) initWithPath:(NSString*) path
//{
//    if (self = [super init]) {
//        if (!path) {
//            return nil;
//        }
//        self.rootURL = [NSURL fileURLWithPath:path];
//    }
//    return self;
//}




//- (NSData*) randomKey
//{
//    unsigned char buffer[32];
//    arc4random_buf(buffer, sizeof(buffer));
//    buffer[31] = 0;
//    return [NSData dataWithBytes:buffer length:sizeof(buffer)];
//}

- (BOOL)fileExistsAtPath:(NSString*) path {
    if (!path) {
        return NO;
    }
    int rr= svn_access([path UTF8String], F_OK);
    return (rr == SVN_OK);
}

- (BOOL)fileExistsAtPath:(NSString*) path isDirectory:(BOOL*)b {
    if (!path) {
        *b = NO;
        return NO;
    }
    SVN_DIR_S* dirInfo = svn_opendir([path UTF8String]);
    if (dirInfo == NULL) {
        *b = NO;
        return [self fileExistsAtPath:path];
    } else {
        *b = YES;
        svn_closedir(dirInfo);
        return YES;
    }
}

- (BOOL)isReadableFileAtPath:(NSString*) path {
    if (!path) {
        return NO;
    }
    int iRet = svn_access([path UTF8String], R_OK);
    if(iRet == -1){
        NSLog(@"isReadable error num:%d", svn_file_geterrno());
    }
    return (iRet == SVN_OK);
}

- (BOOL)isWritableFileAtPath:(NSString*) path {
    if ( !path) {
        return NO;
    }
    int iRet = svn_access([path UTF8String], W_OK);
    if(iRet == -1){
        NSLog(@"isWritable error num:%d",svn_file_geterrno());
    }
    return (iRet == SVN_OK);
}

- (BOOL)isExecutableFileAtPath:(NSString*) path {
    if (!path) {
        return NO;
    }
    int iRet = svn_access([path UTF8String], X_OK);
    
    if(iRet == -1){
        NSLog(@"isExecutable error num:%d", svn_file_geterrno());
    }
    return (iRet == SVN_OK);
}

- (BOOL)isDeletableFileAtPath:(NSString*) path {
    return [self isWritableFileAtPath:path];
}

- (NSData*)contentsAtPath:(NSString*) path {
    int iRet = SVN_OK;
    if (!path) {
        return nil;
    }
    SVN_FILE_S *file = svn_fopen([path UTF8String], "r");

    if (!file) {
        return  nil;
    }
    SVN_STAT_S stats;
    iRet = svn_stat([path UTF8String], &stats);
    if (iRet != SVN_OK) {

        svn_fclose(file);
        return nil;
    }
    NSMutableData* data = [NSMutableData dataWithCapacity:stats.ulSize];
    uint8_t buffer[1024 * 16] = {0};
    unsigned long lRead = 0;
    while ((lRead = svn_fread(buffer, sizeof(uint8_t), sizeof(buffer) / sizeof(uint8_t), file)) > 0){
        [data appendBytes:buffer length:lRead];
    }
    svn_fclose(file);
    return data;
}


- (BOOL)createDirectoryAtPath:(NSString*)path attributes:(NSDictionary*)attributes error:(NSError**)error {
    if (!path) {
        return NO;
    }
    int iRet = svn_mkdir([path UTF8String]);
    
    return (iRet == SVN_OK);
}



- (BOOL)createDirectoryAtPath:(NSString*)path attributes:(NSDictionary*)attributes{
    return [self createDirectoryAtPath:path attributes:attributes error:nil];
}



- (BOOL)createDirectoryAtPath:(NSString*)path withIntermediateDirectories:(BOOL)b attributes:(NSDictionary*)attributes error:(NSError**)error {
    if (!path) {
        return NO;
    }
    int iRet = SVN_OK;
    if (b) {
        iRet = svn_mkdir_ex([path UTF8String]);
        
    } else {
        iRet = svn_mkdir([path UTF8String]);
    }
    return (iRet == SVN_OK);
}

- (BOOL)createDirectoryAtURL:(NSURL*) url withIntermediateDirectories:(BOOL)b attributes:(NSDictionary*)attributes error:(NSError**)error {
    return [self createDirectoryAtPath:url.path withIntermediateDirectories:b attributes:attributes error:error];
}

- (BOOL)createFileAtPath:(NSString*)path contents:(NSData*)data attributes:(NSDictionary*)attributes {
    if (!path) {
        return NO;
    }
    
    SVN_FILE_S *file = svn_fopen([path UTF8String], "w");
    
    if (NULL == file) {
        SVN_ERRNO_E error =  svn_file_geterrno();
        NSLog(@"Create file failed:%d", error);
        return NO;
    }
    
    //NSLog(@"[createFileAtPath]to write data len:%d", [data length]);
    
    
    unsigned long lWrited = 0;
    if (data && data.length > 0) {
        uint8_t buffer[1024 * 4] = {0};
        NSRange writeRange;
        
        while (lWrited < data.length) {
            writeRange.location = lWrited;
            writeRange.length = MIN(data.length - lWrited, sizeof(buffer)/ sizeof(uint8_t));
            [data getBytes:buffer range:writeRange];
            
            
            
            unsigned long writed = svn_fwrite(buffer, sizeof(uint8_t), writeRange.length, file);
            
            //NSLog(@"[createFileAtPath]writeRange.length:%d, writed:%d", writeRange.length, writed);
            if (writed == writeRange.length ) {
                lWrited += writeRange.length;
            } else {
                NSLog(@"[createFileAtPath] write failed writed:%d, data length:", writed, writeRange.length);
                break;
            }
        }
        
        NSLog(@"[createFileAtPath]lWrited:%d", lWrited);
        
        
        if (lWrited < data.length) {
            NSLog(@"[createFileAtPath]lWrited:%d, data length:", lWrited, [data length]);
            svn_fclose(file);
            //svn_remove([path UTF8String]);
            return NO;
        }
    }
    svn_fclose(file);

    return YES;
}

- (BOOL)removeItemAtURL:(NSURL*) url error:(NSError**) error {
    if (!url) {
        return NO;
    }
    return [self removeItemAtPath:url.path error:error];
}

- (BOOL)removeItemAtPath:(NSString*) path error:(NSError**) error {
    int iRet = SVN_OK;
    if (!path) {
        return NO;
    }
    iRet = svn_remove_ex([path UTF8String]);
    return (iRet == SVN_OK);
}

- (BOOL)copyItemAtURL:(NSURL*) fromURL toURL:(NSURL*) toURL error:(NSError**) error {
    return [self copyItemAtPath:[fromURL path] toPath:[toURL path] error:error];
}

- (BOOL)copyItemAtPath:(NSString*)fromPath toPath:(NSString*) toPath error:(NSError**) error {
    
    if (!fromPath || !toPath) {
        return NO;
    }
    
    BOOL		sourceIsDir;
    BOOL		fileExists;
    
    if ([self fileExistsAtPath: toPath] == YES)
    {
        return NO;
    }
    fileExists = [self fileExistsAtPath: fromPath isDirectory: &sourceIsDir];
    if (!fileExists)
    {
        return NO;
    }
    

    if (sourceIsDir)
    {
        
        /* If destination directory is a descendant of source directory copying
         isn't possible. */
        if ([[toPath stringByAppendingString: @"/"]
             hasPrefix: [fromPath stringByAppendingString: @"/"]])
        {
            return NO;
        }
        
        if ([self createDirectoryAtPath:toPath attributes: nil error:error] == NO)
        {
            return NO;
        }
        
        if ([self _copyPath: fromPath toPath: toPath error:error] == NO)
        {
            return NO;
        }
    }
    else
    {
        
        if ([self _copyFile: fromPath toFile: toPath error:error] == NO)
        {
            return NO;
        }
    }

    return YES;
}

- (BOOL)moveItemAtURL:(NSURL*) fromURL toURL:(NSURL*) toURL error:(NSError**) error {
    return [self moveItemAtPath:[fromURL path] toPath:[toURL path] error:error];
}

- (BOOL)moveItemAtPath:(NSString*)fromPath toPath:(NSString*) toPath error:(NSError**) error {
    
    BOOL		sourceIsDir;
    BOOL		fileExists;
    
    if ([self fileExistsAtPath: toPath] == YES)
    {
        return NO;
    }
    fileExists = [self fileExistsAtPath: fromPath isDirectory: &sourceIsDir];
    if (!fileExists)
    {
        return NO;
    }

    /* Check to see if the source and destination's parent are on the same
    physical device so we can perform a rename syscall directly. */


    /* source and destination are on the same device so we can simply
    invoke rename on source. */


    if (svn_rename([fromPath UTF8String], [toPath UTF8String]) != SVN_OK)
    {
        *error = [[NSError alloc] initWithDomain:@"move file error" code:svn_file_geterrno() userInfo:nil];
        return NO;
    }
    return YES;
    

}



- (NSDictionary *)attributesOfItemAtPath:(NSString *)path error:(NSError **)error {
    if (!path) {
        return nil;
    }
    
    NSString* sysPath = [self GetEncFilePath:path];
    NSDictionary* sysInfo = [[NSFileManager defaultManager] attributesOfItemAtPath:sysPath error:error];
    if (!sysInfo) {
        return nil;
    }
    NSMutableDictionary* fileInfo = [NSMutableDictionary dictionaryWithDictionary:sysInfo];
    SVN_STAT_S pstStat;
    int iRet= svn_stat([path UTF8String], &pstStat);
    if ( iRet != SVN_OK )
    {
        return fileInfo;
    }
    NSNumber* fileSize = [NSNumber numberWithUnsignedLong: pstStat.ulSize];
    [fileInfo setObject:fileSize forKey:NSFileType];
    return fileInfo;
}

- (NSString *)GetEncFilePath:(NSString *)fromPath {
    char buffer[1024*4] = {0};
    
    SVN_API_GetEncFilePath([fromPath UTF8String], buffer, 1024*4);
    NSString *from = [NSString stringWithCString:buffer encoding:NSUTF8StringEncoding];
    return from;
}


//- (BOOL)isEncFile:(NSString *)path{
//    if (!path) {
//        return NO;
//    }
//    int isEnc = 0;
//    svn_isencfile([path UTF8String], &isEnc);
//    return isEnc == 1;
//}


-(BOOL)_copyFile:(NSString*)fromPath toFile:(NSString*)toPath error:(NSError**)error{
    if (!fromPath || !toPath) {
        return NO;
    }
    
    SVN_FILE_S *srcFile = svn_fopen([fromPath UTF8String], "r");
    
    if (NULL == srcFile) {
        SVN_ERRNO_E error =  svn_file_geterrno();
        NSLog(@"open file failed:%d", error);
        return NO;
    }
    
    
    SVN_FILE_S *destFile = svn_fopen([toPath UTF8String], "w");
    
    if (!destFile) {
        svn_fclose(srcFile);
        return  NO;
    }
    
    SVN_STAT_S stats;
    int iRet = svn_stat([fromPath UTF8String], &stats);
    if (iRet != SVN_OK) {
        
        svn_fclose(srcFile);
        svn_fclose(destFile);
        return nil;
    }
    NSMutableData* data = [NSMutableData dataWithCapacity:stats.ulSize];
    uint8_t buffer[1024 * 16] = {0};
    unsigned long lRead = 0;
    while ((lRead = svn_fread(buffer, sizeof(uint8_t), sizeof(buffer) / sizeof(uint8_t), srcFile)) > 0){
        
        unsigned long lWrited = 0;
        
        NSRange writeRange;
        
        while (lWrited < lRead) {
            
            writeRange.location = lWrited;
            writeRange.length = lRead - lWrited;
            [data getBytes:buffer range:writeRange];
            if (writeRange.length == svn_fwrite(buffer + lWrited, sizeof(uint8_t), writeRange.length, destFile)) {
                lWrited += writeRange.length;
            } else {
                break;
            }
        }
        if (lWrited < lRead) {
            svn_fclose(srcFile);
            svn_fclose(destFile);
            svn_remove([toPath UTF8String]);
            return NO;
        }
        
    }
    svn_fclose(srcFile);
    svn_fclose(destFile);
    
    return YES;
}

-(BOOL)_copyPath:(NSString*)source toPath:(NSString*)destination error:(NSError**)error{

    
    /*open directory*/
    SVN_DIR_S     *dir     = NULL;
    dir = svn_opendir([source UTF8String]);
    if(NULL == dir)
    {
        NSLog(@"open source dir fail");
        return NO;
    }
    SVN_DIRINFO_S* subdir = NULL;
    subdir = svn_readdir(dir);
    while(subdir != NULL)
    {
        if((strcmp(subdir->acDirName, ".") == 0) ||(strcmp(subdir->acDirName,"..") == 0))
        {
            subdir = svn_readdir(dir);
            continue;
        }
    
        
        
        NSLog(@"subdir->acDirName:%s", subdir->acDirName);
        
        NSString *dirEntry = [NSString stringWithUTF8String:subdir->acDirName];
        
        if(!dirEntry)
        {
            NSLog(@"dirEntry nil");
            continue;
        }
        
        NSString		*sourceFile;
        NSString		*destinationFile;

        

        sourceFile = [source stringByAppendingPathComponent: dirEntry];
        destinationFile = [destination stringByAppendingPathComponent: dirEntry];
        
        
        
        BOOL sourceIsDir;
        
        [self fileExistsAtPath: sourceFile isDirectory: &sourceIsDir];
      
        
        
        if (sourceIsDir)
        {
            BOOL	dirOK;
            
            dirOK = [self createDirectoryAtPath: destinationFile
                                     attributes: nil];
            if (dirOK == NO)
            {
                
                svn_closedir(dir);
                 return NO;
                
                /*
                 * We may have managed to create the directory but not set
                 * its attributes ... if so we can continue copying.
                 */
                if (![self fileExistsAtPath: destinationFile isDirectory: &dirOK])
                {
                    svn_closedir(dir);
                    dirOK = NO;
                }
            }
            if (dirOK == YES)
            {
                if (![self _copyPath: sourceFile
                              toPath: destinationFile
                               error:error])
                {
                    svn_closedir(dir);
                    return NO;
                }
            }
        }
        else
        {
            if (![self _copyFile: sourceFile
                          toFile: destinationFile
                           error:error])
                
            {
                svn_closedir(dir);
                return NO;
            }
        }


        subdir = svn_readdir(dir);
    }
    
    
    
    svn_closedir(dir);
    return YES;
}

/**
 * Returns an array of the contents of the specified directory.<br />
 * The listing does <strong>not</strong> recursively list subdirectories.<br />
 * The special files '.' and '..' are not listed.<br />
 * Indicates an error by returning nil (eg. if path is not a directory or
 * it can't be read for some reason).
 */
- (NSArray*) directoryContentsAtPath: (NSString*)path
{

    NSMutableArray	*content;
    BOOL			is_dir;
    
    /*
     * See if this is a directory (don't follow links).
     */
    if ([self fileExistsAtPath: path isDirectory: &is_dir] == NO || is_dir == NO)
    {
        return nil;
    }

    
    /*open directory*/
    SVN_DIR_S     *dir     = NULL;
    dir = svn_opendir([path UTF8String]);
    if(NULL == dir)
    {
        NSLog(@"open source dir fail");
        return nil;
    }
    
    content = [NSMutableArray arrayWithCapacity: 128];

    SVN_DIRINFO_S* subdir = NULL;
    subdir = svn_readdir(dir);
    while(subdir != NULL)
    {
        if((strcmp(subdir->acDirName, ".") == 0) ||(strcmp(subdir->acDirName,"..") == 0))
        {
            subdir = svn_readdir(dir);
            continue;
        }
        
        
        
        NSLog(@"subdir->acDirName:%s", subdir->acDirName);
        
        NSString *dirEntry = [NSString stringWithUTF8String:subdir->acDirName];
        if(dirEntry)
        {
            [content addObject:dirEntry];
        }
    
        
        
        subdir = svn_readdir(dir);
    }

    svn_closedir(dir);
    return [content copy];
}
@end
