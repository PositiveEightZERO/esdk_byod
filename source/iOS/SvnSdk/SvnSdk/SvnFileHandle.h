//
//  SvnFileHandle.h
//
//  Created by lizhiyong on 6/24/15.
//  Copyright (c) 2014 www.huawei.com. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "svn_define.h"
#import "svn_api.h"
#import "svn_file_api.h"
#import "svn_file_api_ex.h"


typedef SVN_FILE_S* SvnFileDescriptorRef;


/**
 *	@brief	文件操作，主要包括文件的读取、写入
 */
@interface SvnFileHandle : NSFileHandle

@property (readonly) SvnFileDescriptorRef fileDescriptor;

/**
 *	@brief	打开一个文件并准备读取  打开指定路径下的一个文件，并准备读取
 *
 *	@param 	path 	文件路径
 *
 *	@return	一个指定好读操作的SvnFileHandle对象
 */
+ (SvnFileHandle*)fileHandleForReadingAtPath:(NSString*)path;

/**
 *	@brief	打开一个文件并准备读取  打开指定路径下的一个文件，并准备读取
 *
 *	@param 	fileURL 	文件路径
 *	@param 	error 	错误信息
 *
 *	@return	一个指定好写操作的SvnFileHandle对象
 */
+ (SvnFileHandle*)fileHandleForReadingFromURL:(NSURL*)fileURL error:(NSError**)error;

/**
 *	@brief	打开一个文件并准备写入  打开一个指定路径下的文件，并准备往文件下写入数据
 *
 *	@param 	path 	文件路径
 *
 *	@return	一个指定好写操作的SvnFileHandle对象
 */
+ (SvnFileHandle*)fileHandleForWritingAtPath:(NSString*)path;

/**
 *	@brief	打开一个文件并准备写入  打开一个指定路径下的文件，并准备往文件下写入数据
 *
 *	@param 	fileURL 	文件路径
 *	@param 	error 	错误信息
 *
 *	@return	一个指定好写操作的SvnFileHandle对象
 */
+ (SvnFileHandle*)fileHandleForWritingToURL:(NSURL*)fileURL error:(NSError**)error;

- (id)initWithIDeskFileDescriptor:(SvnFileDescriptorRef)fileDescriptor;

/**
 *	@brief	读取其余数据直到文件的末尾  读取文件数据直到文件的末尾 （最多UINT_MAX字节）
 *
 *	@return	文件的数据
 */
- (NSData*)readDataToEndOfFile;

/**
 *	@brief	从文件中读取指定数目bytes的内容
 *
 *	@param 	length 	字节数
 *
 *	@return	文件的数据
 */
- (NSData*)readDataOfLength:(NSUInteger)length;

/**
 *	@brief	写入数据  往SvnFileHandle 指定的文件写入数据
 *
 *	@param 	data 	数据
 *
 *	@return	void
 */
- (void)writeData:(NSData *)data;

/**
 *	@brief	将文件的长度设置为offset字节  将文件的长度设置为offset字节（如果需要，可以填充内容）
 *
 *	@param 	offset 	字节
 *
 *	@return	void
 */
- (void)truncateFileAtOffset:(unsigned long long)offset;

/**
 *	@brief	关闭文件  关闭SvnFileHandle指定路径下的文件
 *
 *	@return	void
 */
- (void)closeFile;

/**
 *	@brief	将当前文件的偏移量定位到文件的末尾
 *
 *	@return	文件偏移量
 */
- (unsigned long long)seekToEndOfFile;

/**
 *	@brief	设置当前文件的偏移量
 *
 *	@param 	offset 	偏移量
 *
 *	@return	void
 */
- (void)seekToFileOffset:(unsigned long long)offset;

/**
 *	@brief	获取当前文件的偏移量
 *
 *	@return	文件的偏移量
 */
- (unsigned long long)offsetInFile;



@end
