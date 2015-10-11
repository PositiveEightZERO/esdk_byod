//
//  RootViewController.h
//  Accordion
//
//  Created by Enriquez Gutierrez Guillermo Ignacio on 8/27/10.
//  Copyright (c) 2010 Nacho4D.
//  See the file license.txt for copying permission.
//

#import <UIKit/UIKit.h>
#import "FileSystemManager.h"






// UIPopoverController+iPhone.h file
@interface UIPopoverController (iPhone)
+ (BOOL)_popoversDisabled;
@end



@class FileSystemManager;
@class N4PromptAlertView;
@class FileBrowseDelegate;

@protocol FileBrowseDelegate <NSObject>

@optional
- (void)fileSelected:(NSString*)filePath withType:(int) type;
@end


@interface FileBrowseViewController : UITableViewController <FileSystemManagerDelegate, UIPopoverControllerDelegate>{
	
	FileSystemManager *datasourceManager;
	NSMutableArray *sortDescriptors;
	UIPopoverController *sorterPopoverController;
	UIPopoverController *fileCreatorPopoverController;
	
	UIAlertView *createFileAlert;
	UIAlertView *createDirectoryAlert;
	UIAlertView *duplicateFileAlert;
}

@property (nonatomic, assign)   id<FileBrowseDelegate> delegate;
@property (nonatomic, assign)   int typeToSelect;

@property (nonatomic, retain) FileSystemManager *datasourceManager;
@property (nonatomic, retain) NSMutableArray *sortDescriptors;

- (IBAction) showSortingMenu:(id)sender;

@end







