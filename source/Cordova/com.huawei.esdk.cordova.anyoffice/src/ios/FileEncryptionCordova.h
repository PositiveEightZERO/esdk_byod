#import <Cordova/CDV.h>

@interface FileEncryptionCordova : CDVPlugin

- (void) fileEncrypt:(CDVInvokedUrlCommand*)command;
- (void) fileDecrypt:(CDVInvokedUrlCommand*)command;
- (void) fileEncryptDownload:(CDVInvokedUrlCommand*)command;


- (void) abortDownload:(CDVInvokedUrlCommand*)command;


- (NSMutableDictionary*)createFileTransferError:(int)code AndSource:(NSString*)source AndTarget:(NSString*)target;

- (NSMutableDictionary*)createFileTransferError:(int)code
                                      AndSource:(NSString*)source
                                      AndTarget:(NSString*)target
                                  AndHttpStatus:(int)httpStatus
                                        AndBody:(NSString*)body;
@property (nonatomic, strong) NSOperationQueue* queue;
@property (nonatomic, strong) NSMutableDictionary* activeTransfers;



@end

struct SVN_FILE_S;

@interface AnyOfficeFileTransferDelegate : NSObject {}

- (void)updateBytesExpected:(long long)newBytesExpected;
- (void)cancelTransfer:(NSURLConnection*)connection;

@property (strong) NSMutableData* responseData; // atomic
@property (nonatomic, strong) NSDictionary* responseHeaders;
@property (nonatomic, assign) UIBackgroundTaskIdentifier backgroundTaskID;
@property (nonatomic, strong) FileEncryptionCordova* command;
//@property (nonatomic, assign) CDVFileTransferDirection direction;
@property (nonatomic, strong) NSURLConnection* connection;
@property (nonatomic, copy) NSString* callbackId;
@property (nonatomic, copy) NSString* objectId;
@property (nonatomic, copy) NSString* source;
@property (nonatomic, copy) NSString* target;
//@property (nonatomic, copy) NSURL* targetURL;
@property (nonatomic, copy) NSString* mimeType;
@property (assign) int responseCode; // atomic
@property (nonatomic, assign) long long bytesTransfered;
@property (nonatomic, assign) long long bytesExpected;
//@property (nonatomic, assign) BOOL trustAllHosts;
@property struct SVN_FILE_S* targetFileHandle;
//@property (nonatomic, strong) CDVFile *filePlugin;

@end