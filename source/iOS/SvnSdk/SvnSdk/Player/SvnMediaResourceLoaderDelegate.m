/*
 
 
     File: SvnMediaResourceLoaderDelegate.m
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
#import "SvnMediaResourceLoaderDelegate.h"
#import <MobileCoreServices/MobileCoreServices.h>

#import "SvnMediaResourceLoader.h"

#import "NSObject+SvnAdditions.h"

@interface SvnMediaResourceLoaderDelegate ()<AVAssetResourceLoaderDelegate>
{
     dispatch_queue_t resourceLoadQueue;
}


/**
 * 每个URL对应一个Resouce Loader
 */
@property (nonatomic,strong)NSMutableDictionary *resourceLoaders;

//@property (nonatomic,strong)NSMutableArray *resourceLoaders;

//@property (strong, nonatomic) NSMutableArray * pendingRequests;
//@property (strong, nonatomic) NSMutableArray * currentOperations;
//@property (strong, nonatomic) NSURLConnection * infoConnection;
//@property (strong, nonatomic) NSURLConnection * dataConnection;
//@property (strong, nonatomic) NSHTTPURLResponse * response;
//@property (strong, nonatomic) NSMutableData * songData;

//@property (strong, nonatomic) NSMutableDictionary * connectionData;

- (void) reportError:(AVAssetResourceLoadingRequest *) loadingRequest withErrorCode:(int) error;

@end

#pragma mark - SvnMediaResourceLoaderDelegate

@implementation SvnMediaResourceLoaderDelegate


-(SvnMediaResourceLoaderDelegate *) init
{
    self = [super init];
    if(self)
    {
        self.resourceLoaders = [[NSMutableDictionary alloc] initWithCapacity:5];
        resourceLoadQueue = dispatch_queue_create("com.huawei.esdk.anyoffice.resource.queue", NULL);
    }
    return self;
}

-(void)cancel
{
    NSLog(@"in delegate cancel request");
    [self cancelAllResourceLoaders];
   
    
    
    
//    NSArray *items = [self.resourceLoaders copy];
//    [self.resourceLoaders removeAllObjects];
//    for(SvnMediaResourceLoader *loader in items){
//        [loader cancel];
//    }
    
    
//    @synchronized(self)
//    {
//        for (AVAssetResourceLoadingRequest * request in [self.pendingRequests copy]) {
//            
//            [request finishLoadingWithError:[NSError errorWithDomain:@"request canceled " code:0 userInfo:nil]];
//        }
//
//        for(SvnMediaResourceLoadOperation *operation  in [self.currentOperations copy])
//        {
//            [operation cancel];
//        }
//        //NSLog(@"add pendingRequest:%@", loadingRequest);
//        //[self.pendingRequests addObject:loadingRequest];
//        
//    }
}

- (void) reportError:(AVAssetResourceLoadingRequest *) loadingRequest withErrorCode:(int) error
{
    [loadingRequest finishLoadingWithError:[NSError errorWithDomain: NSURLErrorDomain code:error userInfo: nil]];
}



#pragma mark - AVAssetResourceLoaderDelegate

- (BOOL)resourceLoader:(AVAssetResourceLoader *)resourceLoader shouldWaitForLoadingOfRequestedResource:(AVAssetResourceLoadingRequest *)loadingRequest{
    NSURL *resourceURL = [loadingRequest.request URL];
    if([resourceURL.scheme hasPrefix:@"media"]){
        SvnMediaResourceLoader *loader = [self resourceLoaderForRequest:loadingRequest];
        if(loader==nil)
        {

            loader = [[SvnMediaResourceLoader alloc] initWithResourceURL:resourceURL];
            
            
            NSLog(@"loader inited:%@", loader);
            
            
            loader.delegate = self;
            [self.resourceLoaders setObject:loader forKey:[self keyForResourceLoaderWithURL:resourceURL]];
            //[self.resourceLoaders addObject:loader];
        }
        [loader addRequest:loadingRequest];
        return YES;
    }
    return NO;
}

- (void)resourceLoader:(AVAssetResourceLoader *)resourceLoader didCancelLoadingRequest:(AVAssetResourceLoadingRequest *)loadingRequest{
    SvnMediaResourceLoader *loader = [self resourceLoaderForRequest:loadingRequest];
    [loader removeRequest:loadingRequest];
}


#pragma mark - SvnMediaResourceLoader

- (void)removeResourceLoader:(SvnMediaResourceLoader *)resourceLoader{
    id <NSCopying> requestKey = [self keyForResourceLoaderWithURL:resourceLoader.resourceURL];
    if(requestKey){
        [self.resourceLoaders removeObjectForKey:requestKey];
    }
    
}

- (void)cancelAndRemoveResourceLoaderForURL:(NSURL *)resourceURL{
    NSString* requestKey = [self keyForResourceLoaderWithURL:resourceURL];
    
//    NSMutableArray * cancleLoaders = [[NSMutableArray alloc] init];
//    
//    NSArray *items = [self.resourceLoaders copy];
//    //[self.resourceLoaders removeAllObjects];
//    for(SvnMediaResourceLoader *loader in items){
//        if ([loader.resourceURL.absoluteString isEqualToString:requestKey]) {
//            [cancleLoaders addObject:loader];
//        }
//    }
//    
//    for (SvnMediaResourceLoader *loader in cancleLoaders) {
//        //[loader cancel];
//        SvnMediaResourceLoader *loader = [self.resourceLoaders objectForKey:requestKey];
//        [self removeResourceLoader:loader];
//        [loader cancel];
//    }
    
    
    SvnMediaResourceLoader *loader = [self.resourceLoaders objectForKey:requestKey];
    [self removeResourceLoader:loader];
    [loader cancel];
}

- (void)cancelAllResourceLoaders{
    NSArray *items = [self.resourceLoaders allValues];
    [self.resourceLoaders removeAllObjects];
    for(SvnMediaResourceLoader *loader in items){
        [loader cancel];
    }
}

- (NSString *)keyForResourceLoaderWithURL:(NSURL *)requestURL{
    if([requestURL.scheme hasPrefix:@"media"]){
        NSString *s = requestURL.absoluteString;
        return s;
    }
    return nil;
}

- (SvnMediaResourceLoader *)resourceLoaderForRequest:(AVAssetResourceLoadingRequest *)loadingRequest{
    NSURL *interceptedURL = [loadingRequest.request URL];
    if([interceptedURL.scheme hasPrefix:@"media"]){
        id <NSCopying> requestKey = [self keyForResourceLoaderWithURL:[loadingRequest.request URL]];
        SvnMediaResourceLoader *loader = [self.resourceLoaders objectForKey:requestKey];
        return loader;
    }
    return nil;
}

- (void)resourceLoader:(SvnMediaResourceLoader *)resourceLoader didFailWithError:(NSError *)error{
    [self cancelAndRemoveResourceLoaderForURL:resourceLoader.resourceURL];
}


- (void)resourceLoader:(SvnMediaResourceLoader *)resourceLoader didLoadResource:(NSURL *)url{
    //[self cancelAndRemoveResourceLoaderForURL:resourceLoader.resourceURL];
}


@end
