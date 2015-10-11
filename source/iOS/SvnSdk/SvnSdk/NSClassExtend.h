//
//  NSClassExtend.h
//  SvnSdkDemo
//
//  Created by l00174413 on 7/25/15.
//
//

#import <Foundation/Foundation.h>

@interface NSData(SvnData)


+ (instancetype)dataWithContentsOfFileSvn:(NSString *)path options:(NSDataReadingOptions)readOptionsMask error:(NSError **)errorPtr;
+ (instancetype)dataWithContentsOfFileSvn:(NSString *)path;

- (instancetype)initWithContentsOfFileSvn:(NSString *)path options:(NSDataReadingOptions)readOptionsMask error:(NSError **)errorPtr;
- (instancetype)initWithContentsOfFileSvn:(NSString *)path;


- (BOOL)writeToFileSvn:(NSString *)path atomically:(BOOL)useAuxiliaryFile;

@end


@interface NSString(SvnString)

+ (instancetype)stringWithContentsOfFileSvn:(NSString *)path encoding:(NSStringEncoding)enc error:(NSError **)error;
+ (instancetype)stringWithContentsOfFileSvn:(NSString *)path usedEncoding:(NSStringEncoding *)enc error:(NSError **)error;


- (instancetype)initWithContentsOfFileSvn:(NSString *)path encoding:(NSStringEncoding)enc error:(NSError **)error;
- (instancetype)initWithContentsOfFileSvn:(NSString *)path usedEncoding:(NSStringEncoding *)enc error:(NSError **)error;




- (BOOL)writeToFileSvn:(NSString *)path atomically:(BOOL)useAuxiliaryFile encoding:(NSStringEncoding)enc error:(NSError **)error;

@end