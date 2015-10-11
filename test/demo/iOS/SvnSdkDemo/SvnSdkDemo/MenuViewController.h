//
//  MenuViewControllerTableViewController.h
//  SvnSdkDemo
//
//  Created by l00174413 on 14-8-25.
//
//

#import <UIKit/UIKit.h>
#import "APLSectionHeaderView.h"


@interface MenuViewController : UITableViewController<SectionHeaderViewDelegate>

@property (nonatomic, strong) NSArray *sections;

@end
