//
//  IDeskFileManager.h
//  iDeskAPI
//
//  Created by yemingxing on 6/13/14.
//  Copyright (c) 2014 www.huawei.com. All rights reserved.
//

#import <Foundation/Foundation.h>

//#define MDM_LAST_LOGON_ACCOUNT_KEY     @"com.huawei.lastLogonAccount"

@class SvnFileHandle;

/**
 *	@brief  加密文件管理系统，主要功能包括：读取文件、创建目录、复制文件、移动文件、测试文件是否存在等。它的一切操作都是在MDM安全目录下进行，通过加密文件或目录路径达到信息安全的目的。
 */
@interface SvnFileManager : NSFileManager


/**
 *	@brief	获取根目录
 */
//@property (retain, nonatomic) NSURL* rootURL;
///**
// *	@brief	当前用户
// */
//@property (copy, readonly) NSString* currentUser;
//@property (strong, nonatomic, readonly) NSData* encKey;

/**
 *	@brief	初始化IDeskFileManager ,返回一个实例
 *
 *	@return	IDeskFileManager实例
 */
+(instancetype)defaultManager;

///**
// *	@brief	设置当前用户 此方法必须在使用文件系统之前设置，通常在登录成功后马上调用
// *
// *	@param 	userAccount 	用户名
// *
// *	@return	void
// */
//+ (void)setDefaultUser:(NSString*)userAccount;

/**
 *	@brief	删除用户数据 测试MDM目录下是否存在用户数据，存在则删除
 *
 *	@return	void
 */
//+ (void)eraseUserData;
//
///**
// *	@brief	是否包含子目录  测试MDM目录下是否存在子目录
// *
// *	@param 	path 	目录路径
// *
// *	@return	YES:代表包括 NO:代表不包含
// */
//+ (BOOL)subPathOfContainer:(NSString*)path;


/**
 *	@brief	测试文件是否存在  测试指定路径下文件是否存在
 *
 *	@param 	path 	文件路径
 *
 *	@return	YES:代表存在 NO:代表不存在
 */
- (BOOL)fileExistsAtPath:(NSString*)path;

/**
 *	@brief	测试文件(或目录)是否存在  测试指定路径下是否存在该文件（或目录）
 *
 *	@param 	path 	文件路径
 *	@param 	b 	是否目录  YES:代表是 NO:代表不是
 *
 *	@return	YES:代表存在 NO:代表不存在
 */
- (BOOL)fileExistsAtPath:(NSString*)path isDirectory:(BOOL*)b;

/**
 *	@brief	测试文件是否存在，并且是否能执行读操作  测试指定路径下文件是否存在，并且是否能执行读操作
 *
 *	@param 	path 	文件路径
 *
 *	@return	YES:代表能执行 NO:代表不能执行
 */
- (BOOL)isReadableFileAtPath:(NSString*)path;

/**
 *	@brief	测试文件是否存在，并且是否能执行写操作  测试指定路径下文件是否存在，并且是否能执行写操作
 *
 *	@param 	path 	文件路径
 *
 *	@return	YES:代表能执行 NO:代表不能执行
 */
- (BOOL)isWritableFileAtPath:(NSString*)path;

/**
 *	@brief	查询判断指定文件或目录是否存在
 *
 *	@param 	path 	文件路径
 *
 *	@return	YES:代表存在 NO:代表不存在
 */
- (BOOL)isExecutableFileAtPath:(NSString*)path;

/**
 *	@brief	测试文件是否存在，并且是否能执行删除操作 测试指定路径下文件是否存在，并且是否能执行删除操作
 *
 *	@param 	path 	文件路径
 *
 *	@return	YES:代表能执行 NO:代表不能执行
 */
- (BOOL)isDeletableFileAtPath:(NSString*)path;


/**
 *	@brief 读取数据  从一个文件中读取数据
 *
 *	@param 	path 	文件路径
 *
 *	@return	文件数据，为NSData
 */
- (NSData*)contentsAtPath:(NSString*)path;

/**
 *	@brief 读取数据  从一个目录中读取数据
 *
 *	@param 	path 	文件路径
 *
 *	@return	目录内容，NSArray
 */
- (NSArray*)directoryContentsAtPath: (NSString*)path;
/**
 *	@brief	创建目录 读取指定路径创建目录
 *
 *	@param 	url 	目录路径
 *	@param 	b 	是否需要中间目录
 *	@param 	attributes 	属性
 *	@param 	error 	错误信息
 *
 *	@return	YES: 创建目录成功 NO:创建目录失败
 */
- (BOOL)createDirectoryAtURL:(NSURL*)url withIntermediateDirectories:(BOOL)b attributes:(NSDictionary*)attributes error:(NSError**)error;

/**
 *	@brief	创建目录 读取指定路径创建目录
 *
 *	@param 	path 	目录路径
 *	@param 	attributes 	属性
 *
 *	@return	YES: 创建目录成功 NO:创建目录失败
 */
- (BOOL)createDirectoryAtPath:(NSString*)path attributes:(NSDictionary*)attributes;


/**
 *	@brief	创建目录 读取指定路径创建目录
 *
 *	@param 	path 	目录路径
 *	@param 	attributes 	属性
 *	@param 	error 	错误信息
 *
 *	@return	YES: 创建目录成功 NO:创建目录失败
 */
- (BOOL)createDirectoryAtPath:(NSString*)path attributes:(NSDictionary*)attributes error:(NSError**)error;
/**
 *	@brief	创建目录 读取指定路径创建目录
 *
 *	@param 	path 	目录路径
 *	@param 	b 	是否需要中间目录
 *	@param 	attributes 	属性
 *	@param 	error 	错误信息
 *
 *	@return	YES: 创建目录成功 NO:创建目录失败
 */
- (BOOL)createDirectoryAtPath:(NSString*)path withIntermediateDirectories:(BOOL)b attributes:(NSDictionary*)attributes error:(NSError**)error;

/**
 *	@brief	向一个文件写入数据 向指定路径下的文件写入数据
 *
 *	@param 	path 	文件路径
 *	@param 	data 	文件数据
 *	@param 	attributes 	文件属性
 *
 *	@return	YES:代表写入成功 NO:代表写入失败
 */
- (BOOL)createFileAtPath:(NSString*)path contents:(NSData*)data attributes:(NSDictionary*)attributes;

/**
 *	@brief	删除一个文件  删除指定路径下的文件
 *
 *	@param 	url 	文件当前路径
 *	@param 	error 	错误信息
 *
 *	@return	YES: 删除文件成功 NO:删除文件失败
 */
- (BOOL)removeItemAtURL:(NSURL*)url error:(NSError**)error;

/**
 *	@brief	删除一个文件  删除指定路径下的文件
 *
 *	@param 	path 	文件当前路径
 *	@param 	error 	错误信息
 *
 *	@return	YES: 删除文件成功 NO:删除文件失败
 */
- (BOOL)removeItemAtPath:(NSString*)path error:(NSError**)error;



/**
 *	@brief  复制文件  从一个路径复制文件到另一个路径
 *
 *	@param 	fromURL 	文件当前路径
 *	@param 	toURL 	文件目标路径
 *	@param 	error 	错误信息
 *
 *	@return	YES: 复制文件成功 NO:复制文件失败
 */
- (BOOL)copyItemAtURL:(NSURL*)fromURL toURL:(NSURL*)toURL error:(NSError**)error;

/**
 *	@brief  复制文件  从一个路径复制文件到另一个路径
 *
 *	@param 	fromURL 	文件当前路径
 *	@param 	toURL 	文件目标路径
 *	@param 	error 	错误信息
 *
 *	@return	YES: 复制文件成功 NO:复制文件失败
 */
- (BOOL)copyItemAtPath:(NSString*)fromPath toPath:(NSString*)toPath error:(NSError**)error;

/**
 *	@brief	移动文件  把一个文件从当前路径移动到指定路径
 *
 *	@param 	fromURL 	文件当前路径
 *	@param 	toURL 	文件目标路径
 *	@param 	error 	错误信息
 *
 *	@return	YES:移动文件成功 NO:移动文件失败
 */
- (BOOL)moveItemAtURL:(NSURL*)fromURL toURL:(NSURL*)toURL error:(NSError**)error;

/**
 *	@brief	移动文件  把一个文件从当前路径移动到指定路径
 *
 *	@param 	fromURL 	文件当前路径
 *	@param 	toURL 	文件目标路径
 *	@param 	error 	错误信息
 *
 *	@return	YES:移动文件成功 NO:移动文件失败
 */
- (BOOL)moveItemAtPath:(NSString*)fromPath toPath:(NSString*)toPath error:(NSError**)error;

/**
 *	@brief	获取加密后路径
 *
 *	@param 	fromPath 	文件原路径
 *
 *	@return	文件加密后路径
 */
- (NSString *)GetEncFilePath:(NSString *)fromPath ;

/**
 *	@brief	获取文件属性
 *
 *	@param 	path 	文件路径
 *	@param 	error 	错误信息
 *
 *	@return	一个记录加密文件属性的NSDictionary对象
 */
- (NSDictionary *)attributesOfItemAtPath:(NSString *)path error:(NSError **)error;


///**
// *	@brief	获取是否是加密文件
// *
// *	@param 	path 	文件路径
// *
// *	@return YES:加密文件 NO:非加密文件
// */
//- (BOOL)isEncFile:(NSString *)path;



@end
