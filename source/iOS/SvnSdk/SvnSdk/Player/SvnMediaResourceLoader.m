//
//  SvnMediaResourceLoader.m
//  ResourceLoader
//
//  Created by Artem Meleshko on 1/31/15.
//  Copyright (c) 2015 LeshkoApps ( http://leshkoapps.com ). All rights reserved.
//


#import "SvnMediaResourceLoader.h"
#import "SvnMediaResourceLoaderDataResponse.h"
#import <MobileCoreServices/MobileCoreServices.h>
#import "NSObject+SvnAdditions.h"
//#import "NSString+LSAdditions.h"
#import "SvnMediaResourceLoaderContentInformation.h"

#import "SvnResourceLoadOperation.h"


NSString * const LSFilePlayerResourceLoaderErrorDomain = @"LSFilePlayerResourceLoaderErrorDomain";

@interface SvnMediaResourceLoader()

@property (nonatomic,strong)NSMutableArray *pendingRequests;
@property (nonatomic,strong)NSMutableArray * currentOperations;

@property (nonatomic,strong)NSURL *resourceURL;
@property (nonatomic,assign)BOOL isCancelled;
@property (nonatomic,copy)NSString *path;
@property (nonatomic,strong)SvnMediaResourceLoaderContentInformation *contentInformation;

@end


@implementation SvnMediaResourceLoader


- (instancetype)initWithResourceURL:(NSURL *)url
{
    self = [super init];
    if(self){
        self.resourceURL = url;
        self.path = [url.absoluteString stringByRemovingPercentEncoding];

        self.isCancelled = NO;
        self.pendingRequests = [[NSMutableArray alloc] init];
        self.currentOperations = [NSMutableArray arrayWithCapacity:5];
    }
    return self;
}

- (void)dealloc{
    //[self complete];
}

- (NSArray *)requests{
    return self.pendingRequests;
}

- (void)addRequest:(AVAssetResourceLoadingRequest *)loadingRequest{
    @synchronized(self)
    {
    
    [self cancelAllPendingRequests];
    
    
    if(self.isCancelled==NO){
        NSURL *interceptedURL = [loadingRequest.request URL];
        //NSAssert([interceptedURL.absoluteString isEqualToString:self.resourceURL.absoluteString], @"Trying to add request with incorrect URL");
        //[self startOperationFromOffset:loadingRequest.dataRequest.requestedOffset length:loadingRequest.dataRequest.requestedLength];
        if([interceptedURL.absoluteString isEqualToString:self.resourceURL.absoluteString])
        {
            SvnResourceLoadOperation *operation = [[SvnResourceLoadOperation alloc] initWithRequest:loadingRequest];
            operation.delegate = self;
            
            
            [self.currentOperations addObject:operation];
            
            [operation start];
            
            [self.pendingRequests addObject:loadingRequest];
        }
        else
        {
            NSLog(@"Trying to add request with incorrect URL");
        }
        
        
    }
    else{
        NSAssert(NO, @"Trying to add request while resource loader isCancelled");
        if(loadingRequest.isFinished==NO){
            [loadingRequest finishLoadingWithError:[self loaderCancelledError]];
        }
    }
    }
}

- (void)removeRequest:(AVAssetResourceLoadingRequest *)loadingRequest{
    
    //loadingRequest
    //[self.pendingRequests removeObject:loadingRequest];
    [self processPendingRequests];
}

- (void)cancelAllPendingRequests{
    
    //[self cancelOperations];
    
    
    for(AVAssetResourceLoadingRequest *pendingRequest in self.pendingRequests){
        if(pendingRequest.isFinished==NO){
            [pendingRequest finishLoadingWithError:[self loaderCancelledError]];
        }
    }
    //[self.pendingRequests removeAllObjects];
    
    [self processPendingRequests];
    
    
}

- (void)cancel{
    self.isCancelled = YES;
    
    [self cancelAllPendingRequests];
    
}



- (NSError *)loaderCancelledError{
    NSError *error = [[NSError alloc] initWithDomain:LSFilePlayerResourceLoaderErrorDomain
                                                code:-3
                                            userInfo:@{NSLocalizedDescriptionKey:@"Resource loader cancelled"}];
    return error;
}

- (void)cancelOperations{

    for(SvnResourceLoadOperation *operation  in [self.currentOperations copy])
    {
        [operation cancel];
    }
    
    
    [self.currentOperations removeAllObjects];


}


#pragma mark - Data Load Callback`s



- (void)processPendingRequests{
    
    @synchronized(self)
    {
        NSMutableArray *requestsCompleted = [[NSMutableArray alloc] init];
        for (AVAssetResourceLoadingRequest *loadingRequest in self.pendingRequests){
            [self fillInContentInformation:loadingRequest.contentInformationRequest];
         
            if(loadingRequest.isFinished || loadingRequest.isCancelled)
            {
                [requestsCompleted addObject:loadingRequest];
            }
        }
        [self.pendingRequests removeObjectsInArray:requestsCompleted];
        
        
        [requestsCompleted removeAllObjects];
        
        
        for (SvnResourceLoadOperation *operation  in self.currentOperations){

            if(operation.currentRequest.isFinished)
            {
                [requestsCompleted addObject:operation];
            }
        }
        [self.currentOperations removeObjectsInArray:requestsCompleted];
        
    }
}



- (void)fillInContentInformation:(AVAssetResourceLoadingContentInformationRequest *)contentInformationRequest{
    if (contentInformationRequest == nil || self.contentInformation == nil){
        return;
    }
    
    contentInformationRequest.byteRangeAccessSupported = self.contentInformation.byteRangeAccessSupported;
    contentInformationRequest.contentType = self.contentInformation.contentType;
    contentInformationRequest.contentLength = self.contentInformation.contentLength;
    
    NSLog(@"fillInContentInformation contentLength:%lld, contentType:%@", contentInformationRequest.contentLength, contentInformationRequest.contentType);
}

- (void)processPendingRequestsWithError:(NSError *)error{
    for (AVAssetResourceLoadingRequest *loadingRequest in self.pendingRequests){
        if(loadingRequest.isFinished==NO){
            [loadingRequest finishLoadingWithError:error];
        }
    }
    [self.pendingRequests removeAllObjects];
}

- (void)completeWithError:(NSError *)error{
    [self processPendingRequestsWithError:error];
    
    [self performBlockOnMainThreadAsync:^{
        if([self.delegate respondsToSelector:@selector(resourceLoader:didFailWithError:)]){
            [self.delegate resourceLoader:self didFailWithError:error];
        }
    }];
}

#pragma mark - Resource Load Operation Delegate


- (void)operationdidFinishLoading:(SvnResourceLoadOperation *)operation
{
    NSLog(@"operationdidFinishLoading:%@", operation);
    //[self.currentOperations removeObject:operation];
    
    [self processPendingRequests];
}

- (void)operation:(SvnResourceLoadOperation *)operation didFailwithError:(NSError *)error
{
    NSLog(@"operationdidFailwithError:%@", operation);
    //[self.currentOperations removeObject:operation];
    
    //[operloadingRequest finishLoadingWithError:error];
    
    [self processPendingRequests];
}

- (void)operation:(SvnResourceLoadOperation *)operation didReceiveContentInformation:(SvnMediaResourceLoaderContentInformation *)info
{
    NSLog(@"didReceiveContentInformation:%@", info);
    self.contentInformation = info;
    [self processPendingRequests];
}




@end
