//
//  Person.h
//  SvnSdkDemo
//
//  Created by l00174413 on 14-6-12.
//
//

#import <Foundation/Foundation.h>

@interface Person : NSObject

@property NSInteger pid;
@property NSString* name;
@property NSInteger age;

-(id)initWithName:(NSString *)name age:(NSString *)age;


@end
