//
//  NetStatusManager.h
//  anyofficesdk
//
//  Created by ljj on 14-8-8.
//  Copyright (c) 2014年 pangqi. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "anyoffice_netstatus.h"

@protocol NetChangeCallbackDelegate;


@interface NetStatusManager : NSObject {
    
}

/**
 * 上层设置的网络状态变化的回调代理对象
 */
@property (nonatomic, assign) id<NetChangeCallbackDelegate> statusCallback;

/**
 * 获取网络管理单例
 * @return: NetStatusManager对象
 */
+(NetStatusManager*)getInstance;

/**
 * 初始化方法，需要设置网络状态变化时的回调代理
 * @return: NetStatusManager对象
 */
-(id)initWithDelegate:(id<NetChangeCallbackDelegate>)delegate;


/**
 * 获取应用网络状态：离线，在线，连接中
 * @return:NET_STATUS_EN
 */
-(NET_STATUS_EN)getNetStatus;


/**
 * 网络状态变化回调处理，由C层调用，原生不需要
 */
-(void)procNetChangeNotifyWithOldStatus:(NET_STATUS_EN)oldStatus newStatus:(NET_STATUS_EN)newStatus errCode:(int)error;


//获取设备网络状态
-(NET_TYPE_EN)getCurNetStatus;

//获取SSID
-(NSString *)getWifiSSID;

//开启邮件的VOIP功能
-(void)enableMailVOIP;

//关闭邮件VOIP功能 关闭后邮件在后台时将不会被socket唤醒
-(void)disableMailVOIP;

@end


/**
 * 网络状态变更处理回调定义
 * 定义网络状态变更处理回调形式
 *
 * @author  y90004712
 * @version  [版本号, 2014-7-24]
 * @see  [相关类/方法]
 * @since  [产品/模块版本]
 */

@protocol NetChangeCallbackDelegate <NSObject>

@required

/**
 * 处理回调
 * 网络状态变更处理回调
 * @param oldStatus 原网络状态
 * @param newStatus 当前网络状态
 * @param errCode 网络断开时对应的错误码
 * @see [类、类#方法、类#成员]
 */
-(void)onNetChangedWithOldStatus:(NET_STATUS_EN)oldStatus newStatus:(NET_STATUS_EN)newStatus errCode:(int)error;

@end
