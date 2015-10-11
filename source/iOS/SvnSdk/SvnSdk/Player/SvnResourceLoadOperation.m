//
//  SvnResourceLoadOperation.m
//  SvnSdkDemo
//
//  Created by l00174413 on 8/20/15.
//
//

#import "SvnResourceLoadOperation.h"
#import "SvnMediaResourceLoaderDataResponse.h"
#import "SvnMediaResourceLoaderContentInformation.h"

#import "SvnFileHandle.h"

//#import <SvnSdk/SvnFileHandle.h>


static inline NSString * contentTypeForPathExtension(NSString *extension) {
#ifdef __UTTYPE__
    NSString *UTI = (__bridge_transfer NSString *)UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, (__bridge CFStringRef)extension, NULL);
    NSString *contentType = (__bridge_transfer NSString *)UTTypeCopyPreferredTagWithClass((__bridge CFStringRef)UTI, kUTTagClassMIMEType);
    if (!contentType) {
        return @"application/octet-stream";
    } else {
        return contentType;
    }
#else
#pragma unused (extension)
    return @"application/octet-stream";
#endif
}

@implementation SvnResourceLoadOperation
{
    BOOL        executing;
    BOOL        finished;
}

-(instancetype)initWithRequest:(AVAssetResourceLoadingRequest *)request
{
    self = [super init];
    if(self)
    {
        self.currentRequest = request;
        executing = NO;
        
        finished = NO;
    }
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
        
        unsigned long long requestedOffset = self.currentRequest.dataRequest.requestedOffset;
        unsigned long long requestedLength = self.currentRequest.dataRequest.requestedLength;
        
        
        NSLog(@"startOperationFromOffset=%lld, length:%lld", requestedOffset, requestedLength);
        
        NSString *bytesString = [NSString stringWithFormat:@"bytes=%lld-%lld",requestedOffset,(requestedOffset+requestedLength-1)];
        NSDictionary *params = @{@"Range":bytesString};
        
        NSString * filePath = [self.currentRequest.request.URL.absoluteString stringByRemovingPercentEncoding];;
        if([filePath hasPrefix:@"mediafile://"])
        {
            filePath = [filePath substringFromIndex:12];
        }
        
        SvnFileHandle *inputHandle = [SvnFileHandle fileHandleForReadingAtPath:filePath];
        
        if(!inputHandle)
        {
            NSError * error = [NSError errorWithDomain:@"Open file error!" code:0 userInfo:@{@"Info":[NSString stringWithFormat:@"Failed to load file :%@", filePath]}];
            
            //failureBlock(error);
            [self completeOperationWithError:error];
            
            return;
        }
        else
        {
            NSString* pathExtension = [filePath stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
            NSString* contentType = contentTypeForPathExtension(pathExtension);
            
            
            
            
            
            //                NSString *mimeType = [response MIMEType];
            //                CFStringRef contentType = UTTypeCreatePreferredIdentifierForTag(kUTTagClassMIMEType, (__bridge CFStringRef)(mimeType), NULL);
            //
            //
            SvnMediaResourceLoaderContentInformation * contentInformationRequest = [[SvnMediaResourceLoaderContentInformation alloc] init];
            
            
            contentInformationRequest.byteRangeAccessSupported = YES;
            contentInformationRequest.contentType = contentType;
            //contentInformationRequest.contentLength = [response expectedContentLength];
            
            [inputHandle seekToEndOfFile];
            
            unsigned long long contentLength = [inputHandle offsetInFile];

            contentInformationRequest.contentLength = contentLength;
            NSLog(@"total contentLength:%lld", contentLength);
            
            if(self.delegate && [self.delegate respondsToSelector:@selector(operation:didReceiveContentInformation:)])
            {
                [self.delegate operation:self didReceiveContentInformation:contentInformationRequest];
            }
//
//            
//            
//            weakSelf.contentInformation = contentInformationRequest;
            
            [inputHandle seekToFileOffset:requestedOffset];
            
            unsigned long long totalReaded = 0;
            
            //uint8_t buffer[1024 * 1024] = {0};
            
            while(totalReaded < requestedLength && requestedOffset + totalReaded < contentLength && totalReaded < 1024*1024*4)
            {
                
                NSLog(@"operation %@ totalReaded length:%lld", self, totalReaded);
                
                
                if(self.isCancelled || self.isFinished)
                {
                    break;
                }
                
                unsigned long long notReaded = requestedLength - totalReaded;
                
                NSUInteger toRead = MIN(notReaded, 1024*64);
                
                NSData * data = [inputHandle readDataOfLength:toRead];
                
                //NSLog(@"readed length:%tu", [data length]);
                
                if([data length] ==0)
                {
                    break;
                }
                
                totalReaded += [data length];
                
                //[weakSelf performBlockOnMainThreadAsync:^{
                
                
                if(!self.isCancelled){
                    SvnMediaResourceLoaderDataResponse *dataResponse = [SvnMediaResourceLoaderDataResponse responseWithRequestedOffset:requestedOffset requestedLength:requestedLength receivedDataLength:totalReaded data:data];
                    BOOL dataComplete = [self respondWithData:dataResponse ForRequest:self.currentRequest.dataRequest];
                    
                    if(dataComplete || totalReaded >= 1024*1024*4)
                    {
                        if(!self.currentRequest.isFinished)
                        {
                            [self.currentRequest finishLoading];
                        }
                        
                        
                        break;
                        
                        
                    }
                }
                //}];
                //[NSThread sleepForTimeInterval:0.005];
                
                
            }
            
            NSLog(@"totalReaded length:%lld", totalReaded);
            [inputHandle closeFile];
  
            if(!self.currentRequest.isFinished)
            {
            
                NSError *error = [NSError errorWithDomain:@"resource load error!" code:-1 userInfo:nil];
                
                [self.currentRequest finishLoadingWithError:error];
                
                
                [self completeOperationWithError:error];
            }
            else
            {
                [self completeOperationWithError:nil];
            }
            
            
        }


        
        
    }
    @catch(...) {
        
        // Do not rethrow exceptions.
        
    }
}



- (BOOL)respondWithData:(SvnMediaResourceLoaderDataResponse *)dataResponse ForRequest:(AVAssetResourceLoadingDataRequest *)dataRequest{
    
    //NSLog(@"data request:%@ \n respondWithData beging---------------- %@\n", dataRequest, dataResponse);
 
    unsigned long long startOffset = dataRequest.requestedOffset;
    if (dataRequest.currentOffset != 0){
        startOffset = dataRequest.currentOffset;
    }

    
    if(startOffset < [dataResponse dataOffset] || startOffset >= [dataResponse currentOffset])
    {
        NSLog(@"startOffset %lld not in dataResponse's range :[%lld , %lld]", startOffset, [dataResponse dataOffset], [dataResponse currentOffset]);
        return NO;
    }
    
    // Don't have any data at all for this request
    //    if (self.receivedDataLength < startOffset){
    //        return NO;
    //    }
    
    // This is the total data we have from startOffset to whatever has been downloaded so far
    
    
    
    
    NSUInteger unreadBytes = [dataResponse currentOffset] - startOffset;
    
    // Respond with whatever is available if we can't satisfy the request fully yet
    NSUInteger numberOfBytesToRespondWith = MIN(dataRequest.requestedLength, unreadBytes);
    
    //NSLog(@"startOffset---------------- %llu", startOffset);
    //NSLog(@"unreadBytes---------------- %u", unreadBytes);
    //NSLog(@"numberOfBytesToRespondWith----------------%u", numberOfBytesToRespondWith);
    
    BOOL didRespondFully = NO;
    
    //NSData *data = [self readCachedData:startOffset length:numberOfBytesToRespondWith];
    
    NSRange range = NSMakeRange(startOffset - [dataResponse dataOffset] , numberOfBytesToRespondWith);
    
    //NSLog(@"data range----------------%@", NSStringFromRange(range));
    
    NSData *data = [dataResponse.data subdataWithRange:range];
    
    if(data){
        [dataRequest respondWithData:data];
        //long long endOffset = startOffset + dataRequest.requestedLength;
        didRespondFully = dataRequest.currentOffset >= dataRequest.requestedOffset + dataRequest.requestedLength;
    }
    
    
    // NSLog(@"respondWithData end----------------didRespondFully:%d\n%@", didRespondFully, dataRequest);
    
    return didRespondFully;
}


-(void)cancel
{
    NSLog(@"operation %@ cancel", self);
    [super cancel];
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


- (void)completeOperationWithError:(NSError *) error{
    
    [self completeOperation];
    
    if(self.delegate)
    {
        
        if(error)
        {
            
            if([self.delegate respondsToSelector:@selector(operation:didFailwithError:)])
            {
                [self.delegate operation:self didFailwithError:error];
            }
            
            
        }
        else
        {
            if([self.delegate respondsToSelector:@selector(operationdidFinishLoading:)])
            {
               [self.delegate operationdidFinishLoading:self];
            }
            
            
            
        }
    }
    
    
}




@end

