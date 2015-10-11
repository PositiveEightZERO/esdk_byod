//
//  MenuViewControllerTableViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 14-8-25.
//
//

#import "MenuViewController.h"

#import "MenuViewCell.h"
#import "MenuSectionInfo.h"

#import "UIHelper.h"
#import "HttpBusinessViewController.h"
#import "FaqsViewController.h"

static NSString *SectionHeaderViewIdentifier = @"SectionHeaderViewIdentifier";

@interface MenuViewController ()
{
    int expandedIndex;
    APLSectionHeaderView *expandedHeaderView;
    
    
}

@end

@implementation MenuViewController

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        
        
    }
    return self;
}


- (IBAction)navigateBack
{
    [self.navigationController popViewControllerAnimated:YES];
}


- (IBAction)navigateFaqs
{
    
    if(expandedIndex != -1 || [self.sections count] == 1)
    {
        [self performSegueWithIdentifier:@"faqs" sender:self];
    }
    //[self.navigationController popViewControllerAnimated:YES];
}


- (void)viewDidLoad
{
    [super viewDidLoad];


    // Custom initialization
    UIButton *backButton = [UIHelper navButton:@"Back"];
    [backButton addTarget:self action:@selector(navigateBack) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *backButtonItem = [[UIBarButtonItem alloc] initWithCustomView:backButton];
    backButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.leftBarButtonItem=backButtonItem;
    
    
    UIButton *faqsButton = [UIHelper navButton:@"FAQs"];
    [faqsButton addTarget:self action:@selector(navigateFaqs) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *faqsButtonItem = [[UIBarButtonItem alloc] initWithCustomView:faqsButton];
    faqsButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.rightBarButtonItem=faqsButtonItem;
    
    
    UINib *sectionHeaderNib = [UINib nibWithNibName:@"SectionHeaderView" bundle:nil];
    [self.tableView registerNib:sectionHeaderNib forHeaderFooterViewReuseIdentifier:SectionHeaderViewIdentifier];
    self.tableView.userInteractionEnabled = TRUE;
    expandedIndex = -1;
    

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return [self.sections count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    
    if ([self.sections count] == 1 || expandedIndex == section)
    {
        MenuSectionInfo * sectionInfo = [self.sections objectAtIndex:section];
        NSString *key = [sectionInfo sectionName];
        // Return the number of rows in the section.
        return [[sectionInfo menuItems] count]; // return rows when expanded
    }
    else
    {
        return 0;
    }
    
    
    
    
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentifier = @"MenuCellIdentifier";
    MenuViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    
    MenuSectionInfo * sectionInfo = [self.sections objectAtIndex:[indexPath section]];
    
    MenuItem * item = [[sectionInfo menuItems] objectAtIndex:[indexPath row]];
    
    cell.itemLabel.text = [item itemName];
    return cell;
}

//- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
//{
//     NSString *key = [[menuItems allKeys] objectAtIndex:section];
//    return  key;
//}


-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    if([self.sections count] <= 1)
    {
        return nil;
    }
    APLSectionHeaderView *sectionHeaderView = [self.tableView dequeueReusableHeaderFooterViewWithIdentifier:SectionHeaderViewIdentifier];
    
    MenuSectionInfo * sectionInfo = [self.sections objectAtIndex:section];
    sectionHeaderView.titleLabel.text = [sectionInfo sectionName];
    sectionHeaderView.section = section;
    sectionHeaderView.delegate = self;
    
    return sectionHeaderView;
}


- (BOOL)tableView:(UITableView *)tableView canCollapseSection:(NSInteger)section
{
    return YES;
    
}



- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return 40.0f;
}


- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {
    
    if ([[self sections] count] <= 1) {
        return 0;
    } else {
        return 48;
    }
}


- (void) tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath
{
    MenuSectionInfo * sectionInfo = [self.sections objectAtIndex:[indexPath section]];
    MenuItem *item = [[sectionInfo menuItems] objectAtIndex:[indexPath row]];
    [self performSegueWithIdentifier:[item itemAction] sender:self];
}


-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    MenuSectionInfo * sectionInfo = [self.sections objectAtIndex:[indexPath section]];
    MenuItem *item = [[sectionInfo menuItems] objectAtIndex:[indexPath row]];
    [self performSegueWithIdentifier:[item itemAction] sender:self];
}


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



// Row display. Implementers should *always* try to reuse cells by setting each cell's reuseIdentifier and querying for available reusable cells with dequeueReusableCellWithIdentifier:
// Cell gets various attributes set automatically based on table (separators) and data source (accessory views, editing controls)
#pragma mark - SectionHeaderViewDelegate

- (void)sectionHeaderView:(APLSectionHeaderView *)sectionHeaderView sectionOpened:(NSInteger)sectionOpened {
    
    NSInteger previousOpenSectionIndex = expandedIndex;
    expandedIndex = sectionOpened;
    
    /*
     Create an array containing the index paths of the rows to insert: These correspond to the rows for each quotation in the current section.
     */
    
    MenuSectionInfo * toInsertSectionInfo = [self.sections objectAtIndex:sectionOpened];
    
    
    NSInteger countOfRowsToInsert = [toInsertSectionInfo.menuItems count];
    NSMutableArray *indexPathsToInsert = [[NSMutableArray alloc] init];
    for (NSInteger i = 0; i < countOfRowsToInsert; i++) {
        [indexPathsToInsert addObject:[NSIndexPath indexPathForRow:i inSection:sectionOpened]];
    }
    
    /*
     Create an array containing the index paths of the rows to delete: These correspond to the rows for each quotation in the previously-open section, if there was one.
     */
    NSMutableArray *indexPathsToDelete = [[NSMutableArray alloc] init];
    
    
    if (previousOpenSectionIndex != -1) {
        
        MenuSectionInfo * toDeleteSectionInfo = [self.sections objectAtIndex:previousOpenSectionIndex];
        [expandedHeaderView toggleOpenWithUserAction:NO];
        NSInteger countOfRowsToDelete = [toDeleteSectionInfo.menuItems count];
        for (NSInteger i = 0; i < countOfRowsToDelete; i++) {
            [indexPathsToDelete addObject:[NSIndexPath indexPathForRow:i inSection:previousOpenSectionIndex]];
        }
    }
    
    // style the animation so that there's a smooth flow in either direction
    UITableViewRowAnimation insertAnimation;
    UITableViewRowAnimation deleteAnimation;
    if (previousOpenSectionIndex == -1 || sectionOpened < previousOpenSectionIndex) {
        insertAnimation = UITableViewRowAnimationTop;
        deleteAnimation = UITableViewRowAnimationBottom;
    }
    else {
        insertAnimation = UITableViewRowAnimationBottom;
        deleteAnimation = UITableViewRowAnimationTop;
    }
    
    // apply the updates
    [self.tableView beginUpdates];
    [self.tableView insertRowsAtIndexPaths:indexPathsToInsert withRowAnimation:insertAnimation];
    [self.tableView deleteRowsAtIndexPaths:indexPathsToDelete withRowAnimation:deleteAnimation];
    [self.tableView endUpdates];
    
    expandedHeaderView = sectionHeaderView;
}


- (void)sectionHeaderView:(APLSectionHeaderView *)sectionHeaderView sectionClosed:(NSInteger)sectionClosed {
    
    /*
     Create an array of the index paths of the rows in the section that was closed, then delete those rows from the table view.
     */
    
    expandedIndex = -1;
    
    NSInteger countOfRowsToDelete = [self.tableView numberOfRowsInSection:sectionClosed];
    
    if (countOfRowsToDelete > 0) {
        NSMutableArray *indexPathsToDelete = [[NSMutableArray alloc] init];
        for (NSInteger i = 0; i < countOfRowsToDelete; i++) {
            [indexPathsToDelete addObject:[NSIndexPath indexPathForRow:i inSection:sectionClosed]];
        }
        [self.tableView deleteRowsAtIndexPaths:indexPathsToDelete withRowAnimation:UITableViewRowAnimationTop];
    }
    
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"menu_asihttprequest"])
    {
        HttpBusinessViewController *httpViewController =  [segue destinationViewController];
        httpViewController.requestType = @"ASIHTTPRequest";
    }
    else if([[segue identifier] isEqualToString:@"menu_nsurlconnection"])
    {
        HttpBusinessViewController *httpViewController =  [segue destinationViewController];
        httpViewController.requestType = @"NSURLConnection";
    }
    else if([[segue identifier] isEqualToString:@"faqs"])
    {
        
        FaqsViewController *faqsViewController =  [segue destinationViewController];
        
        if(expandedIndex != -1)
        {
            faqsViewController.faqsType = expandedIndex + 2;
        }
        else if([self.sections count] == 1)
        {
            faqsViewController.faqsType = 3;
        }
    }


}


@end
