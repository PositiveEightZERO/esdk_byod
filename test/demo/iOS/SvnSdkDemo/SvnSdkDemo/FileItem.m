//
//  File.m
//  MandalaChart
//
//  Created by Ignacio Enriquez Gutierrez on 8/9/10.
//  Copyright (c) 2010 Nacho4D.
//  See the file license.txt for copying permission.
//

#import "FileItem.h"

#import <SvnSdk/svn_file_api.h>

@implementation FileItem
@synthesize name, parentDirectory;
//@synthesize expanded;
//@synthesize level;

//Lazy properties:
- (NSString *) fullName{
	return [parentDirectory stringByAppendingPathComponent:name];
}

- (UIImage *) image{
	if (!image) {
		if ([self type] == FileItemTypeDirectory)
			image = [UIImage imageNamed:@"Folder56.png"];
		else if([self type] == FileItemTypeOriginFile)
			image = [UIImage imageNamed:@"decrypt.png"];
        else
            image = [UIImage imageNamed:@"encrypt.png"];
	}
	return image;
}


- (FileItemType) type{
	if (type == FileItemTypeOriginUnknown) {
		NSFileManager *manager = [NSFileManager defaultManager];
        NSString * fullPath = [parentDirectory stringByAppendingPathComponent:name];
        
        BOOL isDir;
        if ([[NSFileManager defaultManager] fileExistsAtPath:fullPath isDirectory:&isDir] &&isDir) {
            type = FileItemTypeDirectory;
        }
        else if ([[NSFileManager defaultManager] fileExistsAtPath:fullPath isDirectory:&isDir] && !isDir) {
            type = FileItemTypeOriginFile;
        }
        else
        {
            type = FileItemTypeEncryptedFile;
        }
        
	}
	return type;
}

- (BOOL) isDirectory{
	return	([self type] == FileItemTypeDirectory);
}
- (NSString *) description{
	return [NSString stringWithFormat:@"N4File:%@ directory:%d",  [self name], self.type];
}
- (BOOL) isEmptyDirectory{
	if (self.isDirectory){
		
		NSFileManager *fm = [NSFileManager defaultManager];
		NSError *error = nil;
		NSArray *subfiles = [fm contentsOfDirectoryAtPath:[self fullName] error:&error];
		if (error) {
			NSLog(@"Error %s: %@", _cmd, [error localizedDescription]);
		}
		if ([subfiles count])
			return NO;
		else
			return YES;
	}
	return NO;
}

#pragma mark -

- (void) loadMembers{
	//TODO: load all members not loaded yet, this should be better than calling NSFileManager for every property.
	fullName = nil;
	type = nil;
	image = nil;

}

- (void) unloadMembers{
	//TODO: release members and set them to nil
}

- (id) initWithName:(NSString *)aName parentDirectory:(NSString *)aParentDirectory{
	if (self = [super init]) {
		self.name = aName;
		self.parentDirectory = aParentDirectory;
		
		fullName = nil;
		type = nil;
		image = nil;
        
        self.expanded = NO;
		
	}
	return self;
}

- (id) init{
	return [self initWithName:@"" parentDirectory:@""];
}

//- (void) dealloc{
//	[name release];
//	[parentDirectory release];
//	[super dealloc];
//}

#pragma mark -
#pragma mark NSCopying

- (id)copyWithZone:(NSZone *)zone{
	//Shallow copy : We need a shallow copies here since instances of N4File class 
	//will become keys in NSDictionaries as well and comparing pointers is easier much easier.
	return self;
}

@end
