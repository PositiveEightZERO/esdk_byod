//
//  HttpBusinessViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 13-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import <ImageIO/ImageIO.h>
#import <AssetsLibrary/AssetsLibrary.h>

#import "HttpBusinessViewController.h"
#import "GuideViewController.h"
#import "UIHelper.h"

//#import <SvnSdk/SvnHttpURLProtocol.h>
#import <SvnSdk/SecBrowHttpProtocol.h>

#import "SvnASIHTTPRequest.h"
#import "SvnASIFormDataRequest.h"
#import "ASIFormDataRequest.h"



@implementation HttpBusinessViewController
{
    UITextField * currentTextField;
    
    BOOL keyboardIsShown;
    
    UIImagePickerController *imagePicker;
    
    BOOL _connectedSuccess;
    NSMutableData* _receivedData;
    NSFileHandle* _downloadFile;
    NSMutableData* _downloadData;
    
    long long _receivedBytes;
    
}



- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

#pragma mark - View lifecycle

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView
{
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self setTitle:[self requestType]];
    
    // Custom initialization
    UIButton *backButton = [UIHelper navButton:@"Back"];
    [backButton addTarget:self action:@selector(navigateBack) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *backButtonItem = [[UIBarButtonItem alloc] initWithCustomView:backButton];
    backButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.leftBarButtonItem=backButtonItem;
    
    
    UIButton *guideButton = [UIHelper navButton:@"Guide"];
    [guideButton addTarget:self action:@selector(navigateGuide) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *faqsButtonItem = [[UIBarButtonItem alloc] initWithCustomView:guideButton];
    faqsButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.rightBarButtonItem=faqsButtonItem;
    
    UIColor * selectedColor = [UIColor colorWithRed:(0/255.f) green:(153/255.f) blue:(255/255.f) alpha:1.0f];
    self.segmentControl.layer.borderColor = [selectedColor CGColor];
    self.segmentControl.layer.borderWidth = 1.0f;
    self.segmentControl.layer.cornerRadius = 4.0f;
    self.segmentControl.layer.masksToBounds = YES;
    
    UIEdgeInsets insets = UIEdgeInsetsMake(20, 10, 20, 10);
    // 指定为拉伸模式，伸缩后重新赋值
    UIImage *image= [[UIImage imageNamed:@"btn_background.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    UIImage *imageSelected= [[UIImage imageNamed:@"btn_background_selected.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    [self.loginButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.loginButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.userInfoButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.userInfoButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.downloadButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.downloadButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.uploadButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.uploadButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.browseButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.browseButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    
    
    //tapRecognizer for dismiss keyboard.
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc]
                                             initWithTarget:self action:@selector(tapOnView:)];
    [self.view addGestureRecognizer: tapRecognizer];

    
    
    //[editPath setText:@"172.19.110.3:8081/HttpServerDemo/e"];
    //[editPath setText:@"9.1.49.51:8081"];
    [self.pathTextField setText:@"http://172.22.8.206:8080/HttpServerDemo/e/"];
    
    //[self.pathTextField setText:@"http://172.19.110.3:8081/HttpServerDemo/e/"];
    
    [self.subPathTextField setText:@"Login.do"];
    [self.nameTextField setText:@"user1"];
    [self.passwordTextField setText:@"pwd1"];
    
    [self.downloadFileTextField setText:@"中文.jpg"];
    
    
    //注册URLProtocol
    [NSURLProtocol registerClass:[SecBrowHttpProtocol class]];

    
}


-(void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    self.scrollView.contentSize = self.contentView.frame.size;
}



-(void)tapOnView:(UITapGestureRecognizer *)tapRecognizer
{
    [[self view] endEditing: YES];
}

- (IBAction)navigateBack
{
    [self.navigationController popViewControllerAnimated:YES];
}


- (IBAction)navigateGuide
{
    [self performSegueWithIdentifier:@"guide" sender:self];
}



#pragma mark - do HTTP Request

- (IBAction)doLogin:(id)sender {
    
    _receivedData = nil;
    _downloadFile = nil;
    
    if([[self requestType] isEqualToString:@"NSURLConnection"])
    {
        [self doLoginThroughNSURLConnection];
    }
    else if([[self requestType] isEqualToString:@"ASIHTTPRequest"])
    {
        [self doLoginThroughASIHTTPRequest];
    }
    
    
}

- (IBAction)doUserInfo:(id)sender {
    _receivedData = nil;
    _downloadFile = nil;
    
    if([[self requestType] isEqualToString:@"NSURLConnection"])
    {
        [self doUserInfoThroughNSURLConnection];
    }
    else if([[self requestType] isEqualToString:@"ASIHTTPRequest"])
    {
        [self doUserInfoThroughASIHTTPRequest];
    }
}

- (IBAction)doDownload:(id)sender {
    _receivedData = nil;
    _downloadData = nil;
    _downloadFile = nil;
    
    if([[self requestType] isEqualToString:@"NSURLConnection"])
    {
        [self doDownloadThroughNSURLConnection];
    }
    else if([[self requestType] isEqualToString:@"ASIHTTPRequest"])
    {
        [self doDownloadThroughASIHTTPRequest];
    }
    
}

- (IBAction)doUpload:(id)sender {
    _receivedData = nil;
    _downloadFile = nil;
    
    if([[self requestType] isEqualToString:@"NSURLConnection"])
    {
        [self doUploadThroughNSURLConnection];
    }
    else if([[self requestType] isEqualToString:@"ASIHTTPRequest"])
    {
        [self doUploadThroughASIHTTPRequest];
    }
}

- (IBAction)doSelectFile:(id)sender {
    
    imagePicker = [[UIImagePickerController alloc] init];
    imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    imagePicker.allowsEditing = NO;
    imagePicker.delegate = self;
    [self presentModalViewController:imagePicker animated:YES];
}

#pragma mark - HTTP Request By NSURLConnection

- (void)doLoginThroughNSURLConnection
{

    NSString* path = [self.pathTextField text];
    NSString* subPath = [self.subPathTextField text];
    
    NSString *URLPath = [NSString stringWithFormat:@"%@%@", path, subPath ];
    NSURL *URL = [NSURL URLWithString:URLPath];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:60.0*60];
    [request setHTTPShouldHandleCookies:YES];
    
    
    [request setHTTPMethod:@"POST"];
    NSString *query = [NSString stringWithFormat:@"username=%@&password=%@", [self.nameTextField text], [self.passwordTextField text]];
    
    NSString *postData = [[NSString alloc] initWithString:query];
    [request setValue:@"application/x-www-form-urlencoded;charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    
    [request setHTTPBody:[postData dataUsingEncoding:NSUTF8StringEncoding]];
    
    
    [NSURLConnection connectionWithRequest:request delegate:self];
}

- (void)doUserInfoThroughNSURLConnection
{
    
    NSString* path = [self.pathTextField text];
    NSString* subPath = [self.subPathTextField text];
    
    NSString *URLPath = [NSString stringWithFormat:@"%@%@", path, subPath ];
    NSURL *URL = [NSURL URLWithString:URLPath];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:60.0*60];
    [request setHTTPShouldHandleCookies:YES];
    
    
    [request setHTTPMethod:@"GET"];
    
    [NSURLConnection connectionWithRequest:request delegate:self];
}


- (void)doDownloadThroughNSURLConnection
{
    NSString* path = [self.pathTextField text];
    NSString* subPath = [self.subPathTextField text];

    NSString *URLPath = [[NSString stringWithFormat:@"%@%@?fileName=%@", path, subPath,  [self.downloadFileTextField text]] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSURL *URL = [NSURL URLWithString:URLPath];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:60.0*60];
    [request setHTTPShouldHandleCookies:YES];
    
    [request setHTTPMethod:@"GET"];
//    NSString *query = [NSString stringWithFormat:@"fileName=%@", [[self.downloadFileTextField text] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
//    
//    NSString *postData = [[NSString alloc] initWithString:query];
//    [request setValue:@"application/x-www-form-urlencoded;charset=utf-8" forHTTPHeaderField:@"Content-Type"];
//    
//    [request setHTTPBody:[postData dataUsingEncoding:NSUTF8StringEncoding]];
    
    [NSURLConnection connectionWithRequest:request delegate:self];
}

- (void)doUploadThroughNSURLConnection
{
    NSString* path = [self.pathTextField text];
    NSString* subPath = [self.subPathTextField text];
    
    NSString *URLPath = [NSString stringWithFormat:@"%@%@", path, subPath];
    NSURL *URL = [NSURL URLWithString:URLPath];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:60.0*60];
    [request setHTTPShouldHandleCookies:YES];
    
    
    NSString *filePath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingFormat:@"/%@",[self.uploadFileTextField text]];
    
    NSData *paramData = [NSData dataWithContentsOfFile:filePath options:NSDataReadingMappedIfSafe error:nil];
    
    if(paramData == nil || [paramData length] == 0)
    {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:[NSString stringWithFormat:@"%@ not exist!", filePath] delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        
        [alertView show];
        return;
    }
    
    
    [request setHTTPMethod:@"POST"];
    
    
    NSString *boundary = @"---------------------------7dd1842a402f8";
    NSString *endBoundary = [NSString stringWithFormat:@"\r\n%@\r\n", boundary];
    
    NSString *contentType = [NSString stringWithFormat:@"multipart/form-data;boundary=%@",  boundary];
    [request addValue:contentType forHTTPHeaderField: @"Content-Type"];
    
    NSMutableData *tempPostData = [NSMutableData data];
    [tempPostData appendData:[[NSString stringWithFormat:@"--%@\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
    
    
    [tempPostData appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"image\"; filename=\"%@\"\r\n", [self.uploadFileTextField text]] dataUsingEncoding: NSUTF8StringEncoding]];
    
    [tempPostData appendData:[[NSString stringWithString:@"Content-Type: text/plain\r\n\r\n"] dataUsingEncoding:NSUTF8StringEncoding]];
    [tempPostData appendData:paramData];
    [tempPostData appendData:[[NSString stringWithFormat:@"\r\n--%@--\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
    
    
    NSInputStream* bodyStream = [NSInputStream inputStreamWithData:tempPostData];
    [request setHTTPBodyStream:bodyStream];
    [request addValue:[NSString stringWithFormat:@"%ld", [tempPostData length]] forHTTPHeaderField: @"Content-Length"];
    
    [NSURLConnection connectionWithRequest:request delegate:self];
}


#pragma mark - HTTP Request By ASIHTTPRequest

- (void)doLoginThroughASIHTTPRequest
{
    NSString* path = [self.pathTextField text];
    NSString* subPath = [self.subPathTextField text];
    
    NSString *URLPath = [NSString stringWithFormat:@"%@/%@", path, subPath ];
    
    NSURL *url = [NSURL URLWithString:URLPath];
    
    
    SvnASIFormDataRequest *request = [SvnASIFormDataRequest requestWithURL:url];
    [request setDelegate:self];
    [request setPostValue:[self.nameTextField text] forKey:@"username"];
    [request setPostValue:[self.passwordTextField text] forKey:@"password"];
    
    
    [request setUseSVN:YES];
    
    [request startAsynchronous];
}

- (void)doUserInfoThroughASIHTTPRequest
{
    NSString* path = [self.pathTextField text];
    NSString* subPath = [self.subPathTextField text];
    
    NSString *URLPath = [NSString stringWithFormat:@"%@/%@", path, subPath ];
    
    NSURL *url = [NSURL URLWithString:URLPath];
    
    SvnASIHTTPRequest *request = [SvnASIHTTPRequest requestWithURL:url];
    [request setDelegate:self];
    
    [request setUseSVN:YES];
    
    [request startAsynchronous];
    
}


- (void)doDownloadThroughASIHTTPRequest
{
    NSString* path = [self.pathTextField text];
    NSString* subPath = [self.subPathTextField text];
    
   NSString *URLPath = [[NSString stringWithFormat:@"%@%@?fileName=%@", path, subPath,  [self.downloadFileTextField text]] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    
    NSURL *url = [NSURL URLWithString:URLPath];
    
    SvnASIFormDataRequest *request = [SvnASIFormDataRequest requestWithURL:url];
    [request setDelegate:self];
    //[request setPostValue:[self.downloadFileTextField text] forKey:@"fileName"];
    
    NSString *filePath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingFormat:@"/%@",[self.downloadFileTextField text]];
    
    [request setDownloadDestinationPath:filePath];
    [request setDownloadProgressDelegate:self];
    [request setShowAccurateProgress:YES];
    
    [request setUseSVN:YES];
    [request startAsynchronous];
}

- (void)doUploadThroughASIHTTPRequest
{
    NSString* path = [self.pathTextField text];
    NSString* subPath = [self.subPathTextField text];
    
    NSString *URLPath = [NSString stringWithFormat:@"%@/%@", path, subPath ];
    
    NSURL *url = [NSURL URLWithString:URLPath];
    
    NSString *filePath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingFormat:@"/%@",[self.uploadFileTextField text]];
    
    SvnASIFormDataRequest *request = [SvnASIFormDataRequest requestWithURL:url];
    [request setDelegate:self];
    
    [request setFile:filePath forKey:@"image"];
    [request setUploadProgressDelegate:self];
    [request setShowAccurateProgress:YES];
    
    [request setUseSVN:YES];
    
    [request startAsynchronous];

}




#pragma mark - NSURLConnection Delegate
//---------------------------------------
// You have to implement below four methods
//---------------------------------------

- (void)connection:(NSURLConnection *)theConnection didReceiveResponse:(NSURLResponse *)response
{
    NSInteger responseCode = [(NSHTTPURLResponse *)response statusCode];
    _connectedSuccess = responseCode == 200;
    
    NSHTTPURLResponse * httpResponse = (NSHTTPURLResponse*) response;
    
    if ([httpResponse respondsToSelector:@selector(allHeaderFields)]) {
        // 取得所有的请求的头
        NSDictionary *dictionary = [httpResponse allHeaderFields];
        
        [self.requestTextView setText:[dictionary description]];
        
        //NSLog([dictionary description]);
        // 取得http状态码
        //NSLog(@"%d",[responsestatusCode]);
        
    }
    
    
    NSLog(@"response length=%lld", [response expectedContentLength]);
}

- (void)connection:(NSURLConnection *)theConnection didReceiveData:(NSData *)data
// A delegate method called by the NSURLConnection as data arrives.  The 
// response data for a POST is only for useful for debugging purposes, 
// so we just drop it on the floor.
{
    if([[self segmentControl] selectedSegmentIndex] != 2)
    {
        if(_receivedData == nil)
        {
            _receivedData = [[NSMutableData alloc] initWithData:data];
        }
        else
        {
            [_receivedData appendData:data];
        }
    }
    else
    {
        if(_downloadFile == nil)
        {
            NSString *filePath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingFormat:@"/%@",[self.downloadFileTextField text]];
            
            NSFileManager *fileManager = [NSFileManager defaultManager];
            
            if([fileManager fileExistsAtPath:filePath])
            {
                NSError *error = [[NSError alloc] init];
                [fileManager removeItemAtPath:filePath error:&error];
            }
                
            [fileManager createFileAtPath:filePath  
                                                    contents:nil attributes:nil]; 
            _downloadFile = [NSFileHandle fileHandleForWritingAtPath:filePath];

            _receivedBytes = 0;
        }
        //NSLog(@"to write data");
        [_downloadFile writeData:data];
        _receivedBytes += [data length];
        
        [self updateReceivedBytes];

    }

}

- (void)connection:(NSURLConnection *)theConnection didFailWithError:(NSError *)error
// A delegate method called by the NSURLConnection if the connection fails. 
// We shut down the connection and display the failure.  Production quality code 
// would either display or log the actual error.
{
    if(_downloadFile)
    {
        [_downloadFile closeFile];
    }
    _receivedData = nil;
    _downloadFile = nil;
    
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
    
    [alertView show];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)theConnection
// A delegate method called by the NSURLConnection when the connection has been 
// done successfully.  We shut down the connection with a nil status, which 
// causes the image to be displayed.
{
    NSLog(@"connectionDidFinishLoading");
    if (_connectedSuccess) 
    {
        
        if([[self segmentControl] selectedSegmentIndex] != 2)
        {
            NSString* result = [[NSString alloc] initWithData:_receivedData encoding:NSUTF8StringEncoding];
            NSLog(@"result length:%d", [result length]);
            NSLog(@"receive:%@", result);
            [self.responseTextView setText:result];
        }
        else
        {
            //[_downloadFile close];
            
            unsigned long long fsize = [_downloadFile seekToEndOfFile];
            
            
            NSString *message = [NSString stringWithFormat:@"file download completed! downloaded size=%d",fsize];

            UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:message delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
                
            [alertView show];
            
            [_downloadFile closeFile];
           
        }

     
        
    }
    else
    { 
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:@"response code != 200, please check!" delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        
        [alertView show];
        
    }
    
    _receivedData = nil;
    _downloadFile = nil;
    
}


#pragma mark - ASIHTTPRequest Delegate
//---------------------------------------
// You have to implement below four methods
//---------------------------------------

- (void)requestFinished:(ASIHTTPRequest *)request

{
    
    NSLog(@"connectionDidFinishLoading");
    if (_connectedSuccess)
    {
        
        if([[self segmentControl] selectedSegmentIndex] != 2)
        {
            NSString* result = [[NSString alloc] initWithData:_receivedData encoding:NSUTF8StringEncoding];
            NSLog(@"result length:%d", [result length]);
            NSLog(@"receive:%@", result);
            [self.responseTextView setText:result];
        }
        else
        {
            //[_downloadFile close];
            
            unsigned long long fsize = [_downloadFile seekToEndOfFile];
            
            
            NSString *message = [NSString stringWithFormat:@"file download completed! downloaded size=%d",fsize];
            
            UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:message delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
            
            [alertView show];
            
            [_downloadFile closeFile];
            
        }
    }
    else
    {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:@"response code != 200, please check!" delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        
        [alertView show];
        
    }
    
    _receivedData = nil;
    _downloadFile = nil;
    
    
}

- (void)requestFailed:(ASIHTTPRequest *)request
{
    if(_downloadFile)
    {
        [_downloadFile closeFile];
    }
    _receivedData = nil;
    _downloadFile = nil;
    
    
    NSError *error = [request error];
    
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
    
    [alertView show];
    
}

- (void)request:(ASIHTTPRequest *)request didReceiveResponseHeaders:(NSDictionary *)responseHeaders
{
    
    NSInteger responseCode = [request responseStatusCode];
    _connectedSuccess = responseCode == 200;
    
    
    [self.requestTextView setText:[responseHeaders description]];
}


- (void)request:(ASIHTTPRequest *)request didReceiveData:(NSData *)data
{
    if([[self segmentControl] selectedSegmentIndex] != 2)
    {
        if(_receivedData == nil)
        {
            _receivedData = [[NSMutableData alloc] initWithData:data];
        }
        else
        {
            [_receivedData appendData:data];
        }
    }
    else
    {
        if(_downloadFile == nil)
        {
            NSString *filePath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingFormat:@"/%@",[self.downloadFileTextField text]];
            
            NSFileManager *fileManager = [NSFileManager defaultManager];
            
            if([fileManager fileExistsAtPath:filePath])
            {
                NSError *error = [[NSError alloc] init];
                [fileManager removeItemAtPath:filePath error:&error];
            }
            
            [fileManager createFileAtPath:filePath
                                 contents:nil attributes:nil];
            _downloadFile = [NSFileHandle fileHandleForWritingAtPath:filePath];
            
            _receivedBytes = 0;
        }
        //NSLog(@"to write data");
        [_downloadFile writeData:data];
        _receivedBytes += [data length];
        
        [self updateReceivedBytes];
        
    }
    
}




- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];
    NSURL *imageURL = [info valueForKey:UIImagePickerControllerReferenceURL];
    
    //__block NSString *fileName = nil;
    
    ALAssetsLibraryAssetForURLResultBlock resultBlock = ^(ALAsset * asset)
    {
        ALAssetRepresentation *representation = [asset defaultRepresentation];
        NSString *fileName = [representation filename];
        [self.uploadFileTextField setText:fileName];
        NSLog(@"fileName:%@", [representation filename]);
        
         NSString *filePath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingFormat:@"/%@",fileName];
        
        NSURL *urlPath = [NSURL fileURLWithPath:filePath];
        
        //NSURL *urlPath = [[NSURL fileURLWithPath:[[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject]] URLByAppendingPathComponent:fileName];
        
        CGImageDestinationRef ref = CGImageDestinationCreateWithURL((__bridge CFURLRef)urlPath, (CFStringRef)@"public.jpeg", 1, NULL);
        CGImageDestinationAddImage(ref, (CGImageRef)[[asset defaultRepresentation] fullResolutionImage], NULL);
        
        NSDictionary *props = [NSDictionary dictionaryWithObjectsAndKeys:
                                [NSNumber numberWithFloat:1.0], kCGImageDestinationLossyCompressionQuality,
                                nil];
        
        CGImageDestinationSetProperties(ref, (__bridge CFDictionaryRef) props);
        
        CGImageDestinationFinalize(ref);
        CFRelease(ref);        
 
    };
    ALAssetsLibrary *library = [[ALAssetsLibrary alloc] init];
    [library assetForURL:imageURL resultBlock:resultBlock failureBlock:nil];
                       
     
    
    [self dismissModalViewControllerAnimated:YES];
}



- (void) updateReceivedBytes
{
    [self.responseTextView setText:[NSString stringWithFormat:@"%d bytes received", _receivedBytes]];
}


- (IBAction)segmentValueChanged:(UISegmentedControl *) sender
{
    
    switch (sender.selectedSegmentIndex) {
        case 0:
            self.loginInputView.hidden = NO;
            self.userInfoInputView.hidden = YES;
            self.downloadInputView.hidden = YES;
            self.uploadInputView.hidden = YES;
            [self.subPathTextField setText:@"Login.do"];
            break;
        case 1:
            self.loginInputView.hidden = YES;
            self.userInfoInputView.hidden = NO;
            self.downloadInputView.hidden = YES;
            self.uploadInputView.hidden = YES;
            [self.subPathTextField setText:@"Userinfo.do"];
            break;
        case 2:
            self.loginInputView.hidden = YES;
            self.userInfoInputView.hidden = YES;
            self.downloadInputView.hidden = NO;
            self.uploadInputView.hidden = YES;
            [self.subPathTextField setText:@"Download.do"];
            break;
        case 3:
            self.loginInputView.hidden = YES;
            self.userInfoInputView.hidden = YES;
            self.downloadInputView.hidden = YES;
            self.uploadInputView.hidden = NO;
            [self.subPathTextField setText:@"execute_upload.do"];
            break;
        default:
            break;

    }
    
    [self.requestTextView setText:@""];
    [self.responseTextView setText:@""];

}


- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"guide"])
    {
        GuideViewController *guideViewController =  [segue destinationViewController];

        if([[self requestType] isEqualToString:@"NSURLConnection"])
        {
            guideViewController.guideType = @"nsurlconnection";

        }
        else if([[self requestType] isEqualToString:@"ASIHTTPRequest"])
        {
            guideViewController.guideType = @"asihttprequest";

        }

    }      
}
@end
