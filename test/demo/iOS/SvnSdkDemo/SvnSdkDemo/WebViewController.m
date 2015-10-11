//
//  WebViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 13-7-2.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import "WebViewController.h"
#import "GuideViewController.h"
#import "UIHelper.h"

#import <SvnSdk/SvnHttpURLProtocol.h>
#import <SvnSdk/SecBrowHttpProtocol.h>
#import "UIView+Toast.h"

@implementation WebViewController


- (void)ssoCallback:(NSString *)url
{
    NSString *msg = [NSString stringWithFormat:@"callback:%@", url];
    NSLog(msg);
    dispatch_async(dispatch_get_main_queue(), ^{
         [self.view makeToast:msg duration:3.0 position:CSToastPositionBottom];
    });
    
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
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
    [self.goButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.goButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    
    
    
    [self.httpURL setText:@"http://172.22.8.206:8080/"];
    //[self.httpURL setText:@"http://3g.163.com"];
    //[self.httpURL setText:@"http://m.sohu.com"];
    //[self.httpURL setText:@"http://www.sina.com.cn"];
    
    self.httpWebView.delegate = self;
    
    //[NSURLProtocol registerClass:[SvnHttpURLProtocol class]];
    
    [SecBrowHttpProtocol setExceptionAddressList:@"*.huawei.com" defaultUseVPN:YES];
    [NSURLProtocol registerClass:[SecBrowHttpProtocol class]];
    
    [AnyOfficeWebView setSSOCallback:self];
    self.anyofficeWebView.hidden = YES;

}


- (IBAction)navigateBack
{
    [self.navigationController popViewControllerAnimated:YES];
}


- (IBAction)navigateGuide
{
    [self performSegueWithIdentifier:@"guide" sender:self];
}



- (IBAction)btnGoClicked:(id)sender {
    [self.httpURL resignFirstResponder];
    NSURL *url = [NSURL URLWithString:[self.httpURL text]];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    
    if(self.segmentControl.selectedSegmentIndex == 0)
    {
        [self.httpWebView loadRequest:request];
    }
    else
    {
        [self.anyofficeWebView loadRequest:request];
    }

    
}

- (IBAction)segmentValueChanged:(UISegmentedControl *)sender {
    
    switch (sender.selectedSegmentIndex) {
        case 0:
            self.httpWebView.hidden = NO;
            self.anyofficeWebView.hidden = YES;
            break;
        case 1:
            self.httpWebView.hidden = YES;
            self.anyofficeWebView.hidden = NO;
            break;
        default:
            break;
            
    }
}

- (IBAction)backToParent:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}



- (void)webViewDidStartLoad:(UIWebView *)webView
{
    
}
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    NSLog(@"finished loading");
    NSString *htmlSource = [self.httpWebView stringByEvaluatingJavaScriptFromString:@"document.getElementsByTagName('head')[0].outerHTML;"];
    //NSString *htmlSource = [[NSString alloc] initWithData:[[httpWebView request] HTTPBody] encoding:NSASCIIStringEncoding];
    NSLog(@"Page Content Head:\r\n%@", htmlSource);
}
- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    NSLog(@"ERROR : %@",error); //Get informed of the error FIRST
    if([error code] == NSURLErrorCancelled)
        return;
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
    
    [alertView show];
}


- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"guide"])
    {
        GuideViewController *guideViewController =  [segue destinationViewController];
        guideViewController.guideType = @"webview";
       
    }      
}

@end
