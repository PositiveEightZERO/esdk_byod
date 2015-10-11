//
//  SvnHttpURLProtocol.h
//  HTTP协议处理类
//
//  Created by l00174413 on 13-6-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

@class SvnHttpSerialization;

@interface SvnHttpURLProtocol : NSURLProtocol
{
    NSCachedURLResponse *_cachedResponse;
    SvnHttpSerialization /* nonretained */ *_connection;	// where we have been queued up
	//NSMutableArray *_runLoops;				// additional runloops to schedule
	//NSMutableArray *_modes;						// additional modes to schedule
    
    NSURLAuthenticationChallenge *_challenge;

}

- (NSString *) _uniqueKey;	// a key to identify the same server connection

- (void) _setConnection:(SvnHttpSerialization *) connection;
- (SvnHttpSerialization *) _connection;

- (void) didFailWithError:(NSError *) error;
- (void) didLoadData:(NSData *) data;
- (void) didFinishLoading;
- (void) didReceiveResponse:(NSHTTPURLResponse *) response;

@end
