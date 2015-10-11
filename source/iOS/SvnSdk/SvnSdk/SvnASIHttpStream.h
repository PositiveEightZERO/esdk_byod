//
//  SvnASIHttpStream.h
//  SvnSdk
//
//  Created by l00174413 on 13-8-2.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "SvnSocketStream.h"


@interface SvnASIHttpStream : NSInputStream<NSStreamDelegate>
{
    //CFHTTPMessageRef _originRequest;
    // sending
	NSOutputStream *_outputStream;
	NSInputStream *_headerStream;	// header while sending
	//NSInputStream *_bodyStream;		// for sending the body
	BOOL _shouldClose;								// server will close after current request - we must requeue other requests on a new connection
	BOOL _sendChunked;				// sending with transfer-encoding: chunked
	// receiving
	NSInputStream *_inputStream;
	unsigned _statusCode;							// status code defined by response
	NSMutableDictionary *_headers;		// received headers
    CFHTTPMessageRef _responseHeader;
	unsigned long long _contentLength;		// if explicitly specified by header
	NSMutableString *_headerLine;			// current header line

	unsigned int _chunkLength;				// current chunk length for receiver
	char _lastChr;										// previouds character while reading header
	BOOL _readingBody;								// done with reading header
	BOOL _isChunked;									// transfer-encoding: chunked
	BOOL _willClose;									// server has announced to close the connection	
    BOOL _willRedirect;
    volatile long fd;
    
    unsigned long long _bodySended;
    
    BOOL _headerReady;
    
    BOOL _headerReceived;
    
    BOOL _bodyReceived;
    
    BOOL _trailersReceived;

}




+ (NSString *)pathForURL:(NSURL *)url;

+ (NSInputStream *)getReadStreamForStreamedHttpRequest:(CFHTTPMessageRef)request stream:(NSInputStream *) postStream;

+ (NSInputStream *)getReadStreamForHttpRequest:(CFHTTPMessageRef)request;

+ (NSString *)getHostArress:(NSString *)host;




@property (assign)CFHTTPMessageRef originRequest;

@property (retain)NSInputStream* bodyStream;

@end
