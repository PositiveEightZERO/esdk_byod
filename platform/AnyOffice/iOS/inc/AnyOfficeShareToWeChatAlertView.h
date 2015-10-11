//
//  AnyOfficeShareToWeChatAlertView.h
//  AnyOfficeShareToWeChatAlertView
//
//
//  Created by f00291727 on 07/15/15.
//  Copyright (c) 2015 fanjiepeng. All rights reserved.
//


#import <UIKit/UIKit.h>

@protocol AnyOfficeShareToWeChatAlertViewDelegate

- (void)customIOS7dialogButtonTouchUpInside:(id)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end

@interface AnyOfficeShareToWeChatAlertView : UIView<AnyOfficeShareToWeChatAlertViewDelegate>

@property (nonatomic, retain) UIView *parentView;    // The parent view this 'dialog' is attached to
@property (nonatomic, retain) UIView *dialogView;    // Dialog's container view
@property (nonatomic, retain) UIView *containerView; // Container within the dialog (place your ui elements here)

@property (nonatomic, assign) id<AnyOfficeShareToWeChatAlertViewDelegate> delegate;
@property (nonatomic, retain) NSArray *buttonTitles;
@property (nonatomic, assign) BOOL useMotionEffects;

@property (copy) void (^onButtonTouchUpInside)(AnyOfficeShareToWeChatAlertView *alertView, int buttonIndex) ;

- (id)init;

/*!
 DEPRECATED: Use the [AnyOfficeShareToWeChatAlertView init] method without passing a parent view.
 */
- (id)initWithParentView: (UIView *)_parentView __attribute__ ((deprecated));

- (void)show;
- (void)close;

- (IBAction)customIOS7dialogButtonTouchUpInside:(id)sender;
- (void)setOnButtonTouchUpInside:(void (^)(AnyOfficeShareToWeChatAlertView *alertView, int buttonIndex))onButtonTouchUpInside;

- (void)deviceOrientationDidChange: (NSNotification *)notification;
- (void)dealloc;

@end
