//
//  SvnResourceLoadOperation.m
//  BYOD
//
//  Created by l00174413 on 15/4/22.
//
//

#import "SvnResourceLoadOperation.h"

@implementation SvnResourceLoadOperation

-(instancetype)initWithRequest:(AVAssetResourceLoadingRequest *)request
{
    self = [super init];
    self.request = request;
    executing = NO;
    
    finished = NO;
    return self;
}

- (BOOL)isConcurrent {
    
    return YES;
    
}



- (BOOL)isExecuting {
    
    return executing;
    
}



- (BOOL)isFinished {
    
    return finished;
    
}

-(void)start
{
    // Always check for cancellation before launching the task.
    
    if ([self isCancelled])
        
    {
        
        // Must move the operation to the finished state if it is canceled.
        
        [self willChangeValueForKey:@"isFinished"];
        
        finished = YES;
        
        [self didChangeValueForKey:@"isFinished"];
        
        return;
        
    }
    
    
    // If the operation is not canceled, begin executing the task.
    
    [self willChangeValueForKey:@"isExecuting"];
    
    [NSThread detachNewThreadSelector:@selector(main) toTarget:self withObject:nil];
    
    executing = YES;
    
    [self didChangeValueForKey:@"isExecuting"];
}


- (void)main {
    
    @try {
        
        
    NSURL *interceptedURL = [self.request.request URL];
    NSURLComponents *actualURLComponents = [[NSURLComponents alloc] initWithURL:interceptedURL resolvingAgainstBaseURL:NO];
    actualURLComponents.scheme = @"http";
    
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[actualURLComponents URL] cachePolicy:NSURLRequestReloadIgnoringLocalCacheData timeoutInterval:20];
    NSDictionary * allHeaders = [self.request.request allHTTPHeaderFields];
    
    //    if(loadingRequest.contentInformationRequest)
    //    {
    //
    for (NSString *key in [allHeaders allKeys]) {
        [request setValue:allHeaders[key]  forHTTPHeaderField:key];
    }
    //    }
    //
    //    else
    //    {
    //        for (NSString *key in [allHeaders allKeys]) {
    //            if(![key isEqualToString:@"Range"] )
    //            {
    //                [request setValue:allHeaders[key]  forHTTPHeaderField:key];
    //           }
    //
    //        }
    //    }
    
        request.HTTPShouldUsePipelining = YES;
    self.dataConnection = [[NSURLConnection alloc] initWithRequest:request delegate:self startImmediately:NO];
    
    NSOperationQueue *queue = [[NSOperationQueue alloc]init];
    //[dataConnection setDelegateQueue:[NSOperationQueue mainQueue]];
    [self.dataConnection setDelegateQueue:queue];
    

    
//    NSPort* port = [NSPort port];
//    NSRunLoop* rl = [NSRunLoop currentRunLoop]; // Get the runloop
//    [rl addPort:port forMode:NSDefaultRunLoopMode];
//    [self.dataConnection scheduleInRunLoop:rl forMode:NSDefaultRunLoopMode];
    [self.dataConnection start];

        
    }
    @catch(...) {
        
        // Do not rethrow exceptions.
        
    }
}


-(void)cancel
{
    NSLog(@"operation %p cancel, dataconnection:%@", self, self.dataConnection);
    [self.dataConnection cancel];
    [self completeOperation];
}


- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    NSLog(@"didReceiveResponse-------------:%p", self.request);
    [self.delegate operation:self didReceiveResponse:response];
    self.length = 0;
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    self.length += [data length];
    //NSLog(@"didReceiveData-------------total: %llu, len:%zd, :%p", self.length, [data length], self.request);
   [self.delegate operation:self didReceiveData:data];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    NSLog(@"connectionDidFinishLoading-------------:%p", self.request);
    [self.delegate operationDidFinishLoading:self];
    [self completeOperation];
}


- (void)connection:(NSURLConnection *)theConnection didFailWithError:(NSError *)error
// A delegate method called by the NSURLConnection if the connection fails.
// We shut down the connection and display the failure.  Production quality code
// would either display or log the actual error.
{
    NSLog(@"connection didFailWithError-------------%@", error);
    [self.delegate operation:self didFailWithError:error];
    [self completeOperation];
}


- (void)completeOperation {
    
    [self willChangeValueForKey:@"isFinished"];
    
    [self willChangeValueForKey:@"isExecuting"];
    
    
    
    executing = NO;
    
    finished = YES;
    
    
    
    [self didChangeValueForKey:@"isExecuting"];
    
    [self didChangeValueForKey:@"isFinished"];
    
}

@end
