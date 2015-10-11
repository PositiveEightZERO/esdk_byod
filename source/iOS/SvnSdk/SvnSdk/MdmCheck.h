//
//  MdmCheck.h
//  MdmCheck
//
//  Created by Lion User on 27/11/2013.
//  Copyright (c) 2013 Lion User. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef struct tagMDMCheckResult
{
    /**
     * MDM检查是否成功."0"：表示查询正常。 其他：表示查询失败.
     */
    int isSuccess;
    
    /** MDM功能是否启用（AnyOffice MDM配置文件已安装）. */
    BOOL isMDMEnabled;
    
    /** 绑定检查结果. */
    int bindResult;
    
    /** 是否root或者越狱. */
    BOOL isRoot;
    
    /** 锁屏密码是否符合安全策略要求. */
    BOOL isPwdCheckOK;
    
    /** 终端安装应用列表是否符合安全策略要求. */
    BOOL isAppCheckOK;
    
    /** 是否长期未登录. */
    BOOL isLongTimeNoLogin;
    
    /** 其他违规检查是否安全策略要求. */
    BOOL isOtherCheckOK;
    
}MDMCheckResult;



//typedef enum
//{
//    /**
//     * 未安装MDM ,前面检测进程或管理员权限只要返回false，都返回0
//     */
//    NULL_INSTALL_MDM = 0,
//    
//    /**
//     * 已经被当前登录用户绑定 （不区分是否开启多用户功能 ，已阅读保密协议：合法用户，可以进入）
//     */
//    ASSERT_BINDED_BY_USER = 1,
//    /**
//     * 不区分是否开启多用户功能：未绑定&正在审批 （待审批设备，不允许进入）
//     */
//    ASSERT_APPROVING = 2,
//    /**
//     * 不区分是否开启多用户功能：超过允许绑定设备数量 （不允许进入）
//     */
//    ASSERT_BYOND_LIMINT = 3,
//    /**
//     * 该设备已被其他用户绑定 （未开启多用户功能： 登录用户和注册用户为不同用户，不允许进入）
//     */
//    ASSERT_BINDED_BY_OTHERS = 4,
//    /**
//     * 不区分是否开启多用户功能：该设备已被其他用户注册, 正在审批中 （待审批设备，不允许进入）
//     */
//    ASSERT_REG_BY_OTHERS = 5,
//    /**
//     * 未绑定&可绑定 （需要进行注册）
//     */
//    ASSERT_NONBINDED = 6,
//    /**
//     * 不区分是否开启多用户功能：资产绑定失败，请联系管理员 （数据库操作失败）
//     */
//    ASSERT_BINDED_FAILED = 7,
//    /**
//     * 注册通过但是未查看过保密协议 （当前登录用户为资产注册人 ，需要阅读保密协议）
//     */
//    ASSERT_AGREEMENT_NOREAD = 8,
//    /**
//     * 注册通过但是未查看过保密协议 （已开启多用户: 当前登录用户非资产注册人 ，不允许进入）
//     */
//    ASSERT_AGREEMENT_NOREAD_OTHERS = 9,
//    /**
//     * 已经被其它用户绑定----- 与ASSERT_BINDED_BY_OTHERS对应 （ 开启多用户功能：合法用户，已阅读保密协议可以进入 ）
//     */
//    ASSERT_BINDED_MULTIUSER = 10,
//    /**
//     * 该设备超期未登录:对于多用户， 没有任何一个用户登录过该设备
//     */
//    ASSERT_LOGON_TIMEOUT = 12,
//    
//    /**
//     * 该设备超期未登录:对于多用户， 没有任何一个用户登录过该设备
//     */
//    ASSERT_UNKNOWN = 65535
//    
//}MDMBindReult;





@interface MdmCheck : NSObject

/*****************************************************************************
 * 描述              ：根据证书，判断MDM配置文件是否安装
 * 本函数调用的函数    ：
 * 调用本函数的函数    ：
 
 * 输入                ：void
 * 输出                ：无
 * 返回                ：1---MDM配置文件已经安装
                        0---MDM配置文件没有安装
 
*****************************************************************************/
+ (NSInteger) isMdmConfigInstalled;

+ (MDMCheckResult) checkMdmSpecific;

@end
