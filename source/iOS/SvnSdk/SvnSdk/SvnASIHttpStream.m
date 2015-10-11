//
//  SvnASIHttpStream.m
//  SvnSdk
//
//  Created by l00174413 on 13-8-2.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import "SvnASIHttpStream.h"

#import "svn_define.h"
#import "svn_api.h"
#import "svn_socket_api.h"
#import "svn_socket_err.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#import "SvnDNSResolve.h"

//volatile int asi_parsedIP[SVN_MAX_URL_NUM];
//
//void g_ASIParseURLCallback(unsigned long ulIP[SVN_MAX_URL_NUM], void* parsedIp)
//{
//    int* result = (int *)parsedIp;
//    if(ulIP != NULL && result != NULL)
//    {
//        result[0] = ulIP[0];
//        NSLog(@"parseIP result:%d", result[0]);
//    }
//}

@interface  SvnASIHttpStream()
// internal methods

- (void)_sendEvent:(NSStreamEvent)event;

- (BOOL)connectToServer;	// connect to server

- (void)endOfUseability;	// connection became invalid

- (void)startLoadingNextRequest;

- (CFHTTPMessageRef)readHttpResponseHeader;

- (void)processHeaderLine:(NSString *) line;

- (void)headersReceived;

- (void)bodyReceived;

- (void)trailerReceived;
@end

@implementation SvnASIHttpStream
{
    NSStreamStatus streamStatus;
    id <NSStreamDelegate> delegate;
    
    NSError* streamError;
}

@synthesize originRequest;
@synthesize bodyStream;

+ (NSString *)pathForURL:(NSURL *)url
{
	NSString *path=[url path];
    NSString *params = [url parameterString];
    NSString *query = [url query];
    NSString *fragment = [url fragment];
//    NSString *absoluteurl= [url absoluteString];
//    
//    if([absoluteurl hasSuffix:@"/"])
//    {
//        return [path stringByAppendingString:@"/"];
//    }
    
    NSMutableString * result = [[NSMutableString alloc] initWithCapacity:128];
    
    [result appendString:path];
    if(params)
    {
        [result appendFormat:@";%@", params];
    }
    
    if(query)
    {
        [result appendFormat:@"?%@", query];
    }
    
    if(fragment)
    {
        [result appendFormat:@"#%@", fragment];
    }
    
    //NSLog(@"path:%@, query:%@, fragment:%@, relativePath:%@", path, query, fragment, relativePath);
    
    return result;
}


+ (NSString *)getHostArress:(NSString *)host
{
    if(host == nil || [host length] == 0)
    {
        return  nil;
    }
    
    BOOL bIncludeLetter = NO;
    
    for (int i = 0; i< [host length]; i++)
    {
        unichar c = [host characterAtIndex:i];
        if( (c >='a' && c<='z') || (c>='A' && c<='Z'))
        {
            bIncludeLetter = YES;
        }
    }
    
    if(bIncludeLetter)
    {
        //int asi_parsedIP[1] = {0};
        int iAddress = ASYNC_DNS_ParseURL([host cStringUsingEncoding:NSASCIIStringEncoding], [host length]);
        
        //int iAddress = SVN_API_ParseURL([host cStringUsingEncoding:NSASCIIStringEncoding], [host length], &g_ASIParseURLCallback, asi_parsedIP);
        if(iAddress != 0)
        {
//            int count = 0;
//            while(asi_parsedIP == 0 && count < 15*20)//域名解析的最大时间300*50ms = 15s
//            {
//                count++;
//                usleep(50000);
//            }
            int part1 = (iAddress &0x000000FF);
            int part2 = (iAddress &0x0000FF00) >>8;
            int part3 = (iAddress &0x00FF0000) >>16;
            int part4 = (iAddress &0xFF000000) >>24;
            return [NSString stringWithFormat:@"%d.%d.%d.%d",part1, part2, part3, part4];
        }
        
        return nil;        
    }
    
    return host;
    
}

+ (NSInputStream *)getReadStreamForStreamedHttpRequest:(CFHTTPMessageRef)request stream:(NSInputStream *) postStream
{
    CFHTTPMessageRef requestCopy = CFHTTPMessageCreateCopy(kCFAllocatorDefault, request);
    if(!requestCopy)
    {
        return nil;
    }
    
    
    SvnASIHttpStream *stream = [[SvnASIHttpStream alloc] init];
    
    if(!stream)
    {
        return nil;
    }
    
    stream.originRequest = requestCopy;

    stream.bodyStream = postStream;
    
    if(![stream connectToServer])
    {
        return nil;
    }
    
    return stream;
    
}


+ (NSInputStream *)getReadStreamForHttpRequest:(CFHTTPMessageRef)request
{
    CFHTTPMessageRef requestCopy = CFHTTPMessageCreateCopy(kCFAllocatorDefault, request);
    if(!requestCopy)
    {
        return nil;
    }
    
    
    SvnASIHttpStream *stream = [[SvnASIHttpStream alloc] init];
    
    if(!stream)
    {
        return nil;
    }
    
    stream.originRequest = requestCopy;
    
    if(![stream connectToServer])
    {
        return nil;
    }
    
    return stream;
}

- (void)_sendEvent:(NSStreamEvent)event
{
    [[self delegate] stream:self handleEvent:event];
}


- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
        streamStatus = NSStreamStatusNotOpen;
        _bodySended = 0;
        
        //[self setDelegate:self];
    }
    
    return self;
}

- (void)dealloc
{
    
    [self endOfUseability];
    //[super dealloc];
}

#pragma mark - NSStream subclass overrides



- (void)open 
{
    if(streamStatus != NSStreamStatusNotOpen)
    {
        //[self _sendErrorWithDomain:@"already open" code:0];
        return;
    }
    streamStatus = NSStreamStatusOpen;

    [_inputStream setDelegate:self];
    [_outputStream setDelegate:self];
    
    [_inputStream open];
    [_outputStream open];
    [self startLoadingNextRequest];
    
}

- (void)close {
    streamStatus = NSStreamStatusClosed;
    //NSLog(@"SvnSocketInputStream closed.");
    [self endOfUseability];
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

- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode {
    
    // Nothing to do here, because this stream does not need a run loop to produce its data.
}

- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode {
    // Nothing to do here, because this stream does not need a run loop to produce its data.
}


- (id)propertyForKey:(NSString *)key {
    NSString *propertyString = (NSString *)kCFStreamPropertyHTTPRequestBytesWrittenCount;
    if([key isEqualToString: propertyString])
    {
        return [NSNumber numberWithLongLong:_bodySended] ;
    }
    //if(kCFStreamPropertyHTTPRequestBytesWrittenCount)
    
    return [super propertyForKey:key];
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

- (NSInteger)read:(uint8_t *)buffer maxLength:(NSUInteger)len
{
    if(_isChunked && _bodyReceived)
    {
        return 0;
    }
    if(_isChunked && _chunkLength == 0)
    { // reading chunk size

        uint8_t ch[1];
        int read = 0;
        while(_chunkLength == 0)
        {
            read = [_inputStream read:ch maxLength:1];
            
#if 0
            NSLog(@"will process %02x into %@, read=%d", ch[0], _headerLine, read);
#endif
            
            if(ch[0] == '\r')
            {
                continue;
            }
            
            if(ch[0] == '\n')
            {
                if([_headerLine length] > 0)
                { // there should follow a CRLF after the body resulting in a empty line
                    NSScanner *sc=[NSScanner scannerWithString:_headerLine];

                    _chunkLength=0;
                    if(![sc scanHexInt:&_chunkLength])	// is hex coded (we even ignore an optional 0x)
                    {
                        NSLog(@"invalid chunk length %@", _headerLine);
                        streamError = [NSError errorWithDomain:@"invalid chunk length" code:0 userInfo:0];
                        [self _sendEvent:NSStreamEventErrorOccurred];
                        [self endOfUseability];
                        return -1;
                    }
                    
#if 0
                    NSLog(@"chunk length=%@, value:%d", _headerLine, _chunkLength);
#endif
                    
                    // may be followed by ; name=var
                    if(_chunkLength == 0)
                    {
                        _bodyReceived = YES;
                        double delayInSeconds = 0.1;
                        dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
                        dispatch_after(popTime, dispatch_get_current_queue(), ^(void){
                            [self bodyReceived];
                        });
                        //[self bodyReceived];	// done reading body - continue with trailer
                        return 0;
                    }
                    [_headerLine setString:@""];	// has been processed
                }
            }
            else
            {
                [_headerLine appendFormat:@"%c", ch[0]&0xff];
            }
            
        }
        
    }
    
    

    unsigned maxLength = len;


    if(_isChunked && _chunkLength < maxLength)
    {
        maxLength=_chunkLength;	// limit to current chunk size
    }
    else if(_contentLength > 0 && _contentLength < maxLength)
    {
        maxLength=_contentLength;	// limit to expected size
    }


    int count = [_inputStream read:buffer maxLength:maxLength];

    if(_chunkLength > 0)
    {
        _chunkLength-=count;// if this becomes 0 we are looking for the next chunk length	
        
        //NSLog(@"after read %d bytes, _chunkLength = %d", count, _chunkLength);
    }
    else if(_contentLength > 0)
    {
        _contentLength -= count;
        if(_contentLength == 0)
        { 
            // we have received as much as expected
            double delayInSeconds = 0.1;
            dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
            dispatch_after(popTime, dispatch_get_current_queue(), ^(void){
                [self bodyReceived];
            });
            
            
            //[self bodyReceived];
        }
    }


	return count;
}

- (BOOL)getBuffer:(uint8_t **)buffer length:(NSUInteger *)len {
	// Not appropriate for this kind of stream; return NO.
	return NO;
}

- (BOOL)hasBytesAvailable {
	// There are always bytes available.
    if(_willRedirect || !_readingBody)
    {
        return NO;
    }
	return YES;
}




- (BOOL) connectToServer
{ // we have no open connection yet
	//NSURLRequest *request=[_currentRequest request];
    //CFURLRef url 
    
	NSURL *url= (__bridge NSURL*)CFHTTPMessageCopyRequestURL([self originRequest]);
	BOOL isHttps=[[url scheme] isEqualToString:@"https"];	// we assume that ther can't be a http and a https connection in parallel on the same host:port pair
    
	//NSHost *host=[NSHost hostWithName:[url host]];
    
    // try to resolve (NOTE: this may block for some seconds! Therefore, the resolver should be run in a separate thread!
    NSString *host = [SvnASIHttpStream getHostArress:[url host]];
    
    NSLog(@"ip for %@ is:%@", [url host], host);
    
	int port=[[url port] intValue];
	//if(!host) host=[NSHost hostWithAddress:[url host]];	// try dotted notation
	if(!host)
    { // still not resolved
        return NO;
    }
	if(!port) port=isHttps?433:80;	// default port if none is specified
    //
    //
    fd = svn_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd>0)
    {
        
        NSLog(@"svn_socket returns %ld", fd);
        
        struct timeval timeo = {3, 0};
        svn_setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo));
        long noblock = 1;
        int res = svn_ioctl(fd, FIONBIO, &noblock);
        //NSLog(@"svn_ioctl returns %d", res);
        
        struct sockaddr_in server_addr = {0};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr([host cStringUsingEncoding:NSASCIIStringEncoding]);
        server_addr.sin_port = htons(port);
        
        int ret = svn_connect(fd, &server_addr, sizeof(struct sockaddr_in));
        
        if(ret == 0)
        {
            NSLog(@"svn_connect returns %d", ret);
            //_inputStream = [[SvnSocketInputStream alloc] initWithSvnSocketHandle:fd];
            //_outputStream = [[SvnSocketOutputStream alloc] initWithSvnSocketHandle:fd];
           
        }
        else
        {
            NSLog(@"svn_connect returns %d, error:%s", ret, svn_strerror(ret));
            //NSDictionary *info=[NSDictionary
                                //dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:svn_strerror(ret)], @"Error", nil];
            
            //streamError = [NSError errorWithDomain:@"SvnASIHttpStream" code:fd userInfo:info];
        }
        
        svn_fd_set readfd = {0};
        long maxfd = fd + 1;

        struct timeval timeout = {10, 0};


        SVN_FD_ZERO(&readfd);
        SVN_FD_SET(fd, &readfd);
        ret = svn_select(maxfd, NULL, &readfd, NULL, &timeout);
        //NSLog(@"SvnSocketInputStream svn_select returns %d", res);
        if(ret > 0)
        {
            if(SVN_FD_ISSET(fd, &readfd))
            {
                //NSLog(@"svn_select connected.");
                _inputStream = [[SvnSocketInputStream alloc] initWithSvnSocketHandle:fd];
                _outputStream = [[SvnSocketOutputStream alloc] initWithSvnSocketHandle:fd];
                
            }
        }
        else if(ret < 0)
        {
           NSLog(@"svn_select error");
        }
        else
        {
            NSLog(@"svn_select timed out.");
        }
        
        noblock = 0;
        res = svn_ioctl(fd, FIONBIO, &noblock);
        //NSLog(@"svn_ioctl returns %d", res);


    }
    else
    {
        NSDictionary *info=[NSDictionary
                            dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:svn_strerror(fd)], @"Error", nil];
        
        streamError = [NSError errorWithDomain:@"SvnASIHttpStream" code:fd userInfo:info];
    }
    
	//[NSStream getStreamsToHost:host port:port inputStream:&_inputStream outputStream:&_outputStream];
	if(!_inputStream || !_outputStream)
    { // error opening the streams
#if 1
        NSLog(@"could not create streams for %@:%u", host, [[url port] intValue]);
#endif
        [self endOfUseability];
      
        return NO;
    }

#if 0
	NSLog(@"did initialize streams for %@", self);
	NSLog(@"  input %@", _inputStream);
	NSLog(@" output %@", _outputStream);
#endif

	return YES;
}		



- (void) startLoadingNextRequest
{
    if(!_inputStream && !_outputStream)	// connect to server
    {
        NSLog(@"connect to server failed");
		[self endOfUseability];	// current request will be lost
		return;	// we can't connect
    }
    
    _headerReady = NO;

    NSURL *url= (__bridge NSURL*)CFHTTPMessageCopyRequestURL([self originRequest]);
	NSString *method= (__bridge NSString*)CFHTTPMessageCopyRequestMethod([self originRequest]);

    //    
	NSMutableData *headerData;
	NSMutableDictionary *requestHeaders;
	//NSData *body;
	NSEnumerator *e;
	NSString *key;
	NSString *header;
	//NSCachedURLResponse *cachedResponse;
    
    //NSLog(@"startLoadingNextRequest:%@", url);

#if 0
	NSLog(@"startLoading: %@ on %@", [self originRequest], self);
#endif
	headerData=[[NSMutableData alloc] initWithCapacity:200];

    
    NSString *path = [SvnASIHttpStream pathForURL:url];
    
    header = [NSString stringWithFormat:@"%@ %@ HTTP/1.1\r\n", method, [path length] > 0?path:(NSString *)@"/"];
    
#if 1
	NSLog(@"request: %@", header);
#endif
	[headerData appendData:[header dataUsingEncoding:NSUTF8StringEncoding]];	// CHECKME:
	// CHECKME: what about lower/uppercase in the user provided header fields???

    NSDictionary *originHeaders = (__bridge NSDictionary*)CFHTTPMessageCopyAllHeaderFields([self originRequest]);
    if(originHeaders)
    {
        requestHeaders = [originHeaders mutableCopy];
    }
    else
    {
        requestHeaders=[[NSMutableDictionary alloc] initWithCapacity:5];
    }


	if([url port])
		header=[NSString stringWithFormat:@"%@:%u", [url host], [[url port] intValue]];	// non-default port
	else
		header=[url host];
	[requestHeaders setObject:header forKey:@"Host"];

#if 0
	NSLog(@"headers to send: %@", requestHeaders);
#endif
	e=[requestHeaders keyEnumerator];
	while((key=[e nextObject]))
    { // attributes
        NSString *val=[requestHeaders objectForKey:key];
#if 0
        NSLog(@"sending %@: %@", key, val);
#endif
        [headerData appendData:[[NSString stringWithFormat:@"%@: %@\r\n", key, val] dataUsingEncoding:NSUTF8StringEncoding]];
    }
	[headerData appendData:[@"\r\n" dataUsingEncoding:NSUTF8StringEncoding]];
#if 0
    NSLog(@"header length=%d", [headerData length]);
	NSLog(@"header=%@\n", [[NSString alloc] initWithData:headerData encoding:NSUTF8StringEncoding]);
#endif
	_headerStream=[[NSInputStream alloc] initWithData:headerData];	// convert into a stream
	//[headerData release];
	[_headerStream open];
	_shouldClose=(header=[requestHeaders objectForKey:@"Connection"]) && [header caseInsensitiveCompare:@"close"] == NSOrderedSame;	// close after sending the request
	_sendChunked=(header=[requestHeaders objectForKey:@"Transfer-Encoding"]) && [header caseInsensitiveCompare:@"chunked"] == NSOrderedSame;
	//[requestHeaders release];	// dictionary no more needed
	[[self bodyStream] open];				// if any
    _headerReady = YES;
#if 0
	NSLog(@"ready to send");
#endif

	_lastChr=0;	// prepare for reading response
	[_headerLine setString:@""];
     
    _readingBody = NO;
    _isChunked = NO;
    _headers = nil;
    _headerReceived = NO;
    _bodyReceived = NO;
    _trailersReceived = NO;
    
    _bodySended = 0;
  
    
}

- (void)sendData:(unsigned char*)buffer length:(int)len
{
    if(_outputStream == nil || buffer == NULL)
    {
        return;
    }
    
    int sentCount = 0;
    int count = 0;
    while(sentCount < len)
    {
        count = [_outputStream write:(buffer + sentCount) maxLength:len - sentCount];
        if(count < 0)
        {
            
#if 1
            NSLog(@"error while writing for SvnSocketOutputStream stream %s", svn_strerror(count));
#endif                            
            NSDictionary *info=[NSDictionary
                                dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:svn_strerror(count)], @"Error", nil];
            streamError = [NSError errorWithDomain:@"SvnSocketOutputStream" code:count userInfo:info];
            [self _sendEvent:NSStreamEventErrorOccurred];
            
            [self endOfUseability];
            return;
            
        }
        sentCount += count;        
        
    }    
    
}

- (void) handleOutputEvent:(NSStreamEvent) event
{ 
    // send header & body of current request (if any)
	/* e.g.
	 POST /wiki/Spezial:Search HTTP/1.1
	 Host: de.wikipedia.org
	 Content-Type: application/x-www-form-urlencoded
	 Content-Length: 24
	 
	 search=Katzen&go=Artikel  <- body
	 */
	switch(event)
	{
		case NSStreamEventOpenCompleted:
        { // ready to send header
#if 1
            NSLog(@"HTTP output stream opened");
#endif
            return;
        }
		case NSStreamEventHasSpaceAvailable:
		{
            //NSLog(@"NSStreamEventHasSpaceAvailable");
            unsigned char buffer[8192];	// max size of chunks to send to TCP subsystem to avoid blocking
            
            if(_headerStream && _headerReady)
			{ 
                // we are still sending the header
				if([_headerStream hasBytesAvailable])
                { // send next part until done
                    int len=[_headerStream read:buffer maxLength:sizeof(buffer)];	// read next block from stream
                    if(len < 0)
                    {
                        NSDictionary *info=[NSDictionary
                                            dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:svn_strerror(len)], @"Error", nil];
#if 1
                        NSLog(@"error while reading from HTTPHeader stream %s", svn_strerror(len));
#endif
                        streamError = [NSError errorWithDomain:@"HTTPHeaderStream" code:len userInfo:info];
                        [self _sendEvent:NSStreamEventErrorOccurred];
                        
                        [self endOfUseability];
                    }
                    else
                    {
                        //int sendCount = [_outputStream write:buffer maxLength:len];	// send
                        [self sendData:buffer length:len];
#if 1
                        //NSLog(@"%d bytes header sent for request %@", sendCount, _currentRequest);
#endif                  
                        
                        
                        
                        if(![_headerStream hasBytesAvailable])
                        {
#if 1
                            //NSLog(@"header completely sent for %@", _currentRequest);
#endif
                            [_headerStream close];

                            _bodySended = 0;
                        }
                        
                    }
                    
                    
                    return;	// done sending next chunk
                }
    
                
                if([self bodyStream])
                { // we are still sending the body
                    
                    if([[self bodyStream] hasBytesAvailable])	// FIXME: if we send chunked this is not the correct indication and we should stall sending until new data becomes available
                    { // send next part until done
                        int len=[[self bodyStream] read:buffer maxLength:sizeof(buffer)];	// read next block from stream
                        if(len < 0)
                        {
                            NSDictionary *info=[NSDictionary
                                                dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:svn_strerror(len)], @"Error",nil];
#if 1
                            NSLog(@"error while reading from HTTPBody stream %s", svn_strerror(len));
#endif
                            streamError = [NSError errorWithDomain:@"HTTPBodyStream" code:len userInfo:info];
                            [self _sendEvent:NSStreamEventErrorOccurred];
                            [self endOfUseability];
                            return;	// done
                        }
                        else
                        {
                            if(_sendChunked)
                            {
                                char chunkLen[32];
                                sprintf(chunkLen, "%x\r\n", len);
                                
                                //[_outputStream write:(unsigned char *) chunkLen maxLength:strlen(chunkLen)];	// send length
                                [self sendData:chunkLen length:strlen(chunkLen)];
                                
                                
                                [self sendData:buffer length:len];	// send what we have
                                //[_outputStream write:(unsigned char *) "\r\n" maxLength:2];	// and a CRLF
                                [self sendData:"\r\n" length:2];
                                _bodySended += strlen(chunkLen);
                                _bodySended += len;
                                _bodySended += 2;
#if 0
                                NSLog(@"chunk with %d bytes sent\nHeader: %s", len, chunkLen);
#endif
                                if(len != 0)
                                {
                                    return;	// more to send (at least a 0-length header)                                    
                                }

                            }
                            else
                            {
                                [self sendData:buffer length:len];
                                
                                _bodySended += len;
                                      
                                return;	// done
                            }
                        }
                    }
                    
                    if(_sendChunked)
                    {
                        //[_outputStream write:(unsigned char *) "0\r\n\r\n" maxLength:5];
                        [self sendData:(unsigned char *) "0\r\n\r\n" length:5];
                        _bodySended += 5;
                    }
#if 1
                    NSLog(@"body completely sent, total length:%d", _bodySended);
#endif
                    [[self bodyStream] close];	// close body stream (if open)
                    //[_bodyStream release];
                    self.bodyStream=nil;
                }
				_headerStream=nil;
			}
            
       
            return;
		}
		case NSStreamEventEndEncountered:
            if([_headerStream hasBytesAvailable])
            {
                streamError = [NSError errorWithDomain:@"connection closed by server while sending header" code:errno userInfo:nil];
                
                [self _sendEvent:NSStreamEventErrorOccurred];
            }
            else if([[self bodyStream] hasBytesAvailable])
            {
                streamError = [NSError errorWithDomain:@"connection closed by server while sending body" code:errno userInfo:nil];
                [self _sendEvent:NSStreamEventErrorOccurred];
            }
            else
			{
#if 1
                NSLog(@"server has disconnected - can't keep alive: %@", self);
#endif
			}
            [_headerStream close];
            //[_headerStream release];	// done sending header
            _headerStream=nil;
            [[self bodyStream] close];	// close body stream (if open)
            //[_bodyStream release];
            self.bodyStream = nil;
            [self endOfUseability];
            return;
		default:
            break;
	}
	NSLog(@"An error %@ occurred on the event %08x of stream %@ of %@", [_outputStream streamError], event, _outputStream, self);
	streamError = [_outputStream streamError];
    [self _sendEvent:NSStreamEventErrorOccurred];
	[self endOfUseability];
}

- (void) handleInputEvent:(NSStreamEvent) event
{
	switch(event)
	{
		case NSStreamEventOpenCompleted:
        { // ready to receive header
#if 1
            NSLog(@"HTTP input stream opened");
#endif
            return;
        }
		case NSStreamEventHasBytesAvailable:
		{
            [self _sendEvent:NSStreamEventHasBytesAvailable];
            return;
		}
		case NSStreamEventEndEncountered:
		{
#if 1
            NSLog(@"input connection closed by server: %@", self);
#endif
            if(!_readingBody)
            {
                streamError = [NSError errorWithDomain:@"incomplete header received" code:0 userInfo:nil];
                [self _sendEvent:NSStreamEventErrorOccurred];
            }
            else if([_headers objectForKey:@"Content-Length"])
			{
                if(_contentLength > 0)
				{
                    streamError = [NSError errorWithDomain:@"connection closed by server while receiving body" code:0 userInfo:nil];	// we did not receive the announced contentLength
                    [self _sendEvent:NSStreamEventErrorOccurred];
				}
                else
                {
                    [self bodyReceived];
                }
			}
            else
            {
                [self bodyReceived];	// implicit content length defined by EOF
            }
            [self endOfUseability];
            return;
		}
		default:
            break;
	}
	NSLog(@"An error %@ occurred on the event %08x of stream %@ of %@", [_inputStream streamError], event, _inputStream, self);
	streamError = [_inputStream streamError];
    [self _sendEvent:NSStreamEventErrorOccurred];
	[self endOfUseability];
}


- (void) stream:(NSStream *) stream handleEvent:(NSStreamEvent) event
{
#if 0
	NSLog(@"stream:%@ handleEvent:%x for:%@", stream, event, self);
#endif
    if(event == NSStreamEventErrorOccurred)
    {
        NSLog(@"stream:%@ handleEvent:%x for:%@", stream, event, self);
        NSDictionary *info=[NSDictionary
                            dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:"connection     disconnected"], @"Error", nil];
#if 1
        NSLog(@"receive error %s", strerror(errno));
#endif
        streamError =[NSError errorWithDomain:@"receive error" code:-1 userInfo:info];
        [self _sendEvent:NSStreamEventErrorOccurred];
        [self endOfUseability];
        return;
    }
    
	if(stream == _inputStream) 
    {
		[self handleInputEvent:event];
    }
	else if(stream == _outputStream)
    {
        [self handleOutputEvent:event];
    }
}


-(CFHTTPMessageRef)readHttpResponseHeader
{
    
    _headerReceived = NO;
    
    _headerLine=[[NSMutableString alloc] initWithCapacity:50];	// typical length of a header line
    
    
    
    unsigned char buffer[1];
    unsigned maxLength=1;
    
    int len = 0;
    
    while (!_headerReceived)
    {
        len =[_inputStream read:buffer maxLength:maxLength];
        if(len <= 0)
        {
            NSLog(@"NSStreamEventHasBytesAvailable read return %d", len);
            //break;
            return NULL;	// ignore (or when does this occur? - if EOF by server?)
        }
        
#if 0
        NSLog(@"will process %02x _lastChr=%02x into %@", buffer[0], _lastChr, _headerLine);
#endif
        if(_lastChr == '\n')
        { // first character in new line received
            if(buffer[0] != ' ' && buffer[0] != '\t')
            { // process what we have (even if empty)
                [self processHeaderLine:_headerLine];
                [_headerLine setString:@""];	// has been processed
            }
        }
        if(buffer[0] == '\r')
        {
            continue;// ignore in headers
        }
        if(buffer[0] != '\n')
        {
            [_headerLine appendFormat:@"%c", buffer[0]&0xff];	// we should try to optimize that...
        }
        _lastChr=buffer[0];
#if 0
        NSLog(@"did process %02x _lastChr=%02x into", buffer[0], _lastChr, _headerLine);
#endif
        
    }

    return _responseHeader;

}


- (void) processHeaderLine:(NSString *) line
{ // process header line
	NSString *key, *val;
	NSRange colon;
#if 0
	NSLog(@"process header line %@", line);
#endif
    if([line length] == 0)
    { // empty line received
        double delayInSeconds = 0.1;
        dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
            
        if(_isChunked)
        {
            _trailersReceived = YES;
            dispatch_after(popTime, dispatch_get_current_queue(), ^(void){
                [self trailerReceived];
            });
        }
        else if(_headers)
        {
            _headerReceived = YES;
            dispatch_after(popTime, dispatch_get_current_queue(), ^(void){
                [self headersReceived];
            });
        }
        else if(1 == _statusCode/100)
        {
            if(_responseHeader)
            {
                CFRelease(_responseHeader);
                _responseHeader = NULL;
            }
            
        }
        return;	// else CRLF before header - be tolerant according to chapter 19.3
    }
	if(!_responseHeader)
    { // should be/must be the header line
        unsigned major, minor;
        if(sscanf([line UTF8String], "HTTP/%u.%u %u", &major, &minor, &_statusCode) == 3)
        { // response header line
            if(major != 1 || minor > 1)
            {
                return;
            }
            //_headers=[[NSMutableDictionary alloc] initWithCapacity:10];	// start collecting headers
            
            NSString *httpVersion = [NSString stringWithFormat:@"HTTP/%u.%u", major, minor];
            int index = [httpVersion length] + 5; 
            NSString *statusDescription = [line substringFromIndex:index];
            
            _responseHeader = CFHTTPMessageCreateResponse(CFAllocatorGetDefault(), _statusCode, (__bridge_retained CFStringRef)statusDescription, (__bridge_retained CFStringRef)httpVersion);
#if 1
            NSLog(@"Received response: %@", line);
#endif
            return;	// process next line
        }
        else
        {
#if 1
            NSLog(@"Received Invalid header line: %@", line);
#endif
            streamError = [NSError errorWithDomain:@"Invalid HTTP response" code:0 userInfo:nil];
            [self _sendEvent:NSStreamEventErrorOccurred];
            [self endOfUseability];
        }
        return;	// process next line
    }
    
    if(!_headers)
    {
        _headers=[[NSMutableDictionary alloc] initWithCapacity:10];	// start collecting headers
    }
	colon=[line rangeOfString:@":"];
	if(colon.location == NSNotFound)
		return; // no colon found! Ignore to prevent DoS attacks...
	key=[[line substringToIndex:colon.location] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];	// convert key to all lowercase
	val=[[line substringFromIndex:colon.location+1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    
    CFHTTPMessageSetHeaderFieldValue(_responseHeader, (__bridge_retained CFStringRef)key, (__bridge_retained CFStringRef)val);
    
	if([_headers objectForKey:key])
		val=[NSString stringWithFormat:@"%@, %@", [_headers objectForKey:key], val];	// merge multiple headers with same key into a single one - comma separated
	[_headers setObject:val forKey:key];
	if([key isEqualToString:@"warning"])
		NSLog(@"HTTP Warning: %@", val);	// print on console log
#if 0
	NSLog(@"Received header: %@:%@", key, val);
#endif
}

- (void) endOfUseability
{
	//NSArray *keys;
#if 0
	NSLog(@"endOfUseability %@", self);
#endif
    delegate = nil;

    [_headerStream close];
    _headerStream = nil;

    [bodyStream close];
    bodyStream = nil;
    
	[_inputStream close];
	_inputStream=nil;
    
	[_outputStream close];
	_outputStream=nil;
    
    if(fd>0)
    {
        int closeRet = svn_close(fd);
        //NSLog(@"svn_close fd:%d returns %d", fd, closeRet);
        if(closeRet == SVN_OK)
        {
            fd = -1;
        }
    }
	//[self retain];	// the next two lines could otherwise -dealloc and dealloc the request queue
	//[self autorelease];
}	


- (void) headersReceived
{ 
    
    // end of header block received

    //NSURL *url= (__bridge NSURL*)CFHTTPMessageCopyRequestURL([self originRequest]);
	NSString *method= (__bridge NSString*)CFHTTPMessageCopyRequestMethod([self originRequest]);
    
	NSString *header;
#if 1
	NSLog(@"headers received %@", self);
#endif

	_contentLength = (header=[_headers objectForKey:@"Content-Length"])?[header longLongValue]:0;
	_isChunked = (header=[_headers objectForKey:@"Transfer-Encoding"]) && [header caseInsensitiveCompare:@"chunked"] == NSOrderedSame;
	_willClose=(header=[_headers objectForKey:@"Connection"]) && [header caseInsensitiveCompare:@"close"] == NSOrderedSame;	// will close after completing the request

    _willRedirect = NO;
    
    if(_isChunked)
    {
        _readingBody = YES;
    }
    else
    {
        if([_headers valueForKey:@"Location"] && (_statusCode == 302 || _statusCode == 304))
        {
            _willRedirect = YES;
        }
        
        if((header=[_headers objectForKey:@"Content-Length"]) && [header isEqualToString:@"0"])
        {
            _readingBody = NO;
        }
        else
        {
            _readingBody = !(_statusCode/100 == 1 || _statusCode == 204|| _statusCode == 302 || _statusCode == 304 || [method isEqualToString:@"HEAD"]);	// decide if we expect to receive a body
        }
    }
    NSLog(@"Status code:%d, _readingBody:%@", _statusCode, _readingBody?@"YES":@"NO" );
    
    if(!_readingBody)
    {
        [self trailerReceived];
    }

}


- (void) bodyReceived
{
    if(!_readingBody)
    {
        return;
    }
#if 1
	NSLog(@"body received %@", self);
#endif
	_readingBody = NO;	// start over reading headers/trailer
	// apply MD5 checking [_headers objectForKey:@"Content-MD5"]
	// apply content-encoding (after MD5)
	if(!_isChunked)
    {
        [self trailerReceived];	// there is no trailer if not chunked
    }
    else
    {
        _lastChr = '\n';
        [_headerLine setString:@""];
        uint8_t ch[1];
        while (!_trailersReceived)
        {
            int count =[_inputStream read:ch maxLength:1];
            if(count <= 0)
            {
                NSLog(@"NSStreamEventHasBytesAvailable read return %d", count);
                //break;
                break;	// ignore (or when does this occur? - if EOF by server?)
            }
            
#if 0
            NSLog(@"will process %02x _lastChr=%02x into %@", ch[0], _lastChr, _headerLine);
#endif
            if(_lastChr == '\n')
            { // first character in new line received
                if(ch[0] != ' ' && ch[0] != '\t')
                { // process what we have (even if empty)
                    [self processHeaderLine:_headerLine];
                    [_headerLine setString:@""];	// has been processed
                }
            }
            if(ch[0] == '\r')
            {
                continue;// ignore in headers
            }
            if(ch[0] != '\n')
            {
                [_headerLine appendFormat:@"%c", ch[0]&0xff];	// we should try to optimize that...
            }
            _lastChr=ch[0];
#if 0
            NSLog(@"did process %02x _lastChr=%02x into %@", ch[0], _lastChr, _headerLine);
#endif
            
        }

    }
}

- (void) trailerReceived
{
#if 0
	NSLog(@"trailers received %@", self);
#endif
    _isChunked=NO;

    streamError = nil;
    streamStatus = NSStreamEventEndEncountered;
    [self _sendEvent:NSStreamEventEndEncountered];

	//if(_shouldClose)
    //{
        //NSLog(@"close connection after trailers received.");
		[self endOfUseability];
    //}
	
}



@end
