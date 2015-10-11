//
//  SvnHttpURLProtocol.m
//  SvnSdk
//
//  Created by l00174413 on 13-6-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#import <Foundation/NSURLProtocol.h>
#import <Foundation/NSURLRequest.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSRunLoop.h>
#import <Foundation/NSStream.h>
#import <Foundation/NSString.h>
#import <Foundation/NSValue.h>
#import <UIKit/UIKit.h>

#import "SvnSocketStream.h"
#import "SvnHttpURLProtocol.h"
#import "SvnDNSResolve.h"

#import "svn_define.h"
#import "svn_api.h"
#import "svn_socket_api.h"
#import "svn_socket_err.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>


//volatile int parsedIP = NULL;

//void g_ParseURLCallback(unsigned long ulIP[SVN_MAX_URL_NUM], void* parsedIp)
//{
//    int* result = (int *)parsedIp;
//    if(NULL != ulIP && NULL != result)
//    {
//        result[0] = ulIP[0];
//        //NSLog(@"parseIP result:%d", parsedIP);
//    }
//}

#define CAN_GZIP 0

@interface SvnHttpSerialization : NSObject
{ // for handling a single protocol entity according to http://www.faqs.org/ftp/rfc/rfc2616.pdf
	NSMutableArray *_requestQueue;		// all queued requests
	SvnHttpURLProtocol *_currentRequest;	// current request
	// sending
	NSOutputStream *_outputStream;
	NSInputStream *_headerStream;			// header while sending
	NSInputStream *_bodyStream;				// for sending the body
	BOOL _shouldClose;								// server will close after current request - we must requeue other requests on a new connection
	BOOL _sendChunked;								// sending with transfer-encoding: chunked
	// receiving
	NSInputStream *_inputStream;
	unsigned _statusCode;							// status code defined by response
	NSMutableDictionary *_headers;		// received headers
	unsigned long long _contentLength;		// if explicitly specified by header
	NSMutableString *_headerLine;			// current header line
	unsigned int _chunkLength;				// current chunk length for receiver
	char _lastChr;										// previouds character while reading header
	BOOL _readingBody;								// done with reading header
	BOOL _isChunked;									// transfer-encoding: chunked
	BOOL _willClose;									// server has announced to close the connection
    volatile long fd;

    unsigned long long _bodySended;

    BOOL _headerReady;

    NSStringEncoding _encoding;
}

+ (SvnHttpSerialization *) serializerForProtocol:(SvnHttpURLProtocol *) protocol;	// get connection queue for handling this request (may create a new one)
+ (NSString *) pathForURL:(NSURL *)url;
- (void) startLoading:(SvnHttpURLProtocol *) proto;		// add to queue
- (void) stopLoading:(SvnHttpURLProtocol *) proto;		// remove from queue - may cancel/close connection if it is current request or simply stop notifications

// internal methods

- (BOOL) connectToServer;	// connect to server
- (void) headersReceived;
- (void) bodyReceived;
- (void) trailerReceived;
- (void) endOfUseability;	// connection became invalid

- (NSString *)getHostArress:(NSString *)host;

@end


@implementation SvnHttpSerialization

// see http://www.w3.org/Protocols/rfc2616/rfc2616.html
// or http://www.faqs.org/ftp/rfc/rfc2616.pdf
// and a very good tutorial: http://www.jmarshall.com/easy/http/
// http://www.io.com/~maus/HttpKeepAlive.html
// http://java.sun.com/j2se/1.5.0/docs/guide/net/http-keepalive.html

static NSMutableDictionary *_httpConnections = nil;
static NSString *_defaultUserAgent = nil;

#pragma mark get user agent

+ (NSString *)defaultUserAgentString
{
	@synchronized (self)
    {        
		if (!_defaultUserAgent)
        {
			// Attempt to find a name for this application
			NSString *appName = @"SVN SDK";

            
			NSString *appVersion = @"1.0";

            
			NSString *deviceName;
			NSString *OSName;
			NSString *OSVersion;
			NSString *locale = [[NSLocale currentLocale] localeIdentifier];
            
#if TARGET_OS_IPHONE
            UIDevice *device = [UIDevice currentDevice];
            deviceName = [device model];
            OSName = [device systemName];
            OSVersion = [device systemVersion];
            
#else
            deviceName = @"Macintosh";
            OSName = @"Mac OS X";
            
            // From http://www.cocoadev.com/index.pl?DeterminingOSVersion
            // We won't bother to check for systems prior to 10.4, since ASIHTTPRequest only works on 10.5+
            OSErr err;
            SInt32 versionMajor, versionMinor, versionBugFix;
            err = Gestalt(gestaltSystemVersionMajor, &versionMajor);
            if (err != noErr)
            { 
                return nil;
            }
            err = Gestalt(gestaltSystemVersionMinor, &versionMinor);
            if (err != noErr)
            { 
                return nil;
            }
            err = Gestalt(gestaltSystemVersionBugFix, &versionBugFix);
            if (err != noErr)
            { 
                return nil;
            }
            OSVersion = [NSString stringWithFormat:@"%u.%u.%u", versionMajor, versionMinor, versionBugFix];
#endif
            
			// Takes the form "My Application 1.0 (Macintosh; Mac OS X 10.5.7; en_GB)"
			[self setDefaultUserAgentString:[NSString stringWithFormat:@"%@ %@ (%@; %@ %@; %@)", appName, appVersion, deviceName, OSName, OSVersion, locale]];
		}
		return _defaultUserAgent;
	}
}

+ (void)setDefaultUserAgentString:(NSString *)agent
{
	@synchronized (self) {
		if (_defaultUserAgent == agent) {
			return;
		}
		_defaultUserAgent = agent;
	}
}



- (NSString *)getHostArress:(NSString *)host
{
    
    if(nil == host || 0 == [host length])
    {
        return  nil;
    }
    //是否含有字母，是否需要域名解析
    BOOL bIncludeLetter = NO;
    
    for (int i = 0; i< [host length]; i++)
    {
        unichar c = [host characterAtIndex:i];
        if( (c >='a' && c<='z') || (c>='A' && c<='Z'))
        {
            bIncludeLetter = YES;
        }
    }
    
    if(!bIncludeLetter)
    {
        //IP,直接返回
        return host;
    }

    
    static NSDictionary* hostTable = nil;
    if (!hostTable)
    {
        hostTable = [[NSMutableDictionary alloc] initWithCapacity:20];
    }
    
    NSString* cachedIP = (NSString*) [hostTable objectForKey:host];
    
    if(cachedIP)
    {
        //NSLog(@"cachedIP %@ found for host:%@", cachedIP, host);
        return cachedIP;
    }
    //int parsedIP[1] = {0};
   
    int iAddress = ASYNC_DNS_ParseURL([host cStringUsingEncoding:NSASCIIStringEncoding], [host length]);
    if(0 == iAddress)
    {
        //调用失败
        return nil;
    }
    int part1 = (iAddress &0x000000FF);
    int part2 = (iAddress &0x0000FF00) >>8;
    int part3 = (iAddress &0x00FF0000) >>16;
    int part4 = (iAddress &0xFF000000) >>24;
    NSString * result =  [NSString stringWithFormat:@"%d.%d.%d.%d",part1, part2, part3, part4];
    [hostTable setValue:result forKey:host];
    return result;

   }

- (BOOL) willClose
{ // we have announced (in request "Connection: close") to close or server has announced (in reply) - i.e. don't queue up more requests
	return _shouldClose || _willClose;
}

+ (SvnHttpSerialization *) serializerForProtocol:(SvnHttpURLProtocol *) protocol
{ // get connection queue for handling this request (may create a new one)
	//NSString *key=[protocol _uniqueKey];
	//SvnHttpSerialization *ser=[_httpConnections objectForKey:key];	// could also store an array!
	//if(!ser || [ser willClose])
    //{ // not found or server has announced to close connection: we need a new connection
    SvnHttpSerialization *ser = [self new];
#if 1
        //NSLog(@"%@: new serializer %@", key, ser);
#endif
        //if(!_httpConnections)
            //_httpConnections=[[NSMutableDictionary alloc] initWithCapacity:10];
        // we also may open several serializers for the same combination but HTTP 1.1 recommends to use no more than 2 in parallel
        //[_httpConnections setObject:ser forKey:key];

    //}
//#if 1
	//else
    //{
		//NSLog(@"%@: reuse serializer %@", key, ser);
    //}
//#endif
	return ser;
}

+ (NSString *) pathForURL:(NSURL *)url
{
	//NSString *path=[url path];
    NSString *path = (NSString*)CFBridgingRelease(CFURLCopyPath((CFURLRef)url));
    NSString *params = [url parameterString];
    NSString *query = [url query];
    NSString *fragment = [url fragment];
//    NSString *absoluteurl= [url absoluteString];
//    
//    NSLog(@"path:%@, params:%@, query:%@, fragment:%@, absoluteurl:%@", path, params, query, fragment, absoluteurl);

    
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
    
    return result;
}

- (id) init
{
	if((self=[super init]))
    {
		_requestQueue=[NSMutableArray new];
		_headerLine=[[NSMutableString alloc] initWithCapacity:50];	// typical length of a header line
    }
	return self;
}

- (void) dealloc
{
#if 0
	NSLog(@"dealloc %@", self);
	NSLog(@"  connections: %d", [_httpConnections count]);
#endif
	NSAssert([_requestQueue count] == 0, @"unprocessed requests left over!");	// otherwise we loose requests
	[_currentRequest _setConnection:nil];	// has been processed
	

	[_inputStream close];				// if still open
	//[_inputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    _inputStream = nil;
	[_outputStream close];
    _outputStream = nil;
    
    if(fd > 0)
    {
        int closeRet = svn_close(fd);
        NSLog(@"svn_close fd:%d returns %d", fd, closeRet);
        if(SVN_OK == closeRet)
        {
            fd = -1;
        }
    }	//[_outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];

	//[super dealloc];
}

- (void) endOfUseability
{
	//NSArray *keys;
#if 0
	NSLog(@"endOfUseability %@", self);
#endif
    _currentRequest = nil;
    
    [_headerStream close];
    _headerStream = nil;
    
    [_bodyStream close];
    _bodyStream = nil;
    
	[_inputStream close];
	_inputStream=nil;
    
	[_outputStream close];
	_outputStream=nil;
    
	[_inputStream close];
	//[_inputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	//[_inputStream release];
	_inputStream=nil;
	[_outputStream close];
	//[_outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	//[_outputStream release];
	_outputStream=nil;
    
    if(fd > 0)
    {
        int closeRet = svn_close(fd);
        //NSLog(@"svn_close fd:%d returns %d", fd, closeRet);
        if(SVN_OK == closeRet)
        {
            fd = -1;
        }
    }
	//[self retain];	// the next two lines could otherwise -dealloc and dealloc the request queue
	//keys=[_httpConnections allKeysForObject:self];	// get all my keys
	//[_httpConnections removeObjectsForKeys:keys];		// remove us from the list of active connections if we are still there
	//[_requestQueue makeObjectsPerformSelector:@selector(_restartLoading)];	// this removes any pending requests from the queue and reschedules in a new/different serializer queue
	//[self autorelease];
}	

- (BOOL) connectToServer
{ // we have no open connection yet
	NSURLRequest *request=[_currentRequest request];
	NSURL *url=[request URL];
	BOOL isHttps=[[url scheme] isEqualToString:@"https"];	// we assume that ther can't be a http and a https connection in parallel on the same host:port pair
    
	//NSHost *host=[NSHost hostWithName:[url host]];
  
    // try to resolve (NOTE: this may block for some seconds! Therefore, the resolver should be run in a separate thread!
    NSString *host = [self getHostArress:[url host]];
    
    //NSLog(@"ip for %@ is:%@", [url host], host);
    
	int port=[[url port] intValue];
	//if(!host) host=[NSHost hostWithAddress:[url host]];	// try dotted notation
	if(!host)
    { // still not resolved
        [_currentRequest didFailWithError:[NSError errorWithDomain:@"NSURLErrorDomain" code:kCFURLErrorCannotFindHost
                                                          userInfo:[NSDictionary dictionaryWithObjectsAndKeys:
                                                                    url, @"NSErrorFailingURLKey",
                                                                    [url absoluteString], @"NSErrorFailingURLStringKey",
                                                                    @"can't resolve host name", @"NSLocalizedDescription",
                                                                    nil]]];
        return NO;
    }
	if(!port) port=isHttps?433:80;	// default port if none is specified
    //
    //
    fd = svn_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd > 0)
    {
        
        NSLog(@"svn_socket returns %ld", fd);
        
        long noblock = 1;
        svn_ioctl(fd, FIONBIO, &noblock);
        //NSLog(@"svn_ioctl returns %d", res);
        
        struct sockaddr_in server_addr = {0};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr([host cStringUsingEncoding:NSASCIIStringEncoding]);
        server_addr.sin_port = htons(port);
        
        int ret = svn_connect(fd, &server_addr, sizeof(struct sockaddr_in));
        
        if(0 == ret)
        {
            NSLog(@"svn_connect returns %d", ret);
//            _inputStream = [[SvnSocketInputStream alloc] initWithSvnSocketHandle:fd];
//            _outputStream = [[SvnSocketOutputStream alloc] initWithSvnSocketHandle:fd];  
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
        svn_ioctl(fd, FIONBIO, &noblock);
        //NSLog(@"svn_ioctl returns %d", res);
    }
    
	//[NSStream getStreamsToHost:host port:port inputStream:&_inputStream outputStream:&_outputStream];
	if(!_inputStream || !_outputStream)
    { // error opening the streams
#if 1
        //NSLog(@"could not create streams for %@:%u", host, [[url port] intValue]);
#endif
        [_currentRequest didFailWithError:[NSError errorWithDomain:@"NSURLErrorDomain" code:kCFURLErrorCannotConnectToHost
                                                          userInfo:[NSDictionary dictionaryWithObjectsAndKeys:
                                                                    url, @"NSErrorFailingURLKey",
                                                                    host, @"NSErrorFailingURLStringKey",
                                                                    @"can't open connections to host", @"NSLocalizedDescription",
                                                                    nil]]];
        _inputStream=nil;
        _outputStream=nil;
        return NO;
    }
	//[_inputStream retain];
	//[_outputStream retain];	// endOfUseability will do a release
	[_inputStream setDelegate:self];
	[_outputStream setDelegate:self];
	//if(isHttps)
    //{ // use SSL
        //[_inputStream setProperty:NSStreamSocketSecurityLevelNegotiatedSSL forKey:NSStreamSocketSecurityLevelKey];
        //[_outputStream setProperty:NSStreamSocketSecurityLevelNegotiatedSSL forKey:NSStreamSocketSecurityLevelKey];
    //}
#if 0
	NSLog(@"did initialize streams for %@", self);
	NSLog(@"  input %@", _inputStream);
	NSLog(@" output %@", _outputStream);
#endif
	[_inputStream open];
	[_outputStream open];
	//[_inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];	// and schedule for reception
	return YES;
}		

- (void) startLoadingNextRequest
{
    
    _headerReady = NO;
	//NSURLProtocol *protocol=[_requestQueue objectAtIndex:0];
    NSURLProtocol *protocol = _currentRequest;
	NSURLRequest *request=[protocol request];
	NSURL *url=[request URL];
	NSString *method=[request HTTPMethod];
//	NSString *path=[url path];
//    NSString *query = [url query];
//    NSString *fragment = [url fragment];
//    NSString *relativePath = [url relativeString];
//    
//    NSLog(@"path:%@, query:%@, fragment:%@, relativePath:%@", path, query, fragment, relativePath);
//    
	NSMutableData *headerData = nil;
	NSMutableDictionary *requestHeaders = nil;
	NSData *body = nil;
	NSEnumerator *e = nil;
	NSString *key = nil;
	NSString *header = nil;
	NSCachedURLResponse *cachedResponse = nil;
    
    //NSLog(@"startLoadingNextRequest:%@", url);
	//[_currentRequeat release];	// release any done request
	//_currentRequest = protocol ;
	//[_requestQueue removeObjectAtIndex:0];	// remove from queue
	if(!_outputStream && ![self connectToServer])	// connect to server
    {
        NSLog(@"connect to server failed");
		[self endOfUseability];	// current request will be lost
		return;	// we can't connect
    }
#if 0
	NSLog(@"startLoading: %@ on %@", _currentRequest, self);
#endif
	headerData=[[NSMutableData alloc] initWithCapacity:200];

    
    NSString *path = [SvnHttpSerialization pathForURL:url];
    header = [NSString stringWithFormat:@"%@ %@ HTTP/1.1\r\n", method, [path length] > 0?path:(NSString *)@"/"];
    	
#if 0
	NSLog(@"request: %@", header);
#endif
	[headerData appendData:[header dataUsingEncoding:NSUTF8StringEncoding]];	// CHECKME:
	// CHECKME: what about lower/uppercase in the user provided header fields???
	requestHeaders=[[request allHTTPHeaderFields] mutableCopy];		// start with the provided headers first so that we can overwrite and remove spurious headers
	if(!requestHeaders) requestHeaders=[[NSMutableDictionary alloc] initWithCapacity:5];	// no headers provided by request
	if([request HTTPShouldHandleCookies])
    {
       // NSURL* cookieURL = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@:%@", [url host], [url port]]];        
		NSHTTPCookieStorage *cs=[NSHTTPCookieStorage sharedHTTPCookieStorage];
        NSArray*cookies = [cs cookiesForURL:url];
        
        //NSLog(@"Cookies retreived for %@ is:%@", url, cookies);

        
		NSDictionary *cdict=[NSHTTPCookie requestHeaderFieldsWithCookies:cookies];
		[requestHeaders addEntriesFromDictionary:cdict];	// add to headers
    }
	if([url port])
    {
		header=[NSString stringWithFormat:@"%@:%u", [url host], [[url port] intValue]];	// non-default port
    }
	else
    {
		header=[url host];
    }
	[requestHeaders setObject:header forKey:@"Host"];
    
    // Build and set the user agent string if the request does not already have a custom user agent specified
	if (![requestHeaders objectForKey:@"User-Agent"])
    {
        [requestHeaders setObject:[SvnHttpSerialization defaultUserAgentString] forKey:@"User-Agent"];
	}
    
    
	if((cachedResponse=[_currentRequest cachedResponse]) && ([method isEqualToString:@"GET"] || [method isEqualToString:@"HEAD"]))
    { // ask server to send a new version or a 304 so that we use the cached response
        NSHTTPURLResponse *resp=(NSHTTPURLResponse *) [cachedResponse response];
        NSString *lastModified=[[resp allHeaderFields] objectForKey:@"last-modified"];
#if 1
        NSLog(@"last-modified -> if-modified-since %@", lastModified);
#endif
        if(lastModified)
            [requestHeaders setObject:lastModified forKey:@"If-Modified-Since"];	// copy into the new request
    }
#if CAN_GZIP
	[requestHeaders setObject:@"identity, gzip" forKey:@"Accept-Encoding"];	// set what we can uncompress
#else
	[requestHeaders setObject:@"identity" forKey:@"Accept-Encoding"];
#endif
	if((_bodyStream=[request HTTPBodyStream]))
    {	// is provided by a stream object
        //[_bodyStream retain];
        [_bodyStream setProperty:[NSNumber numberWithInt:0] forKey:NSStreamFileCurrentOffsetKey];	// rewind (if possible)
        [requestHeaders setObject:@"chunked" forKey:@"Transfer-Encoding"];	// we must send chunked because we don't know the length in advance
    }
	else if((body=[request HTTPBody]))
    { // fixed NSData object
        unsigned long bodyLength=[body length];
        [requestHeaders setObject:[NSString stringWithFormat:@"%lu", bodyLength] forKey:@"Content-Length"];
        _bodyStream=[[NSInputStream alloc] initWithData:body];	// prepare to send request body from NSData object
    }
	else
		[requestHeaders removeObjectForKey:@"Date"];	// must not send a Date: header if we have no body
	//	[requestHeaders setObject:@"identity" forKey:@"TE"];	// what we accept in responses
	[requestHeaders removeObjectForKey:@"Keep-Alive"];	// HHTP 1.0 feature
    [requestHeaders setObject:@"Keep-Alive" forKey:@"Connection"];
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
#if 1
    NSLog(@"header length=%d", [headerData length]);
	NSLog(@"header=%@\n", [[NSString alloc] initWithData:headerData encoding:NSUTF8StringEncoding]);
#endif
	_headerStream=[[NSInputStream alloc] initWithData:headerData];	// convert into a stream
	//[headerData release];
	[_headerStream open];
	_shouldClose=(header=[requestHeaders objectForKey:@"Connection"]) && [header caseInsensitiveCompare:@"close"] == NSOrderedSame;	// close after sending the request
	_sendChunked=(header=[requestHeaders objectForKey:@"Transfer-Encoding"]) && [header caseInsensitiveCompare:@"chunked"] == NSOrderedSame;
	//[requestHeaders release];	// dictionary no more needed
	[_bodyStream open];				// if any
    _headerReady = YES;
#if 0
	NSLog(@"ready to send");
#endif
	//[_outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];	// start handling output
	_lastChr=0;	// prepare for reading response
	[_headerLine setString:@""];
    _readingBody = NO;
    _isChunked = NO;
    _headers = nil;


}

- (void) startLoading:(SvnHttpURLProtocol *) proto
{ // add to queue
	[[proto _connection] stopLoading:proto];	// remove from other queue (if any)
	//[_requestQueue addObject:proto];	// append to our queue
	[proto _setConnection:self];
	//if(!_currentRequest)
    //{
    _currentRequest = proto;
    [self startLoadingNextRequest];	// is the first request we are waiting for
    //}
    //else
    //{
        //NSLog(@"_currentRequest not nil, current request:%@, new request:%@", _currentRequest, proto);
    //}
    
}

- (void) stopLoading:(SvnHttpURLProtocol *) proto
{ // remove from queue - may cancel/close connection if it is current request or simply stop notifications
	[proto _setConnection:nil];

	//[_requestQueue removeObject:proto];
}

- (void) headersReceived
{ // end of header block received
	NSURLRequest *request=[_currentRequest request];
	NSURL *url=[request URL];
	NSString *header;
#if 1
	NSLog(@"headers received for:%@ is:%@", self, _headers);
#endif
	if([request HTTPShouldHandleCookies])
    { // auto-process cookies if requested
       //NSURL* cookieURL = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@:%@", [url host], [url port]]];  
        //NSArray *cookies = [[NSHTTPCookie alloc]initWithProperties:_headers];
        NSArray *cookies=[NSHTTPCookie cookiesWithResponseHeaderFields:_headers forURL:url];
        [[NSHTTPCookieStorage sharedHTTPCookieStorage] setCookies:cookies forURL:url mainDocumentURL:nil];
        
        //NSLog(@"Cookies saved for %@ is:%@", url, cookies);
        //[NSHTTPCookieStorage sharedHTTPCookieStorage] c
    }
    _encoding = NSASCIIStringEncoding;
    
    if((header = [_headers objectForKey:@"Content-Type"]))
    {
        NSScanner *charsetScanner = [NSScanner scannerWithString: header];
        if ([charsetScanner scanUpToString:@";" intoString:NULL] && [charsetScanner scanLocation] < [header length])
        {
            NSString *charsetSeparator = @"charset=";
            NSString *IANAEncoding = nil;
            
            if ([charsetScanner scanUpToString: charsetSeparator intoString: NULL] && [charsetScanner scanLocation] < [header length])
            {
                [charsetScanner setScanLocation: [charsetScanner scanLocation] + [charsetSeparator length]];
                [charsetScanner scanUpToString: @";" intoString: &IANAEncoding];
            }
            
            if (IANAEncoding)
            {
                CFStringEncoding cfEncoding = CFStringConvertIANACharSetNameToEncoding((CFStringRef)IANAEncoding);
                if (cfEncoding != kCFStringEncodingInvalidId) {
                    _encoding = CFStringConvertEncodingToNSStringEncoding(cfEncoding);
                }
            }

        }
        
       
    }
    
	if((header=[_headers objectForKey:@"Content-Encoding"]))
    { // handle header compression
        if([header isEqualToString:@"gzip"])
            NSLog(@"body is gzip compressed");
        // we have the private method [NSData inflate]
        // we must do that here since we receive the stream here
        // NOTE: this may be a , separated list of encodings to be applied in sequence!
        // so we have to loop over [encoding componentsSeparatedByString:@","] - trimmed and compared case-insensitive
    }
	_contentLength = (header=[_headers objectForKey:@"Content-Length"])?[header longLongValue]:0;
	_isChunked=(header=[_headers objectForKey:@"Transfer-Encoding"]) && [header caseInsensitiveCompare:@"chunked"] == NSOrderedSame;
	_willClose=(header=[_headers objectForKey:@"Connection"]) && [header caseInsensitiveCompare:@"close"] == NSOrderedSame;	// will close after completing the request
	//if(!_isChunked)	// ??? must we notify (partial) response before we send any data ???
    //{
        NSHTTPURLResponse *response = [[NSHTTPURLResponse alloc] initWithURL:url statusCode:_statusCode HTTPVersion:@"HTTP/1.1" headerFields:_headers];
        
		//NSHTTPURLResponse *response=[[NSHTTPURLResponse alloc] initWithURL:url headerFields:_headers andStatusCode:_statusCode] ;
		[_currentRequest didReceiveResponse:response];
    //}

    if(_isChunked)
    {
        _readingBody = YES;
    }
    else
    {
        
        if((header=[_headers objectForKey:@"Content-Length"]) && [header isEqualToString:@"0"])
        {
            _readingBody = NO;
        }
        else
        {
            _readingBody = !(_statusCode/100 == 1 || _statusCode == 204|| _statusCode == 302 || _statusCode == 304 || [[request HTTPMethod] isEqualToString:@"HEAD"]);	// decide if we expect to receive a body
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
#if 0
	NSLog(@"body received %@", self);
#endif
	_readingBody=NO;	// start over reading headers/trailer
	// apply MD5 checking [_headers objectForKey:@"Content-MD5"]
	// apply content-encoding (after MD5)
	if(!_isChunked)
		[self trailerReceived];	// there is no trailer if not chunked
}

- (void) trailerReceived
{
#if 0
	NSLog(@"trailers received %@", self);
#endif
    _isChunked=NO;
//	if(_isChunked)
//    { // notify all headers after receiving trailer
//        //NSHTTPURLResponse *response= [[NSHTTPURLResponse alloc] _initWithURL:[[_currentRequest request] URL] headerFields:_headers andStatusCode:_statusCode] ;
//        NSHTTPURLResponse *response = [[NSHTTPURLResponse alloc] initWithURL:[[_currentRequest request] URL] statusCode:_statusCode HTTPVersion:@"HTTP/1.1" headerFields:_headers];
//        [_currentRequest didReceiveResponse:response];
//        _isChunked=NO;
//    }
	[_currentRequest didFinishLoading];
	//[_headers release];	// have been stored in NSHTTPURLResponse
	_headers=nil;
	[_currentRequest _setConnection:nil];	// has been processed
	//[_currentRequest release];
	_currentRequest=nil;
	//if(_shouldClose)
    //{
        //NSLog(@"close connection after trailers received.");
		[self endOfUseability];
    //}
	//else if([_requestQueue count] > 0)
    //{
        //NSLog(@"trailers received, has queued request, queue count :%d", [_requestQueue count]);
		//[self startLoadingNextRequest];	// send next request from queue
    //}
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
        if(_isChunked)
        {
            [self trailerReceived];
        }
        else if(_headers && [_headers count] > 0)
        {
            [self headersReceived];
        }
        else
        {
            if(1== _statusCode/100)
            {
                //[_headers release];
                _headers = nil;
            }
            else
            {
                NSLog(@"empty header");
            }
        }
        return;	// else CRLF before header - be tolerant according to chapter 19.3
    }
	if(!_headers)
    { // should be/must be the header line
        unsigned major, minor;
        if(sscanf([line UTF8String], "HTTP/%u.%u %u", &major, &minor, &_statusCode) == 3)
        { // response header line
            if(major != 1 || minor > 1)
            {
                [_currentRequest didFailWithError:[NSError errorWithDomain:@"Bad HTTP version received" code:0 userInfo:nil]];
            }
            _headers=[[NSMutableDictionary alloc] initWithCapacity:10];	// start collecting headers
#if 0
            NSLog(@"Received response: %@", line);
#endif
            return;	// process next line
        }
        else
        {
#if 1
            NSLog(@"Received Invalid header line: %@", line);
#endif
            [_currentRequest didFailWithError:[NSError errorWithDomain:@"Invalid HTTP response" code:0 userInfo:nil]];
            [self endOfUseability];
            
            return;	// process next line
        }
        
    }
	colon=[line rangeOfString:@":"];
	if(colon.location == NSNotFound)
		return; // no colon found! Ignore to prevent DoS attacks...
	key=[[line substringToIndex:colon.location] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];	// convert key to all lowercase
	val=[[line substringFromIndex:colon.location+1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
	if([_headers objectForKey:key])
		val=[NSString stringWithFormat:@"%@, %@", [_headers objectForKey:key], val];	// merge multiple headers with same key into a single one - comma separated
	[_headers setObject:val forKey:key];
	if([key isEqualToString:@"warning"])
		NSLog(@"HTTP Warning: %@", val);	// print on console log
#if 0
	NSLog(@"Received header: %@:%@", key, val);
#endif
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
            //NSLog(@"NSStreamEventHasBytesAvailable");
            unsigned char buffer[8192*16];
            unsigned maxLength=sizeof(buffer);
            int len;
            if(_readingBody && (!_isChunked || _chunkLength > 0))
			{
                if(_isChunked && _chunkLength < maxLength)
                {
                    maxLength=_chunkLength;	// limit to current chunk size
                }
                
                if(_contentLength > 0 && _contentLength < maxLength)
                {
                    maxLength=_contentLength;	// limit to expected size
                }
			}
            else
            {
                maxLength=1;	
            }
            // so that we don't miss the Content-Length: header entry even if it directly precedes the \r\n\r\nbody
            len = [_inputStream read:buffer maxLength:maxLength];
            if(0 == len)
            {
                NSLog(@"NSStreamEventHasBytesAvailable read return 0");
                return;	// ignore (or when does this occur? - if EOF by server?)
            }
            else if(len < 0)
			{
                NSDictionary *info=[NSDictionary
                                    dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:svn_strerror(len)], @"Error", nil];
#if 1
                NSLog(@"receive error %s", svn_strerror(len));
#endif
                [_currentRequest didFailWithError:[NSError errorWithDomain:@"receive error" code:len userInfo:info]];
                [self endOfUseability];
                return;
			}
#if 0
            if(len > 1)
            {
                //NSLog(@"received %d bytes", len);
           
                NSString *receivedBytes = [[NSString alloc] initWithBytes:buffer length:len encoding:NSASCIIStringEncoding];
                NSLog(@"received %d bytes data: %@", len, receivedBytes);
            }


#endif
            if(_readingBody)
			{
                if(_isChunked && 0 == _chunkLength)
				{ // reading chunk size
#if 0
					NSLog(@"will process %02x into %@", buffer[0], _headerLine);
#endif
					if('\r' == buffer[0])
						return;	// ignore CR
					if('\n' == buffer[0])
                    { // decode chunk length
                        if([_headerLine length] > 0)
                        { // there should follow a CRLF after the body resulting in a empty line
                            NSScanner *sc=[NSScanner scannerWithString:_headerLine];
#if 0
                            NSLog(@"chunk length=%@", _headerLine);
#endif
                            _chunkLength=0;
                            if(![sc scanHexInt:&_chunkLength])	// is hex coded (we even ignore an optional 0x)
                            {
                                NSLog(@"invalid chunk length %@", _headerLine);
                                [_currentRequest didFailWithError:[NSError errorWithDomain:@"invalid chunk length" code:0 userInfo:0]];
                                [self endOfUseability];
                                return;
                            }
                            // may be followed by ; name=var
                            if(0 == _chunkLength)
                            {
                                [self bodyReceived];	// done reading body - continue with trailer
                            }
                            [_headerLine setString:@""];	// has been processed
                        }
                    }
					else
					{
						[_headerLine appendFormat:@"%c", buffer[0]&0xff];	// we should try to optimize that...
					}
					return;
				}

                NSData* tempData = [NSData dataWithBytes:buffer length:len];
                
                NSString* result = [[NSString alloc] initWithData:tempData encoding:_encoding];
#if 1                
                NSLog(@"Received data len:%d, data:\r\n%@", len, result);
#endif                
                [_currentRequest didLoadData:tempData];	// notify
                
                
                if(_chunkLength > 0)
                {
                    _chunkLength -= len;	// if this becomes 0 we are looking for the next chunk length
                }
                    
                if(_contentLength > 0)
				{
                    _contentLength -= len;
                    if(_contentLength == 0)
					{ // we have received as much as expected
						[self bodyReceived];
					}
				}
                return;
			}
#if 0
            NSLog(@"will process %02x _lastChr=%02x into %@", buffer[0], _lastChr, _headerLine);
#endif
            if('\n' == _lastChr)
			{ // first character in new line received
				if(' ' != buffer[0] && '\t' != buffer[0])
                { // process what we have (even if empty)
                    [self processHeaderLine:_headerLine];
                    [_headerLine setString:@""];	// has been processed
                }
			}
            if('\r' == buffer[0])
            {
                return;	// ignore in headers
            }
            
            
            if('\n' != buffer[0])
            {
                [_headerLine appendFormat:@"%c", buffer[0]&0xff];	// we should try to optimize that...
            }
            _lastChr=buffer[0];
#if 0
            NSLog(@"did process %02x _lastChr=%02x into", buffer[0], _lastChr, _headerLine);
#endif
            return;
		}
		case NSStreamEventEndEncountered:
		{
#if 1
            NSLog(@"input connection closed by server: %@", self);
#endif
            if(!_readingBody)
                [_currentRequest didFailWithError:[NSError errorWithDomain:@"incomplete header received" code:0 userInfo:nil]];
            if([_headers objectForKey:@"Content-Length"])
			{
                if(_contentLength > 0)
				{
                    [_currentRequest didFailWithError:[NSError errorWithDomain:@"connection closed by server while receiving body" code:0 userInfo:nil]];	// we did not receive the announced contentLength
				}
			}
            else
                [self bodyReceived];	// implicit content length defined by EOF
            [self endOfUseability];
            return;
		}
		default:
            break;
	}
	NSLog(@"An error %@ occurred on the event %08x of stream %@ of %@", [_inputStream streamError], event, _inputStream, self);
	[_currentRequest didFailWithError:[_inputStream streamError]];
	[self endOfUseability];
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
            
            [_currentRequest didFailWithError:[NSError errorWithDomain:@"SvnSocketOutputStream" code:count userInfo:info]];
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
            unsigned char buffer[8192*16];	// max size of chunks to send to TCP subsystem to avoid blocking
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
                        [_currentRequest didFailWithError:[NSError errorWithDomain:@"HTTPHeaderStream" code:len userInfo:info]];
                        [self endOfUseability];
                    }
                    else
                    {
                        //int sendCount = [_outputStream write:buffer maxLength:len];	// send
                        [self sendData:buffer length:len];
#if 0
                        //NSLog(@"%d bytes header sent for request %@", sendCount, _currentRequest);
#endif                  
                        

                        
                        if(![_headerStream hasBytesAvailable])
                        {
#if 0
                            NSLog(@"header completely sent for %@", _currentRequest);
#endif
                            [_headerStream close];
                            //[_headerStream release];	// done sending header, continue with body (if available)
                            _bodySended = 0;
                        }


                        // we might send additional headers according to the protocol - but we have already sent them
                        // this would only be useful if we want to allow the client to add/modify headers while generating the body stram
                        // in that case we would have to mark all headers if they are sent before or after the chunked body and send only the minimum headers before
                        

                    }
                    

                    return;	// done sending next chunk
                }
                

                
                
                if(_bodyStream)
                { // we are still sending the body
                    
                    if([_bodyStream hasBytesAvailable])	// FIXME: if we send chunked this is not the correct indication and we should stall sending until new data becomes available
                    { // send next part until done
                        int len=[_bodyStream read:buffer maxLength:sizeof(buffer)];	// read next block from stream
                        if(len < 0)
                        {
                            NSDictionary *info=[NSDictionary
                                                dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:svn_strerror(len)], @"Error",nil];
#if 1
                            NSLog(@"error while reading from HTTPBody stream %s", svn_strerror(len));
#endif
                            [_currentRequest didFailWithError:[NSError errorWithDomain:@"HTTPBodyStream" code:len userInfo:info]];
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
                                
                                _bodySended += len;
                                //_bodySended += 2;
#if 0
                                NSLog(@"chunk with %d bytes sent\nHeader: %s", len, chunkLen);
#endif
                                if(len != 0)
                                {
                                    return;	// more to send (at least a 0-length header)                                    
                                }
                                    
//                                else
//                                {
//                                    [_outputStream write:(unsigned char *) "0\r\n" maxLength:3];
//                                    return;
//                                }
                            }
                            else
                            {
                                [self sendData:buffer length:len];
                                
                                _bodySended += len;
                                
//                                int sendCount = [_outputStream write:buffer maxLength:len];	// send what we have
//#if 1
//                                //NSLog(@"%d bytes body sent for request ", len, _currentRequest);
//#endif
//                                if(sendCount <0)
//                                {
//#if 1
//                                    NSLog(@"error while writing for SvnSocketOutputStream stream %s", strerror(errno));
//#endif
//                                    NSDictionary *info=[NSDictionary
//                                                        dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:strerror(errno)], @"Error",nil];
//                                    [_currentRequest didFailWithError:[NSError errorWithDomain:@"SvnSocketOutputStream" code:errno userInfo:info]];
//                                    [self endOfUseability];
//                                }
//                                else
//                                {
//                                    _bodySended += sendCount;
//                                }
                                
                                return;	// done
                            }
                        }
                    }
                    
                    if(_sendChunked)
                    {
                        //[_outputStream write:(unsigned char *) "0\r\n\r\n" maxLength:5];
                        [self sendData:(unsigned char *) "0\r\n\r\n" length:5];
                        
                    }
#if 1
                    NSLog(@"body completely sent, total length:%d", _bodySended);
#endif
                    [_bodyStream close];	// close body stream (if open)
                    //[_bodyStream release];
                    _bodyStream=nil;
                }
				_headerStream=nil;
			}
            
            
            //if(_shouldClose)
			//{	// we have announced Connection: close
//#if 1
				//NSLog(@"can't keep connection alive because we announced Connection: close");
//#endif
				//[_outputStream close];
				//[_outputStream release];
				//_outputStream=nil;
			//}
            //else
                //[_outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];	// unschedule until we send the next request
            return;
		}
		case NSStreamEventEndEncountered:
            if([_headerStream hasBytesAvailable])
                [_currentRequest didFailWithError:[NSError errorWithDomain:@"connection closed by server while sending header" code:errno userInfo:nil]];
            else if([_bodyStream hasBytesAvailable])
                [_currentRequest didFailWithError:[NSError errorWithDomain:@"connection closed by server while sending body" code:errno userInfo:nil]];
            else
			{
#if 1
                NSLog(@"server has disconnected - can't keep alive: %@", self);
#endif
			}
            [_headerStream close];
            //[_headerStream release];	// done sending header
            _headerStream=nil;
            [_bodyStream close];	// close body stream (if open)
            //[_bodyStream release];
            _bodyStream=nil;
            [self endOfUseability];
            return;
		default:
            break;
	}
	NSLog(@"An error %@ occurred on the event %08x of stream %@ of %@", [_outputStream streamError], event, _outputStream, self);
	[_currentRequest didFailWithError:[_outputStream streamError]];
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
        [_currentRequest didFailWithError:[NSError errorWithDomain:@"receive error" code:-1 userInfo:info]];
        [self endOfUseability];
        return;
    }
    
	if(stream == _inputStream) 
		[self handleInputEvent:event];
	else if(stream == _outputStream)
		[self handleOutputEvent:event];
}

@end

@implementation SvnHttpURLProtocol

+ (BOOL) canInitWithRequest:(NSURLRequest *) request
{
	NSString *scheme = [[request URL] scheme];
	return [scheme isEqualToString:@"http"] || [scheme isEqualToString:@"https"];;
}

+ (NSURLRequest *) canonicalRequestForRequest:(NSURLRequest *) request
{
	NSURL *url=[request URL];
	NSString *frag=[url fragment];
	if([frag length] > 0)
    { // map different fragments to same base file
        NSString *s=[url absoluteString];
        s=[s substringToIndex:[s length]-[frag length]];	// remove fragment
        return [[NSURLRequest alloc] initWithURL:[NSURL URLWithString:s]] ;
    }
	return request;
}

- (void) dealloc
{
#if 0
	NSLog(@"dealloc %@", self);
#endif
	[self stopLoading];		// if still running
	//[super dealloc];
}

- (NSString *) _uniqueKey
{ // all requests with the same uniqueKey *can* be multiplexed over a kept-alive HTTP 1.1 channel
	
    NSURL *url=[[self request] URL];
	return [NSString stringWithFormat:@"%@://%@:%@", [url scheme], [url host], [url port]];	// we can ignore user&password since HTTP does
}

- (void) _setConnection:(SvnHttpSerialization *) c { _connection=c; }	// our shared connection
- (SvnHttpSerialization *) _connection { return _connection; }

- (void) _restartLoading
{
#if 1
	NSLog(@"_restartLoading %@", self);
#endif
	[_connection stopLoading:self];	// remove from current queue
	[[SvnHttpSerialization serializerForProtocol:self] startLoading:self];	// and reschedule (on same or other some other queue)
}

- (void) startLoading
{
    //NSLog(@"startLoading:%@", self);
	static NSDictionary *methods;
	if(_connection)
    {
        NSLog(@"_connection:%@, aleady queued", _connection);
		return;	// already queued
    }
	if(!methods)
    { // initialize
        methods=[[NSDictionary alloc] initWithObjectsAndKeys:
                 self, @"HEAD",
                 self, @"GET",
                 self, @"POST",
                 self, @"PUT",
                 self, @"DELETE",
                 self, @"TRACE",
                 self, @"OPTIONS",
                 self, @"CONNECT",
                 nil];
    }
	if(![methods objectForKey:[[self request] HTTPMethod]])
    { // unknown method
        NSLog(@"Invalid HTTP Method: %@", [self request]);
        NSLog(@"URLProtocol client didFailWithError");
        [[self client] URLProtocol:self didFailWithError:[NSError errorWithDomain:@"Invalid HTTP Method" code:0 userInfo:nil]];
        return;
    }

	[[SvnHttpSerialization serializerForProtocol:self] startLoading:self];	// add our request to (new) queue
    //NSLog(@"URLProtocol client didReceiveResponse");
    //[[self client] URLProtocol:self didReceiveResponse:[[NSURLResponse alloc] init] cacheStoragePolicy:NSURLCacheStorageNotAllowed];
}

- (void) stopLoading
{
	[_connection stopLoading:self];	// interrupt and/or remove us from the queue
}

- (void) didFailWithError:(NSError *) error
{ // forward to client as last message...
    //NSLog(@"URLProtocol client didFailWithError");
	[[self client] URLProtocol:self didFailWithError:error];
	//[(NSObject *)_client release];
	//_client=nil;
}

- (void) didLoadData:(NSData *) data
{ // forward to client

    //NSLog(@"URLProtocol client didLoadData");
	[[self client] URLProtocol:self didLoadData:data];
}

- (void) didFinishLoading
{ // forward to client as last message...
    //NSLog(@"SvnHttpURLProtocol didFinishLoading");
	[[self client] URLProtocolDidFinishLoading:self];
	//[(NSObject *)_client release];
	//_client=nil;
}

- (void) didReceiveResponse:(NSHTTPURLResponse *) response
{
	NSDictionary *headers=[response allHeaderFields];
	NSString *loc;
    
	switch([response statusCode])
	{
		case 100:
            return;	// continue - ignore
		case 401:
		{
            // FIXME: read auth challenge from HTTP headers
            NSURLCredential *_credential;
            NSURLProtectionSpace *space;
            NSString *hdr;
            NSURL *url;
            int failures = 0;
            
            hdr = [headers objectForKey:@"WWW-Authenticate"];
            url = [[self request] URL];
            space = [[NSURLProtectionSpace alloc] initWithHost:[url host] port:[url port] protocol:@"http" realm:nil authenticationMethod:nil];
            
            if (space != nil)
			{
                /* Create credential from user and password
                 * stored in the URL.
                 * Returns nil if we have no username or password.
                 */
                _credential = [[NSURLCredential alloc]
                               initWithUser: [url user]
                               password: [url password]
                               persistence: NSURLCredentialPersistenceForSession];
                if (_credential == nil)
			    {
                    /* No credential from the URL, so we try using the
                     * default credential for the protection space.
                     */
                    _credential = [[NSURLCredentialStorage sharedCredentialStorage]
                            defaultCredentialForProtectionSpace: space];
			    }
			}
            
            if (_challenge != nil)
            {
                /* The failure count is incremented if we have just
                 * tried a request in the same protection space.
                 */
                if (YES == [[_challenge protectionSpace] isEqual: space])
                {
                    failures = [_challenge previousFailureCount] + 1;
                }
            }
            else if ([[self request] valueForHTTPHeaderField:@"Authorization"])
            {
                /* Our request had an authorization header, so we should
                 * count that as a failure or we wouldn't have been
                 * challenged.
                 */
                failures = 1;
            }

            _challenge = [[NSURLAuthenticationChallenge alloc] initWithProtectionSpace:space proposedCredential:_credential previousFailureCount:failures failureResponse:response error:nil sender:self];
            NSLog(@"URLProtocol client didReceiveAuthenticationChallenge:%@", _challenge);
            [[self client] URLProtocol:self didReceiveAuthenticationChallenge:_challenge];
            
            if (_challenge == nil)
			{
                NSError	*e;
                
                /* The client cancelled the authentication challenge
                 * so we must cancel the download.
                 */
                e = [NSError errorWithDomain: @"Authentication cancelled"
                                        code: 0
                                    userInfo: nil];
                [self stopLoading];
                [[self client] URLProtocol: self
                         didFailWithError: e];
                return;
			}
                       // retry or abort?
            break;
		}
		case 407:
            // notify client and add authentication info + repeat
            break;
		case 503:	// retry
            // check if within reasonable future (retry-after) and then repeat
            break;
            // case 206:	// optional
		case 304:
            NSLog(@"URLProtocol client cachedResponseIsValid");
            //[[self client] URLProtocol:self cachedResponseIsValid:_cachedResponse];	// will get data from cache
            NSLog(@"URLProtocol client didLoadData");
            //[[self client] URLProtocol:self didLoadData:[_cachedResponse data]];	// and pass data from cache
            break;
        default:
            break;
	}
	if(([response statusCode]/100 == 3) && (loc=[headers objectForKey:@"Location"]))
    { // redirect
        NSLog(@"redirect location:%@", loc);
        NSMutableURLRequest *nextRequest=[NSMutableURLRequest requestWithURL:[NSURL URLWithString:loc relativeToURL:[[self request] URL]]];	// may be relative to current URL
       
        //NSDictionary *fields = [response allHeaderFields];
        //NSString *cookie = [fields valueForKey:@"Set-Cookie"];
        //[nextRequest addValue:cookie forHTTPHeaderField:@"Cookie"];
        //NSLog(@"URLProtocol client wasRedirectedToRequest");
        [[self client] URLProtocol:self wasRedirectedToRequest:nextRequest redirectResponse:response];	// this may trigger a retry for the new request
        return;
    }
	// FIXME: there are response-headers that control how the response should be cached, i.e. translate into cacheStoragePolicy
    //NSLog(@"URLProtocol client didReceiveResponse");
	[[self client] URLProtocol:self didReceiveResponse:response cacheStoragePolicy:0];	// notify client
	
	/* how do we generate these:
	 - (void) URLProtocol:(NSURLProtocol *) proto didCancelAuthenticationChallenge:(NSURLAuthenticationChallenge *) chall;
	 */
	
}


@end


