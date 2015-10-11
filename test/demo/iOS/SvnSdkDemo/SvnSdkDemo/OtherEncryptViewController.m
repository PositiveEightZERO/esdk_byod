//
//  OtherEncryptViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 8/14/15.
//
//

#import "OtherEncryptViewController.h"

#import "UIHelper.h"
#import <SvnSdk/SvnFileManager.h>
#import <SvnSdk/SvnFileHandle.h>
#import <SvnSdk/SvnFileInputStream.h>
#import <SvnSdk/SvnFileOutputStream.h>
#import <SvnSdk/NSClassExtend.h>

@interface OtherEncryptViewController () <NSStreamDelegate>
{
    NSMutableArray * logArray;
    NSMutableData * streamData;
    
    SvnFileInputStream * inputStream;
}
@end


#define PrintLog(...) \
{\
    NSLog(__VA_ARGS__);\
    [logArray addObject:[NSString stringWithFormat:__VA_ARGS__]];\
}



@implementation OtherEncryptViewController

- (void)viewDidLoad {

    [super viewDidLoad];
    
    logArray = [[NSMutableArray alloc] init];
    streamData = [[NSMutableData alloc] init];
    
    // Custom initialization
    UIButton *backButton = [UIHelper navButton:@"Back"];
    [backButton addTarget:self action:@selector(navigateBack) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *backButtonItem = [[UIBarButtonItem alloc] initWithCustomView:backButton];
    backButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.leftBarButtonItem=backButtonItem;
    

    
    UIEdgeInsets insets = UIEdgeInsetsMake(10, 10, 10, 10);
    // 指定为拉伸模式，伸缩后重新赋值
    UIImage *image= [[UIImage imageNamed:@"btn_background.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    [self.manageTestButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.streamTestButton setBackgroundImage:image forState:UIControlStateNormal];
    
    [self.handleTestButton setBackgroundImage:image forState:UIControlStateNormal];
    
    [self.extendTestButton setBackgroundImage:image forState:UIControlStateNormal];
    
    
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc]
                                             initWithTarget:self action:@selector(tapOnView:)];
    [self.logView addGestureRecognizer: tapRecognizer];
    self.logView.hidden = YES;
    
}


-(void)viewDidLayoutSubviews
{
    
    
    self.contentView.frame = CGRectMake(self.contentView.frame.origin.x
                                        , self.contentView.frame.origin.y, self.contentView.frame.size.width, self.view.frame.size.height);
    
    //NSLog(@"view size:%f, %f", self.view.frame.size.width, self.view.frame.size.height);
    
    NSLog(@"contentView size:%f, %f, origin:%f, %f", self.contentView.frame.size.width, self.contentView.frame.size.height, self.contentView.frame.origin.x, self.contentView.frame.origin.y);
    
    [self.contentView layoutIfNeeded];
    
    [super viewDidLayoutSubviews];
    
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



- (IBAction)navigateBack
{
    [self.navigationController popViewControllerAnimated:YES];
}


/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

- (IBAction)fileManagerTest:(id)sender {
    
    [self clearLogView];
    
    SvnFileManager * fileMangager = [SvnFileManager defaultManager];
    
    //fileMangager
    
    NSArray* tmpPathArray = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentPath = [tmpPathArray objectAtIndex:0];
    NSLog(@"Document Path:%@", documentPath);
    
    
    NSString * testDirPath = [NSString stringWithFormat:@"%@/test", documentPath];
    
    
    //测试文件(或目录)是否存在  测试指定路径下是否存在该文件（或目录）
    
    BOOL isDirectory = NO;
    
    BOOL exist = [fileMangager fileExistsAtPath:testDirPath isDirectory:&isDirectory];
    
    
    
    
    PrintLog(@"test dir exist:%d, isDirectory:%d", exist, isDirectory);
    
    if(exist)
    {
        //删除一个文件  删除指定路径下的文件
        
        BOOL deleted = [fileMangager removeItemAtPath:testDirPath error:nil];
        
        PrintLog(@"test dir deleted:%d", deleted);
        
        //测试文件是否存在  测试指定路径下文件是否存在
        
        exist = [fileMangager fileExistsAtPath:testDirPath];
        
        PrintLog(@"test dir now exist:%d", exist);
        
    }
    
    
    

    //创建目录 读取指定路径创建目录
    
    BOOL created =  [fileMangager createDirectoryAtPath:testDirPath attributes:nil];
    PrintLog(@"test dir created:%d", created);

    exist = [fileMangager fileExistsAtPath:testDirPath isDirectory:&isDirectory];
    PrintLog(@"test dir exist:%d, isDirectory:%d", exist, isDirectory);
    
 
    NSString *subTestDir = [NSString stringWithFormat:@"%@/test1/test2", testDirPath];
    created =  [fileMangager createDirectoryAtPath:subTestDir attributes:nil];
    PrintLog(@"test sub dir must not be created:%d", created);

    //创建目录 读取指定路径创建目录
    
    created =  [fileMangager createDirectoryAtPath:subTestDir withIntermediateDirectories:YES attributes:nil error:nil];
    PrintLog(@"test sub dir now must be created:%d", created);
    
    
    NSString *testFile = [NSString stringWithFormat:@"%@/1.txt", testDirPath];
    
    
    NSString *testData = @"1234567890";
    
    //向一个文件写入数据 向指定路径下的文件写入数据
    NSData * orginData = [testData dataUsingEncoding:NSUTF8StringEncoding];
    created = [fileMangager createFileAtPath:testFile contents:orginData attributes:nil];
    
    PrintLog(@"test file must be created:%d", created);
    
    //测试文件是否存在，并且是否能执行读操作  测试指定路径下文件是否存在，并且是否能执行读操作
    BOOL isReadable = [fileMangager isReadableFileAtPath:testFile];
    
    BOOL isWritable = [fileMangager isWritableFileAtPath:testFile];
    
    BOOL isExecutable = [fileMangager isExecutableFileAtPath:testFile];
    BOOL isDeletable = [fileMangager isDeletableFileAtPath:testFile];
    PrintLog(@"isReadable:%d, isWritable:%d, isExecutable:%d, isDeletable:%d ", isReadable, isWritable, isExecutable, isDeletable);
    
    
    //读取数据  从一个文件中读取数据

    NSData*  readedData = [fileMangager contentsAtPath:testFile];
    
    PrintLog(@"test file content:%@", [[NSString alloc] initWithData:readedData encoding:NSUTF8StringEncoding]);
    
    //读取数据  从一个目录中读取数据
    
    NSArray *  documentDirContents = [fileMangager directoryContentsAtPath:testDirPath];
    //创建目录 读取指定路径创建目录

    PrintLog(@"test dir contents:%@", documentDirContents);

    
    [self showLogView];
}

- (IBAction)fileStreamTest:(id)sender {
    
    
    SvnFileManager * fileMangager = [SvnFileManager defaultManager];
    
    //fileMangager
    
    NSArray* tmpPathArray = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentPath = [tmpPathArray objectAtIndex:0];
    NSLog(@"Document Path:%@", documentPath);
    
    
    NSString * testDirPath = [NSString stringWithFormat:@"%@/test", documentPath];
    
    
    //测试文件(或目录)是否存在  测试指定路径下是否存在该文件（或目录）
    
    BOOL isDirectory = NO;
    
    BOOL exist = [fileMangager fileExistsAtPath:testDirPath isDirectory:&isDirectory];
    
    
    
    
    PrintLog(@"test dir exist:%d, isDirectory:%d", exist, isDirectory);
    
    if(exist)
    {
        //删除一个文件  删除指定路径下的文件
        
        BOOL deleted = [fileMangager removeItemAtPath:testDirPath error:nil];
        
        PrintLog(@"test dir deleted:%d", deleted);
        
        //测试文件是否存在  测试指定路径下文件是否存在
        
        exist = [fileMangager fileExistsAtPath:testDirPath];
        
        PrintLog(@"test dir now exist:%d", exist);
        
    }
    
    
    
    
    //创建目录 读取指定路径创建目录
    
    BOOL created =  [fileMangager createDirectoryAtPath:testDirPath attributes:nil];
    PrintLog(@"test dir created:%d", created);
    
    exist = [fileMangager fileExistsAtPath:testDirPath isDirectory:&isDirectory];
    PrintLog(@"test dir exist:%d, isDirectory:%d", exist, isDirectory);
    
    
    NSString *testFile = [NSString stringWithFormat:@"%@/1.txt", testDirPath];
    
    
    NSString *testData = @"1234567890";
    
    //向一个文件写入数据 向指定路径下的文件写入数据
    NSData * orginData = [testData dataUsingEncoding:NSUTF8StringEncoding];

    
    SvnFileOutputStream *outputStream = [SvnFileOutputStream outputStreamToFileAtPath:testFile append:YES];
    [outputStream open];
    
    for(int i=0; i< 1000; i++)
    {
        [outputStream write:[testData UTF8String] maxLength:[testData length] ];
    }
    
    
    PrintLog(@"test file :%d bytes writed ", [testData length]*1000);
    [outputStream close];
    //__weak typeof (self) weakSelf = self;
    //dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    inputStream = [[SvnFileInputStream alloc ] initWithFileAtPath:testFile];
    
    if(!inputStream)
    {
        NSLog(@"inputStream open failed");
    }
    [streamData setLength:0];
    [inputStream setDelegate:self];
    [inputStream scheduleInRunLoop:[NSRunLoop mainRunLoop]
                           forMode:NSDefaultRunLoopMode];
    
    
    [inputStream open];
    //});
    
    
    

    
}


- (void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode {
    
    NSLog(@"handleEvent------------------");
    
    switch(eventCode) {
        case NSStreamEventHasBytesAvailable:
        {
            uint8_t buf[1024];
            unsigned int len = 0;
            len = [(NSInputStream *)stream read:buf maxLength:1024];
            if(len) {
                [streamData appendBytes:(const void *)buf length:len];
                
                PrintLog(@"test stream readed:%d", len);
                // bytesRead is an instance variable of type NSNumber.
            } else {
                PrintLog(@"read to end, total readed:%d", [streamData length]);
                [self showLogView];
            }
            break;
        }
        case NSStreamEventEndEncountered:
        {
            [self showLogView];
            break;
        }
            // continued
    }
    
}

- (IBAction)fileHandleTest:(id)sender {
    
    
    [self clearLogView];
    
    SvnFileManager * fileMangager = [SvnFileManager defaultManager];
    
    //fileMangager
    
    NSArray* tmpPathArray = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentPath = [tmpPathArray objectAtIndex:0];
    NSLog(@"Document Path:%@", documentPath);
    
    
    NSString * testDirPath = [NSString stringWithFormat:@"%@/test", documentPath];
    
    
    //测试文件(或目录)是否存在  测试指定路径下是否存在该文件（或目录）
    
    BOOL isDirectory = NO;
    
    BOOL exist = [fileMangager fileExistsAtPath:testDirPath isDirectory:&isDirectory];
    
    
    
    
    PrintLog(@"test dir exist:%d, isDirectory:%d", exist, isDirectory);
    
    if(exist)
    {
        //删除一个文件  删除指定路径下的文件
        
        BOOL deleted = [fileMangager removeItemAtPath:testDirPath error:nil];
        
        PrintLog(@"test dir deleted:%d", deleted);
        
        //测试文件是否存在  测试指定路径下文件是否存在
        
        exist = [fileMangager fileExistsAtPath:testDirPath];
        
        PrintLog(@"test dir now exist:%d", exist);
        
    }
    
    
    
    
    //创建目录 读取指定路径创建目录
    
    BOOL created =  [fileMangager createDirectoryAtPath:testDirPath attributes:nil];
    PrintLog(@"test dir created:%d", created);
    
    exist = [fileMangager fileExistsAtPath:testDirPath isDirectory:&isDirectory];
    PrintLog(@"test dir exist:%d, isDirectory:%d", exist, isDirectory);
    

    NSString *testFile = [NSString stringWithFormat:@"%@/1.txt", testDirPath];
    NSString *testData = @"1234567890";
    NSData * orginData = [testData dataUsingEncoding:NSUTF8StringEncoding];
    
    SvnFileHandle * writeHandle = [SvnFileHandle fileHandleForWritingAtPath:testFile];
    [writeHandle writeData:orginData];
    [writeHandle closeFile];
    PrintLog(@"handle %@ data writed ", writeHandle);
    

    SvnFileHandle *readHandle = [SvnFileHandle fileHandleForReadingAtPath:testFile];
    
    //读取数据  从一个文件中读取数据
    
    NSData*  readedData = [readHandle readDataToEndOfFile];
    
    PrintLog(@"test file content:%@", [[NSString alloc] initWithData:readedData encoding:NSUTF8StringEncoding]);
    

    [readHandle seekToFileOffset:2];
    PrintLog(@"test file seekToFileOffset:%d", 2);
    
    unsigned long long offset = [readHandle offsetInFile];
    
    PrintLog(@"test file offset:%llu", offset);
    
    
    NSData * fiveBytesData = [readHandle readDataOfLength:5];
    
    PrintLog(@"test file read %d bytes, data is:%@", 5, [[NSString alloc] initWithData:fiveBytesData encoding:NSUTF8StringEncoding]);
    
    
    [readHandle seekToEndOfFile];
    offset = [readHandle offsetInFile];
    
    PrintLog(@"test file seekToEndOfFile, offsetInFile is:%llu", offset);
    [readHandle closeFile];
    
    [self showLogView];
    
    
}

- (IBAction)classExtendTest:(id)sender {
    
    [self clearLogView];
    
    SvnFileManager * fileMangager = [SvnFileManager defaultManager];
    
    //fileMangager
    
    NSArray* tmpPathArray = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentPath = [tmpPathArray objectAtIndex:0];
    NSLog(@"Document Path:%@", documentPath);
    
    
    NSString * testDirPath = [NSString stringWithFormat:@"%@/test", documentPath];
    
    
    //测试文件(或目录)是否存在  测试指定路径下是否存在该文件（或目录）
    
    BOOL isDirectory = NO;
    
    BOOL exist = [fileMangager fileExistsAtPath:testDirPath isDirectory:&isDirectory];
    
    
    
    
    PrintLog(@"test dir exist:%d, isDirectory:%d", exist, isDirectory);
    
    if(exist)
    {
        //删除一个文件  删除指定路径下的文件
        
        BOOL deleted = [fileMangager removeItemAtPath:testDirPath error:nil];
        
        PrintLog(@"test dir deleted:%d", deleted);
        
        //测试文件是否存在  测试指定路径下文件是否存在
        
        exist = [fileMangager fileExistsAtPath:testDirPath];
        
        PrintLog(@"test dir now exist:%d", exist);
        
    }
    
    
    
    
    //创建目录 读取指定路径创建目录
    
    BOOL created =  [fileMangager createDirectoryAtPath:testDirPath attributes:nil];
    PrintLog(@"test dir created:%d", created);
    
    exist = [fileMangager fileExistsAtPath:testDirPath isDirectory:&isDirectory];
    PrintLog(@"test dir exist:%d, isDirectory:%d", exist, isDirectory);
    
    
    NSString *testFile = [NSString stringWithFormat:@"%@/1.txt", testDirPath];
    
    NSString *testFile2 = [NSString stringWithFormat:@"%@/2.txt", testDirPath];
    
    NSString *testData = @"1234567890";
    NSData * orginData = [testData dataUsingEncoding:NSUTF8StringEncoding];

    
    NSData * svnData = [[NSData alloc] initWithData:orginData];
    BOOL writed = [svnData writeToFileSvn:testFile atomically:YES];
    
    PrintLog(@"test file 1 writed by data:%d", writed);
    
    NSString * svnString1 = [NSString stringWithContentsOfFileSvn:testFile encoding:NSUTF8StringEncoding error:nil];
    
    PrintLog(@"test file 1 readed to string:%@", svnString1);
    
    [svnString1 writeToFileSvn:testFile2 atomically:YES encoding:NSUTF8StringEncoding error:nil];
    PrintLog(@"test file 2 writed by string:%d", writed);
    
    NSData * svnData2 = [NSData dataWithContentsOfFileSvn:testFile2];
    NSString * fileContent = [[NSString alloc] initWithData:svnData2 encoding:NSUTF8StringEncoding];
    PrintLog(@"test file 2 readed to data:%@", fileContent);
    [self showLogView];
}

-(void) clearLogView
{
    [logArray removeAllObjects];
    self.logView.text = @"";
    self.logView.hidden = YES;
}



-(void) showLogView
{
    
    for (NSString * log in logArray){
        self.logView.text =  [NSString stringWithFormat:@"%@\n%@", self.logView.text, log ];
    }
    self.logView.hidden = NO;
    self.logView.editable = NO;

    [self.logView setContentOffset:CGPointMake(0.0, 0.0)];
    
    [self.logView scrollRectToVisible:CGRectMake(0.0, 0.0, 1.0, 1.0) animated:NO];
    
    
    [self.contentView bringSubviewToFront:self.logView];
}


-(void)tapOnView:(UITapGestureRecognizer *)tapRecognizer
{
    self.logView.hidden = YES;
}
@end
