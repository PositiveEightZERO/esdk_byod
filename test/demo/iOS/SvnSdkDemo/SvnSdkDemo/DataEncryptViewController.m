//
//  FsmViewController.m
//  SvnHttpFsmDemo
//
//  Created by wqy on 13-7-8.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import "DataEncryptViewController.h"
#import "GuideViewController.h"

#include "SvnSdk/svn_define.h"
#include "SvnSdk/svn_api.h"
#include "SvnSdk/svn_file_api.h"
#include "SvnSdk/svn_file_api_ex.h"

#import "UIHelper.h"

unsigned char strToChar(char a, char b)
{
    char encoder[3] = {'\0','\0','\0'};
    encoder[0] = a;
    encoder[1] = b;
    return (char) strtol(encoder,NULL,16);
}

@implementation DataEncryptViewController



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
    
    
    //turn off scrolling and set the font details.
    self.originDataTextView.scrollEnabled = YES;
    self.originDataTextView.font = [UIFont fontWithName:@"Helvetica" size:14];
    self.originDataTextView.layer.borderColor = [UIColor grayColor].CGColor;
    self.originDataTextView.layer.borderWidth =1.0;
    self.originDataTextView.layer.cornerRadius =5.0;
    
    
    self.encryptedDataTextView.scrollEnabled = YES;
    self.encryptedDataTextView.font = [UIFont fontWithName:@"Helvetica" size:14];
    self.encryptedDataTextView.layer.borderColor = [UIColor grayColor].CGColor;
    self.encryptedDataTextView.layer.borderWidth = 1.0;
    self.encryptedDataTextView.layer.cornerRadius = 5.0;
    
    UIEdgeInsets insets = UIEdgeInsetsMake(10, 10, 10, 10);
    // 指定为拉伸模式，伸缩后重新赋值
    UIImage *image= [[UIImage imageNamed:@"btn_background.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    [self.encryptButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.decryptButton setBackgroundImage:image forState:UIControlStateNormal];

    self.originDataTextView.delegate = self;
    
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc]
                                      initWithTarget:self action:@selector(tapOnView:)];
    [self.view addGestureRecognizer: tapRecognizer];


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


- (NSString *)hexadecimalString:(NSData *)data
{
    /* Returns hexadecimal string of NSData. Empty string if data is empty.   */
    
    const unsigned char *dataBuffer = (const unsigned char *)[data bytes];
    
    if (!dataBuffer)
    {
        return [NSString string];
    }
    
    NSUInteger          dataLength  = [data length];
    NSMutableString     *hexString  = [NSMutableString stringWithCapacity:(dataLength * 2)];
    
    for (int i = 0; i < dataLength; ++i)
    {
        [hexString appendFormat:@"%02x", (unsigned int)dataBuffer[i]];
    }
    
    return [NSString stringWithString:hexString];
}

- (NSData *) decodeFromHexidecimal:(NSString*)hexString;
{
    const char * bytes = [hexString cStringUsingEncoding: NSUTF8StringEncoding];
    NSUInteger length = strlen(bytes);
    unsigned char * r = (unsigned char *) malloc(length / 2 + 1);
    unsigned char * index = r;
    
    while ((*bytes) && (*(bytes +1))) {
        *index = strToChar(*bytes, *(bytes +1));
        index++;
        bytes+=2;
    }
    *index = '\0';
    
    NSData * result = [NSData dataWithBytes:r length: length / 2];
    free(r);
    
    return result;
}


- (IBAction)encryptData:(id)sender {
    
    [self.originDataTextView resignFirstResponder];

    NSString *originText = self.originDataTextView.text;
    NSData *fileData = [originText dataUsingEncoding:NSUTF8StringEncoding];
    
    int fileLen = [fileData length];
    
    unsigned char origin[fileLen];
    [fileData getBytes:origin length:fileLen];
    unsigned char *outBuf = NULL;
    unsigned long outLen = 0;
    
    //数据加密
    int ret = SVN_API_EncryptLarge(origin, fileLen, &outBuf, &outLen);
    
    NSLog(@"SVN_API_EncryptLarge returns %d, result len:%d", ret, outLen);
    
    NSData *encryptedData = [NSData dataWithBytes:outBuf length:outLen];
    
    NSString *encryptedString = [self hexadecimalString:encryptedData];
    
    self.encryptedDataTextView.text = encryptedString;
    
}

- (IBAction)decryptData:(id)sender {
    
    
    NSString *encryptedText = self.encryptedDataTextView.text;
    NSData *fileData = [self decodeFromHexidecimal:encryptedText];
    
    int fileLen = [fileData length];
    
    unsigned char origin[fileLen];
    [fileData getBytes:origin length:fileLen];
    unsigned char *outBuf = NULL;
    unsigned long outLen = 0;
    
    //数据解密
    int ret = SVN_API_DecryptLarge(origin, fileLen, &outBuf, &outLen);
    NSLog(@"SVN_API_DecryptLarge returns %d, result len:%d", ret, outLen);
    
    
    NSData *decryptedData = [NSData dataWithBytes:outBuf length:outLen];
    
    NSString *decryptedString = [[NSString alloc] initWithData:decryptedData encoding:NSUTF8StringEncoding];
    
    self.originDataTextView.text = decryptedString;
    

}


- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"guide"])
    {
        GuideViewController *guideViewController =  [segue destinationViewController];
        guideViewController.guideType = @"dataencrypt";
        
    }
}



@end
