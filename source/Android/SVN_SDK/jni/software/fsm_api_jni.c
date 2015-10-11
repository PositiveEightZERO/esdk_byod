/*BEGIN Modified by zhangjian z90006922 for Objective c--->c*/
#if defined (ANYOFFICE_ANDROID)

#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <stddef.h>
//#include "vrpcfg.h"
//#include "vos/vospubh/vos_id.h"
#include "svn_file_api_ex.h"
#include "svn_http.h"
#include "svn_define.h"
#include "fsm_api_jni.h"

/*jni 全局变量*/
extern JavaVM* g_JavaVM;

typedef char CHAR;
typedef unsigned long ULONG;
typedef unsigned char UCHAR;
typedef void VOID;

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "SDK", __VA_ARGS__))

#define kUnicodeMaxCodepoint 0x0010FFFF
static ssize_t check_utf8(const char *src)
{
    const char *cur = src;
    size_t ret = 0;
    while (*cur != '\0') {
        const char first_char = *cur++;
        if ((first_char & 0x80) == 0) { // ASCII
            ret += 1;
            continue;
        }
        // (UTF-8's character must not be like 10xxxxxx,
        //  but 110xxxxx, 1110xxxx, ... or 1111110x)
        if ((first_char & 0x40) == 0) {
            return -1;
        }

        int32_t mask, to_ignore_mask;
        size_t num_to_read = 0;
        uint32_t utf32 = 0;
        for (num_to_read = 1, mask = 0x40, to_ignore_mask = 0x80;
             num_to_read < 5 && (first_char & mask);
             num_to_read++, to_ignore_mask |= mask, mask >>= 1) {
            if ((*cur & 0xC0) != 0x80) { // must be 10xxxxxx
                return -1;
            }
            // 0x3F == 00111111
            utf32 = (utf32 << 6) + (*cur++ & 0x3F);
        }
        // "first_char" must be (110xxxxx - 11110xxx)
        if (num_to_read == 5) {
            return -1;
        }
        to_ignore_mask |= mask;
        utf32 |= ((~to_ignore_mask) & first_char) << (6 * (num_to_read - 1));
        if (utf32 > kUnicodeMaxCodepoint) {
            return -1;
        }

        ret += num_to_read;
    }
    return ret;
}
/*jni起始函数，用于存储相关全局变量*/
//jint JNI_OnLoad(JavaVM* vm, void* reserved)
//{
//    g_JavaVM = vm;
  //  return JNI_VERSION_1_4;
//}



/*****************************************************************************
 函 数 名  : FSM_JNI_IsExist
 功能描述  : 判断文件是否存在
 输入参数  : JNIEnv *env   
             jobject jobj  
 输出参数  : 无
 返 回 值  : JNIEXPORT jboolean JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月05日
    作    者   : denghui<00217247>
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jboolean JNICALL  Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_isExist(JNIEnv *env, jobject jobj,jstring filePath)
{
    const char * pcFolder = (*env)->GetStringUTFChars(env, filePath, 0); 
    jboolean ret = (jboolean)(SVN_S_OK==svn_access(pcFolder,0));

    if(NULL != pcFolder)
    {
        (*env)->ReleaseStringUTFChars(env, filePath, pcFolder);
    }    
    return ret;
}

/*****************************************************************************
 函 数 名  : FSM_JNI_OpenFile
 功能描述  : 打开文件
 输入参数  : JNIEnv *env       
             jobject jobj      
             jstring fileName  
             jstring mode      
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_openFile(JNIEnv *env, jobject jobj, jstring fileName, jstring mode)
{
    const char * pcFileName = (*env)->GetStringUTFChars(env, fileName, 0);
    const char * pcMode = (*env)->GetStringUTFChars(env, mode, 0);
    void * pvFile = NULL;

    if ((CHAR *)NULL == pcFileName)
    {
        if ( NULL != pcMode)
        {
            (*env)->ReleaseStringUTFChars(env, mode, pcMode);
        }
        return (jint)0;
    }
    if (NULL == pcMode)
    {
    return (jint)0;
    }
    pvFile = (SVN_FILE_S *)svn_fopen(pcFileName, pcMode);

    LOGE("svn_fopen returns:%d", (jint)pvFile);

    (*env)->ReleaseStringUTFChars(env, fileName, pcFileName);
    (*env)->ReleaseStringUTFChars(env, mode, pcMode);
    return (jint)pvFile;
}

/*****************************************************************************
 函 数 名  : FSM_JNI_GetFileLength
 功能描述  : 获取文件长度
 输入参数  : JNIEnv *env       
             jobject jobj      
             jstring fileName  
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_getFileLength(JNIEnv *env, jobject jobj, jstring fileName)
{
    const char * pcFileName = (*env)->GetStringUTFChars(env, fileName, 0);
    SVN_ERRNO_E enResult = SVN_S_OK;
    ULONG ulFileLen = 0;

    ulFileLen = svn_getsize(pcFileName);
    if( NULL != pcFileName)
    {
        (*env)->ReleaseStringUTFChars(env, fileName, pcFileName);    
    }  
    return (jint)ulFileLen;
}

/*****************************************************************************
 函 数 名  : FSM_JNI_ReadFile
 功能描述  : 读取文件
 输入参数  : JNIEnv *env        
             jobject jobj       
             jbyteArray buffer  
             jint fd            
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_readFile(JNIEnv *env, jobject jobj, jbyteArray buffer, jint offset,jint len,jint fd)
{
    SVN_FILE_S * pstFile = (SVN_FILE_S *)fd;
    //ULONG realLen = 0;
    ULONG uiReadLen = 0;
    UCHAR *pucBuffer = NULL;
    //jbyte *pBytes = NULL;

    if (NULL == pstFile)
    {
        return (jint)-1;
    }
    pucBuffer = (UCHAR *)malloc(len+1);
    if (NULL == pucBuffer)
    {
        return (jint)-1;
    }
    memset(pucBuffer, 0, len+1);

    uiReadLen = svn_fread(pucBuffer, 1,len, pstFile);

    //LOGE("svn_fread returns:%d", uiReadLen);
    //realLen = strlen(pucBuffer);

    (*env)->SetByteArrayRegion(env, buffer, offset, uiReadLen, (jbyte*)pucBuffer);
    free(pucBuffer);
    if(uiReadLen == 0)
    {
        return (jint)-1;
    }

    return (jint)uiReadLen;
}

/*****************************************************************************
 函 数 名  : FSM_JNI_WriteFile
 功能描述  : 写文件
 输入参数  : JNIEnv *env        
             jobject jobj       
             jbyteArray buffer  
             jint fd            
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_writeFile(JNIEnv *env, jobject jobj, jbyteArray buffer, jint fd)
{
    SVN_FILE_S * pstFile = (SVN_FILE_S *)fd;
    ULONG ulBuffLen = 0;
    UCHAR * pucBuffer = NULL;
    jbyte * pbBuffer = NULL;
    ULONG ulRet = 0;
    int iIter = 0;
    
    if (NULL == pstFile)
    {
        return (jint)0;
    }

    ulBuffLen = (ULONG)((*env)->GetArrayLength(env, buffer));
    pbBuffer = (*env)->GetByteArrayElements(env, buffer, NULL);
    
    if (NULL == pbBuffer)
    {
        return (jint)0;
    }

    pucBuffer = (UCHAR *)malloc(ulBuffLen);
    if (NULL == pucBuffer)
    {
         (*env)->ReleaseByteArrayElements(env, buffer, pbBuffer, 0); 
         return (jint)0;
    }

    for(iIter = 0; iIter < ulBuffLen; iIter++)
    {
        pucBuffer[iIter] = (UCHAR)pbBuffer[iIter];
    }
    
    ulRet = svn_fwrite(pucBuffer, 1,ulBuffLen, pstFile);

    (*env)->ReleaseByteArrayElements(env, buffer, pbBuffer, 0);
    free(pucBuffer);
    return (jint)ulRet;
}

/*****************************************************************************
 函 数 名  : FSM_JNI_CloseFile
 功能描述  : 关闭文件
 输入参数  : JNIEnv *env   
             jobject jobj  
             jint fd       
 输出参数  : 无
 返 回 值  : JNIEXPORT void JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_closeFile(JNIEnv *env, jobject jobj, jint fd)
{
    SVN_FILE_S * pstFile = (SVN_FILE_S *)fd;

    if (NULL == pstFile)
    {
        return (jboolean)0;
    }
    
    if (SVN_S_OK !=svn_fclose(pstFile))
    {
        return (jboolean)0;
    }
    return (jboolean)1;
}

/*****************************************************************************
 函 数 名  : FSM_JNI_CreateDir
 功能描述  : 创建目录
 输入参数  : JNIEnv *env      
             jobject jobj     
             jstring dirName  
 输出参数  : 无
 返 回 值  : JNIEXPORT jboolean JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_createDir(JNIEnv *env, jobject jobj, jstring dirName)
{
    const CHAR * pcDirName = (*env)->GetStringUTFChars(env, dirName, 0);
    VOID * pvFile = NULL;

    if (NULL == pcDirName)
    {
        return (jboolean)0;   
    }
    
    if (SVN_S_OK != svn_mkdir_ex(pcDirName))
    {
        (*env)->ReleaseStringUTFChars(env, dirName, pcDirName);
        return (jboolean)0;        
    }
    
     (*env)->ReleaseStringUTFChars(env, dirName, pcDirName);

    return (jboolean)1;
}

/*****************************************************************************
 函 数 名  : FSM_JNI_RenameDir
 功能描述  : 重命名目录
 输入参数  : JNIEnv *env      
             jobject jobj     
             jstring oldName  
             jstring newName  
 输出参数  : 无
 返 回 值  : JNIEXPORT jboolean JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_renameDir(JNIEnv *env, jobject jobj, jstring oldName, jstring newName)
{
    const CHAR * pcOldDirName = (*env)->GetStringUTFChars(env, oldName, 0);
    const CHAR * pcNewDirName = (*env)->GetStringUTFChars(env, newName, 0);
    SVN_ERRNO_E enResult = SVN_E_INPUT;

    if(NULL == pcOldDirName || NULL == pcNewDirName)
    {
        if(NULL != pcOldDirName)
     {
            (*env)->ReleaseStringUTFChars(env, oldName, pcOldDirName);
     }
     if(NULL != pcNewDirName)
     {
            (*env)->ReleaseStringUTFChars(env, newName, pcNewDirName);
     }            
        return (jint)enResult;
    }
    enResult = svn_rename(pcOldDirName, pcNewDirName);
    (*env)->ReleaseStringUTFChars(env, oldName, pcOldDirName);
    (*env)->ReleaseStringUTFChars(env, newName, pcNewDirName);
    return (jint)enResult;
}

/*****************************************************************************
 函 数 名  : FSM_JNI_OpenDir
 功能描述  : 打开目录
 输入参数  : JNIEnv *env      
             jobject jobj     
             jstring dirName  
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_openDir(JNIEnv *env, jobject jobj, jstring dirName)
{
    const CHAR *pcDirName = (*env)->GetStringUTFChars(env, dirName, 0);
    VOID *pvDir = NULL;

    if (NULL == pcDirName)
    {
        return (jint)0;
    }
    pvDir = (void *)svn_opendir(pcDirName);

    (*env)->ReleaseStringUTFChars(env, dirName, pcDirName);

    return (jint)pvDir;
}


/*****************************************************************************
 函 数 名  : FSM_JNI_CloseDir
 功能描述  : 关闭文件夹
 输入参数  : JNIEnv *env   
             jobject jobj  
             jint dd       
 输出参数  : 无
 返 回 值  : JNIEXPORT void JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT void JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_closeDir(JNIEnv *env, jobject jobj, jint dd)
{
    SVN_DIR_S *pstDir = (SVN_DIR_S *)dd;

    if (NULL == pstDir)
    {
        return;
    }

    svn_closedir(pstDir);
}

/*****************************************************************************
 函 数 名  : FSM_JNI_Remove
 功能描述  : 删除文件（夹）
 输入参数  : JNIEnv *env       
             jobject jobj      
             jstring pathName  
 输出参数  : 无
 返 回 值  : JNIEXPORT jboolean JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_remove(JNIEnv *env, jobject jobj, jstring pathName)
{
    const CHAR *pcPathName = (*env)->GetStringUTFChars(env, pathName, 0);
    SVN_ERRNO_E bResult = SVN_S_OK;

    if (NULL == pcPathName)
    {
        return (jboolean)0;
    }
    
    /* 如果底层不支持删除目录中的文件，则还需要调用 */
    bResult = svn_remove(pcPathName);

    (*env)->ReleaseStringUTFChars(env, pathName, pcPathName);

    return (bResult == SVN_S_OK) ? (jboolean)1 : (jboolean)0;
}

/*****************************************************************************
 函 数 名  : initEnv
 功能描述  : 初始化文件加解密工作环境
 输入参数  : JNIEnv *env       
                           jobject jobj      
                           jstring pathName
                           jstring username
                           jstring deviceid
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : caijimiao
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_initFileEncEnv(JNIEnv* env, jobject jobj, jstring pathName)
{
    const CHAR *pcPathName = (*env)->GetStringUTFChars(env, pathName, 0); 
    SVN_ERRNO_E enResult = SVN_E_INPUT;
    if(NULL == pcPathName)
    {
        return (jint)enResult;
    }
    enResult = SVN_API_FileEncInitEnv (pcPathName);
    (*env)->ReleaseStringUTFChars(env, pathName, pcPathName);  
    return (jint)enResult;
}

/*****************************************************************************
 函 数 名  : cleanFileEncEnv
 功能描述  :清理文件加解密工作环境
 输入参数  : JNIEnv *env       
                           jobject jobj
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : caijimiao
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT void JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_cleanFileEncEnv(JNIEnv* env, jobject jobj)
{
    SVN_API_FileEncCleanEnv ();
}

/*****************************************************************************
 函 数 名  : access
 功能描述  : 访问文件
 输入参数  : JNIEnv *env       
                           jobject jobj      
                           jstring filePath
 输出参数  : 无
 返 回 值  : JNIEXPORT jboolean JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : caijimiao
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jboolean JNICALL  Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_access(JNIEnv *env, jobject jobj,jstring filePath,jint accessMode)
{
    char * pcFolder = (*env)->GetStringUTFChars(env, filePath, 0); 
    if(NULL == pcFolder)
    {
        return (jboolean)0;
    }
    jboolean ret = (jboolean)(SVN_S_OK==svn_access(pcFolder,accessMode));

    (*env)->ReleaseStringUTFChars(env, filePath, pcFolder);
    
    return ret;
}

/*****************************************************************************
 函 数 名  : seek
 功能描述  : 指定文件句柄的读/ 写位置
 输入参数  : JNIEnv *env       
                           jobject jobj      
                           jint fd
                           jint len
 输出参数  : 无
 返 回 值  : JNIEXPORT jboolean JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : caijimiao
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jlong JNICALL  Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_seek(JNIEnv *env, jobject jobj,jint fd,jlong len)
{
    SVN_FILE_S *pstFile = (SVN_FILE_S *)fd;
    if(NULL == pstFile)
    {
        return SVN_FILE_JNI_FALSE; 
    }
    unsigned long ret = 0;
    unsigned long curPostion=0;
    unsigned long curPostion2=0;
    curPostion=svn_ftell(pstFile);
    SVN_SEEK_E enOrigin = SVN_SEEK_CUR;
    if(SVN_S_OK==svn_fseek(pstFile, len, enOrigin))
    {
        curPostion2 = svn_ftell(pstFile);
     ret = curPostion2 - curPostion;
    }
    
    return (jlong)ret;
   
}

/*****************************************************************************
 函 数 名  : available
 功能描述  : 指定流估计可用的字节数
 输入参数  : JNIEnv *env       
                           jobject jobj      
                           jint fd
                           输出参数  : 无
 返 回 值  : JNIEXPORT jboolean JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : caijimiao
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jlong JNICALL  Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_available(JNIEnv *env, jobject jobj,jint fd)
{
    SVN_FILE_S *pstFile = (SVN_FILE_S *)fd;
    if(NULL == pstFile)
    {
        return SVN_FILE_JNI_FALSE; 
    }
    unsigned long   ulSize = 0;
    unsigned long ret = 0;
    unsigned long curPostion=0;
    curPostion=svn_ftell(pstFile);
    /* 根据句柄获取大小 */
    if ( SVN_S_OK !=  svn_getsize_for_fd(
                                    pstFile, 
                                    &ulSize))
    {
        ulSize = 0;
    }
    return (jlong) (ulSize - curPostion);
}


/*****************************************************************************
 函 数 名  : stat
 功能描述  : 获取文件上次修改时间
 输入参数  : JNIEnv *env       
                           jobject jobj      
                           jstring filepath
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : caijimiao
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jlong JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_getLastModiTime(JNIEnv* env,jobject jobj,jstring filepath)
{
    const CHAR *pcPath = (*env)->GetStringUTFChars(env, filepath, 0);
    SVN_STAT_S stStat = {0};
    SVN_ERRNO_E enResult = SVN_E_INPUT;
    if(NULL == pcPath)
    {
        return (jint)enResult;
    }
    enResult = svn_stat(pcPath, &stStat);
    (*env)->ReleaseStringUTFChars(env, filepath, pcPath);
    
    if(SVN_S_OK == enResult)
    {
        return (jlong)stStat.ulModifyTime;
    }
    return  (jint)enResult;;
}

/*****************************************************************************
 函 数 名  : encPathname
 功能描述  : 获取指定文件名加密后的名称
 输入参数  : JNIEnv *env       
                           jobject jobj      
                           jstring filepath
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : caijimiao
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jstring JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_encPathname(JNIEnv* env,jobject jobj,jstring filepath)
{
    jstring jEnPath = NULL;    
    const char *pcPath = (*env)->GetStringUTFChars(env, filepath, 0);
    if(NULL == pcPath)
    {
        return jEnPath;
    }
    ULONG ret = SVN_S_OK;
    char *encPath =  (char*)malloc(PATH_LEN);
    if(NULL == encPath)
    {
          (*env)->ReleaseStringUTFChars(env, filepath, pcPath);
      return jEnPath;
    }
    memset(encPath,0,PATH_LEN);
    ret = SVN_API_GetEncFilePath(pcPath, encPath,PATH_LEN);
    if(ret != SVN_S_OK)
    {
         free(encPath);
        (*env)->ReleaseStringUTFChars(env, filepath, pcPath);
      return jEnPath;
    }
    jEnPath = (*env)->NewStringUTF(env, (void *)encPath);
    (*env)->ReleaseStringUTFChars(env, filepath, pcPath);
    free(encPath);
    return jEnPath;
}

/*****************************************************************************
 函 数 名  : list
 功能描述  : 获取指定目录下的所有文件名称
 输入参数  : JNIEnv *env       
                           jobject jobj      
                           jstring dirName
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : caijimiao
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jobject JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_list(JNIEnv *env, jobject jobj, jstring dirName)
{
    const char * pcPath = (*env)->GetStringUTFChars(env,dirName,0) ;
    if( NULL == pcPath )
    {
        return NULL;
    }
    jclass array_class = (*env)->FindClass(env,"java/util/ArrayList");
    if(array_class==NULL)
    {
        (*env)->ReleaseStringUTFChars(env, dirName, pcPath);
        return NULL;
    }
    jmethodID list_init = (*env)->GetMethodID(env, array_class, "<init>", "()V");
    if(list_init==NULL)
    {
        (*env)->DeleteLocalRef(env, array_class);
        (*env)->ReleaseStringUTFChars(env, dirName, pcPath);
        return NULL;
    }
    jobject list_object = (*env)->NewObject(env, array_class, list_init, "");
    if(list_object==NULL)
    {
        (*env)->ReleaseStringUTFChars(env, dirName, pcPath);
        (*env)->DeleteLocalRef(env, array_class);
        return NULL;
    }
    jmethodID arraylist_add = (*env)->GetMethodID(env,array_class,"add","(Ljava/lang/Object;)Z");
    if(arraylist_add==NULL)
    {
        (*env)->ReleaseStringUTFChars(env, dirName, pcPath);
        (*env)->DeleteLocalRef(env, array_class);
        (*env)->DeleteLocalRef(env, list_object);
        return NULL;
    }
    /* end new ArrayList */

    /*open directory*/
    SVN_DIR_S     *dir     = NULL;
    dir = svn_opendir(pcPath);
    if(NULL == dir)
    {
        (*env)->ReleaseStringUTFChars(env, dirName, pcPath);
        (*env)->DeleteLocalRef(env, array_class);
       // (*env)->DeleteLocalRef(env, list_init);
        (*env)->DeleteLocalRef(env, list_object);
        //(*env)->DeleteLocalRef(env, arraylist_add);
        LOGE("svn_opendir return null");
        return NULL;
    }
    SVN_DIRINFO_S* subdir = NULL;
    subdir = svn_readdir(dir);
    while(subdir != NULL)
    {
        if((strcmp(subdir->acDirName,ONE_DOT_FILE) == 0) ||(strcmp(subdir->acDirName,TWO_DOT_FILE) == 0))
        {
            subdir = svn_readdir(dir);
            continue;
        }

        ssize_t utf8Length = check_utf8(subdir->acDirName);
		if(-1 == utf8Length)
		{
			LOGE("subdir->acDirName:%s not utf8", subdir->acDirName);
			subdir = svn_readdir(dir);
			continue;
		}


        LOGE("subdir->acDirName:%s", subdir->acDirName);
//        int i = 0;
//        int len = strlen(subdir->acDirName);
//        for(i=0; i< len; i++)
//        {
//            LOGE("dir name[%d]:%x", i, *(subdir->acDirName+i));
//        }
        jstring result = (*env)->NewStringUTF(env, (void *)subdir->acDirName);
        /*add to list*/        
       // jmethodID arraylist_add = (*env)->GetMethodID(env,array_class,"add","(Ljava/lang/Object;)Z");
        (*env)->CallBooleanMethod(env, list_object, arraylist_add, result);
        (*env)->DeleteLocalRef(env, result);
        subdir = svn_readdir(dir);
    }
    (*env)->ReleaseStringUTFChars(env, dirName, pcPath);
    (*env)->DeleteLocalRef(env, array_class);
   // (*env)->DeleteLocalRef(env, list_init);
   // (*env)->DeleteLocalRef(env, arraylist_add);
    svn_closedir(dir);
    return list_object;
}

/*****************************************************************************
 函 数 名  : setSteadyKey
 功能描述  : 设置固定密钥
 输入参数  : JNIEnv *env       
                           jobject jobj      
                           jstring userName
                           jstring deviceId
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月12日
    作    者   : caijimiao
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_setFileEncSteadyKey(JNIEnv *env, jobject jobj, jstring userName,jstring deviceId)
{
    SVN_ERRNO_E enResult = SVN_E_INPUT;
    const char * pcUserName = (*env)->GetStringUTFChars(env,userName,0) ;
    if( NULL == pcUserName )
    {
        return enResult;
    }
    const char * pcDeviceId = (*env)->GetStringUTFChars(env,deviceId,0) ;
    if( NULL == pcDeviceId )
    {
        (*env)->ReleaseStringUTFChars(env,  userName,pcUserName);
        return enResult;
    }
    enResult = SVN_API_SetSteadyKey(pcUserName,pcDeviceId);
    (*env)->ReleaseStringUTFChars(env, userName,pcUserName);    
    (*env)->ReleaseStringUTFChars(env, deviceId,pcDeviceId);
    return enResult;
}



JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_fsm_SvnFileApiImpl_isEncFile(JNIEnv *env, jobject jobj, jstring fileName)
{
    jboolean ret = (jboolean)0;
    const char * pcFileName = (*env)->GetStringUTFChars(env, fileName, 0) ;
    if( NULL == pcFileName )
    {
        return ret;
    }

    int result = 0;
    int rs = svn_isencfile(pcFileName, &result);
    (*env)->ReleaseStringUTFChars(env, fileName,pcFileName);
    if(rs == SVN_OK)
    {
        return (jboolean)result;
    }

    LOGE("svn_isencfile returns:%d!", rs);
    return ret;
}


#endif
/*END Modified by zhangjian z90006922 for Objective C--->C*/


