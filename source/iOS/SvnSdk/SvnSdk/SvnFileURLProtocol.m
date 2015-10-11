//
//  SvnFileURLProtocol.m
//  iDeskAPI
//
//  Created by yemingxing on 8/13/14.
//  Copyright (c) 2014 www.huawei.com. All rights reserved.
//

#import "SvnFileURLProtocol.h"
#import "SvnFileInputStream.h"
#import "SvnFileHandle.h"
#import <UIKit/UIKit.h>

@interface SvnFileURLProtocol() {
    BOOL _stopLoading;
    NSUInteger _offset;
    NSUInteger _length;
}
@property (nonatomic, retain) SvnFileInputStream* inputStream;
@end

static inline NSString * contentTypeForPathExtension(NSString *extension) {
#ifdef __UTTYPE__
    NSString *UTI = (__bridge_transfer NSString *)UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, (__bridge CFStringRef)extension, NULL);
    NSString *contentType = (__bridge_transfer NSString *)UTTypeCopyPreferredTagWithClass((__bridge CFStringRef)UTI, kUTTagClassMIMEType);
    if (!contentType) {
        return @"application/octet-stream";
    } else {
        return contentType;
    }
#else
#pragma unused (extension)
    return @"application/octet-stream";
#endif
}

static inline BOOL isTextFile(NSString* extension) {
#ifdef __UTTYPE__
    NSString *UTI = (__bridge_transfer NSString *)UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, (__bridge CFStringRef)extension, NULL);
    return  UTTypeConformsTo(UTI, kUTTypeText);
#endif
    return NO;
}

@implementation SvnFileURLProtocol

+(BOOL) canInitWithRequest:(NSURLRequest *)request {
    if (request.URL && [request.URL.scheme compare:@"file" options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        return YES;
    }
    return NO;
}

+ (NSURLRequest *)canonicalRequestForRequest:(NSURLRequest *)request {
    return request;
}

- (NSCachedURLResponse *)cachedResponse {
    return nil;
}

- (id)initWithRequest:(NSURLRequest *)request cachedResponse:(NSCachedURLResponse *)cachedResponse client:(id < NSURLProtocolClient >)client {
    if (self = [super initWithRequest:request cachedResponse:cachedResponse client:client]) {
        NSLog(@"SvnFileURLProtocol: HeaderFields:%@", request.allHTTPHeaderFields);
        //NSLog(@"SvnFileURLProtocol: ServiceType:%d", request.networkServiceType);
        NSString* range = [request.allHTTPHeaderFields objectForKey:@"Range"];
        if (range) {
            NSArray* tokens = [range componentsSeparatedByString:@"="];
            if (tokens.count == 2) {
                range = [tokens lastObject];
                tokens = [range componentsSeparatedByString:@"-"];
                if (tokens.count == 2) {
                    NSString* n1 = [tokens firstObject];
                    NSString* n2 = [tokens lastObject];
                    _offset = [n1 integerValue];
                    _length = [n2 integerValue];
                }
            }
            
        }
    }
    return self;
}

- (void)startLoading {
    NSURLRequest* request = [self request];
    NSString* path = request.URL.path;
//    NSString* parameterString = request.URL.parameterString;
    id<NSURLProtocolClient> client = [self client];
    self.inputStream = [[SvnFileInputStream alloc] initWithFileAtPath:path];
    if (!_inputStream) {
        [client URLProtocol:self didFailWithError:[NSError errorWithDomain:@"Open file error!" code:0 userInfo:@{@"Info":[NSString stringWithFormat:@"Failed to load file :%@", path]}]];
        return;
    } else {
        NSString* pathExtension = [path stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        NSString* contentType = contentTypeForPathExtension(pathExtension);
        NSString* textEncodingName = @"UTF-8";
        NSInteger totalLength = _inputStream.totalLength.integerValue;
        totalLength = totalLength ? totalLength : NSURLResponseUnknownLength;
        NSURLResponse* response = nil;
        if (isTextFile(pathExtension)) {
            SvnFileHandle* handle = [SvnFileHandle fileHandleForReadingAtPath:path];
            
            if(handle)
            {
                NSData* data = [handle readDataOfLength:4];
                if (data) {
                    textEncodingName = [NSString localizedNameOfStringEncoding:[SvnFileURLProtocol getStringEncoding:data]];
                }
                
                [handle closeFile];
            }
          
        }
        response = [[NSURLResponse alloc] initWithURL:request.URL MIMEType:contentType expectedContentLength: (_length ? _length : totalLength) textEncodingName:textEncodingName];
        
        [client URLProtocol:self didReceiveResponse:response cacheStoragePolicy:NSURLCacheStorageNotAllowed];
    }
    _inputStream.delegate = self;
    if (_offset) {
        [_inputStream setProperty:@(_offset) forKey:NSStreamFileCurrentOffsetKey];
    }
    if (_length) {
        _inputStream.lengthToRead = _length;
    }
    [_inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [_inputStream open];
    
    NSLog(@"SvnFileURLProtocol open file:%@", path);
}

- (void)stopLoading {
    if (_inputStream.streamStatus != NSStreamStatusClosed) {
        [_inputStream close];
    }
}

- (void) stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode {
    if (aStream != _inputStream) {
        return;
    }
    uint8_t buffer[1024 * 16] = {0};
    id<NSURLProtocolClient> client = [self client];
    NSInteger readed = 0;
    switch (eventCode) {
        case NSStreamEventHasBytesAvailable:
            while ([_inputStream hasBytesAvailable]) {
                readed = [_inputStream read:buffer maxLength:1024 * 16];
                [client URLProtocol:self didLoadData:[NSData dataWithBytes:buffer length:readed]];
                NSLog(@"SvnFileURLProtocol stream reading..");
            }
            
            break;
        case NSStreamEventEndEncountered:
            [client URLProtocolDidFinishLoading:self];
            NSLog(@"SvnFileURLProtocol stream finisihed.");
            break;
        case NSStreamEventErrorOccurred:
            [client URLProtocol:self didFailWithError:_inputStream.streamError];
            NSLog(@"SvnFileURLProtocol stream error occured!");
            break;
        default:
            break;
    }
}

+ (NSStringEncoding)getStringEncoding:(NSData *)data
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
