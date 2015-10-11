//
//  SvnSocketInputStream.m
//  SvnSdk
//
//  Created by l00174413 on 13-6-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import "SvnSocketStream.h"

#import "svn_define.h"
#import "svn_api.h"
#import "svn_socket_api.h"
#import "svn_socket_err.h"

@interface NSStream (SvnPrivate)


- (void)_sendEvent:(NSStreamEvent) event;
- (void)_sendError:(NSError *) error;
- (void)_sendErrorWithDomain:(NSString *) domain code:(int) code;
- (void)_sendErrorWithDomain:(NSString *) domain code:(int) code userInfo:(NSDictionary*)dict;
@end

@implementation NSStream (SvnPrivate)
- (void) _sendEvent:(NSStreamEvent) event
{
   [[self delegate] stream:self handleEvent:event];
}


- (void) _sendErrorWithDomain:(NSString *) domain code:(int) code
{
    [self _sendError:[NSError errorWithDomain:domain code:code userInfo:nil]];
}


- (void) _sendErrorWithDomain:(NSString *) domain code:(int) code userInfo:(NSDictionary*)dict
{
    [self _sendError:[NSError errorWithDomain:domain code:code userInfo:dict]];
}

@end



@interface SvnSocketInputStream(Private)
- (void) waitForData;
@end

@implementation SvnSocketInputStream
{
    NSStreamStatus streamStatus;
    id <NSStreamDelegate> delegate;
    
    NSError* streamError;

}

- (void)_sendError:(NSError*) err
{
    streamStatus = NSStreamStatusError;
    //streamError = err;
    [delegate stream:self handleEvent:NSStreamEventErrorOccurred];
}

- (SvnSocketInputStream *) initWithSvnSocketHandle:(long) handle
{
    
    self = [self init];
    if(self)
    {
        fd = handle;
    }
    
    return self;
    
}

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
        streamStatus = NSStreamStatusNotOpen;
        
        //[self setDelegate:self];
    }
    
    return self;
}


#pragma mark - NSStream subclass overrides


                     
- (void) _sendDidOpenEvent
{
    [self _sendEvent:NSStreamEventOpenCompleted];
}

- (void)open {
    if(streamStatus != NSStreamStatusNotOpen)
    {
        [self _sendErrorWithDomain:@"already open" code:0];
        return;
    }
    streamStatus = NSStreamStatusOpen;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [self waitForData];
    });
    
}

- (void)close {
    streamStatus = NSStreamStatusClosed;
    //NSLog(@"SvnSocketInputStream closed.");
}

- (id<NSStreamDelegate>)delegate {
    return delegate;
}

- (void)setDelegate:(id<NSStreamDelegate>)aDelegate {
    delegate = aDelegate;
//    if (delegate == nil) {
//    	delegate = self;
//    }
}


- (id)propertyForKey:(NSString *)key {
    return nil;
}

- (BOOL)setProperty:(id)property forKey:(NSString *)key {
    return NO;
}


- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode {
    
    // Nothing to do here, because this stream does not need a run loop to produce its data.
}

- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode {
    // Nothing to do here, because this stream does not need a run loop to produce its data.
}


- (NSStreamStatus)streamStatus {
    return streamStatus;
}

- (NSError *)streamError {
    return streamError;
}


#pragma mark - NSInputStream subclass overrides

- (NSInteger)read:(uint8_t *)buffer maxLength:(NSUInteger)len {
	
	//for (NSUInteger i=0; i<len; ++i) {
		//buffer[i] = arc4random() & 0x00000000ffffffff;
	//}
    
    long count = svn_recv(fd, buffer, len, 0);
    
    if(count == 0)
    {
        
//        count = svn_recv(fd, buffer, len, 0);
//        if(count == 0)
//        {
            streamStatus = NSStreamStatusAtEnd;
            [self _sendEvent:NSStreamEventEndEncountered];
//        }
        
        
    }

	return count;
}

- (BOOL)getBuffer:(uint8_t **)buffer length:(NSUInteger *)len {
	// Not appropriate for this kind of stream; return NO.
	return NO;
}

- (BOOL)hasBytesAvailable {
	// There are always bytes available.
	return YES;
}

- (void)waitForData
{
    //NSLog(@"SvnSocketInputStream waitForData");
    svn_fd_set readfd = {0};
    long maxfd = fd + 1;
    int res = -1;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    while(NSStreamStatusOpen == streamStatus)
    {
        SVN_FD_ZERO(&readfd);
        SVN_FD_SET(fd, &readfd);
        res = svn_select(maxfd, &readfd, NULL, NULL, &timeout);
        //NSLog(@"SvnSocketInputStream svn_select returns %d", res);
        if(res > 0)
        {
            if(SVN_FD_ISSET(fd, &readfd))
            {
                //NSLog(@"SvnSocketInputStream SVN_FD_ISSET for readfd");
                
                [self _sendEvent:NSStreamEventHasBytesAvailable]; 

            }
        }
        else if(res < 0)
        { 
            //[self _sendError:@"socket error occurred!"];
            streamStatus = NSStreamStatusError;
            [self _sendEvent:NSStreamEventErrorOccurred];
        }
//        else
//        {
//            streamStatus = NSStreamStatusAtEnd;
//            [self _sendEvent:NSStreamEventEndEncountered];
//            
//        }
        usleep(1000);
    }
    
    //NSLog(@"SvnSocketInputStream waitForData end.");
}


@end



//
//  SvnSocketOutputStream
//  

@interface SvnSocketOutputStream(Private)
- (void) waitForData;
@end


@implementation SvnSocketOutputStream

{
    NSStreamStatus streamStatus;
    id <NSStreamDelegate> delegate;
    NSError* streamError; 
    
}


- (void)_sendError:(NSError*) err
{
    streamStatus = NSStreamStatusError;
    streamError = err;
    [[self delegate] stream:self handleEvent:NSStreamEventErrorOccurred];
}


- (SvnSocketOutputStream *) initWithSvnSocketHandle:(long) handle
{
    
    self = [self init];
    if(self)
    {
        fd = handle;
    }
    return self;
    
}

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
        streamStatus = NSStreamStatusNotOpen;
        
        [self setDelegate:self];
    }
    
    return self;
}


#pragma mark - NSStream subclass overrides

- (void) _sendDidOpenEvent
{
    [self _sendEvent:NSStreamEventOpenCompleted];
}

- (void)open {
    if(streamStatus != NSStreamStatusNotOpen)
    {
        [self _sendErrorWithDomain:@"already open" code:0];
        return;
    }
    streamStatus = NSStreamStatusOpen;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [self waitForData];
    });}

- (void)close {
    streamStatus = NSStreamStatusClosed;
}

- (id<NSStreamDelegate>)delegate {
    return delegate;
}

- (void)setDelegate:(id<NSStreamDelegate>)aDelegate {
    delegate = aDelegate;
    if (delegate == nil) {
    	delegate = self;
    }
}

- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode {
    
    // Nothing to do here, because this stream does not need a run loop to produce its data.
}

- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode {
    // Nothing to do here, because this stream does not need a run loop to produce its data.
}


- (id)propertyForKey:(NSString *)key {
    return nil;
}

- (BOOL)setProperty:(id)property forKey:(NSString *)key {
    return NO;
}

- (NSStreamStatus)streamStatus {
    return streamStatus;
}

- (NSError *)streamError {
    return streamError;
}


#pragma mark - NSInputStream subclass overrides

- (NSInteger)write:(const uint8_t *)buffer maxLength:(NSUInteger)len
{
	
	//for (NSUInteger i=0; i<len; ++i) {
    //buffer[i] = arc4random() & 0x00000000ffffffff;
	//}
    
    long count = svn_send(fd, buffer, len, 0);
    //NSLog(@"SvnSocketOutputStream send %d bytes", count);
	
	
	return count;
}

- (BOOL)getBuffer:(uint8_t **)buffer length:(NSUInteger *)len {
	// Not appropriate for this kind of stream; return NO.
	return NO;
}

- (BOOL)hasBytesAvailable {
	// There are always bytes available.
	return YES;
}


- (void)waitForData
{
    //NSLog(@"SvnSocketOutputStream waitForData");
    svn_fd_set writefd = {0};
    long maxfd = fd + 1;
    int res = -1;
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    while(NSStreamStatusOpen == streamStatus)
    {
        SVN_FD_ZERO(&writefd);
        SVN_FD_SET(fd, &writefd);
        res = svn_select(maxfd, NULL, &writefd, NULL, &timeout);
        //NSLog(@"SvnSocketOutputStream svn_select returns %d", res); 
 
        if(res > 0)
        {
            if(SVN_FD_ISSET(fd, &writefd))
            {
                //NSLog(@"SvnSocketOutputStream SVN_FD_ISSET for writefd");
                [self _sendEvent:NSStreamEventHasSpaceAvailable];
                
                //dispatch_async(dispatch_get_main_queue(), ^{
                    //if(streamStatus == NSStreamStatusOpen)
                    //{
                        //[self _sendEvent:NSStreamEventHasSpaceAvailable];
                    //}
                    
                //});
            }
        }
        else  if(res < 0)
        { 
            streamStatus = NSStreamStatusError;
            [self _sendEvent:NSStreamEventErrorOccurred];
            //[self _sendError:@"socket error occurred!"];
        }
        usleep(1000);
    }
    //NSLog(@"SvnSocketOutputStream waitForData end.");
}

@end
