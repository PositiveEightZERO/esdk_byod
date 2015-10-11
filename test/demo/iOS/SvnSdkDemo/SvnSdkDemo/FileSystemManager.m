//
//  N4fileSystemManager.m
//  Accordion
//
//  Created by Enriquez Gutierrez Guillermo Ignacio on 8/27/10.
//  Copyright (c) 2010 Nacho4D.
//  See the file license.txt for copying permission.
//

#import "FileSystemManager.h"
#import "FileItem.h"

//#import <SvnSdk/svn_file_api.h>
#import <SvnSdk/SvnFileManager.h>

@interface FileSystemManager ()

@property (nonatomic, retain, readwrite) NSMutableArray * mergedRootBranch;
@end


@implementation FileSystemManager

@synthesize sortDescriptors = _sortDescriptors;
@synthesize mergedRootBranch = _mergedRootBranch;
@synthesize delegate;

- (void) _sortBranches{
    for (NSMutableArray *branch in [_unmergedBranches allValues]){
        [branch sortUsingDescriptors:self.sortDescriptors];
    }
}

- (void) _mergeBranches{
    
    [self.mergedRootBranch setArray:[_unmergedBranches objectForKey:rootDirectory]];
    NSMutableArray *branchKeys = [[_unmergedBranches allKeys] mutableCopy];
    NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:@"level" ascending:YES];
    [branchKeys sortUsingDescriptors:[NSArray arrayWithObject:sortDesc]];
    
    NSInteger index = NSNotFound;
    for (FileItem *branchKey in branchKeys) {
        if (branchKey == rootDirectory) continue;
        NSMutableArray *unmergedBranch = [_unmergedBranches objectForKey:branchKey];
        index = [self.mergedRootBranch indexOfObject:branchKey];
        [self.mergedRootBranch replaceObjectsInRange:NSMakeRange(index+1, 0) withObjectsFromArray:unmergedBranch];
    }
    
}

+ (NSMutableArray *) defaultSortDescriptors{
    
    NSSortDescriptor * sortDescType = [[NSSortDescriptor alloc] initWithKey:@"type" ascending:YES]; // Folders come first
    NSSortDescriptor * sortDescName = [[NSSortDescriptor alloc] initWithKey:@"name" ascending:NO];  // Z ~ A
    //	NSSortDescriptor * sortDescCreationDate = [[NSSortDescriptor alloc] initWithKey:@"creationDate" ascending:YES];
    //	NSSortDescriptor * sortDescModificationDate = [[NSSortDescriptor alloc] initWithKey:@"modificationDate" ascending:YES];
    
    NSMutableArray *descs = [NSMutableArray arrayWithObjects:
                             //sortDescCreationDate,
                             sortDescType,
                             sortDescName,
                             //sortDescModificationDate,
                             nil];
    
    
    
    return descs;
}


- (NSArray*) listDirectory:(NSString*)directory
{
    //    NSMutableArray * resultArray = [[NSMutableArray alloc] init];
    //    /*open directory*/
    //    SVN_DIR_S     *dir     = NULL;
    //    dir = svn_opendir([directory UTF8String]);
    //    if(NULL == dir)
    //    {
    //        return resultArray;
    //    }
    //    SVN_DIRINFO_S* subdir = NULL;
    //    subdir = svn_readdir(dir);
    //    while(subdir != NULL)
    //    {
    //        if((strcmp(subdir->acDirName,".") == 0) ||(strcmp(subdir->acDirName,"..") == 0))
    //        {
    //            subdir = svn_readdir(dir);
    //            continue;
    //        }
    //
    //        NSString * result = [NSString stringWithUTF8String:subdir->acDirName];
    //        if(result)
    //        {
    //            [resultArray addObject:result];
    //        }
    //
    //        subdir = svn_readdir(dir);
    //    }
    //
    //    svn_closedir(dir);
    //
    //    return resultArray;
    
    SvnFileManager * fileManager = [SvnFileManager defaultManager];
    return [fileManager directoryContentsAtPath:directory];
}

- (id) initWithRootPath:(NSString *)path sortDescriptors:(NSMutableArray *)sortDescs{
    
    if (self = [super init]) {
        
        self.sortDescriptors = sortDescs;
        
        _unmergedBranches = [[NSMutableDictionary alloc] init];
        
        
        NSArray *fileNames = [self listDirectory:path];
        
        NSMutableArray *filesAtRootLevel = [[NSMutableArray alloc] init];
        for (NSString * fileName in fileNames)
        {
            FileItem *aFile = [[FileItem alloc] initWithName:fileName parentDirectory:path];
            [aFile setLevel:0];
            [filesAtRootLevel addObject:aFile];
            //			[aFile release];
        }
        
        
        NSString *parentPath = [path stringByDeletingLastPathComponent];
        rootDirectory = [[FileItem alloc] initWithName:[path lastPathComponent] parentDirectory:parentPath];
        //rootDirectory.isDirectory = YES;
        [rootDirectory setLevel:-1];
        [rootDirectory setExpanded:YES];
        
        [_unmergedBranches setObject:filesAtRootLevel forKey:rootDirectory];
        //		[filesAtRootLevel release];
        
        _mergedRootBranch = [[NSMutableArray alloc] init];
        
        [self _sortBranches];
        [self _mergeBranches];
        
    }
    return self;
}

#pragma mark -

- (void) sort{
    [self _sortBranches];
    [self _mergeBranches];
}

- (void) reloadAllBranches{
    //TODO:
}
#pragma mark -

- (void) expandBranchAtIndex:(NSInteger)index{
    FileItem *directoryFile = [self.mergedRootBranch objectAtIndex:index];
    if (directoryFile.isDirectory) {
        
        //Make Branch:
        
        NSArray *fileNames = [self listDirectory:directoryFile.fullName];
        
        NSMutableArray *newBranch = [[NSMutableArray alloc] init];
        for (NSString * fileName in fileNames)
        {
            FileItem *aFile = [[FileItem alloc] initWithName:fileName parentDirectory:directoryFile.fullName];
            [aFile setLevel:directoryFile.level + 1];
            [newBranch addObject:aFile];
            //			[aFile release];
        }
        
        //sort and add it unmerged branches
        [newBranch sortUsingDescriptors:self.sortDescriptors];
        [_unmergedBranches setObject:newBranch forKey:directoryFile];
        //		[newBranch release];
        
        //merge it:
        [_mergedRootBranch replaceObjectsInRange:NSMakeRange(index+1, 0) withObjectsFromArray:newBranch];
        [directoryFile setExpanded:YES];
        
        //call delegate
        if ([delegate respondsToSelector:@selector(fileSystemManager:didInsertRowsAtIndexPaths:)]) {
            NSMutableArray *paths = [NSMutableArray array];
            for (int i = 0; i < [newBranch count]; i++) {
                [paths addObject:[NSIndexPath indexPathForRow:(index + 1 + i) inSection:0]];
            }
            [delegate fileSystemManager:self didInsertRowsAtIndexPaths:paths];
        }
    }
}

- (void) collapseBranchAtIndex:(NSInteger)index;{
    FileItem *directoryFile = [self.mergedRootBranch objectAtIndex:index];
    if (directoryFile.isDirectory) {
        
        NSMutableArray *branchToCollapse = [_unmergedBranches objectForKey:directoryFile];
        
        //recursively collapse expanded branches
        for (int i = 0; i < [branchToCollapse count]; i++) {
            FileItem *file = [branchToCollapse objectAtIndex:i];
            if (file.isDirectory && file.isExpanded) {
                [self collapseBranchAtIndex:index + i + 1]; //+1 is super important!!
            }
        }
        
        NSRange range = NSMakeRange(index+1, [branchToCollapse count]);
        [self.mergedRootBranch removeObjectsInRange:range];
        [_unmergedBranches removeObjectForKey:directoryFile];
        [directoryFile setExpanded:NO];
        
        //call delegate
        if ([delegate respondsToSelector:@selector(fileSystemManager:didRemoveRowsAtIndexPaths:)]) {
            NSMutableArray *paths = [NSMutableArray array];
            for (int i = 0; i < range.length; i++)
            {
                [paths addObject:[NSIndexPath indexPathForRow:(index + 1 + i) inSection:0]];
            }
            [delegate fileSystemManager:self didRemoveRowsAtIndexPaths:paths];
        }
        
    }
    
}

#pragma mark -



- (void) moveFileFromIndex:(NSInteger)fromIndex toIndex:(NSInteger)toIndex{
    
    //if file:
    //get all indexes for root expanded branches.
    //find a position between two indexes, and place
    //replace the phisical object.
    //update datasoruce
    
    //if directory:
    //get all files:
}

- (void) deleteFileAtIndex:(NSInteger)index{
    FileItem *file = [self.mergedRootBranch objectAtIndex:index];
    if (file.isDirectory && file.isExpanded)
        [self collapseBranchAtIndex:index];
    
    SvnFileManager *fm = [SvnFileManager defaultManager];
    NSError *error = nil;
    [fm removeItemAtPath:[file fullName] error:&error];
    if (error)	NSLog(@"Error %s :%@", _cmd, [error localizedDescription]);
    
    //    if(file.type != FileItemTypeEncryptedFile)
    //    {
    //        //delete file from disk:
    //        SvnFileManager *fm = [SvnFileManager defaultManager];
    //        NSError *error = nil;
    //        [fm removeItemAtPath:[file fullName] error:&error];
    //        if (error)	NSLog(@"Error %s :%@", _cmd, [error localizedDescription]);
    //    }
    //    else
    //    {
    //        int ret = svn_remove([file.fullName UTF8String]);
    //        if(ret != 0)
    //        {
    //            NSLog(@"svn_remove returns %d :%@", ret, file.fullName);
    //        }
    //    }
    
    
    
    
    
    //delete file from merged branch
    [self.mergedRootBranch removeObjectAtIndex:index];
    index = NSNotFound;
    
    //delete file from corresponding unmerged branch
    NSEnumerator *enumerator = [_unmergedBranches objectEnumerator];
    id branch;
    while ((branch = [enumerator nextObject])) {
        index = [branch indexOfObject:file];
        if (index != NSNotFound) {
            [branch removeObjectAtIndex:index];
            break;
        }
    }
}

- (void) deleteFileAtIndex2:(NSInteger)index{
    FileItem *file = [self.mergedRootBranch objectAtIndex:index];
    if (file.isDirectory && file.isExpanded)
        [self collapseBranchAtIndex:index];
    
    
    //delete file from disk:
    SvnFileManager *fm = [SvnFileManager defaultManager];
    NSError *error = nil;
    [fm removeItemAtPath:[file fullName] error:&error];
    if (error)	NSLog(@"Error %s :%@", _cmd, [error localizedDescription]);
    
    //delete file from memory
    //delete it from corresponding unmerged branch - get its containerDirectory
    FileItem *containerDirectory = nil;
    NSInteger offsetToContainerDirectory;
    for (int i = index; i > 0 ; --i) {
        FileItem *tempFile = [self.mergedRootBranch objectAtIndex:i];
        if (tempFile.level < file.level) {
            containerDirectory = tempFile;
            offsetToContainerDirectory = index - i - 1;
            break;
        }
    }
    
    if (!containerDirectory) {
        containerDirectory = rootDirectory;
        offsetToContainerDirectory = index;
    }
    
    //I should refactor this, many ifs should not be needed.
    NSMutableArray *branch = [_unmergedBranches objectForKey:containerDirectory];
    if (containerDirectory != rootDirectory) {
        [branch removeObjectAtIndex:offsetToContainerDirectory];
    }
    
    //delete it from merged branch if needed
    if (branch != self.mergedRootBranch)
        [self.mergedRootBranch removeObjectAtIndex:index];
}

- (FileItem *)_directoryToInsertFileAtIndex:(NSInteger)index{
    if (index < 0) return rootDirectory;
    
    FileItem *referenceFile = [self.mergedRootBranch objectAtIndex:index];
    FileItem *containerDirectory = nil;
    
    if (referenceFile.isDirectory){ //if is a directory create the file in it
        if (!referenceFile.isExpanded) [self expandBranchAtIndex:index];
        containerDirectory = referenceFile;
        
    } else if (referenceFile.level == 0) { //if is a file (not directory) in the first level, create in rootDirectory
        containerDirectory = rootDirectory;
        
    }else { //otherwise look for the correct folder to create in
        
        for (int i = index; i > 0 ; --i) {
            FileItem *file = [self.mergedRootBranch objectAtIndex:i];
            if (file.level < referenceFile.level) {
                containerDirectory = file;
                break;
            }
        }
    }
    
    return containerDirectory;
}
- (void) createDirectoryAtIndex:(NSInteger)index withName:(NSString *)fileName{
    
    FileItem *containerDirectory = [self _directoryToInsertFileAtIndex:index];
    
    //create file on disk
    NSString *creationPath = [[containerDirectory fullName] stringByAppendingPathComponent:fileName];
    SvnFileManager *fm = [SvnFileManager defaultManager];
    [fm createDirectoryAtPath:creationPath attributes:nil];
    
    //create file in memory
    FileItem *newFile = [[FileItem alloc] initWithName:fileName parentDirectory:[containerDirectory fullName]];
    [newFile setLevel:[containerDirectory level] +1];
    
    //add it to branch and sort
    NSMutableArray *branch = [_unmergedBranches objectForKey:containerDirectory];
    [branch addObject:newFile];
    [branch sortUsingDescriptors:_sortDescriptors];
    [self _mergeBranches];
    
    NSIndexPath *insertedIndex = [NSIndexPath indexPathForRow:[self.mergedRootBranch indexOfObject:newFile] inSection:0];
    [delegate fileSystemManager:self didInsertRowsAtIndexPaths:[NSArray arrayWithObject:insertedIndex]];
    
    return;
}

- (void) createFileAtIndex:(NSInteger)index withName:(NSString *)fileName{
    
    FileItem *containerDirectory = [self _directoryToInsertFileAtIndex:index];
    
    //create file on disk
    NSString *creationPath = [[containerDirectory fullName] stringByAppendingPathComponent:fileName];
    SvnFileManager *fm = [SvnFileManager defaultManager];
    [fm createFileAtPath:creationPath
                contents:nil
              attributes:nil];
    
    //create file in memory
    FileItem *newFile = [[FileItem alloc] initWithName:fileName parentDirectory:[containerDirectory fullName]];
    [newFile setLevel:[containerDirectory level] +1];
    
    //add it to branch and sort
    NSMutableArray *branch = [_unmergedBranches objectForKey:containerDirectory];
    [branch addObject:newFile];
    [branch sortUsingDescriptors:_sortDescriptors];
    [self _mergeBranches];
    
    NSIndexPath *insertedIndex = [NSIndexPath indexPathForRow:[self.mergedRootBranch indexOfObject:newFile] inSection:0];
    [delegate fileSystemManager:self didInsertRowsAtIndexPaths:[NSArray arrayWithObject:insertedIndex]];
    
    return;
    
}
- (void) duplicateFileAtIndex:(NSInteger)index withName:(NSString *)fileName{
    
    FileItem *referenceFile = [self.mergedRootBranch objectAtIndex:index];
    if (!referenceFile.isDirectory) {
        FileItem *containerDirectory = [self _directoryToInsertFileAtIndex:index];
        
        //create file on disk
        NSString *creationPath = [[containerDirectory fullName] stringByAppendingPathComponent:fileName];
        SvnFileManager *fm = [SvnFileManager defaultManager];
        NSError *error = nil;
        [fm copyItemAtPath:[referenceFile fullName] toPath:creationPath error:&error];
        if (error) NSLog(@"ERROR: %s, %@", [error localizedDescription]);
        
        //create file in memory
        FileItem *newFile = [[FileItem alloc] initWithName:fileName parentDirectory:[containerDirectory fullName]];
        [newFile setLevel:[containerDirectory level] +1];
        
        //add it to branch and sort
        NSMutableArray *branch = [_unmergedBranches objectForKey:containerDirectory];
        [branch addObject:newFile];
        [branch sortUsingDescriptors:_sortDescriptors];
        [self _mergeBranches];
        
        NSIndexPath *insertedIndex = [NSIndexPath indexPathForRow:[self.mergedRootBranch indexOfObject:newFile] inSection:0];
        [delegate fileSystemManager:self didInsertRowsAtIndexPaths:[NSArray arrayWithObject:insertedIndex]];
    }
    return;
}

#pragma mark -





@end
