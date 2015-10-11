//
//  SafeKeyboardStyle.h
//
//

#import <Foundation/Foundation.h>

@protocol SafeKeyboardStyle <NSObject>

+ (CGRect)safeKeyboardFrame;
+ (CGFloat)separator;
+ (UIColor *)safeKeyboardBackgroundColor;

+ (UIFont *)safeKeyboardButtonFont;
+ (UIColor *)safeKeyboardButtonBackgroundColor;
+ (UIColor *)safeKeyboardButtonHighlightedColor;
+ (UIColor *)safeKeyboardButtonTextColor;

+ (UIFont *)functionButtonFont;
+ (UIColor *)functionButtonBackgroundColor;
+ (UIColor *)functionButtonHighlightedColor;
+ (UIColor *)functionButtonTextColor;
+ (UIImage *)clearFunctionButtonImage;
+ (UIImage *)capslockFunctionButtonImage;

@end
