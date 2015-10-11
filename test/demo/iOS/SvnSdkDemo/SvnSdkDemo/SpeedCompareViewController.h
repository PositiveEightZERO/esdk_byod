//
//  SpeedCompareViewController.h
//  SvnSdkDemo
//
//  Created by l00174413 on 13-8-14.
//
//

#import <UIKit/UIKit.h>

@interface SpeedCompareViewController : UIViewController <UITextFieldDelegate, NSURLProtocolClient, UITableViewDataSource, UITableViewDelegate>
{
    BOOL _useSVN;
    long long _totalBytes;
    NSMutableData* _receivedData;
}
@property (weak, nonatomic) IBOutlet UILabel *requestTypeLabel;

@property (weak, nonatomic) IBOutlet UITextField *httpURL;

@property (weak, nonatomic) IBOutlet UITextView *requestResult;
@property (weak, nonatomic) IBOutlet UITextView *timeResult;

@property (weak, nonatomic) IBOutlet UIWebView *webview;
@property (weak, nonatomic) IBOutlet UIButton *requestButton;


- (IBAction)selectRequestType:(id)sender;

- (IBAction)toggleUseSVN:(id)sender;
- (IBAction)doHttpRequest:(id)sender;


@end
