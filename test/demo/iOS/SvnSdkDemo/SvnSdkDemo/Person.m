

//
//  Person.m
//  SvnSdkDemo
//
//  Created by l00174413 on 14-6-12.
//
//

#import "Person.h"

@implementation Person


-(id)initWithName:(NSString *)name age:(NSString *)age
{
    self = [super init];
    if(self)
    {
        self.name = name;
        self.age = age;
    }
	return self;
}
@end
