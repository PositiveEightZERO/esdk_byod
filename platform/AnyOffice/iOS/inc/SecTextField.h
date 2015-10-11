//
//  SecTextField.h
//
//  Created by fanjiepeng on 15/3/24.
//  Copyright (c) 2015年 Fan Jiepeng. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SafeKeyboard.h"

@protocol SecTextFieldDelegate;

@protocol SecTextFieldDelegate <NSObject>
-(void)SecTextFieldDoneButtonClick;
@end


@interface SecTextField : UIView<SafeKeyboardDelegate>

@property (weak, readwrite, nonatomic) id<SecTextFieldDelegate> delegate;

/**
 * 初始化安全密码输入框控件
 * @param frame:控件在屏幕上的位置 useSafeKeyboard:是否使用安全键盘
 * @return 返回安全控件实例
 */
- (id)initWithFrame:(CGRect)frame useSafeKeyboard:(BOOL)useSafeKeyboard;
- (id)initWithFrame:(CGRect)frame useSafeKeyboard:(BOOL)useSafeKeyboard delegate:(id<SecTextFieldDelegate>)delegate;

/**
 * 获取安全密码
 * @param 无
 * @return 返回安全密码
 */
- (NSString *)getSafeCode;


/**
 * 设置安全密码
 * @param code：将安全密码设置为code
 * @return 无
 */
- (void)setSafeCode:(NSString *)code;

//----------------防暴力破解接口--------------
/**
 * 设置计数的最大值。密码输入错误次数达到此值时将不再允许用户输入
 * @param max：计数上限
 * @return 无
 */
- (void)setMaxCounts:(int)max;


/**
 * 计数加1.当用户密码输入错误时调用此接口，使密码输入错误次数加1
 * @param 无
 * @return 无
 */
- (void)increaseCount;


/**
 * 状态重置。重置密码输入错误次数为0
 * @param 无
 * @return 无
 */
- (void)reset;


//----------------安全控件样式设置接口---------------
/**
 * 设置占位符
 * @param placeholder：将安全控件的占位符为placeholder
 * @return 无
 */
- (void)setPlaceholder:(NSString *)placeholder;

/**
 * 设置是否隐藏眼睛按钮
 * @param hidden：是否隐藏眼睛按钮，YES隐藏，NO显示
 * @return 无
 */
- (void)setEyeButtonHidden:(BOOL)hidden;



/**
 * 设置边框样式
 * @param width：边框线条宽度 color:边框颜色 cornerRadius:边框圆角
 * @return 无
 */
- (void)setBorderWidth:(CGFloat)width color:(CGColorRef)color cornerRadius:(CGFloat)radius;


/**
 * 隐藏安全键盘
 * @param 无
 * @return 无
 */
-(void)hiddenSecKeyBoard;

/**
 * 显示安全键盘
 * @param 无
 * @return 无
 */
-(void)showSecKeyBoard;

@end
