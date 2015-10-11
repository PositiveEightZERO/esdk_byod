//
//  MDMCheckViewController.h
//  SvnSdkDemo
//
//  Created by l00174413 on 14-9-2.
//
//

#import <UIKit/UIKit.h>

@interface MDMCheckViewController : UIViewController

@property (weak, nonatomic) IBOutlet UIButton *mdmCheckButton;

@property (weak, nonatomic) IBOutlet UIImageView *checkResultImageView;

@property (weak, nonatomic) IBOutlet UIImageView *mdmEnabledImageView;
@property (weak, nonatomic) IBOutlet UIImageView *deviceBindedImageView;
@property (weak, nonatomic) IBOutlet UIImageView *deviceRootedImageView;
@property (weak, nonatomic) IBOutlet UIImageView *pwdCheckImageView;
@property (weak, nonatomic) IBOutlet UIImageView *appCheckImageView;
@property (weak, nonatomic) IBOutlet UIImageView *otherCheckImageView;


- (IBAction)doMDMCheck:(id)sender;

@end
