//
//  SpeedCompareViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 13-8-14.
//
//

#import "SpeedCompareViewController.h"
#import "SvnSdk/SvnHttpURLProtocol.h"
#import <SvnSdk/SecBrowHttpProtocol.h>
#import "SvnASIHTTPRequest.h"

#import "APLSectionHeaderView.h"

#import "UIHelper.h"

static NSString *SectionHeaderViewIdentifier = @"SectionHeaderViewIdentifier";


@interface SpeedCompareViewController () <SectionHeaderViewDelegate, UIWebViewDelegate, NSURLConnectionDelegate>
{

    NSArray *selections;
    
    int selectedIndex;
    
    NSDate *beginDate;
    NSDate *endDate;
    
    double useSvnInterval;
    double useSvnTotalTime;
    int useSvnTimes;
    
    double noSvnInterval;
    double noSvnTotalTime;
    int noSvnTimes;
    
}


-(void) doHttpRequestThroughNSURLConnection;
-(void) doHttpRequestThroughASI;
-(void) doHttpRequestThroughWebView;


- (void)updateResult:(NSString*)result;

- (void)updateTimeResult;

@end

@implementation SpeedCompareViewController

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
    
    if([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
    {
        self.automaticallyAdjustsScrollViewInsets = NO;
    }
    
    
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
    
    
    selections = [[NSArray alloc] initWithObjects:@"NSURLConnection", @"ASIHTTPRequest", @"UIWebView", nil];
    
    selectedIndex = 0;
    self.requestResult.hidden = NO;
    self.webview.hidden = YES;
    
    self.requestTypeLabel.text = [NSString stringWithFormat:@"Request Type:%@", selections[selectedIndex]];
    
    useSvnTotalTime = 0;
    useSvnTimes = 0;
    
    noSvnTotalTime = 0;
    noSvnTimes = 0;
    _useSVN = YES;

    //[_httpURL setText:@"http://3g.163.com/touch"];
    [_httpURL setText:@"http://172.22.8.206:8080"];
    
    
	// Do any additional setup after loading the view.
}

- (IBAction)navigateBack
{
    [self.navigationController popViewControllerAnimated:YES];
}


- (IBAction)navigateGuide
{
    //[self.navigationController popViewControllerAnimated:YES];
}


- (IBAction)selectRequestType:(id)sender {
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Select Request Type" message: @"Request Type:" delegate:self  cancelButtonTitle:selections[2] otherButtonTitles:selections[0], selections[1], nil];
    [alert show];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    
    NSString *alertSelection = [alertView buttonTitleAtIndex:buttonIndex];
    
    selectedIndex = [selections indexOfObject:alertSelection];
    NSLog(@"selectedIndex:%d", selectedIndex);
    
    self.requestTypeLabel.text = [NSString stringWithFormat:@"Request Type:%@", selections[selectedIndex]];
    
    self.timeResult.text =@"Time Result:";
    
    if(selectedIndex == 2)
    {
        self.requestResult.hidden = YES;
        self.webview.hidden = NO;
    }
    else
    {
        self.requestResult.hidden = NO;
        self.webview.hidden = YES;
    }
    
    useSvnTotalTime = 0;
    useSvnTimes = 0;
    
    noSvnTotalTime = 0;
    noSvnTimes = 0;
    
}

- (IBAction)toggleUseSVN:(id)sender {
    UISwitch* useSVNSwitch = (UISwitch*)sender;
    _useSVN = [useSVNSwitch isOn];
    
    if(_useSVN)
    {
        [_httpURL setText:@"http://172.22.8.206:8080"];
    }
    else
    {
        [_httpURL setText:@"http://10.170.102.180:8080"];
    }
    
}

- (IBAction)doHttpRequest:(id)sender {

    self.requestResult.text = @"";
    
    self.requestButton.userInteractionEnabled = NO;
    
    switch (selectedIndex) {
        case 0:
           [self doHttpRequestThroughNSURLConnection];
            break;
        case 1:
            [self doHttpRequestThroughASI];
            break;
        case 2:
            [self doHttpRequestThroughWebView];
            break;
        default:
            break;
    }
    
    

}




-(void) doHttpRequestThroughNSURLConnection
{
    if(_useSVN)
    {
        [NSURLProtocol registerClass:[SecBrowHttpProtocol class]];
    }
    else
    {
        [NSURLProtocol unregisterClass:[SecBrowHttpProtocol class]];
    }
    
    

    NSURL *URL = [NSURL URLWithString:[_httpURL text]];
    //NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL ];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:60.0*60];
    
    [request setHTTPMethod:@"GET"];
    //[request setValue:@"SvnSdkDemo 1.0 (iPhone; iPhone OS 5.0.1; zh_CN)" forHTTPHeaderField:@"User-Agent"];
    
    [NSURLConnection connectionWithRequest:request delegate:self];
    [self updateResult:@"start request"];
    
    beginDate = [NSDate date];
    
}


-(void) doHttpRequestThroughASI
{
   NSURL *URL = [NSURL URLWithString:[_httpURL text]];
    
    SvnASIHTTPRequest *request = [SvnASIHTTPRequest requestWithURL:URL];
    [request setDelegate:self];
    
    [request setUseSVN:_useSVN];
    
    [request startAsynchronous];
    
    beginDate = [NSDate date];
    [self updateResult:@"start request"];
}


-(void) doHttpRequestThroughWebView
{
    if(_useSVN)
    {
        [NSURLProtocol registerClass:[SecBrowHttpProtocol class]];
    }
    else
    {
        [NSURLProtocol unregisterClass:[SecBrowHttpProtocol class]];
    }
    
    [self.httpURL resignFirstResponder];
    NSURL *URL = [NSURL URLWithString:[_httpURL text]];
    //NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL ];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:60.0*60];
    self.webview.delegate = self;
    
    [self.webview loadRequest:request];
    beginDate = [NSDate date];
    
}




- (void)viewDidUnload {

    [self setHttpURL:nil];
    [super viewDidUnload];
}

- (BOOL) textFieldShouldReturn:(UITextField *)textFieldView
{
    [_httpURL resignFirstResponder];
    return NO;
}

//---------------------------------------
// You have to implement below four methods
//---------------------------------------

- (void)connection:(NSURLConnection *)theConnection didReceiveResponse:(NSURLResponse *)response
{

    [self updateResult:@"didReceiveResponse"];
   
    
}


- (void)connection:(NSURLConnection *)theConnection didFailWithError:(NSError *)error
// A delegate method called by the NSURLConnection if the connection fails.
// We shut down the connection and display the failure.  Production quality code
// would either display or log the actual error.
{
    [self updateResult:@"didFailWithError"];
    
    self.requestButton.userInteractionEnabled = YES;
}

- (void)connectionDidFinishLoading:(NSURLConnection *)theConnection
// A delegate method called by the NSURLConnection when the connection has been
// done successfully.  We shut down the connection with a nil status, which
// causes the image to be displayed.
{
    
    endDate = [NSDate date];

    [self updateResult:@"didFinishLoading"];
    [self updateTimeResult];
    
    self.requestButton.userInteractionEnabled = YES;
}


- (void)URLProtocol:(NSURLProtocol *)protocol wasRedirectedToRequest:(NSURLRequest *)request redirectResponse:(NSURLResponse *)redirectResponse
{
    [self updateResult:[NSString stringWithFormat:@"wasRedirectedToRequest:%@", [request URL]]];
}



- (void)webViewDidStartLoad:(UIWebView *)webView
{
    
}
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    NSLog(@"finished loading");
    endDate = [NSDate date];
    
    
    [self updateTimeResult];
   self.requestButton.userInteractionEnabled = YES;
}
- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    NSLog(@"ERROR : %@",error); //Get informed of the error FIRST
    if([error code] == NSURLErrorCancelled)
        return;
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
    
    [alertView show];
    self.requestButton.userInteractionEnabled = YES;
}




#pragma mark - ASIHTTPRequest Delegate
//---------------------------------------
// You have to implement below four methods
//---------------------------------------

- (void)requestFinished:(ASIHTTPRequest *)request

{
    
    NSLog(@"requestFinished");
    
    [self updateResult:@"requestFinished"];
    endDate = [NSDate date];
    
    
    [self updateTimeResult];
    self.requestButton.userInteractionEnabled = YES;
    
}

- (void)requestFailed:(ASIHTTPRequest *)request
{
    
    NSError *error = [request error];
    
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
    
    [alertView show];
    self.requestButton.userInteractionEnabled = YES;
}

- (void)request:(ASIHTTPRequest *)request didReceiveResponseHeaders:(NSDictionary *)responseHeaders
{
    
   
}





- (void)updateResult:(NSString*)result
{
    NSDate * date = [NSDate date];
    NSString *result1 = [NSString stringWithFormat:@"%@\n%@:%@", self.requestResult.text, date, result];
    self.requestResult.text = result1;
    
}


- (void)updateTimeResult
{
    NSTimeInterval interval = [endDate timeIntervalSinceDate: beginDate];

    NSLog(@"request ellapse:%f",  interval);
    
    double avgTime = 0.0f;
    
    NSString *timeString;
    
    if(_useSVN)
    {
        useSvnInterval = interval;
        useSvnTotalTime += interval;
        useSvnTimes += 1;
        
        avgTime = useSvnTotalTime / useSvnTimes;
        
        
        timeString = [NSString stringWithFormat:@"%d-Use SVN-Time:%f, Avg:%f", useSvnTimes, useSvnInterval, avgTime ];
    }
    else
    {
        noSvnInterval = interval;
        noSvnTotalTime += interval;
        noSvnTimes += 1;
        avgTime = noSvnTotalTime / noSvnTimes;
        
        timeString = [NSString stringWithFormat:@"%d-No SVN-Time:%f, Avg:%f", noSvnTimes, noSvnInterval, avgTime ];
    }
    
    
    self.timeResult.text = [NSString stringWithFormat:@"%@\n%@", timeString, self.timeResult.text ];
    

}

@end
