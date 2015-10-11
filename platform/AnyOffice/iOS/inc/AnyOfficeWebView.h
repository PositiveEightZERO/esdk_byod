//
//  SecBrowWebView.h
//  AnyOffice
//
//  Created by kf1 on 14-1-8.
//
//

//#import "MyWebView.h"

//progress
#undef njk_weak
#if __has_feature(objc_arc_weak)
#define njk_weak weak
#else
#define njk_weak unsafe_unretained
#endif

extern const float f_InitialProgressValue;
extern const float f_InteractiveProgressValue;
extern const float f_FinalProgressValue;
typedef void (^AnyOfficeWebViewBlock)(float progress);
@protocol AnyOfficeWebViewDelegate;
//progeress
@protocol AnyOfficeWebViewSSODelegate <NSObject>
- (void)ssoCallback:(NSString *)url;
@end


@interface AnyOfficeWebView : UIWebView<UIWebViewDelegate,UIActionSheetDelegate>
{
    //long press
    NSURL *_urlToHandle;
    NSInteger _openLinkButtonIndex;
    NSInteger _copyButtonIndex;
}

@property(nonatomic,retain)NSString *pageName;

//progress
@property (nonatomic, njk_weak) id<AnyOfficeWebViewDelegate>progressDelegate;
@property (nonatomic, njk_weak) id<UIWebViewDelegate>webViewProxyDelegate;
@property (nonatomic, copy) AnyOfficeWebViewBlock progressBlock;
@property (nonatomic, readonly) float progress; // 0.0..1.0
- (void)reset;
//progress


- (void)loadRequestSingleSignOn:(NSURLRequest *)request;
+ (void)setSSOCallback:(id<AnyOfficeWebViewSSODelegate>)delegate;
-(BOOL)registerShareToWeChatAppID:(NSString *)appID;
@end



//progress
@protocol AnyOfficeWebViewDelegate <NSObject>
- (void)webViewProgress:(AnyOfficeWebView *)webViewProgress updateProgress:(float)progress;
@end
//progress
