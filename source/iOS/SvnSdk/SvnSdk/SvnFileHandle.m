//
//  SvnFileHandle.m
//
//  Created by lizhiyong on 6/24/15.
//  Copyright (c) 2014 www.huawei.com. All rights reserved.
//

#import "SvnFileHandle.h"



@interface SvnFileHandle()
{
    //SvnFileDescriptorRef _fileDescriptor;

}

@property (nonatomic,strong) NSString * path;

@end

@implementation SvnFileHandle

+ (SvnFileHandle*)fileHandleForReadingAtPath:(NSString*)path
{
    if (!path) {
        return nil;
    }
    
    
    SVN_FILE_S* fs = svn_fopen([path UTF8String], "r+");
    
    
    return [[SvnFileHandle alloc] initWithIDeskFileDescriptor:fs] ;
}

+ (SvnFileHandle*)fileHandleForReadingFromURL:(NSURL*)fileURL error:(NSError**)error
{
    return [SvnFileHandle fileHandleForReadingAtPath:fileURL.path];
}

+ (SvnFileHandle*)fileHandleForWritingAtPath:(NSString*)path
{
    if (!path ) {
        return nil;
    }
    
    SVN_FILE_S* fs = svn_fopen([path UTF8String], "w+");
    if(fs == NULL)
    {
        NSLog(@"svn_fopen returns NULL");
        return nil;
    }
    
    return [[SvnFileHandle alloc] initWithIDeskFileDescriptor:fs];
}

+ (SvnFileHandle*)fileHandleForWritingToURL:(NSURL*)fileURL error:(NSError**)error
{
    return [SvnFileHandle fileHandleForWritingAtPath:fileURL.path];
}

- (id) initWithIDeskFileDescriptor:(SvnFileDescriptorRef) fileDescriptor
{
    if (!fileDescriptor) {
        return nil;
    }
    if (self = [super init]) {
        _fileDescriptor = fileDescriptor;
    }
    return self;
}

- (NSData*)readDataToEndOfFile
{
    
    NSMutableData* data = [NSMutableData data];
    uint8_t buffer[1024 * 16] = {0};
    unsigned long lRead = 0;
    
    while ((lRead = svn_fread(buffer, sizeof(uint8_t), sizeof(buffer) / sizeof(uint8_t), _fileDescriptor)) > 0)
        [data appendBytes:buffer length:lRead];
    return data;

}

- (NSData*)readDataOfLength:(NSUInteger)length
{
    
    NSMutableData* data = [NSMutableData dataWithCapacity:length];
    uint8_t buffer[1024 * 16] = {0};
    unsigned long long lRead = 0;
    while(data.length < length)
    {
        lRead = svn_fread(buffer, sizeof(uint8_t), MIN(length - data.length, sizeof(buffer) / sizeof(uint8_t)), _fileDescriptor);
        if(lRead <=0)
        {
            NSLog(@"svn_fread resturns:%d", lRead);
            break;
        }
        else
        {
            [data appendBytes:buffer length:lRead];
        }
    }
    return data;
}

- (void)writeData:(NSData *)data
{
    unsigned long long lWrited = 0;
    if (data && data.length > 0) {
        uint8_t buffer[1024 * 16] = {0};
        NSRange writeRange;
     
        while (lWrited < data.length) {
            writeRange.location = lWrited;
            writeRange.length = MIN(data.length - lWrited, sizeof(buffer)/ sizeof(uint8_t));
            [data getBytes:buffer range:writeRange];
            if (writeRange.length == svn_fwrite(buffer, sizeof(uint8_t), writeRange.length, _fileDescriptor)) {
                lWrited += writeRange.length;
            } else {
                break;
            }
        }
    }

}

- (void)truncateFileAtOffset:(unsigned long long)offset
{
    svn_ftruncate(_fileDescriptor, (unsigned long)offset);
}

- (void)closeFile
{
    svn_fclose(_fileDescriptor);
}

- (unsigned long long)seekToEndOfFile
{

    if (SVN_OK == svn_fseek(_fileDescriptor, 0, SVN_SEEK_END)) {
        return svn_ftell(_fileDescriptor);
    }
    return 0;
}

- (void)seekToFileOffset:(unsigned long long)offset
{
   
    int iRet = svn_fseek(_fileDescriptor, (long)offset, SVN_SEEK_SET);
    NSLog(@"seekToFileOffset %lld, ret:%d",offset, iRet);
    
}

- (unsigned long long)offsetInFile
{
    return svn_ftell(_fileDescriptor);
}



@end
