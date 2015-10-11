//
//  SvnFileStream.h
//
//  Created by lizhiyong on 6/24/15.
//  Copyright (c) 2014 www.huawei.com. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SvnFileInputStream : NSInputStream <NSStreamDelegate>

@property(readonly, strong, nonatomic) NSNumber* totalReaded;
@property(readonly, strong, nonatomic) NSNumber* totalLength;
@property(nonatomic) NSUInteger lengthToRead;

@property(nonatomic) BOOL shouldNotifyCoreFoundationAboutStatusChange;

@end
