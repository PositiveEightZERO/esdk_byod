//
//  SvnSdk.h
//  SvnSdk
//
//  Created by l00174413 on 13-6-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SvnSocketInputStream : NSInputStream
{   
    long fd;
}

- (SvnSocketInputStream *) initWithSvnSocketHandle:(long) handle;


@end



@interface SvnSocketOutputStream : NSOutputStream
{
    long fd;
}

- (SvnSocketOutputStream *) initWithSvnSocketHandle:(long) handle;

@end