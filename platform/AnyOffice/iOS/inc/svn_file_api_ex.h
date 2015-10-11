/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : svn_file_api_ex.h
  版 本 号   : 初稿
  作    者   : zhaixianqi 90006553
  生成日期   : 2013年4月25日
  最近修改   :
  功能描述   : 文件加解密扩展接口及结构体描述
  函数列表   :
  修改历史   :
  1.日    期   : 2013年4月25日
    作    者   : zhaixianqi 90006553
    修改内容   : 创建文件
  2.日    期   : 2013年3月27日
    作    者   : zhaixianqi 90006553
    修改内容   : 修改FSM_SEEK_E为SVN_SEEK_E

******************************************************************************/

#ifndef __SVN_FILE_API_EX_H__
#define __SVN_FILE_API_EX_H__

#include "svn_file_api.h"

#include <fcntl.h>

/* BEGIN: Modified by zhaixianqi 90006553, for 代码归一，Windows编译 2013/11/8 */
#ifndef WIN32
/* END:   Modified by zhaixianqi 90006553, 2013/11/8 */

#include <sys/mman.h>
#include <sys/stat.h>
#else


#define S_IRWXU  00700  /* 00700 权限，同linux */
#define S_IRUSR  00400  /* 00400 权限，同linux */
#define S_IWUSR  00200  /* 00200 权限，同linux */
#define S_IXUSR  00100  /* 00100 权限，同linux */
             
#define S_IRWXG  00070  /* 00070 权限，同linux */
#define S_IRGRP  00040  /* 00040 权限，同linux */
#define S_IWGRP  00020  /* 00020 权限，同linux */
#define S_IXGRP  00010  /* 00010 权限，同linux */
          
#define S_IRWXO  00007  /* 00007 权限，同linux */
#define S_IROTH  00004  /* 00004 权限，同linux */
#define S_IWOTH  00002  /* 00002 权限，同linux */
#define S_IXOTH  00001  /* 00001 权限，同linux */

#define PROT_READ     0x1         /* 0x1 页内容可读 */
#define PROT_WRITE    0x2         /* 0x2 页内容可写 */
#define PROT_EXEC     0x4         /* 0x4 页内容可执行 */
#define PROT_NONE     0x0         /* 0x0 页内容不可访问 */

#define MAP_SHARED       0x01        /* 0x01 共享映射区，写入相当于输出到文件 */
#define MAP_PRIVATE      0x02        /* 0x02 私有映射，不会影响文件 */

#define MS_ASYNC         1           /* 1    刷新,立即返回 */
#define MS_INVALIDATE    2           /* 2    通知无效 */
#define MS_SYNC          4           /* 4    刷新,等待操作完成返回 */


#endif


#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************
 * 
 * 扩展句柄类型定义
 *
 *********************************************************/
struct tagFSM_FILE_EX;
struct tagFSM_MAPPING;

typedef struct tagFSM_FILE_EX       FSM_FILE_EX_S;          /* 扩展文件句柄 */
typedef struct tagFSM_MAPPING       FSM_MAPPING_S;          /* 映射句柄 */
typedef unsigned long               FSM_HANDLE;             /* 系统句柄值 */



/****************************************************************************
 *
 *  linux打开权限参数
 *
 ***************************************************************************/
typedef enum emFSM_OPENEX_FLAGS
{
    FSM_EX_ONLYREAD = O_RDONLY,             /* 0x0 只读权限 */
    FSM_EX_ONLYWRITE= O_WRONLY,             /* 0x1 只写权限 */
    FSM_EX_RW       = O_RDWR,               /* 0x2 读写权限 */
    FSM_EX_APPEND   = O_APPEND,             /* 0x100 追加模式 */
    FSM_EX_CREATE   = O_CREAT,              /* 0x200 创建模式 */
    FSM_EX_EXCEL    = O_EXCL,               /* 0x1000 如果文件存在，创建失败 */
    FSM_EX_TRUNC    = O_TRUNC               /* 0x2000 如果有写权限，文件清空 */
}FSM_OPENEX_FLAGS_E;


/****************************************************************************
 *
 *  linux打开模式参数 可以位操作
 *
 ***************************************************************************/
typedef enum emFSM_OPENEX_MODE
{
    FSM_S_IRWXU = S_IRWXU,                  /* 00700 权限，同linux */
    FSM_S_IRUSR = S_IRUSR,                  /* 00400 权限，同linux */
    FSM_S_IWUSR = S_IWUSR,                  /* 00200 权限，同linux */
    FSM_S_IXUSR = S_IXUSR,                  /* 00100 权限，同linux */

    FSM_S_IRWXG = S_IRWXG,                  /* 00070 权限，同linux */
    FSM_S_IRGRP = S_IRGRP,                  /* 00040 权限，同linux */
    FSM_S_IWGRP = S_IWGRP,                  /* 00020 权限，同linux */
    FSM_S_IXGRP = S_IXGRP,                  /* 00010 权限，同linux */

    FSM_S_IRWXO = S_IRWXO,                  /* 00007 权限，同linux */
    FSM_S_IROTH = S_IROTH,                  /* 00004 权限，同linux */
    FSM_S_IWOTH = S_IWOTH,                  /* 00002 权限，同linux */
    FSM_S_IXOTH = S_IXOTH,                  /* 00001 权限，同linux */

    /* BEGIN: Added by zhaixianqi 90006553, 2013/11/21   问题单号:OR.SDK.002*/
    FSM_S_WATCHLOG = 0x1000                 /* 不记录监控日志 */
    /* END:   Added by zhaixianqi 90006553, 2013/11/21 */

}FSM_OPENEX_MODE_E;

/****************************************************************************
 *
 *  内存保护标志
 *
 ***************************************************************************/
typedef enum emFSM_PROT
{
    FSM_PROT_READ = PROT_READ,              /* 0x1 页内容可读 */
    FSM_PROT_WRITE= PROT_WRITE,             /* 0x2 页内容可写 */
    FSM_PROT_EXEC = PROT_EXEC,              /* 0x4 页内容可执行 */
    FSM_PROT_NONE = PROT_NONE               /* 0x0 页内容不可访问 */
}FSM_PROT_E;

/****************************************************************************
 *
 *  映射对象类型
 *
 ***************************************************************************/
typedef enum emFSM_MAP_FLAGS
{
    FSM_MAP_SHARED = MAP_SHARED,            /* 0x01 共享映射区，写入相当于输出到文件 */
    FSM_MAP_PRIVATE = MAP_PRIVATE           /* 0x02 私有映射，不会影响文件 */
}FSM_MAP_FLAGS_E;



/****************************************************************************
 *
 *  刷新设置
 *
 ***************************************************************************/
typedef enum emFSM_MS_FLAGS
{
    FSM_MS_ASYNC = MS_ASYNC,                /* 1 刷新,立即返回 */
    FSM_MS_INVAL = MS_INVALIDATE,           /* 2 通知无效 */
    FSM_MS_SYNC = MS_SYNC                   /* 4 刷新,等待操作完成返回 */
}FSM_MS_FLAGS_E;



/*****************************************************************************
 函 数 名  : SVN_API_SetJudgeKeyCallBack
 功能描述  : 设置指定文件夹使用指定密钥
 输入参数  : JudgeKeyCallBack pfJudgeKeyFunction  
 输出参数  : 无
 返 回 值  : unsigned long
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月15日
    作    者   : zhaixianqi 90006553
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long SVN_API_SetJudgeKeyCallBack(JudgeKeyCallBack pfJudgeKeyFunction);


/*****************************************************************************
 函 数 名  : SVN_API_SetJudgePathCallBack
 功能描述  : 设置路径加密判断函数
 输入参数  : JudgePathCallBack pfJudgePathFunction  
 输出参数  : 无
 返 回 值  : unsigned long
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月21日
    作    者   : zhaixianqi 90006553
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long SVN_API_SetJudgePathCallBack(JudgePathCallBack pfJudgePathFunction);





/*****************************************************************************
 函 数 名  : svn_getsize
 功能描述  : 获取文件大小,密文获取对应明文大小
 输入参数  : const char* pcPath                 文件完整路径
 输出参数  : N/A
 返 回 值  : unsigned long                      文件大小,获取失败返回-1
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int svn_getsize(const char* pcPath);


/*****************************************************************************
 函 数 名  : svn_fopen_ex
 功能描述  : linux形式.扩展文件操作.打开文件,获取文件操作句柄
 输入参数  : const char* pcPath                 文件路径,完整绝对路径
             FSM_OPENEX_FLAGS_E enFlag          打开参数
             FSM_OPENEX_MODE_E enMode           文件权限
 输出参数  : 无
 返 回 值  : FSM_FILE_EX_S*                     文件句柄
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
FSM_FILE_EX_S* svn_fopen_ex(const char* pcPath, 
                                            FSM_OPENEX_FLAGS_E enFlag, 
                                            int enMode);

/*****************************************************************************
 函 数 名  : FSM_API_CloseFilEx
 功能描述  : linux形式.扩展文件操作.关闭文件操作句柄,释放资源
 输入参数  : FSM_FILE_EX_S* pstFileexHandle     svn_fopen_ex获得的文件句柄
 输出参数  : 无
 返 回 值  : FSM_BOOL                           操作是否成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int svn_fclose_ex(FSM_FILE_EX_S* pstFileexHandle);

/*****************************************************************************
 函 数 名  : svn_fread_ex
 功能描述  : linux形式.扩展文件操作.读取文件内容
 输入参数  : FSM_FILE_EX_S* pstFileexHandle     svn_fopen_ex获得的文件句柄
             unsigned char* pucOutBuf           外部调用申请的缓存
             unsigned long ulOutBufSize         缓存大小
 输出参数  : 无
 返 回 值  : unsigned long                      成功读取的字节数
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long svn_fread_ex(FSM_FILE_EX_S* pstFileexHandle, 
                                            unsigned char* pucOutBuf, 
                                            unsigned long ulOutBufSize);


/*****************************************************************************
 函 数 名  : svn_fwrite_ex
 功能描述  : linux形式.扩展文件操作.写入文件数据
 输入参数  : FSM_FILE_EX_S* pstFileexHandle     svn_fopen_ex获得的文件句柄
             const unsigned char* pucInBuf      待写入的数据缓存
             unsigned long ulInCount            数据大小
 输出参数  : 无
 返 回 值  : unsigned long                      成功写入的字节数
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long svn_fwrite_ex(FSM_FILE_EX_S* pstFileexHandle, 
                                            const unsigned char* pucInBuf, 
                                            unsigned long ulInDataCount);
                                            
/*****************************************************************************
 函 数 名  : svn_fseek_ex
 功能描述  : linux形式.扩展文件操作.定位文件游标
 输入参数  : FSM_FILE_EX_S* pstFileexHandle     svn_fopen_ex获得的文件句柄
             long lOffset                       游标偏移量
             SVN_SEEK_E enOrigin                游标偏移初始位置
 输出参数  : 无
 返 回 值  : unsigned long                      当前游标位置
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int svn_fseek_ex(FSM_FILE_EX_S* pstFileexHandle, 
                                            long lOffset, 
                                            SVN_SEEK_E enOrigin);
                                            
/*****************************************************************************
 函 数 名  : svn_ftell_ex
 功能描述  : linux形式.扩展文件操作.获取当前文件游标
 输入参数  : FSM_FILE_EX_S* pstFileexHandle     svn_fopen_ex获得的文件句柄
 输出参数  : 无
 返 回 值  : unsigned long                      当前游标位置
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
long svn_ftell_ex(FSM_FILE_EX_S* pstFileexHandle);

/*****************************************************************************
 函 数 名  : svn_mmap
 功能描述  : linux形式.扩展文件操作.映射文件内容到内存,获取操作句柄
 输入参数  : void* pvReserved                   保留
             unsigned long ulSize               映射区大小
             FSM_PROT_E enProt                  映射区保护模式
             FSM_MAP_FLAGS_E enFlags            映射区访问模式
             unsigned long ulOffset             映射区偏移
             FSM_FILE_EX_S* pstFileexHandle     映射文件的文件句柄,svn_fopen_ex获得的文件句柄
 输出参数  : 无
 返 回 值  : FSM_MAPPING_S*                     映射句柄
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
FSM_MAPPING_S* svn_mmap(        void* pvReserved, 
                                    unsigned long ulSize, 
                                    FSM_PROT_E enProt, 
                                    FSM_MAP_FLAGS_E enFlags, 
                                    unsigned long ulOffset, 
                                    FSM_FILE_EX_S* pstFileexHandle);

                                    
/*****************************************************************************
 函 数 名  : svn_msync
 功能描述  : linux形式.扩展文件操作.刷新映射区
 输入参数  : FSM_MAPPING_S* pstAddr             映射句柄
             unsigned long ulSize               刷新字节数
             FSM_MS_FLAGS_E enFlags             刷新参数
 输出参数  : 无
 返 回 值  : FSM_BOOL                           操作是否成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int svn_msync(FSM_MAPPING_S* pstMapHandle, 
                                    unsigned long ulSize, 
                                    FSM_MS_FLAGS_E enFlags);

/*****************************************************************************
 函 数 名  : svn_munmap
 功能描述  : linux形式.扩展文件操作.取消映射区
 输入参数  : FSM_MAPPING_S* pstAddr             映射句柄
             unsigned long ulSize               字节数
 输出参数  : 无
 返 回 值  : FSM_BOOL                           操作是否成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int svn_munmap(FSM_MAPPING_S* pstMapHandle, unsigned long ulSize);

/*****************************************************************************
 函 数 名  : FSM_API_GetOperator
 功能描述  : 从映射句柄中获取内存操作指针
 输入参数  : FSM_MAPPING_S* pstAddr             映射句柄
 输出参数  : 无
 返 回 值  : void*                              内存操作指针
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
void* svn_getmapoperator(FSM_MAPPING_S* pstAddr);


/*****************************************************************************
 函 数 名  : svn_gethandle_ex
 功能描述  : linux形式.扩展文件操作.获取系统文件操作句柄
 输入参数  : const FSM_FILE_EX_S* pstFileexHandle   文件句柄
 输出参数  : 无
 返 回 值  : FSM_HANDLE                             系统文件句柄,如int, FILE*, HANDLE
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
FSM_HANDLE svn_gethandle_ex(const FSM_FILE_EX_S* pstFileexHandle);

/*****************************************************************************
 函 数 名  : svn_ftruncate_ex
 功能描述  : 修改文件大小
 输入参数  : FSM_FILE_EX_S* pstFileHandle          svn_fopen获得的文件句柄
             unsigned long lNewSize             新的文件长度
 输出参数  : 无
 返 回 值  : FSM_BOOL                           成功失败
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int svn_ftruncate_ex(FSM_FILE_EX_S* pstFileexHandle, unsigned long ulNewSize);


#ifdef __cplusplus
}
#endif

#endif /* __SVN_FILE_API_EX_H__ */
