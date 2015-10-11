//
//  File.h
//  MandalaChar
//
//  Created by Ignacio Enriquez Gutierrez on 8/9/10.
//  Copyright (c) 2010 Nacho4D.
//  See the file license.txt for copying permission.
//
#import <UIKit/UIKit.h>

typedef enum{
    FileItemTypeOriginUnknown,
	FileItemTypeOriginFile,
    FileItemTypeEncryptedFile,
	FileItemTypeDirectory
}FileItemType;


@interface FileItem : NSObject <NSCopying>
{
	
	NSString *name;
	NSString *parentDirectory;
	
	NSString *fullName;
    
    
	FileItemType type;
	
	UIImage *image;
	
	BOOL expanded;
	NSInteger level; //used for cell indentation

}

@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSString *parentDirectory;

@property (nonatomic, readonly, assign) NSString *fullName;
@property (nonatomic, readonly, assign) FileItemType type;

@property (nonatomic, readonly, assign) UIImage *image;

@property (nonatomic, readonly) BOOL isDirectory;
@property (nonatomic, getter=isExpanded) BOOL expanded;
@property (nonatomic) NSInteger level;

- (id) initWithName:(NSString *)aName parentDirectory:(NSString *)aParentDirectory;

@end
