//
//  FaqsTableViewCell.h
//  SvnSdkDemo
//
//  Created by l00174413 on 14-9-2.
//
//

#import <UIKit/UIKit.h>

@interface FaqsTableViewCell : UITableViewCell

@property (weak, nonatomic) IBOutlet UITextView *questionTextView;
@property (weak, nonatomic) IBOutlet UITextView *answerTextView;

@end
