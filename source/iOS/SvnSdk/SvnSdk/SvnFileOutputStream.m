//
//  SvnFileOutputStream.m
//  iDeskAPI
//
//  Created by yemingxing on 6/23/14.
//  Copyright (c) 2014 www.huawei.com. All rights reserved.
//

#import "SvnFileOutputStream.h"


#import "svn_define.h"
#import "svn_api.h"
#import "svn_file_api.h"
#import "svn_file_api_ex.h"

@interface SvnFileOutputStream() {
    SVN_FILE_S* _fileDes;
    BOOL bClosed;
    
    NSStreamStatus _status;
}

@end

@implementation SvnFileOutputStream

+ (id)outputStreamToFileAtPath:(NSString *)path append:(BOOL)shouldAppend
{
    return [[SvnFileOutputStream alloc] initToFileAtPath:path append:shouldAppend] ;
}

+ (id) outputStreamWithURL:(NSURL *)url append:(BOOL)shouldAppend
{
    return [SvnFileOutputStream outputStreamToFileAtPath:url.path append:shouldAppend];
}

- (id)initToFileAtPath:(NSString *)path append:(BOOL)shouldAppend
{
    if (!path ) {
        return nil;
    }
    if (self = [super init]) {
        
        _fileDes = svn_fopen([path UTF8String], shouldAppend ? "a+" : "a");
        
        _bytesWritten = [[NSNumber alloc] initWithUnsignedInteger:0];
        if (!_fileDes) {
            return nil;
        }
        
        [self setStatus:NSStreamStatusOpening];
    }
    return self;
}

- (void) open
{
    [self setStatus:NSStreamStatusOpen];
}

- (void) close
{
    
    if (!bClosed) {
        bClosed = YES;
        svn_fclose(_fileDes);
        [self setStatus:NSStreamStatusClosed];
    }
    
}

- (void) dealloc
{
    if (!bClosed) {
        [self close];
    }

}

- (id)initWithURL:(NSURL *)url append:(BOOL)shouldAppend
{
    return [self initToFileAtPath:url.path append:shouldAppend];
}

- (NSInteger)write:(const uint8_t *)buffer maxLength:(NSUInteger)length
{
    unsigned long written = svn_fwrite(buffer, sizeof(uint8_t), length, _fileDes);
    NSUInteger totalWritten = self.bytesWritten.unsignedIntegerValue;
    totalWritten += written;

    _bytesWritten = [[NSNumber alloc] initWithUnsignedInteger:totalWritten];
    return written;
}

- (BOOL)hasSpaceAvailable
{
    return YES;
}


- (void)setStatus:(NSStreamStatus)aStatus {
    _status = aStatus;
}


- (NSStreamStatus)streamStatus {
    return _status;
}

@end
