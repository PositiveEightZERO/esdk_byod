//
//  MoreEncryptViewController.h
//  SvnSdkDemo
//
//  Created by l00174413 on 13-10-23.
//
//

#import <UIKit/UIKit.h>
#import <QuickLook/QuickLook.h>


#import "FileBrowseViewController.h"

@interface FileEncryptViewController : UIViewController<FileBrowseDelegate>
{

}
@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;

@property (weak, nonatomic) IBOutlet UILabel *basePathLabel;


@property (weak, nonatomic)IBOutlet UITextView *originPathTextView;
@property (weak, nonatomic)IBOutlet UITextView *encryptPathTextView;


@property (weak, nonatomic) IBOutlet UIButton *selectOriginButton;
@property (weak, nonatomic) IBOutlet UIButton *viewOriginButton;
@property (weak, nonatomic) IBOutlet UIButton *encryptButton;


@property (weak, nonatomic) IBOutlet UIButton *selectEncryptButton;
@property (weak, nonatomic) IBOutlet UIButton *viewEncryptedButton;
@property (weak, nonatomic) IBOutlet UIButton *decryptButton;


- (IBAction)viewOriginFile:(id)sender;
- (IBAction)encryptFile:(id)sender;
- (IBAction)viewEncryptedFile:(id)sender;
- (IBAction)decryptFile:(id)sender;


@end
