#import "FileEncryptionCordova.h"

#import <SvnSdk/svn_file_api.h>
#import <SvnSdk/SecBrowHttpProtocol.h>

enum AnyOfficeFileTransferError {
    FILE_NOT_FOUND_ERR = 1,
    INVALID_URL_ERR = 2,
    CONNECTION_ERR = 3,
    CONNECTION_ABORTED = 4
};

typedef int AnyOfficeFileTransferError;

@implementation FileEncryptionCordova


- (void)pluginInitialize {
    self.activeTransfers = [[NSMutableDictionary alloc] init];
}

- (void)fileEncrypt:(CDVInvokedUrlCommand*)command
{
    NSString *srcFilePath = [command.arguments objectAtIndex:0];
    NSString *dstFilePath = [command.arguments objectAtIndex:1];
    
    CDVPluginResult* result = [CDVPluginResult
                               resultWithStatus:CDVCommandStatus_OK];
    
    BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:srcFilePath];
 
    if(!fileExists)
    {
        result = [CDVPluginResult
                                   resultWithStatus:CDVCommandStatus_ERROR
                                   messageAsString:@"source file not exist."];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
        return;
    }
    
    
    NSString* parentPath = [dstFilePath stringByDeletingLastPathComponent];
    
    // create parent directories if needed
    int res = svn_mkdir_ex([parentPath UTF8String]);
    
    if(res != 0)
    {
        result = [CDVPluginResult
                  resultWithStatus:CDVCommandStatus_ERROR
                  messageAsString:@"Could not create path to save dest file."];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
        return;
    }
    
    SVN_FILE_S *file =  svn_fopen([dstFilePath UTF8String], "w");
    
    if (NULL == file) {
        NSLog(@"open file:%@ error!", dstFilePath);
        result = [CDVPluginResult
                  resultWithStatus:CDVCommandStatus_ERROR
                  messageAsString:@"open dest file error."];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
        return;
    }
    
    NSData *data = [NSData dataWithContentsOfFile:srcFilePath];
    
    int iRet = 0;
    
    const int buf_size = 1024;
    char buffer[1024];
    memset(buffer, 0, buf_size);
    
    int srcRemain = [data length];
    int srcLocation = 0;
    BOOL hasWriteError = NO;
    while (srcRemain > 0) {
        
        int len = srcRemain > buf_size ? buf_size:srcRemain;
        
        NSRange bufRange = NSMakeRange(srcLocation, len);
        
        
        srcLocation += len;
        srcRemain -= len;
        
        [data getBytes:buffer range:bufRange];
        
        iRet = svn_fwrite((unsigned char *)buffer, 1, len, file);
        if ( 0 > iRet ) {
            hasWriteError = YES;
            NSLog(@"write file error! iRet=%d", iRet);
            break;
        }
    }
    
    
    svn_fclose(file);
    
    if(hasWriteError)
    {
        result = [CDVPluginResult
                  resultWithStatus:CDVCommandStatus_ERROR
                  messageAsString:@"Write dest file error."];

    }

    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
}


- (void)fileDecrypt:(CDVInvokedUrlCommand*)command
{
    NSLog(@"Cordova:fileDecrypt...");
    NSString *srcFilePath = [command.arguments objectAtIndex:0];
    NSString *dstFilePath = [command.arguments objectAtIndex:1];
    
    CDVPluginResult* result = [CDVPluginResult
                               resultWithStatus:CDVCommandStatus_OK];
    
    //打开加密文件
    SVN_FILE_S *file =  svn_fopen([srcFilePath UTF8String], "r");
    
    if (NULL == file) {
        NSLog(@"open file:%@ error!", srcFilePath);
        result = [CDVPluginResult
                  resultWithStatus:CDVCommandStatus_ERROR
                  messageAsString:@"Could not open source file."];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
        return;
    }
    
    NSMutableData *data = [[NSMutableData alloc] init];
    
    const int buf_size = 1024;
    char buffer[1024];
    memset(buffer, 0, buf_size);
    
    int count = 0;
    //读出加密内容
    while ((count = svn_fread(buffer, 1, buf_size, file)) > 0) {
        [data appendBytes:buffer length:count];
    }
    //关闭文件
    svn_fclose(file);
    
    NSString* parentPath = [dstFilePath stringByDeletingLastPathComponent];
    
    NSError * error = nil;
    [[NSFileManager defaultManager] createDirectoryAtPath:parentPath withIntermediateDirectories:YES attributes:nil error:nil];
    
    BOOL isDir = NO;
    
    // create parent directories if needed

    if (![[NSFileManager defaultManager] fileExistsAtPath:parentPath isDirectory:&isDir] || !isDir) {
        result = [CDVPluginResult
                  resultWithStatus:CDVCommandStatus_ERROR
                  messageAsString:@"Could not create path to save dest file."];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
        return;
    }
        
    
 
    
    BOOL writeRes = [data writeToFile:dstFilePath atomically:YES];
  
    if(!writeRes)
    {
        result = [CDVPluginResult
                  resultWithStatus:CDVCommandStatus_ERROR
                  messageAsString:@"Wirte to dest file error."];
        

    }
         
    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
}


- (void)fileEncryptDownload:(CDVInvokedUrlCommand*)command
{
    NSLog(@"File Transfer downloading file...");
    NSString* source = [command.arguments objectAtIndex:0];
    NSString* target = [command.arguments objectAtIndex:1];

    NSString* objectId = [command.arguments objectAtIndex:2];
    NSDictionary* headers = [command.arguments objectAtIndex:3 withDefault:nil];
    
    CDVPluginResult* result = nil;
    AnyOfficeFileTransferError errorCode = 0;
    
    
    NSURL* sourceURL = [NSURL URLWithString:source];
    
    if (!sourceURL) {
        errorCode = INVALID_URL_ERR;
        NSLog(@"File Transfer Error: Invalid server URL %@", source);
    }
//    
//    else
//    {
//            NSLog(@"File Transfer Error: Invalid file path or URL %@", target);
//    }
    
    if (errorCode > 0) {
        result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:[self createFileTransferError:errorCode AndSource:source AndTarget:target]];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
        return;
    }
    
    [NSURLProtocol registerClass:[SecBrowHttpProtocol class]];
    
    
    NSMutableURLRequest* req = [NSMutableURLRequest requestWithURL:sourceURL];
    //[self applyRequestHeaders:headers toRequest:req];
    
    AnyOfficeFileTransferDelegate* delegate = [[AnyOfficeFileTransferDelegate alloc] init];
    delegate.command = self;
    //delegate.direction = CDV_TRANSFER_DOWNLOAD;
    delegate.callbackId = command.callbackId;
    delegate.objectId = objectId;
    delegate.source = source;
    delegate.target = target;
    //delegate.targetURL = targetURL;
//    delegate.trustAllHosts = trustAllHosts;
    //delegate.filePlugin = [self.commandDelegate getCommandInstance:@"File"];
    delegate.backgroundTaskID = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:^{
        [delegate cancelTransfer:delegate.connection];
    }];
    
    delegate.connection = [[NSURLConnection alloc] initWithRequest:req delegate:delegate startImmediately:NO];
    
    if (self.queue == nil) {
        self.queue = [[NSOperationQueue alloc] init];
    }
    [delegate.connection setDelegateQueue:self.queue];
    
    @synchronized (self.activeTransfers) {
        self.activeTransfers[delegate.objectId] = delegate;
    }
    // Downloads can take time
    // sending this to a new thread calling the download_async method
    dispatch_async(
                   dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, (unsigned long)NULL),
                   ^(void) { [delegate.connection start];}
                   );
}


- (void)abortDownload:(CDVInvokedUrlCommand*)command
{
    NSString* objectId = [command.arguments objectAtIndex:0];
    @synchronized (self.activeTransfers) {
        AnyOfficeFileTransferDelegate* delegate = self.activeTransfers[objectId];
        if (delegate != nil) {
            [delegate cancelTransfer:delegate.connection];
            CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:[self createFileTransferError:CONNECTION_ABORTED AndSource:delegate.source AndTarget:delegate.target]];
            [self.commandDelegate sendPluginResult:result callbackId:delegate.callbackId];
        }
    }
}



- (NSMutableDictionary*)createFileTransferError:(int)code AndSource:(NSString*)source AndTarget:(NSString*)target
{
    NSMutableDictionary* result = [NSMutableDictionary dictionaryWithCapacity:3];
    
    [result setObject:[NSNumber numberWithInt:code] forKey:@"code"];
    if (source != nil) {
        [result setObject:source forKey:@"source"];
    }
    if (target != nil) {
        [result setObject:target forKey:@"target"];
    }
    NSLog(@"FileTransferError %@", result);
    
    return result;
}

- (NSMutableDictionary*)createFileTransferError:(int)code
                                      AndSource:(NSString*)source
                                      AndTarget:(NSString*)target
                                  AndHttpStatus:(int)httpStatus
                                        AndBody:(NSString*)body
{
    NSMutableDictionary* result = [NSMutableDictionary dictionaryWithCapacity:5];
    
    [result setObject:[NSNumber numberWithInt:code] forKey:@"code"];
    if (source != nil) {
        [result setObject:source forKey:@"source"];
    }
    if (target != nil) {
        [result setObject:target forKey:@"target"];
    }
    [result setObject:[NSNumber numberWithInt:httpStatus] forKey:@"http_status"];
    if (body != nil) {
        [result setObject:body forKey:@"body"];
    }
    NSLog(@"FileTransferError %@", result);
    
    return result;
}




@end



@implementation AnyOfficeFileTransferDelegate

//@synthesize callbackId, connection = _connection, source, target, responseData, responseHeaders, command, bytesTransfered, bytesExpected, direction, responseCode, objectId, targetFileHandle;

- (void)connectionDidFinishLoading:(NSURLConnection*)connection
{
    NSString* uploadResponse = nil;
    NSString* downloadResponse = nil;
    NSMutableDictionary* uploadResult;
    CDVPluginResult* result = nil;
    
    NSLog(@"File Transfer Finished with response code %d", self.responseCode);


        if (self.targetFileHandle != NULL) {

            svn_fclose(self.targetFileHandle);
            self.targetFileHandle = NULL;
            NSLog(@"File Transfer Download success");
            
            result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:self.source];
        } else {
            downloadResponse = [[NSString alloc] initWithData:self.responseData encoding:NSUTF8StringEncoding];
            result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:[self.command createFileTransferError:CONNECTION_ERR AndSource:self.source AndTarget:self.target AndHttpStatus:self.responseCode AndBody:downloadResponse]];
        }
   
    
    [self.command.commandDelegate sendPluginResult:result callbackId:self.callbackId];
    
    // remove connection for activeTransfers
    @synchronized (self.command.activeTransfers) {
        [self.command.activeTransfers removeObjectForKey:self.objectId];
        // remove background id task in case our upload was done in the background
        [[UIApplication sharedApplication] endBackgroundTask:self.backgroundTaskID];
        self.backgroundTaskID = UIBackgroundTaskInvalid;
    }
}

- (void)removeTargetFile
{
    if(self.targetFileHandle != NULL)
    {
        svn_fclose(self.targetFileHandle);
        
        int ret = svn_remove([self.target UTF8String]);
        NSLog(@"svn_remove returns:%d", ret);
    }
   
}

- (void)cancelTransfer:(NSURLConnection*)connection
{
    [connection cancel];
    @synchronized (self.command.activeTransfers) {
        AnyOfficeFileTransferDelegate* delegate = self.command.activeTransfers[self.objectId];
        [self.command.activeTransfers removeObjectForKey:self.objectId];
        [[UIApplication sharedApplication] endBackgroundTask:delegate.backgroundTaskID];
        delegate.backgroundTaskID = UIBackgroundTaskInvalid;
    }
    
    [self removeTargetFile];
}

- (void)cancelTransferWithError:(NSURLConnection*)connection errorMessage:(NSString*)errorMessage
{
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_IO_EXCEPTION messageAsDictionary:[self.command createFileTransferError:FILE_NOT_FOUND_ERR AndSource:self.source AndTarget:self.target AndHttpStatus:self.responseCode AndBody:errorMessage]];
    
    NSLog(@"File Transfer Error: %@", errorMessage);
    [self cancelTransfer:connection];
    [self.command.commandDelegate sendPluginResult:result callbackId:self.callbackId];
}



- (void)connection:(NSURLConnection*)connection didReceiveResponse:(NSURLResponse*)response
{
    NSError* __autoreleasing error = nil;
    
    self.mimeType = [response MIMEType];
    self.targetFileHandle = nil;
    
    // required for iOS 4.3, for some reason; response is
    // a plain NSURLResponse, not the HTTP subclass
    if ([response isKindOfClass:[NSHTTPURLResponse class]]) {
        NSHTTPURLResponse* httpResponse = (NSHTTPURLResponse*)response;
        
        self.responseCode = [httpResponse statusCode];
        self.bytesExpected = [response expectedContentLength];
        self.responseHeaders = [httpResponse allHeaderFields];

    } else if ([response.URL isFileURL]) {
        NSDictionary* attr = [[NSFileManager defaultManager] attributesOfItemAtPath:[response.URL path] error:nil];
        self.responseCode = 200;
        self.bytesExpected = [attr[NSFileSize] longLongValue];
    } else {
        self.responseCode = 200;
        self.bytesExpected = NSURLResponseUnknownLength;
    }
    if ((self.responseCode >= 200) && (self.responseCode < 300)) {
        // Download response is okay; begin streaming output to file
        
        NSString* parentPath = [self.target stringByDeletingLastPathComponent];
        
        // create parent directories if needed
        int res = svn_mkdir_ex([parentPath UTF8String]);
        
        if(res != 0)
        {
            [self cancelTransferWithError:connection errorMessage:@"Could not create path to save downloaded file"];
            return;
        }
        
        // create target file
        self.targetFileHandle  = svn_fopen([self.target UTF8String], "w");
        if(self.targetFileHandle == NULL)
        {
            [self cancelTransferWithError:connection errorMessage:@"Could not open target file for writing"];
            return;
        }

    
        NSLog(@"Streaming to file %@", self.target);
    }
}

- (void)connection:(NSURLConnection*)connection didFailWithError:(NSError*)error
{
    NSString* body = [[NSString alloc] initWithData:self.responseData encoding:NSUTF8StringEncoding];
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:[self.command createFileTransferError:CONNECTION_ERR AndSource:self.source AndTarget:self.target AndHttpStatus:self.responseCode AndBody:body]];
    
    NSLog(@"File Transfer Error: %@", [error localizedDescription]);
    
    [self cancelTransfer:connection];
    [self.command.commandDelegate sendPluginResult:result callbackId:self.callbackId];
}

- (void)connection:(NSURLConnection*)connection didReceiveData:(NSData*)data
{
    self.bytesTransfered += data.length;
    //NSLog(@"bytesTransfered:%lld", self.bytesTransfered);
    if (self.targetFileHandle != NULL) {
        //[self.targetFileHandle writeData:data];
        
        int iRet = 0;
        
        const int buf_size = 1024;
        char buffer[1024];
        memset(buffer, 0, buf_size);
        
        int srcRemain = [data length];
        int srcLocation = 0;
        while (srcRemain > 0)
        {
            
            int len = srcRemain > buf_size ? buf_size:srcRemain;
            
            NSRange bufRange = NSMakeRange(srcLocation, len);
            
            
            srcLocation += len;
            srcRemain -= len;
            
            [data getBytes:buffer range:bufRange];
            
            iRet = svn_fwrite((unsigned char *)buffer, 1, len, self.targetFileHandle);
            if ( 0 > iRet ) {
                
                NSLog(@"write file error! iRet=%d", iRet);
                [self cancelTransferWithError:connection errorMessage:@"write file error!"];
                return;
            }
        }
        
    }
    else {
        [self.responseData appendData:data];
    }
    [self updateProgress];
}

- (void)updateBytesExpected:(long long)newBytesExpected
{
    NSLog(@"Updating bytesExpected to %lld", newBytesExpected);
    self.bytesExpected = newBytesExpected;
    [self updateProgress];
}

- (void)updateProgress
{
    BOOL lengthComputable = (self.bytesExpected != NSURLResponseUnknownLength);

    NSMutableDictionary* downloadProgress = [NSMutableDictionary dictionaryWithCapacity:3];
    [downloadProgress setObject:[NSNumber numberWithBool:lengthComputable] forKey:@"lengthComputable"];
    [downloadProgress setObject:[NSNumber numberWithLongLong:self.bytesTransfered] forKey:@"loaded"];
    [downloadProgress setObject:[NSNumber numberWithLongLong:self.bytesExpected] forKey:@"total"];
    
    //NSLog(@"update progress:%@", downloadProgress);
    

    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:downloadProgress];
    [result setKeepCallbackAsBool:true];
    [self.command.commandDelegate sendPluginResult:result callbackId:self.callbackId];
}


// for self signed certificates
- (void)connection:(NSURLConnection*)connection willSendRequestForAuthenticationChallenge:(NSURLAuthenticationChallenge*)challenge
{
    if ([challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust]) {
        
        NSURLCredential* credential = [NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust];
        [challenge.sender useCredential:credential forAuthenticationChallenge:challenge];
      
        [challenge.sender continueWithoutCredentialForAuthenticationChallenge:challenge];
    } else {
        [challenge.sender performDefaultHandlingForAuthenticationChallenge:challenge];
    }
}

- (id)init
{
    if ((self = [super init])) {
        self.responseData = [NSMutableData data];
        self.targetFileHandle = nil;
    }
    return self;
}

@end