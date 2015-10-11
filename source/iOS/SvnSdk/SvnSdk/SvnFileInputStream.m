//
//  SvnFileStream.m
//  iDeskAPI
//
//  Created by yemingxing on 6/23/14.
//  Copyright (c) 2014 www.huawei.com. All rights reserved.
//

#import "SvnFileInputStream.h"

#import <objc/runtime.h>

#import "svn_define.h"
#import "svn_api.h"
#import "svn_file_api.h"
#import "svn_file_api_ex.h"


static const void *SvnRetainCallBack(CFAllocatorRef allocator, const void *value) { return CFRetain(value); }
static void SvnReleaseCallBack(CFAllocatorRef allocator, const void *value)       { CFRelease(value); }

static void SvnRunLoopPerformCallBack(void *info);

@interface SvnFileInputStream() {
    SVN_FILE_S* _ideskFileDes;
    BOOL bClose;
    
    CFRunLoopSourceRef _runLoopSource;
    CFReadStreamClientCallBack _clientCallBack;
    CFStreamClientContext _clientContext;
    CFOptionFlags _clientFlags;
    CFMutableSetRef _runLoopsSet;
    CFMutableDictionaryRef _runLoopsModes;
    NSStreamEvent _pendingEvents;
    NSStreamStatus _status;
    id<NSStreamDelegate> _delegate;
    NSUInteger _readOffset;

    NSError *_error;
}
@property (nonatomic, copy) NSString* path;
@end

@implementation SvnFileInputStream

+ (id)inputStreamWithFileAtPath:(NSString *)path
{
    SvnFileInputStream* stream = [[SvnFileInputStream alloc] initWithFileAtPath:path];
    return stream;
}

+ (id)inputStreamWithURL:(NSURL *)url
{
    return [SvnFileInputStream inputStreamWithFileAtPath:url.path];
}

- (void) open
{
    if (_status != NSStreamStatusNotOpen) {
        return;
    }
    [self setStatus:NSStreamStatusOpening];
    if (!_path) {
        [self setError:[NSError errorWithDomain:@"Open file error!" code:NSFileNoSuchFileError userInfo:nil]];
        return;
    }
    _ideskFileDes = svn_fopen([_path UTF8String], "r+");
    if (!_ideskFileDes) {
        [self setError:[NSError errorWithDomain:@"Open file error!" code:NSFileNoSuchFileError userInfo:nil]];
        return;
    }
    
    NSNumber * currentOffset = (NSNumber *)[self propertyForKey: NSStreamFileCurrentOffsetKey];
    
    if(currentOffset)
    {
        _readOffset = [currentOffset unsignedIntegerValue];
    }
    
    if (_readOffset) {
        NSLog(@"Seek in file %tu-%tu", _readOffset, _lengthToRead ? _lengthToRead : _totalLength.longLongValue);
        
        int ret = svn_fseek(_ideskFileDes, _readOffset, SVN_SEEK_SET);
        if (0 != ret) {
            [self setError:[NSError errorWithDomain:@"Open file error!" code:NSFileReadUnknownError userInfo:nil]];
            NSLog(@"Failed to seek!");
            return;
        }
    }
    
//    unsigned long fileSize = svn_getsize([_path UTF8String]);
//    
//    _totalLength = [[NSNumber alloc] initWithUnsignedInteger:fileSize];
    _totalReaded = [[NSNumber alloc] initWithUnsignedInteger:0];
    [self setStatus:NSStreamStatusOpen];
    [self enqueueEvent:NSStreamEventOpenCompleted];
    if (_totalLength.unsignedIntegerValue > 0) {
        [self enqueueEvent:NSStreamEventHasBytesAvailable];
    } else {
        [self enqueueEvent:NSStreamEventEndEncountered];
    }
}

- (void) close
{
    if (!bClose) {
        bClose = YES;
        svn_fclose(_ideskFileDes);
        [self unscheduleFromAllRunLoops];
        [self setStatus:NSStreamStatusClosed];
    }
    
}

- (NSError*) streamError {
    return _error;
}

- (id<NSStreamDelegate>)delegate {
    return _delegate;
}

- (void)setDelegate:(id<NSStreamDelegate>)delegate {
    _delegate = delegate;
    if (!_delegate) {
        _delegate = self;
    }
}

- (id)propertyForKey:(NSString *)key {
    if (![key isEqualToString:NSStreamFileCurrentOffsetKey]) {
        return nil;
    }
    return [NSNumber numberWithUnsignedInteger: _readOffset];
}

- (BOOL)setProperty:(id)property forKey:(NSString *)key {
    
    NSLog(@"setProperty for _readOffset");
    
    if (![key isEqualToString:NSStreamFileCurrentOffsetKey]) {
        return NO;
    }
    if (![property isKindOfClass:[NSNumber class]]) {
        return NO;
    }
    
    NSNumber * number = (NSNumber *) property;
    
//    long requestedOffest = [number unsignedIntegerValue] ;
//    if (requestedOffest < 0) {
//        return NO;
//    }
    _readOffset = [number unsignedIntegerValue];
    NSLog(@"set %tu for _readOffset", _readOffset);
    return YES;
}

- (id)initWithFileAtPath:(NSString *)path
{
    if (!path) {
        return nil;
    }
    
    if (self = [super init]) {
        self.path = path;
        
        unsigned long fileSize = svn_getsize([_path UTF8String]);
        
        _totalLength = [[NSNumber alloc] initWithUnsignedInteger:fileSize];
        
        
        CFRunLoopSourceContext runLoopSourceContext = {
            0, (__bridge void *)(self), NULL, NULL, NULL, NULL, NULL, NULL, NULL, SvnRunLoopPerformCallBack
        };
        _runLoopSource = CFRunLoopSourceCreate(NULL, 0, &runLoopSourceContext);
        _status = NSStreamStatusNotOpen;
        _delegate = self;
        _clientCallBack = NULL;
        _clientContext = (CFStreamClientContext) { 0 };
        CFSetCallBacks runLoopsSetCallBacks = {
            0, NULL, NULL, NULL, CFEqual, CFHash // CFRunLoop retains CFStream, so we will not.
        };
        _runLoopsSet = CFSetCreateMutable(NULL, 0, &runLoopsSetCallBacks);
        CFDictionaryKeyCallBacks runLoopsModesKeyCallBacks = {
            0, NULL, NULL, NULL, CFEqual, CFHash
        };
        CFDictionaryValueCallBacks runLoopsModesValueCallBacks = {
            0, SvnRetainCallBack, SvnReleaseCallBack, NULL, CFEqual
        };
        _runLoopsModes = CFDictionaryCreateMutable(NULL, 0, &runLoopsModesKeyCallBacks, &runLoopsModesValueCallBacks);
        _shouldNotifyCoreFoundationAboutStatusChange = YES;

    }
    return self;
}

- (void) dealloc
{
    self.delegate = nil;
    if (!bClose) {
        [self close];
    }


    
    if (_clientContext.release) {
        _clientContext.release(_clientContext.info);
    }
    CFRelease(_runLoopSource);
    CFRelease(_runLoopsSet);
    CFRelease(_runLoopsModes);

}

- (BOOL)isOpen {
    return (_status != NSStreamStatusNotOpen &&
            _status != NSStreamStatusOpening &&
            _status != NSStreamStatusClosed &&
            _status != NSStreamStatusError);
    
}

- (NSInteger)read:(uint8_t *)buffer maxLength:(NSUInteger)len
{
    if (![self isOpen]) {
        return -1;
    }
    NSInteger _currentReaded = 0;
    NSInteger _tryToRead = len;
    NSUInteger totalRead = _totalReaded.unsignedIntegerValue;
    NSUInteger totalExpeted = _lengthToRead ? _lengthToRead : _totalLength.unsignedIntegerValue;
    _tryToRead = MIN(len, totalExpeted - totalRead);
    if (_tryToRead < 0) {
        _currentReaded = 0;
    } else {
        _currentReaded = svn_fread(buffer, sizeof(uint8_t), _tryToRead, _ideskFileDes);
        
    }
    _totalReaded = [[NSNumber alloc] initWithUnsignedInteger:(_currentReaded + totalRead)];
    if ([self hasBytesAvailable]) {
        [self enqueueEvent:NSStreamEventHasBytesAvailable];
    } else {
        [self enqueueEvent:NSStreamEventEndEncountered];
    }
    
    NSLog(@"SvnFileInputStream readed:%tu", _currentReaded);
    return _currentReaded;
}

- (BOOL)getBuffer:(uint8_t **)buffer length:(NSUInteger *)len
{
    return NO;
}

- (BOOL)hasBytesAvailable
{
    NSUInteger totalExpeted = _lengthToRead ? MIN(_lengthToRead, _totalLength.unsignedIntegerValue) : _totalLength.unsignedIntegerValue;
    //30212096, _totalReaded:0, expeted:4267678
    
    
    
    NSLog(@"SvnFileInputStream %p _readOffset:%tu, _totalReaded:%tu, expeted:%tu", self, _readOffset, _totalReaded.unsignedIntegerValue, MIN(totalExpeted, _totalLength.unsignedIntegerValue));
    
    
    return (_totalReaded.unsignedIntegerValue  + _readOffset) < _totalLength.unsignedIntegerValue && _totalReaded.unsignedIntegerValue < totalExpeted;
}

- (void)setStatus:(NSStreamStatus)aStatus {
    _status = aStatus;
}

- (void)setError:(NSError *)theError {
    [self setStatus:NSStreamStatusError];
    [self enqueueEvent:NSStreamEventErrorOccurred];
    _error = theError;
}

- (void)enqueueEvent:(NSStreamEvent)event {
    _pendingEvents |= event;
    CFRunLoopSourceSignal(_runLoopSource);
    [self enumerateRunLoopsUsingBlock:^(CFRunLoopRef runLoop) {
        CFRunLoopWakeUp(runLoop);
    }];
}

- (NSStreamEvent)dequeueEvent {
    if (_pendingEvents == NSStreamEventNone) {
        return NSStreamEventNone;
    }
    NSStreamEvent event = 1UL << __builtin_ctz(_pendingEvents);
    _pendingEvents ^= event;
    return event;
}

- (void)streamEventTrigger {
    if (_status == NSStreamStatusClosed) {
        return;
    }
    NSStreamEvent event = [self dequeueEvent];
    while (event != NSStreamEventNone) {
        if ([_delegate respondsToSelector:@selector(stream:handleEvent:)]) {
            [_delegate stream:self handleEvent:event];
        }
        if (_clientCallBack && (event & _clientFlags) && _shouldNotifyCoreFoundationAboutStatusChange) {
            _clientCallBack((__bridge CFReadStreamRef)self, (CFStreamEventType)event, _clientContext.info);
        }
        event = [self dequeueEvent];
    }
}

- (void)enumerateRunLoopsUsingBlock:(void (^)(CFRunLoopRef runLoop))block {
    CFIndex runLoopsCount = CFSetGetCount(_runLoopsSet);
    if (runLoopsCount > 0) {
        CFTypeRef runLoops[runLoopsCount];
        CFSetGetValues(_runLoopsSet, runLoops);
        for (CFIndex i = 0; i < runLoopsCount; ++i) {
            block((CFRunLoopRef)runLoops[i]);
        }
    }
}

- (void)addMode:(CFStringRef)mode forRunLoop:(CFRunLoopRef)runLoop {
    CFMutableSetRef modes = NULL;
    if (!CFDictionaryContainsKey(_runLoopsModes, runLoop)) {
        CFSetCallBacks modesSetCallBacks = {
            0, SvnRetainCallBack, SvnReleaseCallBack, NULL, CFEqual, CFHash
        };
        modes = CFSetCreateMutable(NULL, 0, &modesSetCallBacks);
        CFDictionaryAddValue(_runLoopsModes, runLoop, modes);
    } else {
        modes = (CFMutableSetRef)CFDictionaryGetValue(_runLoopsModes, runLoop);
    }
    CFStringRef modeCopy = CFStringCreateCopy(NULL, mode);
    CFSetAddValue(modes, modeCopy);
    CFRelease(modeCopy);
}

- (void)removeMode:(CFStringRef)mode forRunLoop:(CFRunLoopRef)runLoop {
    if (!CFDictionaryContainsKey(_runLoopsModes, runLoop)) {
        return;
    }
    CFMutableSetRef modes = (CFMutableSetRef)CFDictionaryGetValue(_runLoopsModes, runLoop);
    CFSetRemoveValue(modes, mode);
}

#pragma mark - NSObject

+ (BOOL)resolveInstanceMethod:(SEL)selector {
    NSString *name = NSStringFromSelector(selector);
    if ([name hasPrefix:@"_"]) {
        name = [name substringFromIndex:1];
        SEL aSelector = NSSelectorFromString(name);
        Method method = class_getInstanceMethod(self, aSelector);
        if (method) {
            class_addMethod(self,
                            selector,
                            method_getImplementation(method),
                            method_getTypeEncoding(method));
            return YES;
        }
    }
    return [super resolveInstanceMethod:selector];
}

- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode {
    [self scheduleInCFRunLoop:[aRunLoop getCFRunLoop] forMode:(CFStringRef) mode];
}

- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode {
    [self unscheduleFromCFRunLoop:[aRunLoop getCFRunLoop] forMode:(CFStringRef) mode];
}

- (NSStreamStatus)streamStatus {
    if (_status == NSStreamStatusError && !_shouldNotifyCoreFoundationAboutStatusChange) {
        return NSStreamStatusOpen;
    }
    return _status;
}

- (void)scheduleInCFRunLoop:(CFRunLoopRef)runLoop forMode:(CFStringRef)mode {
    CFSetAddValue(_runLoopsSet, runLoop);
    [self addMode:mode forRunLoop:runLoop];
    CFRunLoopAddSource(runLoop, _runLoopSource, mode);
}

- (void)unscheduleFromCFRunLoop:(CFRunLoopRef)runLoop forMode:(CFStringRef)mode {
    CFRunLoopRemoveSource(runLoop, _runLoopSource, mode);
    [self removeMode:mode forRunLoop:runLoop];
    CFSetRemoveValue(_runLoopsSet, runLoop);
}

- (void)unscheduleFromAllRunLoops {
    [self enumerateRunLoopsUsingBlock:^(CFRunLoopRef runLoop) {
        CFMutableSetRef runLoopModesSet = (CFMutableSetRef)CFDictionaryGetValue(_runLoopsModes, runLoop);
        CFIndex runLoopModesCount = CFSetGetCount(runLoopModesSet);
        if (runLoopModesCount > 0) {
            CFTypeRef runLoopModes[runLoopModesCount];
            CFSetGetValues(runLoopModesSet, runLoopModes);
            for (CFIndex j = 0; j < runLoopModesCount; ++j) {
                [self unscheduleFromCFRunLoop:runLoop forMode:(CFStringRef)runLoopModes[j]];
            }
        }
    }];
}

- (BOOL)setCFClientFlags:(CFOptionFlags)flags
                callback:(CFReadStreamClientCallBack)callBack
                 context:(CFStreamClientContext *)context {
    if (context && context->version != 0) {
        return NO;
    }
    if (_clientContext.release) {
        _clientContext.release(_clientContext.info);
    }
    _clientContext = (CFStreamClientContext) { 0 };
    if (context) {
        _clientContext = *context;
    }
    if (_clientContext.retain) {
        _clientContext.retain(_clientContext.info);
    }
    _clientFlags = flags;
    _clientCallBack = callBack;
    return YES;
}

#pragma mark - Core Foundation callbacks implementations

void SvnRunLoopPerformCallBack(void *info) {
    SvnFileInputStream *stream = (__bridge SvnFileInputStream *)info;
    [stream streamEventTrigger];
}
@end
