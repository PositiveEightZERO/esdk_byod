//
//  SvnResourceLoadOperation.h
//  SvnSdkDemo
//
//  Created by l00174413 on 8/20/15.
//
//

#import <Foundation/Foundation.h>

#import <AVFoundation/AVFoundation.h>
#import "SvnMediaResourceLoaderContentInformation.h"

@protocol SvnResourceLoadOperationDelegate;

@interface SvnResourceLoadOperation : NSOperation

@property (weak, nonatomic) id<SvnResourceLoadOperationDelegate> delegate;

@property (strong, nonatomic) AVAssetResourceLoadingRequest* currentRequest;

- (instancetype)initWithRequest:(AVAssetResourceLoadingRequest*) request;

@end





@protocol SvnResourceLoadOperationDelegate <NSObject>

@optional

- (void)operation:(SvnResourceLoadOperation *)operation  didReceiveContentInformation:(SvnMediaResourceLoaderContentInformation *)info;

- (void)operationdidFinishLoading:(SvnResourceLoadOperation *)operation;

- (void)operation:(SvnResourceLoadOperation *)operation didFailwithError:(NSError *)error;

@end



