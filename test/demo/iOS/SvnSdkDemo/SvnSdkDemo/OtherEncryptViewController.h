//
//  OtherEncryptViewController.h
//  SvnSdkDemo
//
//  Created by l00174413 on 8/14/15.
//
//

#import <UIKit/UIKit.h>

@interface OtherEncryptViewController : UIViewController


@property (weak, nonatomic) IBOutlet UIView *contentView;

@property (weak, nonatomic) IBOutlet UITextView *logView;

@property (weak, nonatomic) IBOutlet UIButton *manageTestButton;
@property (weak, nonatomic) IBOutlet UIButton *handleTestButton;
@property (weak, nonatomic) IBOutlet UIButton *streamTestButton;
@property (weak, nonatomic) IBOutlet UIButton *extendTestButton;

- (IBAction)fileManagerTest:(id)sender;

- (IBAction)fileStreamTest:(id)sender;

- (IBAction)fileHandleTest:(id)sender;

- (IBAction)classExtendTest:(id)sender;

@end
