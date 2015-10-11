//
//  MoreEncryptViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 13-10-23.
//
//
#import <QuickLook/QuickLook.h>



#import <SvnSdk/svn_file_api.h>
#import <SvnSdk/svn_ios_api.h>

#import <SvnSdk/PreviewView.h>
#import <Svnsdk/DocumentViewController.h>
#import <SvnSdk/SvnFileHandle.h>
#import <SvnSdk/SvnFileManager.h>
#import <SvnSdk/NSClassExtend.h>


#import <SvnSdk/SvnMediaPlayer.h>

#import "FileEncryptViewController.h"
#import "GuideViewController.h"
#import "UIHelper.h"
//#import "SvnMediaPlayer.h"



@interface FileEncryptViewController ()

@end

@implementation FileEncryptViewController
{

}

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
    
    //self.scrollView.frame = CGRectMake(0, 0, 320, 416);
    [self.scrollView setContentSize:CGSizeMake(320,480 )];
    
    
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
    
    
    //tapRecognizer for dismiss keyboard.
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc]
                                             initWithTarget:self action:@selector(tapOnView:)];
    [self.view addGestureRecognizer: tapRecognizer];
    
    
    
    //turn off scrolling and set the font details.
    self.originPathTextView.scrollEnabled = YES;
    self.originPathTextView.font = [UIFont fontWithName:@"Helvetica" size:14];
    self.originPathTextView.layer.borderColor = [UIColor grayColor].CGColor;
    self.originPathTextView.layer.borderWidth =1.0;
    self.originPathTextView.layer.cornerRadius =5.0;
    
    
    self.encryptPathTextView.scrollEnabled = YES;
    self.encryptPathTextView.font = [UIFont fontWithName:@"Helvetica" size:14];
    self.encryptPathTextView.layer.borderColor = [UIColor grayColor].CGColor;
    self.encryptPathTextView.layer.borderWidth = 1.0;
    self.encryptPathTextView.layer.cornerRadius = 5.0;
    
    UIEdgeInsets insets = UIEdgeInsetsMake(20, 10, 20, 10);
    // 指定为拉伸模式，伸缩后重新赋值
    UIImage *image= [[UIImage imageNamed:@"btn_background.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    UIImage *imageSelected= [[UIImage imageNamed:@"btn_background_selected.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    [self.selectOriginButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.selectOriginButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.viewOriginButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.viewOriginButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.encryptButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.encryptButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.selectEncryptButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.selectEncryptButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.viewEncryptedButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.viewEncryptedButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.decryptButton setBackgroundImage:image forState:UIControlStateNormal];
    [self.decryptButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    
    NSString *documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSLog(@"vvvv== %@",documentPath);
    
    [self.basePathLabel setText:documentPath];
    //自动折行设置
    self.basePathLabel.lineBreakMode = UILineBreakModeWordWrap;
    self.basePathLabel.numberOfLines = 0;
    
    [self registerForKeyboardNotifications];

//    [_originPath setText:documentPath];
//    [_encryptPath setText:[NSString stringWithFormat:@"%@/enc", documentPath]];
    
}

- (void) viewDidLayoutSubviews {
    
    UIEdgeInsets newInsets = UIEdgeInsetsMake(0, 0, 0, 0);
    self.scrollView.contentInset = newInsets;
    
    
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


- (IBAction)viewOriginFile:(id)sender {
    
    NSString *srcFilePath = [NSString stringWithFormat:@"%@/%@", self.basePathLabel.text ,self.originPathTextView.text];
    
    BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:srcFilePath];
    
    if([self.originPathTextView text].length == 0 || !fileExists)
    {
        [self showAlert:@"Please select a file to view!"];
        return;
    }
    
    
    //判断是否音视频文件
    NSArray *extensions = [NSArray arrayWithObjects:@"mp3", @"aac", @"3gp", @"wav", @"wma", @"avi", @"mp4", @"m4a", @"mpg", @"wmv", nil];
    
    
    NSPredicate *avPredicate = [NSPredicate predicateWithFormat:@"pathExtension IN %@", extensions];
    
    BOOL matched = [avPredicate evaluateWithObject: srcFilePath];
    
    if (matched){
        
        CGRect screenRect = [[UIScreen mainScreen] bounds];
        CGFloat screenWidth = screenRect.size.width;
        CGFloat screenHeight = screenRect.size.height;
        
        CGRect frame = CGRectMake(0, screenHeight/4, screenWidth, screenHeight/2);
        
        [SvnMediaPlayer playMediaFile:srcFilePath frame:frame];
        return;
    }
    

    
    
    
//    PreviewView *previewController = [[PreviewView alloc] init];
//    [previewController previewDocument:srcFilePath];
//    [self presentViewController:previewController animated:YES completion:nil];
    
    DocumentViewController *documentController = [[DocumentViewController alloc] initDocViewWithFilePath :srcFilePath];

    [self presentViewController:documentController animated:YES completion:nil];
    
}

- (IBAction)encryptFile:(id)sender {

    NSString *srcFilePath = [NSString stringWithFormat:@"%@/%@", self.basePathLabel.text ,[self.originPathTextView text]];
    NSString *dstFilePath = [NSString stringWithFormat:@"%@/%@", self.basePathLabel.text ,[self.encryptPathTextView text]];

    
    BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:srcFilePath];
    
    if([self.originPathTextView text].length == 0 || !fileExists)
    {
        [self showAlert:@"Please select a file to encrypt!"];
        return;
    }
    
    if([self.encryptPathTextView text].length == 0)
    {
        [self showAlert:@"Please select a encrypted file to decrypt!"];
        return;
    }
    
    
    NSData *data = [NSData dataWithContentsOfFile:srcFilePath];
    
    
    
//    SvnFileHandle* fileHandle = [SvnFileHandle fileHandleForWritingAtPath:dstFilePath];
//    
//    [fileHandle writeData:data];
//    
//    [fileHandle closeFile];
    
    SvnFileManager *fileManager = [SvnFileManager defaultManager];
    
    BOOL fileExist = [fileManager fileExistsAtPath:dstFilePath];
    
    if(fileExist)
    {
        NSError *error;
        [fileManager removeItemAtPath:dstFilePath error:&error];
    }
    
    [fileManager createFileAtPath:dstFilePath contents:data attributes:nil];
    
//    SvnFileHandle *fileHandle = [SvnFileHandle fileHandleForWritingAtPath:dstFilePath];
//    unsigned long pos = [fileHandle seekToEndOfFile];
//    NSLog(@"seekToEndOfFile pos:%d!", pos);
//    
//    [fileHandle writeData:data];
//    
//    [fileHandle writeData:data];
//    
//    [fileHandle writeData:data];
//    
//    [fileHandle closeFile];
    
    
//    
//    //打开目标文件，加密写入
//    SVN_FILE_S *file =  svn_fopen([dstFilePath UTF8String], "w");
//    
//    if (NULL == file) {
//        NSLog(@"open file:%@ error!", dstFilePath);
//        [self showAlert:@"Open encrypt file failed!"];
//        return;
//    }
//    
//    NSData *data = [NSData dataWithContentsOfFile:srcFilePath];
//    
//    int iRet = 0;
//
//    const int buf_size = 1024;
//    char buffer[1024];
//    memset(buffer, 0, buf_size);
//    
//    int srcRemain = [data length];
//    int srcLocation = 0;
//    while (srcRemain > 0) {
//        
//        int len = srcRemain > buf_size ? buf_size:srcRemain;
//        
//        NSRange bufRange = NSMakeRange(srcLocation, len);
//        
//        
//        srcLocation += len;
//        srcRemain -= len;
//        
//        [data getBytes:buffer range:bufRange];
//        //加密写入
//        iRet = svn_fwrite((unsigned char *)buffer, 1, len, file);
//        if ( 0 > iRet ) {
//            
//            NSLog(@"write file error! iRet=%d", iRet);
//            [self showAlert:@"Write encrypt file error!"];
//            break;
//        }
//    }
//
//   
//    //关闭文件
//    iRet = svn_fclose(file);
//    if ( 0 > iRet ) {
//        NSLog(@"close file error! iRet=%d", iRet);
//        [self showAlert:@"Close encrypted file error!"];
//        return;
//    }
    
    
  
}

- (IBAction)viewEncryptedFile:(id)sender {
    
    
    
    //NSString * srcFilePath = @"http://172.22.8.206:8180/test.mp4";
    

    NSString *srcFilePath = [NSString stringWithFormat:@"%@/%@", self.basePathLabel.text ,[self.encryptPathTextView text]];
    
    if([self.encryptPathTextView text].length == 0)
    {
        [self showAlert:@"Please select a encrypted file to view!"];
        return;
    }
    
    
    
    //判断是否音视频文件
    NSArray *extensions = [NSArray arrayWithObjects:@"mp3", @"aac", @"3gp",@"wav", @"wma", @"avi", @"mp4", @"m4a", @"mpg", @"wmv", nil];
   
    
    NSPredicate *avPredicate = [NSPredicate predicateWithFormat:@"pathExtension IN %@", extensions];
    
    BOOL matched = [avPredicate evaluateWithObject: srcFilePath];
    
    if (matched){
        
        CGRect screenRect = [[UIScreen mainScreen] bounds];
        CGFloat screenWidth = screenRect.size.width;
        CGFloat screenHeight = screenRect.size.height;
        
        CGRect frame = CGRectMake(0, screenHeight/4, screenWidth, screenHeight/2);
        
        NSLog(@"video frame:%@", NSStringFromCGRect(frame));
        
        [SvnMediaPlayer playMediaFile:srcFilePath frame:frame];
        return;
    }

    
    
    //根据路径判断是否RMS文件
    int result1 = SVN_API_RecognizeRMSDoc([srcFilePath UTF8String]);
    
    NSData *fileData = [NSData dataWithContentsOfFile:srcFilePath];
    u_char buffer[6*1024];
    
    int len = 6*1024;
    if(len > [fileData length])
    {
        len = [fileData length];
    }
    
    [fileData getBytes:buffer length:len];
    
    //根据内容判断是否RMS文件
    int result2 = SVN_API_GetAttachmentTypeByNameContent([[srcFilePath lastPathComponent] UTF8String], buffer, len);
    
    NSString * result = [NSString stringWithFormat:@"SVN_API_RecognizeRMSDoc result:%d, SVN_API_GetAttachmentTypeByNameContent result:%d", result1, result2];
    
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:result delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
    
    [alertView show];
    
    if(result1 == 0 && result2 == 0)
    {
        //非RMS文件，使用文件预览
//        PreviewView *previewController = [[PreviewView alloc] init];
//        [previewController previewDocument:srcFilePath];
//        [self presentViewController:previewController animated:YES completion:nil];
        
        
        DocumentViewController *documentController = [[DocumentViewController alloc] init];
        
        [documentController setFilePath:srcFilePath];
        [self presentViewController:documentController animated:YES completion:nil];
    }
    
}

- (IBAction)decryptFile:(id)sender {
   
    NSString *srcFilePath = [NSString stringWithFormat:@"%@/%@", self.basePathLabel.text ,[self.encryptPathTextView text]];
    NSString *dstFilePath = [NSString stringWithFormat:@"%@/%@", self.basePathLabel.text ,[self.originPathTextView text]];
   
    
    if([self.encryptPathTextView text].length == 0)
    {
        [self showAlert:@"Please select a encrypted file to decrypt!"];
        return;
    }
    
    if([self.originPathTextView text].length == 0)
    {
        [self showAlert:@"Please select a file path to decrypt!"];
        return;
    }

//    //打开加密文件
//    SVN_FILE_S *file =  svn_fopen([srcFilePath UTF8String], "r");
//    
//    if (NULL == file) {
//        NSLog(@"open file:%@ error!", dstFilePath);
//        [self showAlert:@"Open encrypted file failed!"];
//        return;
//    }
//    
//    NSMutableData *data = [[NSMutableData alloc] init];
//
//    const int buf_size = 1024;
//    char buffer[1024];
//    memset(buffer, 0, buf_size);
//    
//    int count = 0;
//    //读出加密内容
//    while ((count = svn_fread(buffer, 1, buf_size, file)) > 0) {
//        [data appendBytes:buffer length:count];
//    }
//    
//    [data writeToFile:dstFilePath atomically:YES];
//    //关闭文件
//    int iRet = svn_fclose(file);
//    if ( 0 > iRet ) {
//        NSLog(@"close file error! iRet=%d", iRet);
//        [self showAlert:@"Close encrypted file error!"];
//        return;
//    }
    
    SvnFileHandle *srcFileHandle = [SvnFileHandle fileHandleForReadingAtPath:srcFilePath];
    NSData * data = [srcFileHandle readDataToEndOfFile];
    
    [srcFileHandle closeFile];
    
    [[NSFileManager defaultManager] createFileAtPath:dstFilePath contents:nil attributes:nil];
    
    NSFileHandle *dstFileHandle = [NSFileHandle fileHandleForWritingAtPath:dstFilePath];
    [dstFileHandle truncateFileAtOffset:0];
    
    
    [dstFileHandle writeData:data];
    [dstFileHandle closeFile];
    
}







- (void)fileSelected:(NSString *)filePath withType:(int) type
{
    NSRange range = [filePath rangeOfString:self.basePathLabel.text];
    if (range.location == NSNotFound) {
        NSLog(@"base path was not found");
    } else {
        filePath = [filePath substringFromIndex:range.length + 1];
    }
    
    NSString *name = @"";
    
    NSString * last =[filePath lastPathComponent];
    
    NSLog(@"%@ lastPathComponent %@", filePath, last);
    
    if([filePath hasSuffix:@"/"])
    {
        if(type == 0)
        {
            name = [self.encryptPathTextView.text lastPathComponent];
        }
        else if(type == 1)
        {
            name = [self.originPathTextView.text lastPathComponent];
        }
        
        if([name isEqualToString:@"/"] || [name isEqualToString:@""])
        {
            [self showAlert:@"You select a directory, please select a file!"];
            return;
        }
    }
    

    
    if(type == 0)
    {
        self.originPathTextView.text = [filePath stringByAppendingString:name];
    }
    else if(type == 1)
    {
        self.encryptPathTextView.text = [filePath stringByAppendingString:name];
    }
}

- (void) showAlert:(NSString*)message
{
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:message delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
    
    [alertView show];
}


// Call this method somewhere in your view controller setup code.

- (void)registerForKeyboardNotifications

{
    
    [[NSNotificationCenter defaultCenter] addObserver:self
     
                                             selector:@selector(keyboardDidShow:)
     
                                                 name:UIKeyboardDidShowNotification object:nil];
    
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self
     
                                             selector:@selector(keyboardWillBeHidden:)
     
                                                 name:UIKeyboardWillHideNotification object:nil];
    
    
    
}



// Called when the UIKeyboardDidShowNotification is sent.

- (void)keyboardDidShow:(NSNotification*)notification
{
    NSDictionary* info = [notification userInfo];
    CGRect kbRect = [[info objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    kbRect = [self.view convertRect:kbRect fromView:nil];
    
    CGFloat height = 0.0f;
    height += [[UIApplication sharedApplication] statusBarFrame].size.height;
    height += self.navigationController.navigationBar.frame.size.height;
    
    UIEdgeInsets contentInsets = UIEdgeInsetsMake( 0.0,  0.0 ,kbRect.size.height, 0.0);
    self.scrollView.contentInset = contentInsets;
    self.scrollView.scrollIndicatorInsets = contentInsets;
    self.scrollView.contentSize = CGSizeMake(self.scrollView.frame.size.width, self.scrollView.frame.size.height);
    
    //self.scrollView.contentOffset = CGPointMake(0, kbRect.size.height);
    
    //[self.scrollView setContentOffset:CGPointMake(0, kbRect.size.height) animated:YES];
    [UIView animateWithDuration:.25 animations:^{
        self.scrollView.contentOffset = CGPointMake(0, kbRect.size.height);
    }];
}



// Called when the UIKeyboardWillHideNotification is sent

- (void)keyboardWillBeHidden:(NSNotification*)notification

{
    
    CGFloat height = 0.0f;
    height += [[UIApplication sharedApplication] statusBarFrame].size.height;
    height += self.navigationController.navigationBar.frame.size.height;

//    [UIView animateWithDuration:.25 animations:^{
//        self.scrollView.contentOffset = CGPointMake(0, 0);
//    }];
    
    
    UIEdgeInsets contentInsets = UIEdgeInsetsZero;
    
    //contentInsets.top += [[UIApplication sharedApplication] statusBarFrame].size.height;
    //contentInsets.top += self.navigationController.navigationBar.frame.size.height;
    
    self.scrollView.contentInset = contentInsets;
    
    self.scrollView.scrollIndicatorInsets = contentInsets;
    
}


- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"guide"])
    {
        GuideViewController *guideViewController =  [segue destinationViewController];
        guideViewController.guideType = @"fileencrypt";
        
    }
    else
    {
        FileBrowseViewController *browseController =  [segue destinationViewController];
        
        browseController.delegate = self;
        
        if(sender == [self selectOriginButton])
        {
            browseController.typeToSelect = 0;
        }
        else if(sender == [self selectEncryptButton])
        {
            browseController.typeToSelect = 1;
        }
    }
}

@end
