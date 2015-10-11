//
//  RefreshWebAppActivity.h
//  anyofficesdk
//
//  Created by f00291727 on 12/29/14.
//  Copyright (c) 2014 pangqi. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol RefreshWebAppDelegate <NSObject>

-(void)performRefreshWebAppActivity;

@end

@interface RefreshWebAppActivity : UIActivity


- (id) initWithTitle:(NSString *)title delegate:(id<RefreshWebAppDelegate>)delegate;


@end
