//
//  MenuViewCell.h
//  SvnSdkDemo
//
//  Created by l00174413 on 14-8-26.
//
//

#import <UIKit/UIKit.h>

@interface MenuViewCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UITextView *itemLabel;
- (IBAction)disclosureButtonClicked:(id)sender;

@end
