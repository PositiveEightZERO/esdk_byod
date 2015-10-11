//
//  MenuViewCell.m
//  SvnSdkDemo
//
//  Created by l00174413 on 14-8-26.
//
//

#import "MenuViewCell.h"

@implementation MenuViewCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code
    }
    return self;
}

- (void)awakeFromNib
{
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
    
    // Configure the view for the selected state
}

- (IBAction)disclosureButtonClicked:(id)sender {
    //self.
    // UIButton * button = (UIButton *)sender;
    //MenuViewCell *cell = (MenuViewCell*)button.superview.superview;
    UIView *v = self;
    while (v && ![v isKindOfClass:[UITableView class]]) v = v.superview;
    
    
    UITableView *tableView = (UITableView*)v;
    NSIndexPath *indexPath = [tableView indexPathForCell:self];
    [tableView.delegate tableView:tableView accessoryButtonTappedForRowWithIndexPath:indexPath];
}
@end
