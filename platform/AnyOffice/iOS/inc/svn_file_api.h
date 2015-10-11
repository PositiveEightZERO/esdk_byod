/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : svn_file_api.h
  版 本 号   : 初稿
  作    者   : zhaixianqi 90006553
  生成日期   : 2013年3月22日
  最近修改   :
  功能描述   : 文件加解密模块的接口函数及结构体
  函数列表   :
*
*

  修改历史   :
  1.日    期   : 2013年3月22日
    作    者   : zhaixianqi 90006553
    修改内容   : 创建文件
  2.日    期   : 2013年3月27日
    作    者   : zhaixianqi 90006553
    修改内容   : 修改FSM_SEEK_E为SVN_SEEK_E

******************************************************************************/

#ifndef __SVN_FILE_API_H__
#define __SVN_FILE_API_H__


#ifdef __cplusplus
extern "C"
{
#endif



/****************************************************************************
 *
 *  简单常量定义
 *
 ***************************************************************************/
#define PATH_LEN        (512)       /* 目录长度 */
#define KEY_LEN     (64)        /* 密钥长度 */
/* BEGIN: Added by liugang 90004755, 2013/11/27   PN:迭代七需求 SDK应用数据擦除*/
#define IDENTIFIER_LEN  (256)       /*应用包名长度*/
#define SANDBOX_PATH_PREFIX     "/mnt/sdcard/sandbox"       /*沙盒路径前缀*/
/* END:   Added by liugang 90004755, 2013/11/27 */

#ifndef SVN_F_OK
#  define SVN_F_OK 0
#  define SVN_X_OK 1
#  define SVN_W_OK 2
#  define SVN_R_OK 4
#endif


/****************************************************************************
 *
 *  目录类型
 *
 ***************************************************************************/
typedef enum emSVN_DIR_TYPE
{
    SVN_TYPE_DIR = 0,           /* 目录 */
    SVN_TYPE_FILE,              /* 文件 */
    SVN_TYPE_UNKNOWN            /* 未定义 */
}SVN_DIR_TYPE_E;


/****************************************************************************
 *
 *  目录信息
 *
 ***************************************************************************/
typedef struct tagSVN_DIRINFO
{
    char            acDirName[PATH_LEN];    /* 目录名称 */
    SVN_DIR_TYPE_E  enType;                 /* 目录类型 */
}SVN_DIRINFO_S;


typedef enum enEncryptFlag
{
    E_ENCRYPT_NOSEC = 0,        /* 不加密 */
    E_ENCRYPT_DEFAULTSEC = -1,  /* 采用默认密钥加密 */
    E_ENCRYPT_ASSIGNKEY = 1,    /* 采用指定密钥加密*/
} ENCRYPT_FLAG_E;

typedef struct tagSvnKeyInfo
{
    ENCRYPT_FLAG_E enEncrypt; /* 加解密密钥标识 */
    unsigned char aucAbsKey[KEY_LEN]; /* 密钥内容 */
}SVN_KEY_INFO_S;


typedef enum enSvnEncMode
{
    E_SVN_ENC_MODE_STEADYKEY = 0, /* 表示固定密钥*/
    E_SVN_ENC_MODE_ONLINEKEY = 1,/* 表示在线密钥*/
}SVN_ENC_MODE_E;



typedef struct tagSvnLoginInfo
{
    unsigned long ulIp;                     /* 网关IP */
    unsigned short usPort;                  /* 网关端口 */
    unsigned short usUserId;                /* 登录UserID */
    unsigned short usSessionId;             /* 登录SessionID */
}SVN_LOGIN_INFO_S;



/****************************************************************************
 *
 *  文件指针定位类型
 *
 ***************************************************************************/
typedef enum emSVN_SEEK
{
    SVN_SEEK_SET = 0,           /* 文件起始位置 */
    SVN_SEEK_CUR,               /* 文件当前位置 */
    SVN_SEEK_END                /* 文件末尾位置 */
}SVN_SEEK_E;


struct tagSVN_FILE;
struct tagSVN_DIR;


typedef struct tagSVN_FILE          SVN_FILE_S;             /* 文件句柄 */
typedef struct tagSVN_DIR           SVN_DIR_S;              /* 目录句柄 */



/****************************************************************************
 *
 *  错误代码
 *
 ***************************************************************************/
typedef enum emSVN_ERRNO
{
    SVN_E_UNAPPAUTH = -1002, /* 应用未校验 */
    SVN_E_UNKNOWN = -1000,
    SVN_E_NOTEXIST = -32,         /*文件不存在*/
    SVN_E_UNSUPORTFILE = -31,     /*打开文件时，出现不识别的文件*/					
    SVN_E_UPGRADEERR = -30,     /*升级异常*/		
    SVN_E_EXSISTSAMEFILE_APPEND = -29,     /*追加模式下已存在明文同名文件*/		
    SVN_E_NOINIT = -28,     /*未初始*/
    SVN_E_FILEPATH = -27,   /* 不再工作路径 */
    SVN_E_DIRTYPE = -26,
    SVN_E_CALLDLL = -25,
    SVN_E_DLLFUNC = -24,
    SVN_E_DLLOPEN = -23,
    SVN_E_OVERRECU = -22,
    SVN_E_OVERDEEP = -21,
    SVN_E_GETDECKEY = -20,
    SVN_E_GETENCKEY = -19,
    SVN_E_GETKEY = -18,
    SVN_E_DECRYPT = -17,
    SVN_E_DECRYPTINIT = -16,
    SVN_E_ENCRYPT = -15,
    SVN_E_ENCRYPTINIT = -14,
    SVN_E_NOALG = -13,
    SVN_E_B64ENCODE = -12,
    SVN_E_PARAM = -11,          /* 参数异常 */
    SVN_E_ENCFILE = -10,
    SVN_E_FILEPOINT = -9,
    SVN_E_OUTOFFILE = -8,
    SVN_E_INPUT = -7,           /* 参数输入有误 */
    SVN_E_EMPTYDRIVER = -6,
    SVN_E_OUTOFMEMORY = -5,
    SVN_E_CHECK = -4,
    SVN_E_NOTAIL = -3,
    SVN_E_ACCESS = -2,
    SVN_E_FAIL = -1,
    SVN_S_OK = 0,
    SVN_S_EXIST = 1,
    SVN_S_EOF = 2,
    SVN_S_NOSECURITY = 3,
    SVN_S_NOUPDATE = 4,
    SVN_S_NOFILENAME = 5,
    SVN_S_SPECIALFILE = 6,    
}SVN_ERRNO_E;

/****************************************************************************
 *
 *  文件或目录详细信息
 *
 ***************************************************************************/
typedef struct tagSVN_STAT
{
    unsigned short ulMode;          /* 文件权限 */
    unsigned long ulCreateTime;     /* 文件创建时间 */
    unsigned long ulModifyTime;     /* 文件修改时间 */
    unsigned long ulAccessTime;     /* 文件最后访问时间 */
    unsigned long ulSize;           /* 文件大小 */
}SVN_STAT_S;



/* 设置指定目录指定密钥的函数类型 */
typedef long (*JudgeKeyCallBack)(const char* pcFolder, SVN_KEY_INFO_S* pstKeyInfo);


/* 路径加密判断函数类型 */
typedef char* (*JudgePathCallBack)(const char* pcFolder, unsigned long ulFolderLength);


/****************************************************************************
 *
 *  初始化操作函数
 *
 ***************************************************************************/
/*****************************************************************************
 函 数 名  : SVN_API_FileEncInitEnv
 功能描述  : 初始化工作目录，初始化加解密模块
 输入参数  : const char* pcWorkPath
 输出参数  : 无
 返 回 值  : unsigned long
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月15日
    作    者   : zhaixianqi 90006553
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long SVN_API_FileEncInitEnv (const char* pcWorkPath);

/* BEGIN: Added by liugang 90004755, 2013/11/27   PN:迭代七需求 SDK应用数据擦除*/
/*****************************************************************************
 Prototype    : SVN_API_FileInitSandbox
 Description  : 当FSM模块初始化应用包名不为空的时候，初始化应用沙盒路径
 Input        : None
 Output       : None
 Return Value : unsigned
 Calls        :
 Called By    :

  History        :
  1.Date         : 2013/11/28
    Author       : liugang 90004755
    Modification : Created function

*****************************************************************************/
/* BEGIN: Added by liugang 90004755, 2013/12/12   PN:DTS2013121201292 【B050-3】安全SDK数据擦除，iOS不能使用沙盒初始化擦除相关接口，需要按平台屏蔽接口*/
/* 只有Android 需要沙盒初始化清除相关接口:SVN_API_FileInitSandbox、SVN_API_FileEraseSandboxFile、SVN_API_FileClearSandbox*/
#if (VOS_OS_VER == VOS_ANDROID)
/* END:   Added by liugang 90004755, 2013/12/12 */
unsigned long SVN_API_FileInitSandbox(const char* pcIdentifier);

/*****************************************************************************
 Prototype    : SVN_API_FileEraseSandboxFile
 Description  : 清理沙盒路径应用包名目录下的所有文件和目录，包括应用包名目录
                。
 Input        : const char* pcIdentifier
 Output       : None
 Return Value : unsigned
 Calls        :
 Called By    :

  History        :
  1.Date         : 2013/12/5
    Author       : liugang 90004755
    Modification : Created function

*****************************************************************************/
unsigned long SVN_API_FileEraseSandboxFile(const char* pcIdentifier);

/*****************************************************************************
 Prototype    : SVN_API_FileClearSandbox
 Description  : 根据SDK应用包名来删除应用的SDK数据
 Input        : None
 Output       : None
 Return Value : unsigned
 Calls        :
 Called By    :

  History        :
  1.Date         : 2013/11/30
    Author       : liugang 90004755
    Modification : Created function

*****************************************************************************/
unsigned long SVN_API_FileClearSandbox();
/* BEGIN: Added by liugang 90004755, 2013/12/12   PN:DTS2013121201292 【B050-3】安全SDK数据擦除，iOS不能使用沙盒初始化擦除相关接口，需要按平台屏蔽接口*/
#endif
/* END:   Added by liugang 90004755, 2013/12/12 */
/*****************************************************************************
 Prototype    : SVN_API_FileCleanDir
 Description  : 使用Libc库重命名文件夹并且删除。
 Input        : const char* pcDir
 Output       : None
 Return Value : unsigned
 Calls        :
 Called By    :

  History        :
  1.Date         : 2013/11/30
    Author       : liugang 90004755
    Modification : Created function

*****************************************************************************/
unsigned long SVN_API_FileCleanDir(const char* pcDir);
/* END:   Added by liugang 90004755, 2013/11/27 */

typedef  unsigned long (*funtypeSVN_API_WriteDeviceInfoGroupItem)(char *pcItemName, char *pcItemValue);
typedef  unsigned long (*funtypeSVN_API_ReadDeviceInfoGroupItem)(char *pcItemName, char **ppcItemValue);
typedef  char * (*funtypeSVN_API_GetKeySeedFromKeychain)();
void SVN_API_SetCallbackWriteDeviceInfoGroupItem(funtypeSVN_API_WriteDeviceInfoGroupItem callback);
void SVN_API_SetCallbackReadDeviceInfoGroupItem(funtypeSVN_API_ReadDeviceInfoGroupItem callback);
void SVN_API_SetCallbackGetKeySeedFromKeychain(funtypeSVN_API_GetKeySeedFromKeychain callback);


/*****************************************************************************
 函 数 名  : SVN_API_SetSteadyKey
 功能描述  : 设置固定密钥种子参数
 输入参数  :
                const char* pcUserName : 用户名
                const char* pcDeviceID : 设备ID

 输出参数  : 无
 返 回 值  : unsigned long
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月7日
    作    者   : zhaixianqi 90006553
    修改内容   : 新生成函数

*****************************************************************************/
long SVN_API_SetSteadyKey( const char* pcUserName, const char* pcDeviceID );

/*****************************************************************************
 函 数 名  : SVN_API_FileEncCleanEnv
 功能描述  : 去初始化文件夹解密环境
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月20日
    作    者   : zhaixianqi 90006553
    修改内容   : 新生成函数

*****************************************************************************/
void SVN_API_FileEncCleanEnv ();


/*****************************************************************************
 函 数 名  : SVN_API_SetFileEncMode
 功能描述  : 设置文件加解密密钥策略
 输入参数  : SVN_ENC_MODE_E enMode
 输出参数  : 无
 返 回 值  : unsigned long
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月21日
    作    者   : zhaixianqi 90006553
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long SVN_API_SetFileEncMode(SVN_ENC_MODE_E enMode, SVN_LOGIN_INFO_S *pstLoginInfo);



/*****************************************************************************
 函 数 名  : SVN_API_GetEncFilePath
 功能描述  : 获取加密后的文件路径
 输入参数  : const char* pcPath
             char* pcEncPath
             unsigned long ulEncPathLen
 输出参数  : 无
 返 回 值  : unsigned long
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月26日
    作    者   : zhaixianqi 90006553
    修改内容   : 新生成函数

*****************************************************************************/
long SVN_API_GetEncFilePath(const char* pcPath, char* pcEncPath, unsigned long ulEncPathLen);


/****************************************************************************
 *
 *  libc形式文件操作函数
 *
 ***************************************************************************/
 /*****************************************************************************
 函 数 名  : svn_fopen
 功能描述  : 标准c形式.打开文件,获取文件句柄
 输入参数  : const char* pcPath                 文件路径
             const char* pcMode                 文件打开模式
 输出参数  : 无
 返 回 值  : SVN_FILE_S*                        文件操作句柄
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
SVN_FILE_S*     svn_fopen(
                        const char*             pcPath,
                        const char*             pcMode);


/*****************************************************************************
 函 数 名  : svn_fclose
 功能描述  : 标准c形式.关闭文件句柄,清理资源
 输入参数  : SVN_FILE_S* pstFileHandle          文件操作句柄
 输出参数  : 无
 返 回 值  : FSM_BOOL                           成功/失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_fclose(SVN_FILE_S*             pstFileHandle);


/*****************************************************************************
 函 数 名  : svn_fread
 功能描述  : 标准c形式.读取文件内容
 输入参数  : unsigned char* pucOutBuf           外部调用申请的缓存
             unsigned long ulOutBufSize         缓存大小
             SVN_FILE_S* pstFileHandle          FSM_API_OpenFile获得的文件句柄
 输出参数  : 无
 返 回 值  : unsigned long                      成功读取的字节数
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long   svn_fread(
                        unsigned char*          pucOutBuf,
                        unsigned long           ulSize,
                        unsigned long           ulCount,
                        SVN_FILE_S*             pstFileHandle);

unsigned long  svn_readcert(const unsigned char * pcPath, unsigned char ** pucOutBuf,unsigned long *  ulSize);
/*****************************************************************************
 函 数 名  : svn_readline
 功能描述  : 标准c形式.读取文件一行内容
 输入参数  : unsigned char* pucOutBuf           外部调用申请的缓存
             unsigned long ulOutBufSize         缓存大小
             SVN_FILE_S* pstFileHandle          FSM_API_OpenFile获得的文件句柄
 输出参数  : 无
 返 回 值  : unsigned long                      成功读取的字节数
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long   svn_readline(
                        unsigned char*          pucOutBuf,
                        unsigned long           ulOutBufSize,
                        SVN_FILE_S*             pstFileHandle);


/*****************************************************************************
 函 数 名  : svn_fwrite
 功能描述  : 标准c形式.写入文件内容
 输入参数  : const unsigned char* pucInBuf      待写入的数据缓存
             unsigned long ulInCount            数据缓存大小
             SVN_FILE_S* pstFileHandle          FSM_API_OpenFile获得的文件句柄
 输出参数  : 无
 返 回 值  : unsigned long                      成功写入的字节数
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long svn_fwrite(
                        const unsigned char*    pucInBuf,
                        unsigned long           ulSize,
                        unsigned long           ulCount,
                        SVN_FILE_S*             pstFileHandle);


/*****************************************************************************
 函 数 名  : svn_fseek
 功能描述  : 标准c形式.定位文件游标
 输入参数  : SVN_FILE_S* pstFileHandle          FSM_API_OpenFile获得的文件句柄
             long lOffset                       游标偏移量
             SVN_SEEK_E enOrigin                游标初始偏移位置
 输出参数  : 无
 返 回 值  : FSM_BOOL                           操作是否成功
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_fseek(
                        SVN_FILE_S*             pstFileHandle,
                        long                    lOffset,
                        SVN_SEEK_E              enOrigin);


/*****************************************************************************
 函 数 名  : svn_ftell
 功能描述  : 标准c形式.获取当前函数游标位置
 输入参数  : SVN_FILE_S* pstFileHandle          FSM_API_OpenFile获得的文件句柄
 输出参数  : 无
 返 回 值  : unsigned long                      当前游标偏移
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
long   svn_ftell(
                        SVN_FILE_S*             pstFileHandle);


/*****************************************************************************
 函 数 名  : svn_ftruncate
 功能描述  : 修改文件大小
 输入参数  : SVN_FILE_S* pstFileHandle          FSM_API_OpenFile获得的文件句柄
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
int        svn_ftruncate(
                        SVN_FILE_S*             pstFileHandle,
                        unsigned long           lNewSize);


/****************************************************************************
 *
 *  普通目录操作函数
 *
 ***************************************************************************/
/*****************************************************************************
 函 数 名  : svn_mkdir
 功能描述  : 标准c形式.创建目录
 输入参数  : const char* pcFolder                 目录完整绝对路径
 输出参数  : 无
 返 回 值  : FSM_BOOL                           操作是否成功
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_mkdir(
                        const char*             pcFolder);


/*****************************************************************************
 函 数 名  : svn_rename
 功能描述  : 标准c形式.重命名目录或文件
 输入参数  : const char* pcOldFolder              旧目录完整绝对路径
             const char* pcNewFolder              新目录完整绝对路径
 输出参数  : 无
 返 回 值  : FSM_BOOL                           操作是否成功
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_rename(
                        const char*             pcOldFolder,
                        const char*             pcNewFolder);


/*****************************************************************************
 函 数 名  : svn_remove
 功能描述  : 标准c形式.删除目录或文件
 输入参数  : const char* pcPath                 目录完整绝对路径
 输出参数  : 无
 返 回 值  : FSM_BOOL                           操作是否成功
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_remove(
                        const char*             pcPath);


/*****************************************************************************
 函 数 名  : svn_access
 功能描述  : 查询判断指定文件或目录是否存在
 输入参数  : const char* pcPath                 完整绝对路径
 输出参数  : 无
 返 回 值  : FSM_BOOL                           是否存在
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_access(
                        const char*             pcPath, int iMode);

/*****************************************************************************
 函 数 名  : svn_isencfile
 功能描述  : 查询判断指定文件是否为加密文件
 输入参数  : const char* pcPath                 完整绝对路径
 输出参数  : 无
 返 回 值  : 
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_isencfile(
                        const char* pcPath,int *pbIsEnc);

/****************************************************************************
 *
 *  枚举目录函数
 *
 ***************************************************************************/
/*****************************************************************************
 函 数 名  : svn_opendir
 功能描述  : linux形式.获取目录操作句柄
 输入参数  : const char* pcFolder                 完整绝对路径
 输出参数  : 无
 返 回 值  : SVN_DIR_S*                         目录句柄,用于后续FSM_API_ReadDir操作
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
SVN_DIR_S*      svn_opendir(
                        const char*             pcFolder);

/*****************************************************************************
 函 数 名  : svn_readdir
 功能描述  : linux形式.读取当前目录下的下一个子目录信息
 输入参数  : SVN_DIR_S* pstDirHandle            目录句柄,通过FSM_API_OpenDir获取
 输出参数  : 无
 返 回 值  : SVN_DIRINFO_S*                     当前子目录信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
SVN_DIRINFO_S*  svn_readdir(
                        SVN_DIR_S*              pstDirHandle);


/*****************************************************************************
 函 数 名  : svn_closedir
 功能描述  : linux形式.关闭目录操作句柄
 输入参数  : SVN_DIR_S* pstDirHandle            目录句柄
 输出参数  : 无
 返 回 值  : FSM_BOOL                           操作是否成功
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_closedir(
                        SVN_DIR_S*              pstDirHandle);


/****************************************************************************
 *
 *  其他
 *
 ***************************************************************************/
/*****************************************************************************
 函 数 名  : svn_file_geterrno
 功能描述  : 获取错误代码
 输入参数  : 无
 输出参数  : 无
 返 回 值  : SVN_ERRNO_E
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
SVN_ERRNO_E     svn_file_geterrno();


/****************************************************************************
 *
 *  扩展功能
 *
 ***************************************************************************/
/*****************************************************************************
 函 数 名  : svn_mkdir_ex
 功能描述  : 创建完整目录,循环创建不存在的目录
 输入参数  : const char* pcPath                 文件完整路径
 输出参数  : N/A
 返 回 值  : FSM_BOOL                           是否创建成功
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_mkdir_ex(const char* pcPath);


/*****************************************************************************
 函 数 名  : svn_remove_ex
 功能描述  : 循环移除文件
 输入参数  : const char* pcPath                 完整路径
 输出参数  : N/A
 返 回 值  : FSM_BOOL                           成功失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年6月19日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_remove_ex(const char* pcPath);


/*****************************************************************************
 函 数 名  : svn_stat
 功能描述  : 获取指定路径文件或目录信息
 输入参数  : const char* pcPath                 完整路径
             SVN_STAT_S* pstStat                信息
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年8月30日
    作    者   : 李方翔<l00218420>
    修改内容   : 新生成函数

*****************************************************************************/
int        svn_stat(const char* pcPath, SVN_STAT_S* pstStat);

/*added by suyaodong 00272240 temp build for anyofficesdk hide openssl sha1 should be delete*/


/* BEGIN: Modified by zhangtailei 00218689, 2015/4/9   PN:DTS2015031906958 【B086.34】【工作台】【现网问题-华为IT】【android】首次迭代9升级到迭代10，首次启动 anyoffice很慢*/
unsigned long SVN_API_SetClearPath(const char* pcPath);
unsigned long SVN_API_ClearPath();
unsigned long SVN_API_SetUpgradeExcludePath(const char* pcPath);
unsigned long SVN_API_UpgradeData (const char *pcPath);
unsigned long SVN_API_HandleupgradeExcludePath(const char* pcSrcPath, const char* pcDstPath);
/* END:   Modified by zhangtailei 00218689, 2015/4/9 */

#ifdef __cplusplus
}
#endif


#endif /* __SVN_FILE_API_H__ */
