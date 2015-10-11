//
//  DocumentViewController.h
//  anyofficesdk
//
//  Created by ui1 on 14-6-9.
//  Copyright (c) 2014年 pangqi. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol DocumentDelegate <NSObject>

-(void)goBack;

@end

@interface DocumentViewController : UIViewController <UIWebViewDelegate,UIGestureRecognizerDelegate,DocumentDelegate,UITableViewDataSource,UITableViewDelegate,UIDocumentInteractionControllerDelegate, UIScrollViewDelegate>
/**
 *  openFile 2打开文件
 *
 *  @param filePath 文件路径
 */
-(void)initFileEnv;
-(void)setFilePath:(NSString *)filePath;
//-(void)openFile:(NSString *)filePath;
-(id)initDocViewWithFilePath:(NSString *)filePath;
-(void)setFileEncodePath:(NSString *)path;
-(void)setSeadyKey:(NSString *)userName withDeiceID:(NSString *)deviceID;
@end
