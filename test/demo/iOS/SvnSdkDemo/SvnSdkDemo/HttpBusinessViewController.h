//
//  HttpBusinessViewController.h
//  SvnSdkDemo
//
//  Created by l00174413 on 13-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HttpBusinessViewController : UIViewController <UITextFieldDelegate, NSURLConnectionDataDelegate, UIImagePickerControllerDelegate>
@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;

@property (weak, nonatomic) IBOutlet UIView *contentView;

@property (weak, nonatomic) IBOutlet UISegmentedControl *segmentControl;

@property (weak, nonatomic) IBOutlet UIView *loginInputView;
@property (weak, nonatomic) IBOutlet UIView *userInfoInputView;
@property (weak, nonatomic) IBOutlet UIView *downloadInputView;
@property (weak, nonatomic) IBOutlet UIView *uploadInputView;

@property (weak, nonatomic) IBOutlet UIButton *loginButton;

@property (weak, nonatomic) IBOutlet UIButton *userInfoButton;
@property (weak, nonatomic) IBOutlet UIButton *downloadButton;
@property (weak, nonatomic) IBOutlet UIButton *uploadButton;
@property (weak, nonatomic) IBOutlet UIButton *browseButton;

@property (weak, nonatomic) IBOutlet UITextField *pathTextField;
@property (weak, nonatomic) IBOutlet UITextField *subPathTextField;

@property (weak, nonatomic) IBOutlet UITextField *nameTextField;
@property (weak, nonatomic) IBOutlet UITextField *passwordTextField;

@property (weak, nonatomic) IBOutlet UITextField *downloadFileTextField;

@property (weak, nonatomic) IBOutlet UITextField *uploadFileTextField;

@property (weak, nonatomic) IBOutlet UITextView *requestTextView;

@property (weak, nonatomic) IBOutlet UITextView *responseTextView;



@property (strong, nonatomic) NSString *requestType;


- (IBAction)segmentValueChanged:(id)sender;

- (IBAction)doLogin:(id)sender;
- (IBAction)doUserInfo:(id)sender;
- (IBAction)doDownload:(id)sender;
- (IBAction)doUpload:(id)sender;
- (IBAction)doSelectFile:(id)sender;


- (void) updateReceivedBytes;

@end
