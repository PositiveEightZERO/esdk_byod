//
//  SvnMediaPlayerView.m
//  SvnMediaPlayerView
//
//  Created by Guilherme Araújo on 08/12/14.
//  Copyright (c) 2014 Guilherme Araújo. All rights reserved.
//

#import "SvnMediaPlayerView.h"
#import "SvnMediaPlayerSlider.h"
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <MediaPlayer/MediaPlayer.h>

#import "SvnMediaResourceLoaderDelegate.h"


NSString * const kStatusKey                     = @"status";
NSString * const kRateKey                       = @"rate";
NSString * const kCurrentItemKey                = @"currentItem";
NSString * const kLoadedTimeRanges              = @"loadedTimeRanges";



@interface SvnMediaPlayerView () <AVAssetResourceLoaderDelegate, NSURLConnectionDataDelegate>


//@property (strong, nonatomic) AVURLAsset *asset;

@property (strong, nonatomic) AVPlayer *player;
@property (strong, nonatomic) AVPlayerLayer *playerLayer;
//@property (strong, nonatomic) AVPlayerItem *currentItem;
@property (strong, nonatomic) SvnMediaResourceLoaderDelegate * resourceDelegate;

@property (strong, nonatomic) UIView *controllersView;
//@property (strong, nonatomic) UILabel *airPlayLabel;

@property (strong, nonatomic) UIButton *closeButton;
@property (strong, nonatomic) UIButton *playButton;
@property (strong, nonatomic) UIButton *fullscreenButton;
@property (strong, nonatomic) MPVolumeView *volumeView;
@property (strong, nonatomic) SvnMediaPlayerSlider *progressIndicator;
@property (strong, nonatomic) UILabel *currentTimeLabel;
@property (strong, nonatomic) UILabel *remainingTimeLabel;
//@property (strong, nonatomic) UILabel *liveLabel;

@property (strong, nonatomic) UIView *spacerView;

@property (strong, nonatomic) UIActivityIndicatorView *activityIndicator;
//@property (strong, nonatomic) NSTimer *progressTimer;
@property (strong, nonatomic) NSTimer *controllersTimer;
@property (assign, nonatomic) BOOL seeking;
@property (assign, nonatomic) BOOL fullscreen;
@property (assign, nonatomic) CGRect defaultFrame;

@property (strong, nonatomic) NSObject *playbackLikelyToKeepUpKVOToken;

@property (nonatomic,strong)id scrubberTimeObserver;
@property (nonatomic,strong)id clockTimeObserver;

@property (nonatomic,assign)BOOL pauseReasonForcePause;
@property (nonatomic,assign)double timeObservingPrecision;

@property (nonatomic, assign) BOOL restorePlayStateAfterScrubbing;


@end

@implementation SvnMediaPlayerView


#pragma mark - View Life Cycle

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    self.defaultFrame = frame;
    self.timeObservingPrecision = 0.0;
    
    NSLog(@"defaultFrame;%@", NSStringFromCGRect(self.defaultFrame));
    [self setup];
    return self;
}

- (instancetype)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    self.timeObservingPrecision = 0.0;
    [self setup];
    return self;
}


//-(void)dealloc{
//    [[NSNotificationCenter defaultCenter] removeObserver:self];
//}

- (void)setup {
    // Set up notification observers
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerDidFinishPlaying:)
//                                                 name:AVPlayerItemDidPlayToEndTimeNotification object:nil];
//    
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerFailedToPlayToEnd:)
//                                                 name:AVPlayerItemFailedToPlayToEndTimeNotification object:nil];
//    
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerStalled:)
//                                                 name:AVPlayerItemPlaybackStalledNotification object:nil];
    //
    //  [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(airPlayAvailabilityChanged:)
    //                                               name:MPVolumeViewWirelessRoutesAvailableDidChangeNotification object:nil];
    //
    //  [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(airPlayActivityChanged:)
    //                                               name:MPVolumeViewWirelessRouteActiveDidChangeNotification object:nil];
    
    [self setBackgroundColor:[UIColor blackColor]];
    
    NSArray *horizontalConstraints;
    NSArray *verticalConstraints;
    /** Loading Indicator ***********************************************************************************************/
    self.activityIndicator = [UIActivityIndicatorView new];
    [self.activityIndicator stopAnimating];
    
    CGRect frame = self.frame;
    frame.origin = CGPointZero;
    [self.activityIndicator setFrame:frame];
    
    [self addSubview:self.activityIndicator];
    
    /** Container View **************************************************************************************************/
    self.controllersView = [UIView new];
    [self.controllersView setTranslatesAutoresizingMaskIntoConstraints:NO];
    [self.controllersView setBackgroundColor:[UIColor colorWithWhite:0.0f alpha:0.45f]];
    
    [self addSubview:self.controllersView];
    
    horizontalConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[CV]|"
                                                                    options:0
                                                                    metrics:nil
                                                                      views:@{@"CV" : self.controllersView}];
    
    verticalConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:[CV(40)]|"
                                                                  options:0
                                                                  metrics:nil
                                                                    views:@{@"CV" : self.controllersView}];
    [self addConstraints:horizontalConstraints];
    [self addConstraints:verticalConstraints];
    
    
    /** AirPlay View ****************************************************************************************************/
    
    //  airPlayLabel = [UILabel new];
    //  [airPlayLabel setTranslatesAutoresizingMaskIntoConstraints:NO];
    //  [airPlayLabel setText:@"AirPlay is enabled"];
    //  [airPlayLabel setTextColor:[UIColor lightGrayColor]];
    //  [airPlayLabel setFont:[UIFont fontWithName:@"HelveticaNeue-Light" size:13.0f]];
    //  [airPlayLabel setTextAlignment:NSTextAlignmentCenter];
    //  [airPlayLabel setNumberOfLines:0];
    //    //[airPlayLabel setHidden:YES];
    //
    //  [self addSubview:airPlayLabel];
    //
    //  horizontalConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[AP]|"
    //                                                                  options:0
    //                                                                  metrics:nil
    //                                                                    views:@{@"AP" : airPlayLabel}];
    //
    //  verticalConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[AP]-40-|"
    //                                                                options:0
    //                                                                metrics:nil
    //                                                                  views:@{@"AP" : airPlayLabel}];
    //  [self addConstraints:horizontalConstraints];
    //  [self addConstraints:verticalConstraints];
    
    /** UI Controllers **************************************************************************************************/

    self.playButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.playButton setTranslatesAutoresizingMaskIntoConstraints:NO];

    NSBundle* bundle = [NSBundle bundleWithURL:[[NSBundle mainBundle] URLForResource:@"SvnSdkResource" withExtension:@"bundle"]];

    
    [self.playButton setImage:[UIImage imageNamed:@"svn_mp_play" inBundle:bundle ofType:@"png"] forState:UIControlStateNormal];
    [self.playButton setImage:[UIImage imageNamed:@"svn_mp_pause" inBundle:bundle ofType:@"png"]forState:UIControlStateSelected];
    
//    volumeView = [MPVolumeView new];
//    [volumeView setTranslatesAutoresizingMaskIntoConstraints:NO];
//    [volumeView setShowsRouteButton:YES];
//    [volumeView setShowsVolumeSlider:YES];
    //[volumeView setAutoresizingMask:UIViewAutoresizingFlexibleWidth];
    
    self.fullscreenButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.fullscreenButton setTranslatesAutoresizingMaskIntoConstraints:NO];
    [self.fullscreenButton setImage:[UIImage imageNamed:@"svn_mp_expand" inBundle:bundle ofType:@"png"] forState:UIControlStateNormal];
    [self.fullscreenButton setImage:[UIImage imageNamed:@"svn_mp_shrink" inBundle:bundle ofType:@"png"] forState:UIControlStateSelected];
    
    self.currentTimeLabel = [UILabel new];
    [self.currentTimeLabel setTranslatesAutoresizingMaskIntoConstraints:NO];
    [self.currentTimeLabel setFont:[UIFont fontWithName:@"HelveticaNeue-Light" size:13.0f]];
    [self.currentTimeLabel setTextAlignment:NSTextAlignmentCenter];
    [self.currentTimeLabel setTextColor:[UIColor whiteColor]];
    
    self.remainingTimeLabel = [UILabel new];
    [self.remainingTimeLabel setTranslatesAutoresizingMaskIntoConstraints:NO];
    [self.remainingTimeLabel setFont:[UIFont fontWithName:@"HelveticaNeue-Light" size:13.0f]];
    [self.remainingTimeLabel setTextAlignment:NSTextAlignmentCenter];
    [self.remainingTimeLabel setTextColor:[UIColor whiteColor]];
    
    self.progressIndicator = [SvnMediaPlayerSlider new];
    [self.progressIndicator setTranslatesAutoresizingMaskIntoConstraints:NO];
    [self.progressIndicator setContinuous:YES];
    
    //  liveLabel = [UILabel new];
    //  [liveLabel setTranslatesAutoresizingMaskIntoConstraints:NO];
    //  [liveLabel setFont:[UIFont fontWithName:@"HelveticaNeue-Light" size:13.0f]];
    //  [liveLabel setTextAlignment:NSTextAlignmentCenter];
    //  [liveLabel setTextColor:[UIColor whiteColor]];
    //  [liveLabel setText:@"Live"];
    //[liveLabel setHidden:YES];
    
    self.spacerView = [UIView new];
    [self.spacerView setTranslatesAutoresizingMaskIntoConstraints:NO];
    
    [self.controllersView addSubview:self.playButton];
    [self.controllersView addSubview:self.fullscreenButton];
    //[controllersView addSubview:volumeView];
    [self.controllersView addSubview:self.currentTimeLabel];
    [self.controllersView addSubview:self.progressIndicator];
    [self.controllersView addSubview:self.remainingTimeLabel];
    //[controllersView addSubview:liveLabel];
    [self.controllersView addSubview:self.spacerView];
    
    horizontalConstraints = [NSLayoutConstraint
                             constraintsWithVisualFormat:@"H:|[P(40)][S(10)][C]-5-[I]-5-[R][F(40)]|"
                             options:0
                             metrics:nil
                             views:@{@"P" : self.playButton,
                                     @"S" : self.spacerView,
                                     @"C" : self.currentTimeLabel,
                                     @"I" : self.progressIndicator,
                                     @"R" : self.remainingTimeLabel,
//                                     @"V" : volumeView,
                                     @"F" : self.fullscreenButton}];
    
    [self.controllersView addConstraints:horizontalConstraints];
    

    
    //[volumeView hideByWidth:YES];
    //[spacerView hideByWidth:YES];
    
    //  horizontalConstraints = [NSLayoutConstraint
    //                           constraintsWithVisualFormat:@"H:|-5-[L]-5-|"
    //                           options:0
    //                           metrics:nil
    //                           views:@{@"L" : liveLabel}];
    //
    //  [controllersView addConstraints:horizontalConstraints];
    
    for (UIView *view in [self.controllersView subviews]) {
        verticalConstraints = [NSLayoutConstraint
                               constraintsWithVisualFormat:@"V:|-0-[V(40)]"
                               options:NSLayoutFormatAlignAllCenterY
                               metrics:nil
                               views:@{@"V" : view}];
        [self.controllersView addConstraints:verticalConstraints];
    }
    
    
    self.closeButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.closeButton setTranslatesAutoresizingMaskIntoConstraints:NO];
    [self.closeButton setImage: [UIImage imageNamed:@"svn_mp_close" inBundle:bundle ofType:@"png"] forState:UIControlStateNormal];
    //    [closeButton setTitle:@"Done" forState:UIControlStateNormal];
    //    [closeButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    
    [self addSubview:self.closeButton];
    horizontalConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|-10-[AP(40)]"
                                                                    options:0
                                                                    metrics:nil
                                                                      views:@{@"AP" : self.closeButton}];
    
    verticalConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|-10-[AP(40)]"
                                                                  options:0
                                                                  metrics:nil
                                                                    views:@{@"AP" : self.closeButton}];
        [self addConstraints:horizontalConstraints];
        [self addConstraints:verticalConstraints];
    
    
    /** Actions Setup ***************************************************************************************************/
    [self.closeButton addTarget:self action:@selector(closePlayer:) forControlEvents:UIControlEventTouchUpInside];
    [self.playButton addTarget:self action:@selector(togglePlay:) forControlEvents:UIControlEventTouchUpInside];
    [self.fullscreenButton addTarget:self action:@selector(toggleFullscreen:) forControlEvents:UIControlEventTouchUpInside];
    
    [self.progressIndicator addTarget:self action:@selector(scrub:) forControlEvents:UIControlEventValueChanged];
    [self.progressIndicator addTarget:self action:@selector(beginScrubbing:) forControlEvents:UIControlEventTouchDown];
    [self.progressIndicator addTarget:self action:@selector(endScrubbing:) forControlEvents:UIControlEventTouchUpInside | UIControlEventTouchUpOutside | UIControlEventTouchCancel];
    
    [self addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(showControllers)]];
    [self showControllers];
    
    self.controllersTimeoutPeriod = 3;
}

#pragma mark - UI Customization

- (void)setTintColor:(UIColor *)tintColor {
    
    if([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
    {
        [super setTintColor:tintColor];
        
        [self.progressIndicator setTintColor:tintColor];
    }
}

- (void)setBufferTintColor:(UIColor *)tintColor {
    [self.progressIndicator setSecondaryTintColor:tintColor];
}

//- (void)setLiveStreamText:(NSString *)text {
//  [liveLabel setText:text];
//}

//- (void)setAirPlayText:(NSString *)text {
//  [airPlayLabel setText:text];
//}



#pragma mark - Player View Actions

- (void)closePlayer:(UIButton *)button {
    NSLog(@"close button clicked...........");
    [self clean];
}



- (void)togglePlay:(UIButton *)button {
    
    NSLog(@"in togglePlay------------");
    if ([button isSelected]) {
        [button setSelected:NO];
        [self pause];
        
        if ([self.delegate respondsToSelector:@selector(playerDidPause)]) {
            [self.delegate playerDidPause];
        }
    } else {
        [button setSelected:YES];
        [self play];
        
        if ([self.delegate respondsToSelector:@selector(playerDidResume)]) {
            [self.delegate playerDidResume];
        }
    }
    
    [self showControllers];
}

- (void)toggleFullscreen:(UIButton *)button {
    if (self.fullscreen) {
        if ([self.delegate respondsToSelector:@selector(playerWillLeaveFullscreen)]) {
            [self.delegate playerWillLeaveFullscreen];
        }
        
        [UIView animateWithDuration:0.2f animations:^{
            [self setTransform:CGAffineTransformMakeRotation(0)];
            [self setFrame:self.defaultFrame];
            
            CGRect frame = self.defaultFrame;
            frame.origin = CGPointZero;
            [self.playerLayer setFrame:frame];
            [self.activityIndicator setFrame:frame];
        } completion:^(BOOL finished) {
            self.fullscreen = NO;
            
            if ([self.delegate respondsToSelector:@selector(playerDidLeaveFullscreen)]) {
                [self.delegate playerDidLeaveFullscreen];
            }
        }];
        
        [button setSelected:NO];
    } else {
        UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
        
        CGFloat width = [[UIScreen mainScreen] bounds].size.width;
        CGFloat height = [[UIScreen mainScreen] bounds].size.height;
        CGRect frame;
        
        if (UIInterfaceOrientationIsPortrait(orientation)) {
            CGFloat aux = width;
            width = height;
            height = aux;
            frame = CGRectMake((height - width) / 2, (width - height) / 2, width, height);
        } else {
            frame = CGRectMake(0, 0, width, height);
        }
        
        if ([self.delegate respondsToSelector:@selector(playerWillEnterFullscreen)]) {
            [self.delegate playerWillEnterFullscreen];
        }
        
        [UIView animateWithDuration:0.2f animations:^{
            [self setFrame:frame];
            [self.playerLayer setFrame:CGRectMake(0, 0, width, height)];
            
            [self.activityIndicator setFrame:CGRectMake(0, 0, width, height)];
            if (UIInterfaceOrientationIsPortrait(orientation)) {
                [self setTransform:CGAffineTransformMakeRotation(M_PI_2)];
                [self.activityIndicator setTransform:CGAffineTransformMakeRotation(M_PI_2)];
            }
            
        } completion:^(BOOL finished) {
            self.fullscreen = YES;
            
            if ([self.delegate respondsToSelector:@selector(playerDidEnterFullscreen)]) {
                [self.delegate playerDidEnterFullscreen];
            }
        }];
        
        [button setSelected:YES];
    }
    
    [self showControllers];
}

//- (void)seek:(UISlider *)slider {
//    //NSLog(@"in seek---------");
//    int timescale = self.currentItem.asset.duration.timescale;
//    float time = slider.value * (self.currentItem.asset.duration.value / timescale);
//    [self.player seekToTime:CMTimeMakeWithSeconds(time, timescale)];
//    
//    [self showControllers];
//}
//
//- (void)pauseRefreshing {
//    self.seeking = YES;
//    [self pause];
//}
//
//- (void)resumeRefreshing {
//    self.seeking = NO;
//    [self play];
//}



- (void)beginScrubbing:(id)sender{
    
    NSLog(@"beginScrubbing--------");
    
    if([self isPlaying]){
        self.restorePlayStateAfterScrubbing = YES;
        [self.player pause];
        [self.playButton setSelected:NO];
    }
    
    [self stopTimeObserving];
}

- (void)scrub:(id)sender{
    
        //NSLog(@"in seek---------");
    UISlider* slider = sender;
    
    int timescale = self.player.currentItem.asset.duration.timescale;
    float time = slider.value * (self.player.currentItem.asset.duration.value / timescale);
    [self.player seekToTime:CMTimeMakeWithSeconds(time, timescale)];
    
    [self refreshProgressIndicator];

    
    
    
//    UISlider* slider = sender;
//    double duration = self.player.duration;
//    if (isfinite(duration)) {
//        double currentTime = floor(duration * slider.value);
//        double timeLeft = floor(duration - currentTime);
//        
//        if (currentTime <= 0) {
//            currentTime = 0;
//            timeLeft = floor(duration);
//        }
//        
//        [self.playSliderView.leftLabel setText:[NSString stringWithFormat:@"%@ ", [NSString stringFormattedTimeFromSeconds:&currentTime]]];
//        [self.playSliderView.rightLabel setText:[NSString stringWithFormat:@"-%@", [NSString stringFormattedTimeFromSeconds:&timeLeft]]];
//    }
}

- (void)endScrubbing:(id)sender{
    NSLog(@"endScrubbing--------");
    UISlider* slider = sender;
    double duration = self.duration;
    if (isfinite(duration)) {
        double currentTime = floor(duration * slider.value);
        double timeLeft = floor(duration - currentTime);
        
        if (currentTime <= 0) {
            currentTime = 0;
            timeLeft = floor(duration);
        }
        [self seekToTime:currentTime];
    }
    
    
    NSLog(@"restorePlayStateAfterScrubbing--------%d", self.restorePlayStateAfterScrubbing);
    
    
    
    if (self.restorePlayStateAfterScrubbing){
        self.player.rate = 1.0;
        [self.player play];

        [self.playButton setSelected:YES];
        self.restorePlayStateAfterScrubbing = NO;
    }
    
    [self showControllers];
    
    [self startTimeObserving:nil];
}






- (void)refreshProgressIndicator {
    CGFloat duration = CMTimeGetSeconds(self.player.currentItem.asset.duration);
    
    if (duration == 0 || isnan(duration)) {
        // Video is a live stream
        [self.currentTimeLabel setText:nil];
        [self.remainingTimeLabel setText:nil];
        //[progressIndicator setHidden:YES];
        //[liveLabel setHidden:NO];
    }
    
    else {
        
        CGFloat current = self.seeking ? self.progressIndicator.value * duration :   CMTimeGetSeconds(self.player.currentTime);      // If seeking, reflects the position of the slider , Otherwise, use the actual video position
        
        [self.progressIndicator setValue:(current / duration)];
        [self.progressIndicator setSecondaryValue:([self playerItemAvailableDuration] / duration)];
        
        // Set time labels
        NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
        [formatter setDateFormat:(duration >= 3600 ? @"hh:mm:ss": @"mm:ss")];
        [formatter setTimeZone:[NSTimeZone timeZoneWithName:@"UTC"]];
        NSDate *currentTime = [NSDate dateWithTimeIntervalSince1970:current];
        NSDate *remainingTime = [NSDate dateWithTimeIntervalSince1970:(duration - current)];
        
        
        [self.currentTimeLabel setText:[formatter stringFromDate:currentTime]];
        [self.remainingTimeLabel setText:[NSString stringWithFormat:@"-%@", [formatter stringFromDate:remainingTime]]];
        
        [self.progressIndicator setHidden:NO];
        //[liveLabel setHidden:YES];
    }
}

- (void)showControllers {
    [UIView animateWithDuration:0.2f animations:^{
        [self.closeButton setAlpha:1.0f];
        [self.controllersView setAlpha:1.0f];
    } completion:^(BOOL finished) {
        [self.controllersTimer invalidate];
        
        if (self.controllersTimeoutPeriod > 0) {
            self.controllersTimer = [NSTimer scheduledTimerWithTimeInterval:self.controllersTimeoutPeriod
                                                                target:self
                                                              selector:@selector(hideControllers)
                                                              userInfo:nil
                                                               repeats:NO];
        }
    }];
}

- (void)hideControllers {
    [UIView animateWithDuration:0.5f animations:^{
        [self.closeButton setAlpha:0.0f];
        [self.controllersView setAlpha:0.0f];
    }];
}

#pragma mark - Public Methods

- (void)prepareAndPlayAutomatically:(BOOL)playAutomatically {
    
    [self cancelAllAndClearPlayer];
    
    AVURLAsset *asset = [AVURLAsset URLAssetWithURL:self.videoURL options:nil];
   
    
    self.resourceDelegate = [[SvnMediaResourceLoaderDelegate alloc] init];
    
    [asset.resourceLoader setDelegate:self.resourceDelegate queue:dispatch_get_main_queue()];
    
    //[resourceLoader setDelegate:self.resourceDelegate queue:dispatch_queue_create([[videoURL absoluteString] UTF8String], nil)];
    
    AVPlayerItem *item = [AVPlayerItem playerItemWithAsset:asset];
    
    [self addObserversForPlayerItem:item];
    
    [self createPlayerWithItem:item];
    
    //[self notifyDidChangeCurrentItem];
    
    if(playAutomatically)
    {
        [self playIfPossible];
    }
    
    
  
    
//    NSArray *keys = [NSArray arrayWithObject:@"playable"];
//    
//    
//    
//    [asset loadValuesAsynchronouslyForKeys:keys completionHandler:^{
//        
//        NSLog(@"asset now playable...............");
//        
//        currentItem = [AVPlayerItem playerItemWithAsset:self.asset];
//        [player replaceCurrentItemWithPlayerItem:currentItem];
//        
//        if (playAutomatically) {
//            dispatch_sync(dispatch_get_main_queue(), ^{
//                [self play];
//            });
//        }
//    }];
//
//    [player setAllowsExternalPlayback:YES];
   
    
    //[[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
    
    [self.activityIndicator startAnimating];
    
}



- (void)clean {
//    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:nil];
//    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemFailedToPlayToEndTimeNotification object:nil];
//    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemPlaybackStalledNotification object:nil];
//    //    [[NSNotificationCenter defaultCenter] removeObserver:self name:MPVolumeViewWirelessRoutesAvailableDidChangeNotification object:nil];
//    //    [[NSNotificationCenter defaultCenter] removeObserver:self name:MPVolumeViewWirelessRouteActiveDidChangeNotification object:nil];
//    NSLog(@"delegate cancel request");
//    [self.resourceDelegate cancel];
//    
//    [self.player setAllowsExternalPlayback:NO];
//    [self stop];
//    [self.player removeObserver:self forKeyPath:@"rate"];
    
    [self cancelAllAndClearPlayer];

    
    [self removeFromSuperview];
}



#pragma mark - Player Control
- (void)play {
    
    NSLog(@"player to play.............");
    
    //[player play];

    if(self.isPlaying == NO)
    {
        self.pauseReasonForcePause = NO;
        
        self.player.rate = 1.0;
        [self.player play];
        
        [self.playButton setSelected:YES];
        
//        self.progressTimer = [NSTimer scheduledTimerWithTimeInterval:0.1f
//                                                         target:self
//                                                       selector:@selector(refreshProgressIndicator)
//                                                       userInfo:nil
//                                                        repeats:YES];
    }
}

- (void)pause {
    
    if (self.isPlaying) {
        
        self.pauseReasonForcePause = YES;
        
        
        [self.player pause];
        [self.playButton setSelected:NO];
        
        if ([self.delegate respondsToSelector:@selector(playerDidPause)]) {
            [self.delegate playerDidPause];
        }
    }
   
}

- (void)stop {
    
    self.pauseReasonForcePause = YES;
    
    if([self isPlaying]){
        [self pause];
    }
}



- (void)seekToTime:(NSTimeInterval)time{
//    [self performBlockOnMainThreadSync:^{
        @try{[self.player seekToTime:CMTimeMakeWithSeconds(time, NSEC_PER_SEC)];}@catch (NSException *exc) {}
//    }];
}



- (void)playIfPossible{
    if (self.isPlaying==NO && self.pauseReasonForcePause==NO){
        [self play];
    }
}





#pragma mark - Player Info

- (NSTimeInterval)duration{
    return CMTimeGetSeconds([self playerItemDuration]);
}

- (NSTimeInterval)currentTime{
    return CMTimeGetSeconds([self.player currentTime]);
}

- (BOOL)isPlaying{
    if(!self.player)
    {
        return NO;
    }
    return self.player && self.player.rate > 0.f;
}

- (BOOL)isCurrentItemReadyToPlay{
    return (self.player && self.player.currentItem.status==AVPlayerItemStatusReadyToPlay);
}

//- (float)rate{
//    return self.player.rate;
//}

- (float)preloadProgress{
    float progress = 0.0;
    if ([self.player currentItem].status == AVPlayerItemStatusReadyToPlay){
        float durationTime = CMTimeGetSeconds([self playerItemDuration]);
        float bufferTime = [self playerItemAvailableDuration];
        if(durationTime>0.0){
            progress = bufferTime/durationTime;
        }
    }
    return progress;
}


//- (NSTimeInterval)availableDuration {
//    NSTimeInterval result = 0;
//    NSArray *loadedTimeRanges = self.player.currentItem.loadedTimeRanges;
//    
//    if ([loadedTimeRanges count] > 0) {
//        CMTimeRange timeRange = [[loadedTimeRanges objectAtIndex:0] CMTimeRangeValue];
//        Float64 startSeconds = CMTimeGetSeconds(timeRange.start);
//        Float64 durationSeconds = CMTimeGetSeconds(timeRange.duration);
//        result = startSeconds + durationSeconds;
//    }
//    
//    return result;
//}



#pragma mark - AV Player Notifications and Observers

- (void)playerDidFinishPlaying:(NSNotification *)notification {
    [self stop];
    
    if (self.fullscreen) {
        [self toggleFullscreen:self.fullscreenButton];
    }
    
    if ([self.delegate respondsToSelector:@selector(playerDidEndPlaying)]) {
        [self.delegate playerDidEndPlaying];
    }
}

- (void)playerFailedToPlayToEnd:(NSNotification *)notification {
    [self stop];
    
    if ([self.delegate respondsToSelector:@selector(playerFailedToPlayToEnd)]) {
        [self.delegate playerFailedToPlayToEnd];
    }
}

- (void)playerStalled:(NSNotification *)notification {
    
    NSLog(@"playerStalled:%@", notification);
    //[self togglePlay:playButton];
    
    if ([self.delegate respondsToSelector:@selector(playerStalled)]) {
        [self.delegate playerStalled];
        [self continuePlaying];
    }
}


//- (void)airPlayAvailabilityChanged:(NSNotification *)notification {
//  [UIView animateWithDuration:0.4f
//                   animations:^{
//                     if ([volumeView areWirelessRoutesAvailable]) {
//                       [volumeView hideByWidth:NO];
//                     } else if (! [volumeView isWirelessRouteActive]) {
//                       [volumeView hideByWidth:YES];
//                     }
//                     [self layoutIfNeeded];
//                   }];
//}
//

//- (void)airPlayActivityChanged:(NSNotification *)notification {
//    NSLog(@"airPlayActivityChanged----------");
//  [UIView animateWithDuration:0.4f
//                   animations:^{
//                     if ([volumeView isWirelessRouteActive]) {
//                       if (fullscreen)
//                         [self toggleFullscreen:fullscreenButton];
//
//                       [playButton hideByWidth:YES];
//                       [fullscreenButton hideByWidth:YES];
//                       [spacerView hideByWidth:NO];
//
//                         [//airPlayLabel setHidden:NO];
//
//                       controllersTimeoutPeriod = 0;
//                       [self showControllers];
//                     } else {
//                       [playButton hideByWidth:NO];
//                       [fullscreenButton hideByWidth:NO];
//                       [spacerView hideByWidth:YES];
//
//                         //[airPlayLabel setHidden:YES];
//
//                       controllersTimeoutPeriod = 3;
//                       [self showControllers];
//                     }
//                     [self layoutIfNeeded];
//                   }];
//}









#pragma mark - Player State Changing

-(void)continuePlaying
{
    
    if (!self.player.currentItem.playbackLikelyToKeepUp)
    {
        [self.activityIndicator startAnimating];
        
    }
    
    
}

#pragma mark - Player Items Switching

- (void)cancelAllAndClearPlayer{
    [self clearPlayer];
    //[self cancelAllResourceLoaders];
    [self.resourceDelegate cancel];
}

//- (void)fetchAndPlayFileAtURL:(NSURL *)fileURL session:(SvnMediaResourceLoadSession *)session{
//    
//    [self performBlockOnMainThreadSync:^{
//        
//        self.fileURL = fileURL;
//        self.session = session;
//        
//        [self cancelAllAndClearPlayer];
//        
//        AVURLAsset *asset = [AVURLAsset URLAssetWithURL:fileURL options:nil];
//        [asset.resourceLoader setDelegate:self queue:dispatch_get_main_queue()];
//        
//        AVPlayerItem *item = [AVPlayerItem playerItemWithAsset:asset];
//        [self addObserversForPlayerItem:item];
//        
//        [self createPlayerWithItem:item];
//        
//        [self notifyDidChangeCurrentItem];
//        
//        [self playAudioIfPossible];
//    }];
//    
//}



#pragma mark - Player Observers

//
//- (void)updatePlayerControls{
// 
//        if(self.player.isCurrentItemReadyToPlay){
//            switch (self.player.status) {
//                case LSPlayerStatusPlaying:
//                    [self.playerControlsToolbar setItems:@[self.pauseItem]];
//                    break;
//                case LSPlayerStatusPause:
//                    [self.playerControlsToolbar setItems:@[self.playItem]];
//                    break;
//                default:
//                    [self.playerControlsToolbar setItems:@[self.loadingItem]];
//                    break;
//            }
//        }
//        else{
//            [self.playerControlsToolbar setItems:@[self.loadingItem]];
//        }
//    
//}

//- (void)updatePlayerView{
//    [self updatePlaybackProgress];
//    [self syncScrubber];
//    [self syncPlayClock];
//    [self syncPreloadProgress];
//    //[self updatePlayerControls];
//}



#pragma mark - Scrubber control

//- (void)updatePlaybackProgress{
//    if(self.playSliderView.sliderView){
//        self.player.timeObservingPrecision = CGRectGetWidth([self.playSliderView.sliderView bounds]);
//        NSError *err = nil;
//        [self.player startTimeObserving:&err];
//    }
//}
//
//- (void)syncScrubber{
//    double duration = [self.player duration];
//    if (isfinite(duration)) {
//        float minValue = [self.playSliderView.sliderView minimumValue];
//        float maxValue = [self.playSliderView.sliderView maximumValue];
//        double time = self.player.currentTime;
//        [self.playSliderView.sliderView setValue:(maxValue - minValue) * time / duration + minValue];
//    }
//    else{
//        self.playSliderView.sliderView.value = 0.0;
//    }
//}
//
//- (void)syncPlayClock{
//    double duration = [self.player duration];
//    if (isfinite(duration)) {
//        double currentTime = floor(self.player.currentTime);
//        double timeLeft = floor(duration - currentTime);
//        if (currentTime <= 0) {
//            currentTime = 0;
//            timeLeft = floor(duration);
//        }
//        [self.playSliderView.leftLabel setText:[NSString stringWithFormat:@"%@", [NSString stringFormattedTimeFromSeconds:&currentTime]]];
//        [self.playSliderView.rightLabel setText:[NSString stringWithFormat:@"-%@", [NSString stringFormattedTimeFromSeconds:&timeLeft]]];
//    }
//    else{
//        [self.playSliderView.rightLabel setText:@"--:--"];
//        [self.playSliderView.leftLabel setText:@"--:--"];
//    }
//}
//
//- (void)syncPreloadProgress{
//    float progress = [self.player preloadProgress];
//    [self.playSliderView.progressView setProgress:progress animated:NO];
//}



//- (void)enableScrubber{
//    self.playSliderView.sliderView.enabled = YES;
//    self.playSliderView.progressView.alpha = 1.0;
//}
//
//- (void)disableScrubber{
//    self.playSliderView.sliderView.enabled = NO;
//    self.playSliderView.progressView.progress = 0.0;
//    self.playSliderView.progressView.alpha = 0.2;
//}

#pragma mark - LSPlayerDelegate



- (void)startTimeObserving:(NSError **)error{
    if(self.player){
        [self addPlayerTimeObservers:error];
    }
}

- (void)stopTimeObserving{
    if(self.player){
        [self removePlayerTimeObservers];
    }
}

- (void)addPlayerTimeObservers:(NSError **)error{
    
    [self removePlayerTimeObservers];
    
    double interval = .1f;
    
    CMTime playerDuration = [self playerItemDuration];
    if (CMTIME_IS_INVALID(playerDuration)){
        if(error){
            *error = [self errorWithCode:-1 description:nil];
        }
        return;
    }
    double duration = CMTimeGetSeconds(playerDuration);
    if (CMTIME_IS_INDEFINITE(playerDuration) || duration <= 0) {
        if(error){
            *error = [self errorWithCode:-1 description:nil];
        }
        //[self syncPlayClock];
        [self refreshProgressIndicator];
        return;
    }
    
    float precision = self.timeObservingPrecision;
    
    if(precision>0){
        interval = 0.5f * duration / precision;
    }
    
    __weak typeof(self) weakSelf = self;
    self.scrubberTimeObserver = [self.player addPeriodicTimeObserverForInterval:CMTimeMakeWithSeconds(interval, NSEC_PER_SEC)
                                                                          queue:NULL
                                                                     usingBlock:^(CMTime time){
                                                                         //[weakSelf syncScrubber];
                                                                         [weakSelf refreshProgressIndicator];
                                                                     }];
    
    
    self.clockTimeObserver = [self.player addPeriodicTimeObserverForInterval:CMTimeMakeWithSeconds(1, NSEC_PER_SEC)
                                                                       queue:NULL
                                                                  usingBlock:^(CMTime time) {
                                                                      //[weakSelf syncPlayClock];
                                                                  }];
}

- (void)removePlayerTimeObservers{
    if (self.scrubberTimeObserver){
        [self.player removeTimeObserver:self.scrubberTimeObserver];
        self.scrubberTimeObserver = nil;
    }
    if (self.clockTimeObserver){
        [self.player removeTimeObserver:self.clockTimeObserver];
        self.clockTimeObserver = nil;
    }
}



- (void)addObserversForPlayer{
    if(self.player){
        [self.player addObserver:self
                      forKeyPath:kCurrentItemKey
                         options:NSKeyValueObservingOptionInitial | NSKeyValueObservingOptionNew
                         context:NULL];
        [self.player addObserver:self
                      forKeyPath:kStatusKey
                         options:NSKeyValueObservingOptionInitial | NSKeyValueObservingOptionNew
                         context:NULL];
        [self.player addObserver:self
                      forKeyPath:kRateKey
                         options:NSKeyValueObservingOptionInitial | NSKeyValueObservingOptionNew
                         context:NULL];
    }
}

- (void)removeObserversFromPlayer{
    if(self.player){
        [self.player removeObserver:self forKeyPath:kCurrentItemKey];
        [self.player removeObserver:self forKeyPath:kRateKey];
        [self.player removeObserver:self forKeyPath:kStatusKey];
        [self removePlayerTimeObservers];
    }
}

#pragma mark - Player Create/Destroy

- (void)createPlayerWithItem:(AVPlayerItem *)playerItem{
    self.player = [AVPlayer playerWithPlayerItem:playerItem];
    [self addObserversForPlayer];
    
    self.playerLayer = [AVPlayerLayer playerLayerWithPlayer:self.player];
    [self.layer addSublayer:self.playerLayer];
    
    self.defaultFrame = self.frame;
    
    CGRect frame = self.frame;
    frame.origin = CGPointZero;
    [self.playerLayer setFrame:frame];
    
    [self bringSubviewToFront:self.controllersView];
    
    [self bringSubviewToFront:self.closeButton];
    
    
}

- (void)clearPlayer{
    [self stop];
    [self removeObserversFromPlayer];
    [self removeObserversFromPlayerItem:self.player.currentItem];
    self.player = nil;
    self.pauseReasonForcePause = NO;
    
    [self.playerLayer removeFromSuperlayer];
    self.playerLayer = nil;
    
    
}

//- (LSPlayerStatus)status{
//    if ([self isPlaying]){
//        return LSPlayerStatusPlaying;
//    }
//    else if (self.pauseReasonForcePause){
//        return LSPlayerStatusPause;
//    }
//    else{
//        return LSPlayerStatusUnknown;
//    }
//}

#pragma mark - Player Item

- (CMTime)playerItemDuration{
    AVPlayerItem *playerItem = [self.player currentItem];
    if (playerItem.status == AVPlayerItemStatusReadyToPlay){
        return([playerItem duration]);
    }
    return(kCMTimeInvalid);
}

- (double)playerItemAvailableDuration{
    NSArray *loadedTimeRanges = [[self.player currentItem] loadedTimeRanges];
    if ([loadedTimeRanges count] > 0) {
        CMTimeRange timeRange = [[loadedTimeRanges objectAtIndex:0] CMTimeRangeValue];
        double startSeconds = CMTimeGetSeconds(timeRange.start);
        double durationSeconds = CMTimeGetSeconds(timeRange.duration);
        return (startSeconds + durationSeconds);
    } else {
        return 0.0f;
    }
}

- (void)addObserversForPlayerItem:(AVPlayerItem *)item{
    if(item){
        
        NSLog(@"addObserversForPlayerItem");
        [item addObserver:self
               forKeyPath:kStatusKey
                  options:NSKeyValueObservingOptionNew
                  context:NULL];
        
        [item addObserver:self
               forKeyPath:kLoadedTimeRanges
                  options:NSKeyValueObservingOptionNew
                  context:NULL];
        
        

        
        
//        [[NSNotificationCenter defaultCenter] addObserver:self
//                                                 selector:@selector(playerDidFinishPlaying:)
//                                                     name:AVPlayerItemDidPlayToEndTimeNotification
//                                                   object:item];
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerDidFinishPlaying:)
                                                     name:AVPlayerItemDidPlayToEndTimeNotification object:item];
    
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerFailedToPlayToEnd:)
                                                     name:AVPlayerItemFailedToPlayToEndTimeNotification object:item];
    
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerStalled:)
                                                     name:AVPlayerItemPlaybackStalledNotification object:item];
    }
    
    
}

- (void)removeObserversFromPlayerItem:(AVPlayerItem *)item{
    if(item){
         NSLog(@"removeObserversFromPlayerItem");
        [item removeObserver:self forKeyPath:kStatusKey];
        [item removeObserver:self forKeyPath:kLoadedTimeRanges];
        
//        [[NSNotificationCenter defaultCenter] removeObserver:self
//                                                        name:AVPlayerItemDidPlayToEndTimeNotification
//                                                      object:item];
        
        [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:item];
        [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemFailedToPlayToEndTimeNotification object:item];
        [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemPlaybackStalledNotification object:item];
        
    }
}


#pragma mark - Errors

- (NSError *)errorWithCode:(int)code description:(NSString *)errorDescription{
    NSMutableDictionary *errorDict = [NSMutableDictionary dictionary];
    if(errorDescription){
        [errorDict setObject:errorDescription forKey:NSLocalizedDescriptionKey];
    }
    NSError *error = [NSError errorWithDomain:@"" code:code userInfo:errorDict];
    return error;
}

- (NSError *)errorWithCode:(int)code{
    return [self errorWithCode:code description:nil];
}

#pragma mark - Key Value Observing

- (void)observeValueForKeyPath:(NSString*)path
                      ofObject:(id)object
                        change:(NSDictionary*)change
                       context:(void*)context{
    
    if(object==self.player){
        if ([path isEqualToString:kRateKey]){
            float newRate = [[change objectForKey:NSKeyValueChangeNewKey] floatValue];
            BOOL willPlay = (newRate!= 0.f);
            if(willPlay){
                if (self.player.rate > 0) {
                    [self.activityIndicator stopAnimating];
                    
                    
                    //[player play];
                }
                [self addPlayerTimeObservers:nil];
            }
            //[self notifyDidChangeRate:newRate];
        }
        else if ([path isEqualToString:kCurrentItemKey]){
            
            AVPlayerItem *newPlayerItem = [change objectForKey:NSKeyValueChangeNewKey];
            AVPlayerItem *currentAVPlayerItem = newPlayerItem;
            
            if (newPlayerItem == (id)[NSNull null]){
                currentAVPlayerItem = nil;
                [self removePlayerTimeObservers];
            }
            
            BOOL failed = (newPlayerItem != (id)[NSNull null] && newPlayerItem.status == AVPlayerStatusFailed);
            //[self currentAVPlayerItemDidChange:currentAVPlayerItem];
            if(failed){
                //[self currentAVPlayerItemDidFailedWithError:currentAVPlayerItem.error];
            }
        }
        else if ([path isEqualToString:kStatusKey]){
            if (self.player.status == AVPlayerStatusReadyToPlay) {
                //[self notifyDidChangeReadyToPlayStatus:LSPlayerReadyToPlayPlayer];
                [self playIfPossible];
            } else if (self.player.status == AVPlayerStatusFailed) {
                [self stop];
                //[self notifyDidFailWithStatus:LSPlayerFailedPlayer error:self.player.error];
            }
        }
    }
    else if(object==self.player.currentItem){
        
        AVPlayerItem *currentPlayerItem = (AVPlayerItem *)object;
        
        if ([path isEqualToString:kStatusKey]){
            
            AVPlayerStatus status = [[change objectForKey:NSKeyValueChangeNewKey] integerValue];
            
            if(status==AVPlayerStatusReadyToPlay){
                [self addPlayerTimeObservers:nil];
                //[self notifyDidChangeReadyToPlayStatus:LSPlayerReadyToPlayCurrentItem];
                [self playIfPossible];
                
            }
            else if(status==AVPlayerStatusFailed){
                //[self currentAVPlayerItemDidFailedWithError:currentPlayerItem.error];
                if ([self.delegate respondsToSelector:@selector(playerFailedToPlayToEnd)]) {
                    [self.delegate playerFailedToPlayToEnd];
                }
            }
        }
        else if ([path isEqualToString:kLoadedTimeRanges]){
            NSArray *timeRanges = (NSArray *)[change objectForKey:NSKeyValueChangeNewKey];
            if (timeRanges && [timeRanges count]) {
                //[self notifyDidPreLoadCurrentItemWithProgress:[self preloadProgress]];
            }
        }
    }
}


//- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
//    
//    if ([keyPath isEqualToString:@"status"]) {
//        NSLog(@"observeValueForKeyPath status:%d", self.currentItem.status);
//        if (currentItem.status == AVPlayerItemStatusFailed) {
//            if ([delegate respondsToSelector:@selector(playerFailedToPlayToEnd)]) {
//                [delegate playerFailedToPlayToEnd];
//            }
//        }
//    }
//    
//    if ([keyPath isEqualToString:@"rate"]) {
//        NSLog(@"observeValueForKeyPath rate:%f", [player rate]);
//        CGFloat rate = [player rate];
//        if (rate > 0) {
//            [activityIndicator stopAnimating];
//            
//            
//            //[player play];
//        }
//        
//        //              if (self.player.rate == 0 && CMTimeGetSeconds(self.currentItem.duration) != CMTimeGetSeconds(self.currentItem.currentTime))
//        //              {
//        //                  [self continuePlaying];
//        //              }
//    }
//    
//    if ([keyPath isEqualToString:@"playbackLikelyToKeepUp"]) {
//        NSLog(@"observeValueForKeyPath playbackLikelyToKeepUp:%d", self.currentItem.playbackLikelyToKeepUp);
//    }
//}

@end



@implementation UIImage(CustomBundle)
+ (UIImage *)imageNamed:(NSString *)name inBundle:(NSBundle *)bundle ofType:(NSString *)type
{
    int scale = 1;
    if([[UIScreen mainScreen] scale] == 3.0)
    {
        scale = 3;
    }
    else if([[UIScreen mainScreen] scale] == 2.0)
    {
        scale = 2;
    }
    else if([[UIScreen mainScreen] scale] == 1.0)
    {
        scale = 1;
    }
        
    NSString * imagePath = [bundle pathForResource:[NSString stringWithFormat:@"%@@%dx", name, scale]  ofType:type];
    
    UIImage * result;
    
    result = [UIImage imageWithContentsOfFile:imagePath];
    
    if(!result)
    {
        imagePath = [bundle pathForResource:name ofType:type];
        result = [UIImage imageWithContentsOfFile:imagePath];
    }
    
    return result;
}
@end

