//
//  FsmViewController.h
//  SvnHttpFsmDemo
//
//  Created by wqy on 13-7-8.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DataEncryptViewController : UIViewController

@property (weak, nonatomic) IBOutlet UIButton *encryptButton;
@property (weak, nonatomic) IBOutlet UIButton *decryptButton;

@property (weak, nonatomic) IBOutlet UITextView *originDataTextView;

@property (weak, nonatomic) IBOutlet UITextView *encryptedDataTextView;

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;

@property (weak, nonatomic) IBOutlet UIView *contentView;

- (IBAction)encryptData:(id)sender;
- (IBAction)decryptData:(id)sender;

@end
