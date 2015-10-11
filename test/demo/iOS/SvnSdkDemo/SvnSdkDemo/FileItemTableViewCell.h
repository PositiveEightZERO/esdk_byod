//
//  N4FileTreeCellView.h
//  Accordion
//
//  Created by Ignacio Enriquez Gutierrez on 8/28/10.
//  Copyright (c) 2010 Nacho4D.
//  See the file license.txt for copying permission.
//

#import <Foundation/Foundation.h>

typedef enum{
	FileBrowseTableViewCellTypeFile,
    FileBrowseTableViewCellTypeEncryptedFile,
	FileBrowseTableViewCellTypeDirectory
}FileBrowseTableViewCellType;


@interface FileItemTableViewCell : UITableViewCell
{

@private
	IBOutlet UIImageView *directoryAccessoryImageView;
	FileBrowseTableViewCellType cellType;
	BOOL expanded;
}

@property (nonatomic) FileBrowseTableViewCellType cellType;
@property (nonatomic, getter=isExpanded) BOOL expanded;
@property (nonatomic, retain) IBOutlet UIImageView *directoryAccessoryImageView;

- (id) initWithReuseIdentifier:(NSString *)reuseIdentifier;

@end
