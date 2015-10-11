//
//  IDeskFileOutputStream.h
//  iDeskAPI
//
//  Created by yemingxing on 6/23/14.
//  Copyright (c) 2014 www.huawei.com. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SvnFileOutputStream : NSOutputStream

@property (nonatomic, readonly, strong) NSNumber* bytesWritten;

@end
