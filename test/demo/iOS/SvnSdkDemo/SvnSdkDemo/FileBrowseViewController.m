//
//  RootViewController.m
//  Accordion
//
//  Created by Enriquez Gutierrez Guillermo Ignacio on 8/27/10.
//  Copyright (c) 2010 Nacho4D.
//  See the file license.txt for copying permission.
//

#import <QuartzCore/QuartzCore.h>
#import "FileBrowseViewController.h"
//#import "DetailViewController.h"

#import "FileItem.h"
#import "FileItemTableViewCell.h"

#import "UIHelper.h"

// UIPopoverController+iPhone.m file
@implementation UIPopoverController (iPhone)
+ (BOOL)_popoversDisabled {
    return NO;
}
@end

@implementation FileBrowseViewController


@synthesize datasourceManager, sortDescriptors;

- (NSString *)applicationDocumentsDirectory {
    return [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
}

- (void) createTestData{
	
	NSLog(@"Creating some sample Data...");
	
	NSFileManager *fm = [NSFileManager defaultManager];
	
	[fm createDirectoryAtPath:[[self applicationDocumentsDirectory] stringByAppendingPathComponent:@"OriginFile"] attributes:nil];
    
    const char* testData = "1234567890";
	
//    NSData *data = [[NSData alloc] initWithBytes:testData length:10];
//    [fm createFileAtPath:[[self applicationDocumentsDirectory] stringByAppendingPathComponent:@"OriginFile/1.txt"] contents:data attributes:nil];

	[fm createDirectoryAtPath:[[self applicationDocumentsDirectory] stringByAppendingPathComponent:@"EncryptedFile"] attributes:nil];
	
	NSLog(@"Creating some sample Data... Finished");
		

}


#pragma mark -
#pragma mark UIViewController methods

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Custom initialization
    UIButton *backButton = [UIHelper navButton:@"Select"];
    [backButton addTarget:self action:@selector(doSelectFile) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *backButtonItem = [[UIBarButtonItem alloc] initWithCustomView:backButton];
    backButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.leftBarButtonItem=backButtonItem;
    
    
    UIButton *editButton = [UIHelper navButton:@"Edit"];
    [editButton addTarget:self action:@selector(changeToEditMode) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *editButtonItem = [[UIBarButtonItem alloc] initWithCustomView:editButton];
    editButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.rightBarButtonItem=editButtonItem;
 
    self.tableView.rowHeight = 44;
    self.contentSizeForViewInPopover = CGSizeMake(320.0, 600.0);
	
	[self createTestData];
	
	sortDescriptors = [FileSystemManager defaultSortDescriptors];
	datasourceManager = [[FileSystemManager alloc] initWithRootPath:[self applicationDocumentsDirectory]
																   sortDescriptors:sortDescriptors];
	datasourceManager.delegate = self;
	

}

- (void) showAlert:(NSString*)message
{
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:message delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
    
    [alertView show];
}

- (IBAction) doSelectFile
{
    NSIndexPath *selectedIndexPath = [[self tableView] indexPathForSelectedRow];
    if (!selectedIndexPath) {
        [self showAlert:@"Please select a file"];
        return;
    }
    
    
    FileItem *file = [datasourceManager.mergedRootBranch objectAtIndex:selectedIndexPath.row];
    
    if(self.typeToSelect == 0)
    {
        if(file.type == FileItemTypeEncryptedFile)
        {
            [self showAlert:@"Please select a unencrypted file"];
            return;
        }
    }
    else if(self.typeToSelect == 1)
    {
        if(file.type == FileItemTypeOriginFile)
        {
            [self showAlert:@"Please select a encrypted file"];
            return;
        }
    }
    NSString *filePath = [file fullName];
    
    if([file isDirectory])
    {
        filePath = [filePath stringByAppendingString:@"/"];
    }
    
    [self.navigationController popViewControllerAnimated:YES];
    
    if(_delegate && [_delegate respondsToSelector:@selector(fileSelected:withType:)])
    {
        [_delegate fileSelected:filePath withType:[self typeToSelect]];
    }
}

- (IBAction) changeToEditMode
{
    UIButton *editButton = [UIHelper navButton:@"Done"];
    [editButton addTarget:self action:@selector(changeToNormalMode) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *editButtonItem = [[UIBarButtonItem alloc] initWithCustomView:editButton];
    editButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.rightBarButtonItem=editButtonItem;
    
    
	[self.tableView setEditing:YES animated:YES];
	
}




- (IBAction) changeToNormalMode
{
    
    UIButton *editButton = [UIHelper navButton:@"Edit"];
    [editButton addTarget:self action:@selector(changeToEditMode) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *editButtonItem = [[UIBarButtonItem alloc] initWithCustomView:editButton];
    editButtonItem.style = UIBarButtonItemStylePlain;
    self.navigationItem.rightBarButtonItem=editButtonItem;
    
	[self.tableView setEditing:NO animated:YES];
	
}



#pragma mark - 
#pragma mark N4fileSystemManagerDelegate methods

- (void) fileSystemManager:(FileSystemManager *) manager didInsertRowsAtIndexPaths:(NSArray *)indexPaths{
	[self.tableView insertRowsAtIndexPaths:indexPaths 
						  withRowAnimation: UITableViewRowAnimationLeft];
	if ([indexPaths count] == 1) { //ok here? 
		[self.tableView selectRowAtIndexPath:[indexPaths objectAtIndex:0] 
									animated:YES scrollPosition:UITableViewScrollPositionNone];
	}
}
- (void) fileSystemManager:(FileSystemManager *) manager didRemoveRowsAtIndexPaths:(NSArray *)indexPaths{
	[self.tableView deleteRowsAtIndexPaths:indexPaths 
						  withRowAnimation:UITableViewRowAnimationLeft];
}

- (void) fileSystemManager:(FileSystemManager *) manager didCreateSuccessfullyFile:(FileItem *) file{
	
}
- (void) fileSystemManager:(FileSystemManager *) manager didFailOnCreationofFile:(FileItem *)file error:(NSError *)error{
	
}




#pragma mark -
#pragma mark Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)aTableView {
    return 1;
}


- (NSInteger)tableView:(UITableView *)aTableView numberOfRowsInSection:(NSInteger)section {

	return [datasourceManager.mergedRootBranch count];
}


- (UITableViewCell *)tableView:(UITableView *)aTableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"MyCellIdentifier";
	FileItemTableViewCell *cell = (FileItemTableViewCell *)[aTableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
		cell = [[FileItemTableViewCell alloc] initWithReuseIdentifier:CellIdentifier] ;
        cell.accessoryType = UITableViewCellAccessoryNone;
		cell.indentationWidth = 30.0;
    }
    
	FileItem *file = [datasourceManager.mergedRootBranch objectAtIndex:indexPath.row];
    if(file.type == FileItemTypeDirectory)
    {
        cell.cellType = FileBrowseTableViewCellTypeDirectory;
    }
    else if(file.type == FileItemTypeOriginFile)
    {
        cell.cellType = FileBrowseTableViewCellTypeFile;
    }
    else
    {
        cell.cellType = FileBrowseTableViewCellTypeEncryptedFile;

    }
	
	cell.directoryAccessoryImageView.image = (file.isDirectory)? [UIImage imageNamed:@"TriangleSmall.png"] : nil;
	cell.imageView.image = [file image];
	cell.textLabel.text = [file name]; 
	cell.detailTextLabel.text = [file name];

	return cell;
}


/*
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
	N4File * file = [datasourceManager.mergedRootBranch objectAtIndex:indexPath.row];
	if (!file.isDirectory || file.isEmptyDirectory) return YES;
	else return NO;
}
*/

- (void)tableView:(UITableView *)aTableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (editingStyle == UITableViewCellEditingStyleDelete) {
		
		FileItem * file = [datasourceManager.mergedRootBranch objectAtIndex:indexPath.row];
		
		[datasourceManager deleteFileAtIndex:indexPath.row];
        [aTableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:YES];
		
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
		
		[datasourceManager createFileAtIndex:indexPath.row withName:[NSString stringWithFormat:@"%@", [NSDate date]]];
		[aTableView insertRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationMiddle];
        
    }   
}


// Override to support rearranging the table view.
//- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
//}




// Override to support conditional rearranging of the table view.
//- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
	//
//    return YES;
//}




#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)aTableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {

	FileItem *file = [datasourceManager.mergedRootBranch objectAtIndex:indexPath.row];
	if (file.isDirectory) {
		if (file.isExpanded){
			FileItemTableViewCell *curCell = (FileItemTableViewCell *)[aTableView cellForRowAtIndexPath:indexPath];
			curCell.expanded = NO;
			[datasourceManager collapseBranchAtIndex:indexPath.row];
			file.expanded = NO;
		}
		else{
			FileItemTableViewCell *curCell = (FileItemTableViewCell *)[aTableView cellForRowAtIndexPath:indexPath];
			curCell.expanded = YES;
			
			[datasourceManager expandBranchAtIndex:indexPath.row];
			file.expanded = YES;
		}
		
		//[self.tableView reloadData]; //do not update datasource or tableview here		
		//rows will be inserted/deleted using datasourceManager delegate methods
		
	}
	else{
//		detailViewController.detailItem = [NSString stringWithFormat:@"%@", file.name];
//		detailViewController.backgroundImageVIew.image = [file imageBig];
//		[detailViewController addFile:file];
		
		
	}
}

- (NSInteger)tableView:(UITableView *)tableView indentationLevelForRowAtIndexPath:(NSIndexPath *)indexPath{
	FileItem *file = [datasourceManager.mergedRootBranch objectAtIndex:indexPath.row];
	return file.level; 
}





#pragma mark -
#pragma mark UIPopoverControllerDelegate methods

- (BOOL)popoverControllerShouldDismissPopover:(UIPopoverController *)popoverController{
	return YES;
}

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController{

	
	if (popoverController == sorterPopoverController) {
		//do not do [self.tableview reloadData] here. it is done by FileSorterViewControllerDelegate

		sorterPopoverController = nil;
	}
	else if (popoverController == fileCreatorPopoverController) {
		fileCreatorPopoverController = nil;
	}

}




- (void) showCreateNewFileMenu:(id)sender{
	
	if (!fileCreatorPopoverController.popoverVisible) {
		
		//[self.tableView deselectRowAtIndexPath:[self.tableView indexPathForSelectedRow] animated:YES];
		
		int w = 240, h = 44;
		UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0, 0, w, h*4)];
		UIButton *button1 = [[UIButton alloc] initWithFrame:CGRectMake(0, 44, w, h)];
		[button1 addTarget:self action:@selector(createDirectoryAlert) forControlEvents:UIControlEventTouchUpInside];
		[button1 setTitle:@"New Directory ... " forState:UIControlStateNormal];
		[container addSubview:button1];

		
		UIButton *button2 = [[UIButton alloc] initWithFrame:CGRectMake(0, 88, w, h)];
		[button2 addTarget:self action:@selector(createFileAlert) forControlEvents:UIControlEventTouchUpInside];
		[button2 setTitle:@"New File ... " forState:UIControlStateNormal];
		[container addSubview:button2];

		
		UIButton *button3 = [[UIButton alloc] initWithFrame:CGRectMake(0, 132, w, h)];
		[button3 addTarget:self action:@selector(duplicateFileAlert) forControlEvents:UIControlEventTouchUpInside];
		[button3 setTitle:@"Duplicate File ... " forState:UIControlStateNormal];
		[container addSubview:button3];
		NSIndexPath *selectedIndexPath = [self.tableView indexPathForSelectedRow];
		if (!selectedIndexPath ||
			((FileItem *)[datasourceManager.mergedRootBranch objectAtIndex:selectedIndexPath.row]).isDirectory) [button3 setEnabled:NO];

		
		UIViewController *vc = [[UIViewController alloc] init];
		vc.view = container;

		
		fileCreatorPopoverController = [[UIPopoverController alloc] initWithContentViewController:vc];

		
		
		[fileCreatorPopoverController setPopoverContentSize:CGSizeMake(w, h*4)];
		[fileCreatorPopoverController presentPopoverFromBarButtonItem:sender 
										permittedArrowDirections:UIPopoverArrowDirectionAny
														animated:YES];
		[fileCreatorPopoverController setDelegate:self];
	}

}

#pragma mark -
- (void) createDirectoryAlert {
	
	
	
	NSIndexPath *selectedIndexPath = [self.tableView indexPathForSelectedRow];
	NSString *message;
	if (!selectedIndexPath)
		message = @"New directory will be created at the top level";
	else{
		FileItem *file = [datasourceManager.mergedRootBranch objectAtIndex:selectedIndexPath.row];
		if (file.isDirectory) 
			message = [NSString stringWithFormat:@"New directory will be created inside of %@", [file name]];
		else
			message = [NSString stringWithFormat:@"New directory will be created at same level of %@", [file name]];
	}
					   
	createDirectoryAlert = [[UIAlertView alloc] initWithTitle:@"New directory" 
												 message:message
												delegate:self 
									   cancelButtonTitle:@"Cancel" 
									   otherButtonTitles:@"OK", nil];
	[createDirectoryAlert show];
}
- (void) createFileAlert{
	

	NSIndexPath *selectedIndexPath = [self.tableView indexPathForSelectedRow];
	NSString *message;
	if (!selectedIndexPath)
		message = @"New directory will be created at the top level";
	else{
		FileItem *file = [datasourceManager.mergedRootBranch objectAtIndex:selectedIndexPath.row];
		if (file.isDirectory) 
			message = [NSString stringWithFormat:@"New directory will be created inside of %@", [file name]];
		else
			message = [NSString stringWithFormat:@"New directory will be created at same level of %@", [file name]];
	}
	
	createFileAlert = [[UIAlertView alloc] initWithTitle:@"New directory" 
												 message:message
												delegate:self 
									   cancelButtonTitle:@"Cancel" 
									   otherButtonTitles:@"OK", nil];
	[createFileAlert show];

}
- (void) duplicateFileAlert{
	
	NSIndexPath *selectedIndexPath = [self.tableView indexPathForSelectedRow];
	NSString *message;
	if (!selectedIndexPath)
		message = @"File will be duplicated at the top level";
	else{
		FileItem *file = [datasourceManager.mergedRootBranch objectAtIndex:selectedIndexPath.row];
		if (file.isDirectory) 
			message = [NSString stringWithFormat:@"File will be duplicated inside of %@", [file name]];
		else
			message = [NSString stringWithFormat:@"File will be duplicated at same level of %@", [file name]];
	}
	
	duplicateFileAlert = [[UIAlertView alloc] initWithTitle:@"New directory" 
												 message:message
												delegate:self 
									   cancelButtonTitle:@"Cancel" 
									   otherButtonTitles:@"OK", nil];
	[duplicateFileAlert show];

}


- (void) createDirectory{
	NSIndexPath *selectedIndexPath = [self.tableView indexPathForSelectedRow];
	NSInteger selectedIndex = (!selectedIndexPath) ? -1 : selectedIndexPath.row;
	[self.tableView deselectRowAtIndexPath:[self.tableView indexPathForSelectedRow] animated:YES];
	[datasourceManager createDirectoryAtIndex:selectedIndex withName:[NSString stringWithFormat:@"Directory%i", (arc4random()%1000)]];
	
}
- (void) createFile{
	NSIndexPath *selectedIndexPath = [self.tableView indexPathForSelectedRow];
	NSInteger selectedIndex = (!selectedIndexPath) ? -1 : selectedIndexPath.row;
	[self.tableView deselectRowAtIndexPath:[self.tableView indexPathForSelectedRow] animated:YES];
	[datasourceManager createFileAtIndex:selectedIndex withName:[NSString stringWithFormat:@"Directory%i", (arc4random()%1000)]];

}
- (void) duplicateFile{
	NSIndexPath *selectedIndexPath = [self.tableView indexPathForSelectedRow];
	NSInteger selectedIndex = (!selectedIndexPath) ? -1 : selectedIndexPath.row;
	[self.tableView deselectRowAtIndexPath:[self.tableView indexPathForSelectedRow] animated:YES];
	[datasourceManager duplicateFileAtIndex:selectedIndex withName:[NSString stringWithFormat:@"Duplicate%i", (arc4random()%1000)]];

}

- (void) willPresentAlertView:(UIAlertView *)alertView{
	[fileCreatorPopoverController dismissPopoverAnimated:YES];

	fileCreatorPopoverController = nil;
	
}

- (void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)index{
	if (index != 0) {
		if (alertView == createFileAlert) [self createFile];
		else if (alertView == createDirectoryAlert) [self createDirectory];
		else if (alertView == duplicateFileAlert) [self duplicateFile];
	}
}
- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    //[((UIAlertView *)alertView).textField resignFirstResponder];
    //[((UIAlertView *)alertView).textField removeFromSuperview];  
    //[((UIAlertView *)alertView).textField release];  
}






@end
