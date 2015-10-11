//
//  NSClassExtend.m
//  SvnSdkDemo
//
//  Created by l00174413 on 7/25/15.
//
//

#import "NSClassExtend.h"

#import "SvnFileHandle.h"


@implementation NSData(SvnData)


+ (instancetype) dataWithContentsOfFileSvn:(NSString *)path options:(NSDataReadingOptions)readOptionsMask error:(NSError **)errorPtr
{
    NSLog(@"dataWithContentsOfFile:%@", path);
    return  [[NSData alloc]  initWithContentsOfFileSvn:path];
}


+ (instancetype) dataWithContentsOfFileSvn:(NSString *)path
{
    NSLog(@"dataWithContentsOfFile:%@", path);
    return  [[NSData alloc]  initWithContentsOfFileSvn:path];
}

- (instancetype) initWithContentsOfFileSvn:(NSString *)path options:(NSDataReadingOptions)readOptionsMask error:(NSError **)errorPtr
{
    return [self  initWithContentsOfFileSvn:path];
}

- (instancetype) initWithContentsOfFileSvn:(NSString *)path
{
    if (!path) {
        return nil;
    }
    
    NSData * result = nil;
    
    SvnFileHandle *fileHandle = [SvnFileHandle fileHandleForReadingAtPath:path];
    
    if (fileHandle) {
        result = [fileHandle readDataToEndOfFile];
        [fileHandle closeFile];
    }
    
    self = [self initWithData:result];
    
    
    return self;
}

- (BOOL) writeToFileSvn:(NSString *)path atomically:(BOOL)useAuxiliaryFile
{
    
    NSLog(@"writeToFile:%@", path);
    if (!path) {
        return NO;
    }
    
    SvnFileHandle *fileHandle = [SvnFileHandle fileHandleForWritingAtPath:path];
    
    if (fileHandle) {
        [fileHandle writeData:self];
        [fileHandle closeFile];
        return YES;
    }
    
    return NO;
}

@end


@implementation NSString(SvnString)

+ (instancetype) stringWithContentsOfFileSvn:(NSString *)path encoding:(NSStringEncoding)enc error:(NSError **)error
{
    NSLog(@"stringWithContentsOfFile:%@", path);
    return [[NSString alloc]  initWithContentsOfFileSvn:path encoding:enc error:error];
}

+ (instancetype) stringWithContentsOfFileSvn:(NSString *)path usedEncoding:(NSStringEncoding *)enc error:(NSError **)error
{
    NSLog(@"stringWithContentsOfFile:%@", path);
    return [[NSString alloc]  initWithContentsOfFileSvn:path usedEncoding:enc error:error];
   
}

- (BOOL) writeToFileSvn:(NSString *)path atomically:(BOOL)useAuxiliaryFile encoding:(NSStringEncoding)enc error:(NSError **)error
{
    NSLog(@"string writeToFile:%@", path);
    NSData * data = [self dataUsingEncoding:enc];
    
    if(!data)
    {
        return NO;
    }
    
    SvnFileHandle *fileHandle = [SvnFileHandle fileHandleForWritingAtPath:path];
    
    if (fileHandle) {
        [fileHandle writeData:data];
        [fileHandle closeFile];
        return YES;
    }
    
    return NO;
}


- (instancetype) initWithContentsOfFileSvn:(NSString *)path encoding:(NSStringEncoding)enc error:(NSError **)error
{
    if(!path)
    {
        return nil;
    }
    
    NSData * result = nil;
    
    SvnFileHandle *fileHandle = [SvnFileHandle fileHandleForReadingAtPath:path];
    
    if (fileHandle) {
        result = [fileHandle readDataToEndOfFile];
        
        [fileHandle closeFile];
    }
    
    return [self initWithData:result encoding:enc];
}


- (instancetype) initWithContentsOfFileSvn:(NSString *)path usedEncoding:(NSStringEncoding *)enc error:(NSError **)error
{
    if(!path)
    {
        return nil;
    }
    
    
    NSData * result = nil;
    
    SvnFileHandle *fileHandle = [SvnFileHandle fileHandleForReadingAtPath:path];
    
    if (fileHandle) {
        result = [fileHandle readDataToEndOfFile];
        [fileHandle closeFile];
    }
    
    *enc = [NSString  getStringEncodingSvn:result];
    
    return [self initWithData:result encoding:(*enc)];
}




+ (NSStringEncoding) getStringEncodingSvn:(NSData *)data
{
    static Byte utf8HeaderBytes[]    = { 0xEF, 0xBB, 0xBF };
    static Byte utf16leHeaderBytes[] = { 0xFF, 0xFE };
    static Byte utf16beHeaderBytes[] = { 0xFE, 0xFF };
    static Byte utf32leHeaderBytes[] = { 0xFF, 0xFE, 0x00, 0x00 };
    static Byte utf32beHeaderBytes[] = { 0x00, 0x00, 0xFE, 0xFF };
    
    NSData *utf8Header    = [NSData dataWithBytes:utf8HeaderBytes length:sizeof(utf8HeaderBytes)];
    NSData *utf16leHeader = [NSData dataWithBytes:utf16leHeaderBytes length:sizeof(utf16leHeaderBytes)];
    NSData *utf16beHeader = [NSData dataWithBytes:utf16beHeaderBytes length:sizeof(utf16beHeaderBytes)];
    NSData *utf32leHeader = [NSData dataWithBytes:utf32leHeaderBytes length:sizeof(utf32leHeaderBytes)];
    NSData *utf32beHeader = [NSData dataWithBytes:utf32beHeaderBytes length:sizeof(utf32beHeaderBytes)];
    
    if ([self doesData:data startWithHeader:utf32leHeader])
    {
        return NSUTF32LittleEndianStringEncoding;
    }
    else if ([self doesData:data startWithHeader:utf32beHeader])
    {
        return NSUTF32BigEndianStringEncoding;
    }
    else if ([self doesData:data startWithHeader:utf8Header])
    {
        return NSUTF8StringEncoding;
    }
    else if ([self doesData:data startWithHeader:utf16leHeader])
    {
        return NSUTF16LittleEndianStringEncoding;
    }
    else if ([self doesData:data startWithHeader:utf16beHeader])
    {
        return NSUTF16BigEndianStringEncoding;
    }
    return NSUTF8StringEncoding;
}


+ (BOOL)doesData:(NSData *)data startWithHeader:(NSData *)header
{
    if (data.length < header.length)
    {
        return NO;
    }
    NSData *firstBytes = [data subdataWithRange:NSMakeRange(0, header.length)];
    return [firstBytes isEqualToData:header];
}

@end