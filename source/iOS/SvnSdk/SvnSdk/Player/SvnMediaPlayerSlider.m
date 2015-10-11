//
//  SvnMediaPlayerSlider.m
//  SvnMediaPlayerView
//
//  Created by Guilherme Araújo on 08/12/14.
//  Copyright (c) 2014 Guilherme Araújo. All rights reserved.
//

#import "SvnMediaPlayerSlider.h"

@interface SvnMediaPlayerSlider ()

@property (strong, nonatomic) UIProgressView *progressView;

@end

@implementation SvnMediaPlayerSlider

@synthesize progressView;

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    [self setup];
    return self;
}

- (void)setup {
    [self setMaximumTrackTintColor:[UIColor clearColor]];
    
    progressView = [UIProgressView new];
    [progressView setTranslatesAutoresizingMaskIntoConstraints:NO];
    [progressView setClipsToBounds:YES];
    [[progressView layer] setCornerRadius:1.0f];
    
    if([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
    {
        CGFloat hue, sat, bri;
        [[self tintColor] getHue:&hue saturation:&sat brightness:&bri alpha:nil];
        [progressView setTintColor:[UIColor colorWithHue:hue saturation:(sat * 0.6f) brightness:bri alpha:1]];
    }
    
    [self addSubview:progressView];
    
    NSArray *constraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[PV]|"
                                                                   options:0
                                                                   metrics:nil
                                                                     views:@{@"PV" : progressView}];
    
    [self addConstraints:constraints];
    
    constraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(20)-[PV]"
                                                          options:0
                                                          metrics:nil
                                                            views:@{@"PV" : progressView}];
    
    [self addConstraints:constraints];
}

- (void)setSecondaryValue:(float)value {
    [progressView setProgress:value];
}

- (void)setTintColor:(UIColor *)tintColor {
    
    if([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
    {
        [super setTintColor:tintColor];
        
        CGFloat hue, sat, bri;
        [[self tintColor] getHue:&hue saturation:&sat brightness:&bri alpha:nil];
        [progressView setTintColor:[UIColor colorWithHue:hue saturation:(sat * 0.6f) brightness:bri alpha:1]];
    }
}

- (void)setSecondaryTintColor:(UIColor *)tintColor {
    
    if([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
    {
        [progressView setTintColor:tintColor];
    }
    
}

@end
