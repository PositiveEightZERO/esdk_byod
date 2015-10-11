//
//  N4fileSystemManager.h
//  Accordion
//
//  Created by Enriquez Gutierrez Guillermo Ignacio on 8/27/10.
//  Copyright (c) 2010 Nacho4D.
//  See the file license.txt for copying permission.
//

#import <UIKit/UIKit.h>

@class FileItem;
@class FileSystemManager;
@protocol FileSystemManagerDelegate <NSObject>
@required
- (void) fileSystemManager:(FileSystemManager *) manager didInsertRowsAtIndexPaths:(NSArray *)indexPaths;
- (void) fileSystemManager:(FileSystemManager *) manager didRemoveRowsAtIndexPaths:(NSArray *)indexPaths;
- (void) fileSystemManager:(FileSystemManager *) manager didCreateSuccessfullyFile:(FileItem *)file;
- (void) fileSystemManager:(FileSystemManager *) manager didFailOnCreationofFile:(FileItem *) file error:(NSError *)error;


@end


@interface FileSystemManager : NSObject {
	
	NSMutableArray *_sortDescriptors;
	NSMutableArray *_mergedRootBranch;
	NSMutableDictionary *_unmergedBranches;
	FileItem *rootDirectory;
}

@property (nonatomic, retain) NSMutableArray *sortDescriptors;
@property (nonatomic, retain, readonly) NSMutableArray * mergedRootBranch;
@property (nonatomic, assign) id<FileSystemManagerDelegate> delegate;

+ (NSMutableArray *) defaultSortDescriptors;
- (id) initWithRootPath:(NSString *)path sortDescriptors:(NSMutableArray *)sortDescs;
- (void) sort;

- (void) expandBranchAtIndex:(NSInteger)index;
- (void) collapseBranchAtIndex:(NSInteger)index;

- (void) moveFileFromIndex:(NSInteger)fromIndex toIndex:(NSInteger)toIndex;

- (void) deleteFileAtIndex:(NSInteger)index;

- (void) createDirectoryAtIndex:(NSInteger)index withName:(NSString *)fileName;
- (void) createFileAtIndex:(NSInteger)index withName:(NSString *)fileName;
- (void) duplicateFileAtIndex:(NSInteger)index withName:(NSString *)fileName;




@end
