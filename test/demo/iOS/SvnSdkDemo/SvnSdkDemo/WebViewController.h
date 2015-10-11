//
//  WebViewController.h
//  SvnSdkDemo
//
//  Created by l00174413 on 13-7-2.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <SvnSdk/AnyOfficeWebView.h>

@interface WebViewController : UIViewController<UIWebViewDelegate, AnyOfficeWebViewSSODelegate>

@property (weak, nonatomic) IBOutlet UIWebView *httpWebView;
@property (weak, nonatomic) IBOutlet AnyOfficeWebView *anyofficeWebView;
@property (weak, nonatomic) IBOutlet UISegmentedControl *segmentControl;


@property (weak, nonatomic) IBOutlet UITextField *httpURL;
@property (weak, nonatomic) IBOutlet UIButton *goButton;

- (IBAction)btnGoClicked:(id)sender;

- (IBAction)segmentValueChanged:(id)sender;

@end
