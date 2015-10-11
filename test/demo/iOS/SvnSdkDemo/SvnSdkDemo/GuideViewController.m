//
//  GuideViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 14-9-3.
//
//

#import "GuideViewController.h"

#import "UIHelper.h"

@interface GuideViewController ()

@end

@implementation GuideViewController

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
    
    UIButton *backButton = [UIHelper navButton:@"Back"];
    [backButton addTarget:self action:@selector(navigateBack) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *backButtonItem = [[UIBarButtonItem alloc] initWithCustomView:backButton];
    backButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.leftBarButtonItem=backButtonItem;

    if([self guideType])
    {
        [self loadGuide:[NSString stringWithFormat:@"guide_%@.txt", [self guideType]]];
    }

    
   
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


- (void)loadGuide:(NSString *)faqsFile
{
    
    NSString *path = [[NSBundle mainBundle] pathForResource:faqsFile ofType:nil];
    NSString* fileContents =
    [NSString stringWithContentsOfFile:path
                              encoding:NSUTF8StringEncoding error:nil];
    
    if(fileContents)
    {
        fileContents = [fileContents stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        NSRange range = [fileContents rangeOfCharacterFromSet: [NSCharacterSet newlineCharacterSet]];
        if(range.location != NSNotFound)
        {
            NSString *title = [fileContents substringToIndex:range.location];
            NSString *guide = [fileContents substringFromIndex:range.location + range.length];
            
            
            self.titleTextView.text = [title stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
            
            self.guideTextView.text = [guide stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
            
            
        }
        
        
        
    }
    else
    {
        NSLog(@"open file %@ failed!", faqsFile);
    }
    
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

@end
