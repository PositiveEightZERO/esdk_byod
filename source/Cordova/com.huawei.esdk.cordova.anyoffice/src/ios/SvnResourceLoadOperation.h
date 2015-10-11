//
//  SvnResourceLoadOperation.h
//  BYOD
//
//  Created by l00174413 on 15/4/22.
//
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVAssetResourceLoader.h>

@class SvnResourceLoadOperation;
@protocol SvnResourceLoadOperationDelegate <NSObject>

@optional
- (void)operation:(SvnResourceLoadOperation*)operation didReceiveResponse:(NSURLResponse *)response;
- (void)operation:(SvnResourceLoadOperation*)operation didReceiveData:(NSData *)data;
- (void)operationDidFinishLoading:(SvnResourceLoadOperation*)operation;
- (void)operation:(SvnResourceLoadOperation*)operation didFailWithError:(NSError *)error;


@end




@interface SvnResourceLoadOperation : NSOperation <NSURLConnectionDataDelegate>
{
    BOOL        executing;
    
    BOOL        finished;
}
@property (strong, nonatomic) AVAssetResourceLoadingRequest * request;

@property (strong, nonatomic) NSURLConnection * dataConnection;
@property (strong, nonatomic) NSMutableData * songData;

@property (weak, nonatomic) id<SvnResourceLoadOperationDelegate> delegate;

@property (nonatomic) unsigned long long length;

-(instancetype)initWithRequest:(AVAssetResourceLoadingRequest *)request;

@end
