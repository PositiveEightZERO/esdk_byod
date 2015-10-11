//
//  GuideViewController.h
//  SvnSdkDemo
//
//  Created by l00174413 on 14-9-3.
//
//

#import <UIKit/UIKit.h>

@interface GuideViewController : UIViewController
@property (weak, nonatomic) IBOutlet UITextView *titleTextView;
@property (weak, nonatomic) IBOutlet UITextView *guideTextView;


@property (assign, nonatomic) NSString *guideType;

@end
