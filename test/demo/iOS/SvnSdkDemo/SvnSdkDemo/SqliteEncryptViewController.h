//
//  SqliteDBViewController.h
//  SvnSdkDemo
//
//  Created by l00174413 on 14-6-12.
//
//

#import <UIKit/UIKit.h>
#import <SvnSdk/anyoffice_sqlite3.h>

@interface SqliteEncryptViewController : UIViewController<UITableViewDataSource, UITableViewDelegate>


@property (weak, nonatomic) IBOutlet UISegmentedControl *segmentControl;


@property (weak, nonatomic) IBOutlet UITextField *nameText;
@property (weak, nonatomic) IBOutlet UITextField *ageText;

@property (weak, nonatomic) IBOutlet UIButton *executeButton;

@property (weak, nonatomic) IBOutlet UILabel *statusText;
@property (weak, nonatomic) IBOutlet UILabel *countsText;

@property (weak, nonatomic) IBOutlet UITableView *tableView;


- (IBAction)operationButtonClicked:(id)sender;
- (IBAction)executeSqlOperation:(id)sender;
- (IBAction)segmentValueChange:(id)sender;

@property (strong, nonatomic) NSString *databasePath;
@property (nonatomic) sqlite3 *personDB;

@end
