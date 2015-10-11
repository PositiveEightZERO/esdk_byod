//
//  MDMCheckViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 14-9-2.
//
//

#import "MDMCheckViewController.h"

#import "UIHelper.h"

#import <SvnSdk/MdmCheck.h>
#import <SvnSdk/MdmDeviceIdInfo.h>

@interface MDMCheckViewController ()

@end

@implementation MDMCheckViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    // Custom initialization
    UIButton *backButton = [UIHelper navButton:@"Back"];
    [backButton addTarget:self action:@selector(navigateBack) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *backButtonItem = [[UIBarButtonItem alloc] initWithCustomView:backButton];
    backButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.leftBarButtonItem=backButtonItem;
    

    
    UIEdgeInsets insets = UIEdgeInsetsMake(20, 10, 20, 10);
    // 指定为拉伸模式，伸缩后重新赋值
    UIImage *image= [[UIImage imageNamed:@"btn_background.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    UIImage *imageSelected= [[UIImage imageNamed:@"btn_background_selected.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    [self.mdmCheckButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.mdmCheckButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (IBAction)navigateBack
{
    [self.navigationController popViewControllerAnimated:YES];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/



- (IBAction)doMDMCheck:(id)sender {
    
    NSString *deviceId = [MdmDeviceIdInfo getDeviceID];
    NSLog(@"deviceId:%@", deviceId);
    
    
    UIImage *okImage= [UIImage imageNamed:@"icon_correct.png"];
    UIImage *failImage= [UIImage imageNamed:@"icon_wrong.png"];
    
    
    self.checkResultImageView.image = failImage;
    self.mdmEnabledImageView.image = failImage;
    self.deviceBindedImageView.image = failImage;
    self.deviceRootedImageView.image = failImage;
    
    self.pwdCheckImageView.image = failImage;
    self.appCheckImageView.image = failImage;
    self.otherCheckImageView.image = failImage;
    
    
    MDMCheckResult result = [MdmCheck checkMdmSpecific];
    NSLog(@"MDMCheckResult-isSuccess:%d, isMDMEnabled:%@, bindResult:%d, isRoot:%@, isPwdCheckOK:%@, isAppCheckOK:%@, isLongTimeNoLogin:%@, isOtherCheckOK:%@", result.isSuccess, result.isMDMEnabled?@"true":@"false", result.bindResult, result.isRoot?@"true":@"false", result.isPwdCheckOK?@"true":@"false", result.isAppCheckOK?@"true":@"false", result.isLongTimeNoLogin?@"true":@"false", result.isOtherCheckOK?@"true":@"false");
    
  
    //0为成功
    if(result.isSuccess == 0)
    {
        self.checkResultImageView.image = okImage;
        
        if(result.isMDMEnabled)
        {
            self.mdmEnabledImageView.image = okImage;
        }
        
        if(result.bindResult == 1 || result.bindResult == 10)
        {
            self.deviceBindedImageView.image = okImage;
            
            if(!result.isRoot)
            {
                self.deviceRootedImageView.image = okImage;
            }
            
            if(result.isPwdCheckOK)
            {
                self.pwdCheckImageView.image = okImage;
            }
            
            if(result.isAppCheckOK)
            {
                self.appCheckImageView.image = okImage;
            }
            
            if(result.isOtherCheckOK)
            {
                self.otherCheckImageView.image = okImage;
            }
        }
      
       

    }
   
}
@end
