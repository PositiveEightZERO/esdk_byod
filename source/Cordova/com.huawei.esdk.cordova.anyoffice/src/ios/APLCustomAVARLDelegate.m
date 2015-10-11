/*
 
 
     File: APLCustomAVARLDelegate.m
 Abstract: Custom delegate class implementation. 
  Version: 1.0
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2014 Apple Inc. All Rights Reserved.
 
 
 */
#import "APLCustomAVARLDelegate.h"
#import <MobileCoreServices/MobileCoreServices.h>
#import <SvnSdk/SecBrowHttpProtocol.h>
#import "SvnResourceLoadOperation.h"


@interface APLCustomAVARLDelegate ()
{
    
}







@property (strong, nonatomic) NSMutableArray * pendingRequests;
@property (strong, nonatomic) NSMutableArray * currentOperations;
//@property (strong, nonatomic) NSURLConnection * infoConnection;
//@property (strong, nonatomic) NSURLConnection * dataConnection;
@property (strong, nonatomic) NSHTTPURLResponse * response;
//@property (strong, nonatomic) NSMutableData * songData;

@property (strong, nonatomic) NSMutableDictionary * connectionData;

- (void) reportError:(AVAssetResourceLoadingRequest *) loadingRequest withErrorCode:(int) error;

@end

#pragma mark - APLCustomAVARLDelegate

@implementation APLCustomAVARLDelegate


-(APLCustomAVARLDelegate *) init
{
    self = [super init];
        if(self)
    {
        self.pendingRequests = [NSMutableArray arrayWithCapacity:5];
        self.connectionData = [[NSMutableDictionary alloc] initWithCapacity:5];
        self.currentOperations = [NSMutableArray arrayWithCapacity:5];
    }
    return self;
}

-(void)cancel
{
     NSLog(@"in delegate cancel request");
    @synchronized(self)
    {
//        for (AVAssetResourceLoadingRequest * request in self.pendingRequests) {
//            
//            [request finishLoading];
//        }

        for(SvnResourceLoadOperation *operation  in [self.currentOperations copy])
        {
            [operation cancel];
        }
        //NSLog(@"add pendingRequest:%@", loadingRequest);
        //[self.pendingRequests addObject:loadingRequest];
        
    }
}

- (void) reportError:(AVAssetResourceLoadingRequest *) loadingRequest withErrorCode:(int) error
{
    [loadingRequest finishLoadingWithError:[NSError errorWithDomain: NSURLErrorDomain code:error userInfo: nil]];
}



- (BOOL)resourceLoader:(AVAssetResourceLoader *)resourceLoader shouldWaitForLoadingOfRequestedResource:(AVAssetResourceLoadingRequest *)loadingRequest
{
    NSLog(@"shouldWaitForLoadingOfRequestedResource:%@",loadingRequest);

    SvnResourceLoadOperation *operation = [[SvnResourceLoadOperation alloc] initWithRequest:loadingRequest];
    operation.delegate = self;
    
    
    [self.currentOperations addObject:operation];
    
    [operation start];
    
    @synchronized(self)
    {
        NSLog(@"add pendingRequest:%@", loadingRequest);
        [self.pendingRequests addObject:loadingRequest];
        
    }
    NSLog(@"shouldWaitForLoadingOfRequestedResource:%@ returns",loadingRequest);
    return YES;
}

- (void)resourceLoader:(AVAssetResourceLoader *)resourceLoader didCancelLoadingRequest:(AVAssetResourceLoadingRequest *)loadingRequest
{
    NSLog(@"cancel pendingRequest:%@", loadingRequest);
    @synchronized(self.pendingRequests)
    {
        [self.pendingRequests removeObject:loadingRequest];
    }
}


- (void)operation:(SvnResourceLoadOperation*)operation didReceiveResponse:(NSURLResponse *)response
{
    AVAssetResourceLoadingRequest *request = operation.request;
    
    [self fillInContentInformation:request.contentInformationRequest withResponse:(NSHTTPURLResponse *)response];
    
}
- (void)operation:(SvnResourceLoadOperation*)operation didReceiveData:(NSData *)data
{
    AVAssetResourceLoadingRequest *request = operation.request;
    
    //BOOL respondFully = [self respondWithData:data forRequest:[request.dataRequest] ];
    
    [request.dataRequest respondWithData:data];
    long long endOffset = request.dataRequest.requestedOffset + request.dataRequest.requestedLength;
    BOOL didRespondCompletely = request.dataRequest.currentOffset>= endOffset;

    
    if (didRespondCompletely)
    {
        NSLog(@"didRespondCompletely -------------%d", didRespondCompletely);
        //self.dataConnection = nil;
        //[requestsCompleted addObject:loadingRequest];

        [request finishLoading];
        [self.pendingRequests removeObject:request];
    }
}
- (void)operationDidFinishLoading:(SvnResourceLoadOperation*)operation
{

    AVAssetResourceLoadingRequest *request = operation.request;
    
    //BOOL respondFully = [self respondWithData:data forRequest:[request.dataRequest] ];
    long long endOffset = request.dataRequest.requestedOffset + request.dataRequest.requestedLength;
    BOOL didRespondCompletely = request.dataRequest.currentOffset>= endOffset;
    if(!didRespondCompletely)
    {
        NSLog(@"operationDidFinishLoading but data remains");
         [request finishLoadingWithError:nil];
    }
    else
    {
        if (!request.isFinished)
        {
            
            //self.dataConnection = nil;
            //[requestsCompleted addObject:loadingRequest];
            
            [request finishLoading];
            [self.pendingRequests removeObject:request];
            
        }
    }
    
    
    
}
- (void)operation:(SvnResourceLoadOperation*)operation didFailWithError:(NSError *)error
{
    AVAssetResourceLoadingRequest *request = operation.request;
    
    [request finishLoadingWithError:error];
    
    [self.pendingRequests removeObject:request];
}



//- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
//{
//    NSLog(@"didReceiveResponse-------------");
//    
//    NSMutableData *songData = [NSMutableData data];
//    
//    [self.connectionData setObject:songData forKey:connection];
//    
//    
//    self.response = (NSHTTPURLResponse *)response;
//    
//    [self processPendingRequests];
//}
//
//- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
//{
//    NSMutableData *songData
//    [songData appendData:data];
//    
//    NSLog(@"didReceiveData-------------total:%ul", (unsigned long)self.songData.length);
//    
//    [self processPendingRequests];
//}
//
//- (void)connectionDidFinishLoading:(NSURLConnection *)connection
//{
//    [self processPendingRequests];
//    NSLog(@"connectionDidFinishLoading-------------");
//    // Done loading, cache the file locally
////    NSString *cachedFilePath = [NSTemporaryDirectory() stringByAppendingPathComponent:@"cached.mp4"];
////    [self.songData writeToFile:cachedFilePath atomically:YES];
//}
//
//
//- (void)connection:(NSURLConnection *)theConnection didFailWithError:(NSError *)error
//// A delegate method called by the NSURLConnection if the connection fails.
//// We shut down the connection and display the failure.  Production quality code
//// would either display or log the actual error.
//{
//    NSLog(@"connection didFailWithError-------------%@", error);
//}



- (void)processPendingRequests
{
    
        
        
//        NSMutableArray *requestsCompleted = [NSMutableArray array];
//   
//        for (AVAssetResourceLoadingRequest *loadingRequest in [self.pendingRequests copy])
//        {
//            //NSLog(@"to fillInContentInformation -------------");
//            [self fillInContentInformation:loadingRequest.contentInformationRequest ];
//            
//            BOOL didRespondCompletely = [self respondWithDataForRequest:loadingRequest.dataRequest];
//            
//            
//            if (didRespondCompletely)
//            {
//                NSLog(@"didRespondCompletely -------------%d", didRespondCompletely);
//                //self.dataConnection = nil;
//                [requestsCompleted addObject:loadingRequest];
//                
//                [loadingRequest finishLoading];
//            }
//        }
//    
//   
//        if([requestsCompleted count] > 0)
//        {
//            @synchronized(self.pendingRequests)
//            {
//                NSLog(@"remove pendingRequests:%@", requestsCompleted);
//                [self.pendingRequests removeObjectsInArray:requestsCompleted];
//            }
//    
//       
//        
//        }
}

- (void)fillInContentInformation:(AVAssetResourceLoadingContentInformationRequest *)contentInformationRequest withResponse:(NSHTTPURLResponse *) response
{
    if (contentInformationRequest == nil || response == nil)
    {
        return;
    }
    
    NSString *mimeType = [response MIMEType];
    CFStringRef contentType = UTTypeCreatePreferredIdentifierForTag(kUTTagClassMIMEType, (__bridge CFStringRef)(mimeType), NULL);
    
    contentInformationRequest.byteRangeAccessSupported = YES;
    contentInformationRequest.contentType = CFBridgingRelease(contentType);
    contentInformationRequest.contentLength = [response expectedContentLength];
    
    long long contentLength = 0;
    NSString * contentRange = [[response allHeaderFields] objectForKey:@"Content-Range"];
    if(contentRange)
    {
        NSRange loc = [contentRange rangeOfString:@"/"];
        if(loc.location != NSNotFound)
        {
            contentLength = [[contentRange substringFromIndex:loc.location + 1] longLongValue];
        }
    }
    
    if(contentLength != 0)
    {
        contentInformationRequest.contentLength = contentLength;
        NSLog(@"total contentLength:%lld, real:%lld", contentLength, [response expectedContentLength]);
    }
    
}

//- (BOOL)respondWithDataForRequest:(AVAssetResourceLoadingDataRequest *)dataRequest
//{
//    long long startOffset = dataRequest.requestedOffset;
//    if (dataRequest.currentOffset != 0)
//    {
//        startOffset = dataRequest.currentOffset;
//    }
//    
//    // Don't have any data at all for this request
//    if (self.songData.length = endOffset;
//        
//        return didRespondFully;
//}

        
//- (BOOL)respondWithData:(NSData *)responseData forRequest:(AVAssetResourceLoadingDataRequest *)dataRequest
//{
//    
////    NSLog(@"dataRequest:%@", dataRequest);
////    long long startOffset = dataRequest.requestedOffset;
////    if (dataRequest.currentOffset != 0){
////        startOffset = dataRequest.currentOffset;
////    }
////    // Don't have any data at all for this request
////    if (responseData.length < startOffset){
////        return NO;
////    }
////    // This is the total data we have from startOffset to whatever has been downloaded so far
////    NSUInteger unreadBytes = responseData.length - startOffset;
////    // Respond with whatever is available if we can't satisfy the request fully yet
////    NSUInteger numberOfBytesToRespondWith = MIN(dataRequest.requestedLength, unreadBytes);
////    BOOL didRespondFully = NO;
////    //NSData *data = [self readCachedData:startOffset length:numberOfBytesToRespondWith];
////    
////    NSRange rang = NSMakeRange(startOffset, numberOfBytesToRespondWith);
////    NSData *data = [responseData subdataWithRange:rang];
////    if(data){
////        [dataRequest respondWithData:data];
////        long long endOffset = startOffset + dataRequest.requestedLength;
////        didRespondFully = responseData.length >= endOffset;
////    }
////    //NSLog(@"didRespondFully:%d", didRespondFully);
////    return didRespondFully;
//    
//    [dataRequest respondWithData:responseData];
//    long long endOffset = dataRequest.requestedOffset + dataRequest.requestedLength;
//    BOOL didRespondFully = NO;
//    didRespondFully = dataRequest.currentOffset>= endOffset;
//    
//    return  didRespondFully;
//}


@end
