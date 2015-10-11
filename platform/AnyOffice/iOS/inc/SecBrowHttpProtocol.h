//
//  SecBrowHttpProtocol.h
//  AnyOffice
//
//  Created by Lion User on 15/10/2013.
//
//

#import <Foundation/Foundation.h>

#define SEC_L4_NOUSE        0
#define SEC_L4_USE          1
#define ALERTVIEW_DOWNLOAD  100
#define ALERTVIEW_AUTH      101
#define USERPW_FREE         0
#define USERPW_REMEMBER     1
#define HTTPAUTH_KEY @"httpAuth"
#define HTTP_AUTH_SYMBOL @"WWW-Authenticate"
#define HTTP_AUTH_BASIC  @"Basic"
#define HTTP_AUTH_NTLM   @"NTLM"
#define HTTP_AUTH_DIGEST @"Digest"
#define COOKIE @"cookie"
#define COOKIESTATE @"onOff"


enum httpResponseStatuCode
{
    HTTP_STATUS_CODE_100 = 100,
    HTTP_STATUS_CODE_204 = 204,
    HTTP_STATUS_CODE_401 = 401,
    HTTP_STATUS_CODE_OTHRE,
};
#define REDIRECT_OPINION(x)   ( 3 == ((x)/100))

size_t writeCallback(void* ptr, size_t size, size_t nmemb, void* data);
size_t headerCallback(char* ptr, size_t size, size_t nmemb, void* data);
int AnyOffice_iOS_certificate_verify_callback(void * curl, void * sslctx, void * parm);

int progressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

typedef long (*onAuthCallBack)(void* job, bool useCachedCredentials, const char* realm);
extern long authCallBack(void* job, bool useCachedCredentials, const char* realm);

@interface SecBrowHttpProtocol : NSURLProtocol<UITextFieldDelegate,UIAlertViewDelegate>
{
    UITextField *_userNameField ;
    UITextField *_passWordField ;
    UISegmentedControl * _rememberCredentials;
}
@property (assign)NSInteger _authCount;
@property (assign)unsigned int statusCode;
@property (nonatomic ,retain)NSMutableDictionary *headers;
@property (nonatomic,assign)NSInputStream *bodyStream;
@property (assign)NSInteger dialogResult;
@property (nonatomic,retain)NSString *realm;
@property (nonatomic,retain)NSURLAuthenticationChallenge *authChallenge;
@property (assign)BOOL needAuth;
@property (assign)BOOL authCancel;
@property (assign)BOOL cookieState;
@property (assign)BOOL isCancelled;
@property (assign)BOOL isGetResponse;
//begin added by fengzhengyu 2015-3-10 haihang sso
@property (assign)BOOL isUseTunnel;
//end added by fengzhengyu 2015-3-10 haihang sso
@property (nonatomic,retain)NSMutableDictionary *certDetailInfo;
- (NSNumber *)getRequestPort;
- (void) processHeaderLine:(NSString *) line;
- (void)didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;
- (void)didCancelAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;
- (void) didFinishLoading;
- (void) didFailLoadWithErrorCode:(int)errCode info:(const char*)errInfo;
- (BOOL) shouldHandleCookie;

+ (BOOL) setExceptionAddressList:(NSString*)directAccessList defaultUseVPN:(BOOL)useVPN;

- (BOOL) isUseVpn:(NSString*)host;
- (void)alertCertificateVerify:(SecBrowHttpProtocol *)protocol;
- (NSString *)getRequestHostPort;
@end
