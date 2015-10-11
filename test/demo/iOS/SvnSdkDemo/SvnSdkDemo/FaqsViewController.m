//
//  FaqsViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 14-9-2.
//
//

#import "FaqsViewController.h"
#import "FaqsTableViewCell.h"
#import "UIHelper.h"

@interface FaqsViewController ()
{
    NSMutableArray *faqs;
}

@end

@implementation FaqsViewController

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
    
    // Custom initialization
    UIButton *backButton = [UIHelper navButton:@"Back"];
    [backButton addTarget:self action:@selector(navigateBack) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *backButtonItem = [[UIBarButtonItem alloc] initWithCustomView:backButton];
    backButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.leftBarButtonItem=backButtonItem;
    
    faqs = [[NSMutableArray alloc] init];
    
    switch (self.faqsType) {
        case 1:
            [self loadFaqs:@"faqs_tunnel.txt"];
            break;
        case 2:
            [self loadFaqs:@"faqs_http.txt"];
            break;
        case 3:
            [self loadFaqs:@"faqs_encrypt.txt"];
            break;
        case 4:
            [self loadFaqs:@"faqs_mdm.txt"];
            break;
   
        default:
            break;
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



- (void)loadFaqs:(NSString *)faqsFile
{
    
    NSString *path = [[NSBundle mainBundle] pathForResource:faqsFile ofType:nil];
    NSString* fileContents =
    [NSString stringWithContentsOfFile:path
                              encoding:NSUTF8StringEncoding error:nil];
    
    if(fileContents)
    {
        
        NSError *error = NULL;
        NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"(\\[===[Q|A]===\\])" options:NSRegularExpressionCaseInsensitive error:&error];
        
        NSArray * matches = [regex matchesInString:fileContents options:0 range:NSMakeRange(0, [fileContents length])];
        
        //NSLog(@"%@", matches);
        if (matches.count != 0)
        {
            for (int i=0; i<matches.count -1; i+=2)
            {
                NSTextCheckingResult *questionStart = matches[i];
                NSTextCheckingResult *answerStart = matches[i + 1];
                
                int questionStartIndex = questionStart.range.location + questionStart.range.length;
                int questionEndIndex = answerStart.range.location;
                
                NSString *question = [fileContents substringWithRange:NSMakeRange(questionStartIndex,  questionEndIndex - questionStartIndex)];
                
                NSString *answer = nil;
                
                if(i>=matches.count -2)
                {
                    answer = [fileContents substringFromIndex:answerStart.range.location + questionStart.range.length];
                }
                else
                {
                    NSTextCheckingResult *nextQuestionStart = matches[i + 2];
                    
                    int answerStartIndex = answerStart.range.location + questionStart.range.length;
                    int answerEndIndex = nextQuestionStart.range.location;
                    
                    
                    answer = [fileContents substringWithRange:NSMakeRange(answerStartIndex, answerEndIndex - answerStartIndex )];
                }
                
                //NSLog(@"%@,%@",question,answer);
                
                if(question && answer)
                {
                    [faqs addObject:question];
                    [faqs addObject:answer];
                }
            }
        }
    }
    else
    {
        NSLog(@"open file %@ failed!", faqsFile);
    }
    
}



#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    
    return [faqs count] /2 ;
    
    
    
    
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentifier = @"FaqsCellIdentifier";
    FaqsTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    
    NSString *question = faqs[indexPath.row*2];
    NSString *answer = faqs[indexPath.row*2 + 1];
    
    cell.questionTextView.text = [question stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    [cell.questionTextView sizeToFit];
    
    cell.answerTextView.text = [answer stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    [cell.answerTextView sizeToFit];
    return cell;
}

/*
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:<#@"reuseIdentifier"#> forIndexPath:indexPath];
    
    // Configure the cell...
    
    return cell;
}
*/

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/

/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
{
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

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
