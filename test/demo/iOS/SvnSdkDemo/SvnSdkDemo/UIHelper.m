//
//  UIImage_Extension.m
//  SvnSdkDemo
//
//  Created by l00174413 on 14-5-13.
//
//
#import "UIHelper.h"

@implementation UIImage(Extension)

+ (UIImage *)imageWithColor:(UIColor *)color size:(CGSize)size
{
    CGRect rect = CGRectMake(0, 0, size.width, size.height);
    UIGraphicsBeginImageContext(rect.size);
    CGContextRef context = UIGraphicsGetCurrentContext();

    CGContextSetFillColorWithColor(context, [color CGColor]);
    CGContextFillRect(context, rect);

    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return image;
}
@end


@implementation  UIHelper
+ (UIButton *)navButton:(NSString *)title
{
    UIColor * selectedColor = [UIColor colorWithRed:(0/255.f) green:(153/255.f) blue:(255/255.f) alpha:1.0f];
    UIColor * normalColor = [UIColor colorWithRed:(246/255.f) green:(246/255.f) blue:(246/255.f) alpha:1.0f];


    UIButton *button = [UIButton buttonWithType:(UIButtonTypeCustom)];
    //[backButton setImage:[UIImage imageNamed:@"btn_back.png"] forState:(UIControlStateNormal)];
    [button setTitle:title forState:(UIControlStateNormal)];
    button.titleLabel.font = [UIFont systemFontOfSize:14.0];
    //[button.titleLabel sizeToFit];
    button.titleLabel.adjustsFontSizeToFitWidth = YES;
    //button.titleLabel.minimumScaleFactor = 0.4;
    
    button.titleLabel.numberOfLines = 1;
    button.titleLabel.adjustsFontSizeToFitWidth = YES;
    button.titleLabel.lineBreakMode = NSLineBreakByClipping;
    
    [button setTitleColor:selectedColor forState:(UIControlStateNormal)];
    [button setImageEdgeInsets:(UIEdgeInsetsMake(0, -2, 0, 0))];
    [button setTitleEdgeInsets:(UIEdgeInsetsMake(0, 3, 0, 0))];
    button.frame = CGRectMake(0, 0, 80, 44);
    [button sizeToFit];
    
    return button;
}
@end

