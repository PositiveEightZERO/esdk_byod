/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : mdmsdk.c
  版 本 号   : 初稿
  作    者   : kongsheng
  生成日期   : 2013年12月6日
  最近修改   :
  功能描述   : 资产管理模块相关函数
  函数列表   :
  修改历史   :
  1.日    期   : 2013年12月6日
    作    者   : kongsheng
    修改内容   : 创建文件

******************************************************************************/
#ifndef __MDMSDK_API_H__
#define __MDMSDK_API_H__

typedef enum tagAssert_Bind_Result
{
    ASSERT_BINDED_BY_USER = 1,     /* 已经被当前登录用户绑定
                        （不区分是否开启多用户功能，已阅读保密协议：合法用户，可以进入） */
    ASSERT_APPROVING = 2,                   /*不区分是否开启多用户功能：未绑定&正在审批
                        （待审批设备，不允许进入） */
    ASSERT_BYOND_LIMINT = 3,               /*不区分是否开启多用户功能：超过允许绑定设备数量
                         （不允许进入）*/
    ASSERT_BINDED_BY_OTHERS = 4,       /*   该设备已被其他用户绑定
                         （未开启多用户功能：登录用户和注册用户为不同用户，不允许进入） */
    ASSERT_REG_BY_OTHERS = 5,           /* 不区分是否开启多用户功能：该设备已被其他用户注册,正在审批中
                        （待审批设备，不允许进入） */
    ASSERT_NONBINDED = 6,                  /*  未绑定&可绑定
                        （需要进行注册）  */
    ASSERT_BINDED_FAILED = 7,             /* 不区分是否开启多用户功能：资产绑定失败，请联系管理员（数据库操作失败） */
    ASSERT_AGREEMENT_NOREAD = 8,     /*  注册通过但是未查看过保密协议
                  （当前登录用户为资产注册人，需要阅读保密协议） */
     ASSERT_AGREEMENT_NOREAD_OTHERS = 9,/*  注册通过但是未查看过保密协议
                   （已开启多用户:当前登录用户非资产注册人，不允许进入） */
     ASSERT_BINDED_MULTIUSER = 10,  /* 已经被其它用户绑定-----与ASSERT_BINDED_BY_OTHERS对应
                   （开启多用户功能：合法用户，已阅读保密协议可以进入） */
     ASSERT_LOGON_TIMEOUT = 12,        /* 该设备超期未登录:对于多用户，没有任何一个用户登录过该设备 */
    /* BEGIN: Added by hexinxin, 2014/2/17   PN:迭代八需求分级管理*/
    ASSERT_REJECT_APPROVE = 13 /*管理员拒绝审批*/
    /* END:   Added by hexinxin, 2014/2/17 */
} Assert_Bind_Result_E;

#define MDMSDK_KICKOFF             15
#define MDMSDK_NO_CHECKRESULT      16
#define MDMDSDK_UNKOWN_ERROR       0xFFFF


unsigned long SVN_API_CheckBind( CHAR* pszInBuff,  LONG lInBuffLen);

/* BEGIN:   Added for PN:DTS2014041602406 by LiJingjin 90004727, 2014/4/16 */
int SVN_API_GetMdmViolationResult(char **ppcMdmCheckResult, unsigned long *pulOutLen);
/* END:   Added for PN:DTS2014041602406 by LiJingjin 90004727, 2014/4/16 */

#endif
