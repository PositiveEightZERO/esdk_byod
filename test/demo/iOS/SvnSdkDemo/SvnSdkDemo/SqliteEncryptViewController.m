//
//  SqliteDBViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 14-6-12.
//
//

#import "SqliteEncryptViewController.h"
#import "GuideViewController.h"
#import "Person.h"
#import "UIHelper.h"

static NSString *TableViewCellIdentifier = @"MyCells";

@interface SqliteEncryptViewController ()
{
   
    NSMutableArray * personList;
}

- (UIImage *)imageWithColor:(UIColor *)color;
@end

@implementation SqliteEncryptViewController

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
    
    UIImage *imageEmpty = [[UIImage imageNamed:@"btn_background.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    
    UIImage *imageSelected = [[UIImage imageNamed:@"btn_background_selected.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    [self.executeButton setBackgroundImage:imageEmpty forState:UIControlStateNormal];
    [self.executeButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    
    
    //tapRecognizer for dismiss keyboard.
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc]
                                             initWithTarget:self action:@selector(tapOnView:)];
    [tapRecognizer setCancelsTouchesInView:NO];
    //tapRecognizer.delegate = self;
    [self.view addGestureRecognizer: tapRecognizer];

    
    // Do any additional setup after loading the view.
    personList = [[NSMutableArray alloc] initWithCapacity:10];
    
    [self.tableView registerClass:[UITableViewCell class]
           forCellReuseIdentifier:TableViewCellIdentifier];
    self.tableView.dataSource = self;
    self.tableView.delegate = self;

    
    
    NSString *docsDir;
    NSArray *dirPaths;
    
    // Get the documents directory
    dirPaths = NSSearchPathForDirectoriesInDomains(
                                                   NSDocumentDirectory, NSUserDomainMask, YES);
    
    docsDir = dirPaths[0];
    
    // Build the path to the database file
    _databasePath = [[NSString alloc]
                     initWithString: [docsDir stringByAppendingPathComponent:
                                      @"person.db"]];
    
    NSFileManager *filemgr = [NSFileManager defaultManager];
    
    if ([filemgr fileExistsAtPath: _databasePath ] == NO)
    {
        const char *dbpath = [_databasePath UTF8String];
        
        int sqliteFlags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE ;
        int ret = sqlite3_open_s(dbpath, &_personDB, sqliteFlags, NULL, "sdk");
        
        if ( ret == SQLITE_OK)
        {
            char *errMsg;
            const char *sql_stmt =
            "CREATE TABLE IF NOT EXISTS person (personid integer primary key autoincrement, name text, age INTEGER)";
            
            if (sqlite3_exec(_personDB, sql_stmt, NULL, NULL, &errMsg) != SQLITE_OK)
            {
                _statusText.text = @"Failed to create table";
            }
            sqlite3_close(_personDB);
        } else {
            _statusText.text = @"Failed to open/create database";
        }
    }
    else
    {
        sqlite3_stmt *statement;
        const char *dbpath = [_databasePath UTF8String];
        int ret = sqlite3_open_s(dbpath, &_personDB, SQLITE_OPEN_READWRITE, NULL, "sdk");
        
        if (ret == SQLITE_OK)
        {
            NSString *querySQL = [NSString stringWithFormat:
                                  @"select * from  person",
                                  _nameText.text, _ageText.text];
            
            const char *query_stmt = [querySQL UTF8String];
            ret = sqlite3_prepare_v2(_personDB, query_stmt,
                                     -1, &statement, NULL);
            [personList removeAllObjects];
            while ((ret = sqlite3_step(statement)) == SQLITE_ROW)
            {
                Person * p = [[Person alloc] init];
                p.pid = sqlite3_column_int(statement, 0);
                p.name = [NSString stringWithUTF8String:sqlite3_column_text(statement, 1)];
                p.age = sqlite3_column_int(statement, 2);
                [personList addObject:p];
            }
            sqlite3_finalize(statement);
            sqlite3_close(_personDB);
            
            //重新加载数据
            [self.tableView reloadData];
            
            _countsText.text =[NSString stringWithFormat:@"%ld", personList.count];
        }
    }
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

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
 {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */
- (void)doInsertOperation
{
    sqlite3_stmt    *statement;
    int ret;
    const char *dbpath = [_databasePath UTF8String];
    
    
    //加密方式打开数据库
    if (sqlite3_open_s(dbpath, &_personDB, SQLITE_OPEN_READWRITE, NULL, "sdk") != SQLITE_OK)
    {
        _statusText.text = @"Failed to open person db";
        return;
    }
    
    //其他操作与原SQLite接口一致
    NSString *insertSQL = [NSString stringWithFormat:
                           @"insert into person (name, age) VALUES (\"%@\", %@)",
                           _nameText.text, _ageText.text];
    
    const char *insert_stmt = [insertSQL UTF8String];
    ret = sqlite3_prepare_v2(_personDB, insert_stmt,
                             -1, &statement, NULL);
    if(ret == SQLITE_OK)
    {
        if (sqlite3_step(statement) == SQLITE_DONE)
        {
            _statusText.text = @"person added";
            _nameText.text = @"";
            _ageText.text = @"";
            
        }
        else {
            _statusText.text = @"Failed to add person";
        }
    }
    else {
        _statusText.text = @"Failed to add person";
    }
    sqlite3_finalize(statement);
    
    
    NSString *querySQL = @"select * from person";
    
    const char *query_stmt = [querySQL UTF8String];
    sqlite3_prepare_v2(_personDB, query_stmt,
                       -1, &statement, NULL);
    [personList removeAllObjects];
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        Person * p = [[Person alloc] init];
        p.pid = sqlite3_column_int(statement, 0);
        p.name = [NSString stringWithUTF8String:sqlite3_column_text(statement, 1)];
        p.age = sqlite3_column_int(statement, 2);
        [personList addObject:p];
    }
    sqlite3_finalize(statement);
    sqlite3_close(_personDB);

}

- (void)doDeleteOperation
{
    sqlite3_stmt    *statement;
    int ret;
    const char *dbpath = [_databasePath UTF8String];
    
    if (sqlite3_open_s(dbpath, &_personDB, SQLITE_OPEN_READWRITE, NULL, "sdk") != SQLITE_OK)
    {
        _statusText.text = @"Failed to open person db";
        return;
    }
    
    NSIndexPath *selectedIndexPath = [[self tableView] indexPathForSelectedRow];
    if(!selectedIndexPath)
    {
        sqlite3_close(_personDB);
        return;
    }
    
    Person * person = personList[selectedIndexPath.row];
    NSString *deleteSQL = [NSString stringWithFormat:
                           @"delete from person where personid=%d",
                           person.pid];
    
    const char *delete_stmt = [deleteSQL UTF8String];
    ret = sqlite3_prepare_v2(_personDB, delete_stmt,
                             -1, &statement, NULL);
    if(ret == SQLITE_OK)
    {
        ret = sqlite3_step(statement);
        if (ret == SQLITE_DONE)
        {
            _statusText.text = @"person deleted";
            _nameText.text = @"";
            _ageText.text = @"";
            [personList removeObject:person];
            
        } else {
            _statusText.text = @"Failed to delete person";
        }
    }
    else {
        _statusText.text = @"Failed to delete person";
    }
    sqlite3_finalize(statement);
    sqlite3_close(_personDB);
}


- (void)doUpdateOperation
{
    sqlite3_stmt    *statement;
    int ret;
    const char *dbpath = [_databasePath UTF8String];
    
    if (sqlite3_open_s(dbpath, &_personDB, SQLITE_OPEN_READWRITE, NULL, "sdk") != SQLITE_OK)
    {
        _statusText.text = @"Failed to open person db";
        return;
    }
    
    NSIndexPath *selectedIndexPath = [[self tableView] indexPathForSelectedRow];
    if(!selectedIndexPath)
    {
        sqlite3_close(_personDB);
        return;
    }
    
    Person * person = personList[selectedIndexPath.row];
    
    
    NSString *updateSQL = [NSString stringWithFormat:
                           @"update person set name=\"%@\", age=%@ where personid=%d", _nameText.text, _ageText.text,
                           person.pid];
    
    const char *update_stmt = [updateSQL UTF8String];
    ret = sqlite3_prepare_v2(_personDB, update_stmt,
                             -1, &statement, NULL);
    if(ret == SQLITE_OK)
    {
        ret = sqlite3_step(statement);
        if (ret == SQLITE_DONE)
        {
            _statusText.text = @"person update";
            
            person.name =_nameText.text;
            person.age =[_ageText.text integerValue];
            
            _nameText.text = @"";
            _ageText.text = @"";
            
        }
        else
        {
            _statusText.text = @"Failed to update person";
        }
    }
    else
    {
        _statusText.text = @"Failed to update person";
    }
    sqlite3_finalize(statement);
    sqlite3_close(_personDB);
}

- (void)doQueryOperation
{
    sqlite3_stmt    *statement;
    int ret;
    const char *dbpath = [_databasePath UTF8String];
    
    if (sqlite3_open_s(dbpath, &_personDB, SQLITE_OPEN_READWRITE, NULL, "sdk") != SQLITE_OK)
    {
        _statusText.text = @"Failed to open person db";
        return;
    }
    
    NSString *nameCondition = [_nameText.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    
    NSString *ageCondition = [_ageText.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    
    
    NSMutableString * querySQL = [[NSMutableString alloc] init];
    [querySQL appendString:@"select * from person "];
    
    if([nameCondition length] > 0 && [ageCondition length] > 0)
    {
        [querySQL appendFormat:@"where name=\"%@\" and age=%@", nameCondition, ageCondition];
    }
    else
    {
        if([nameCondition length] > 0)
        {
            [querySQL appendFormat:@"where name=\"%@\"", nameCondition];
        }
        else if([ageCondition length] > 0)
        {
            [querySQL appendFormat:@"where age=%@", ageCondition];
        }
        
    }
    
    
    
    const char *query_stmt = [querySQL UTF8String];
    sqlite3_prepare_v2(_personDB, query_stmt,
                       -1, &statement, NULL);
    [personList removeAllObjects];
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        Person * p = [[Person alloc] init];
        p.pid = sqlite3_column_int(statement, 0);
        p.name = [NSString stringWithUTF8String:sqlite3_column_text(statement, 1)];
        p.age = sqlite3_column_int(statement, 2);
        [personList addObject:p];
    }
    sqlite3_finalize(statement);
    sqlite3_close(_personDB);
    
}

- (IBAction)executeSqlOperation:(id)sender {
    
    switch (self.segmentControl.selectedSegmentIndex)
    {
        case 0:
            [self doInsertOperation];
            break;
        case 1:
            [self doDeleteOperation];
            break;
        case 2:
            [self doUpdateOperation];
            break;
        case 3:
            [self doQueryOperation];
            break;
        default:
            break;
            
    }

    //重新加载数据
    [self.tableView reloadData];
    
    _countsText.text =[NSString stringWithFormat:@"%ld", personList.count];
}

- (IBAction)segmentValueChange:(UISegmentedControl*)sender {
    
    switch (sender.selectedSegmentIndex) {
        case 0:
            [self.executeButton setTitle:@"Insert" forState:UIControlStateNormal];
            break;
        case 1:
            [self.executeButton setTitle:@"Delete" forState:UIControlStateNormal];
            break;
        case 2:
            [self.executeButton setTitle:@"Update" forState:UIControlStateNormal];
            break;
        case 3:
            [self.executeButton setTitle:@"Query" forState:UIControlStateNormal];
            break;
        default:
            break;
            
    }

}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return personList.count;
}

// Row display. Implementers should *always* try to reuse cells by setting each cell's reuseIdentifier and querying for available reusable cells with dequeueReusableCellWithIdentifier:
// Cell gets various attributes set automatically based on table (separators) and data source (accessory views, editing controls)

//- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
//{
//    return personList[indexPath.row];
//}

- (UITableViewCell *) tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    UITableViewCell *cell = nil;
    Person * person = personList[indexPath.row];
    if ([tableView isEqual:self.tableView])
    {
        cell = [tableView
                dequeueReusableCellWithIdentifier:TableViewCellIdentifier
                forIndexPath:indexPath];
        cell.textLabel.text = [NSString stringWithFormat:
                               @"id: %ld, name: %@ , age: %ld",person.pid, person.name, person.age];
        
    }
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    Person * person = personList[indexPath.row];
    _nameText.text = [person name];
    _ageText.text =  [NSString stringWithFormat:@"%d",[person age]];
}


- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"guide"])
    {
        GuideViewController *guideViewController =  [segue destinationViewController];
        guideViewController.guideType = @"sqlite";
        
    }
}



@end
