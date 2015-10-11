//
//  previewViewController.h
//  test_read
//
//  Created by Lion User on 06/08/2013.
//  Copyright (c) 2013 Lion User. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MyWebView.h"
extern int SVN_API_RecognizeRMSDoc(char *pcFilePath);
extern int SVN_API_GetAttachmentTypeByNameContent(const char* pcFileName, const char* pcFileContent,unsigned long ulBufLen);
@interface PreviewView : UIViewController <UIWebViewDelegate, UIGestureRecognizerDelegate>

@property (strong, nonatomic) NSString *filePath;
@property (strong, nonatomic) NSString *titleOfBar;
@property (strong, nonatomic) MyWebView *webView;
@property (strong, nonatomic) UINavigationBar *bar;


- (void) previewDocument: (NSString *)fileName;
@end
