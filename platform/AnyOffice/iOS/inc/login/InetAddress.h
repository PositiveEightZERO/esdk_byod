//
//  InetAddress.h
//  anyofficesdk
//
//  Created by z00103873 on 14-7-4.
//  Copyright (c) 2014年 pangqi. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface InetAddress : NSObject

@property(strong, nonatomic)NSString* addr;
@property(assign, nonatomic)NSInteger port;


-(NSString *)convertInetAddr;
@end
