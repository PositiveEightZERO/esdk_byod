//
//  UIImage_Extension.h
//  SvnSdkDemo
//
//  Created by l00174413 on 14-8-21.
//
//

#import <UIKit/UIKit.h>

@interface UIImage (Extension)

+ (UIImage *)imageWithColor:(UIColor *)color size:(CGSize)size;

@end


@interface UIHelper : NSObject 

+ (UIButton *)navButton:(NSString *)title;

@end
