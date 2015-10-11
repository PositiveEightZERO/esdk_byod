//
//  MenuSectionInfo.h
//  SvnSdkDemo
//
//  Created by l00174413 on 14-8-27.
//
//

#import <Foundation/Foundation.h>

@interface MenuSectionInfo : NSObject

@property (nonatomic, strong) NSString *sectionName;
@property (nonatomic, strong) NSArray *menuItems;

@end



@interface MenuItem : NSObject

@property (nonatomic, strong) NSString *itemName;
@property (nonatomic, strong) NSString *itemAction;

@end