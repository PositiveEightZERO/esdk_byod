package com.huawei.svn.sdk.fsm;

import java.util.ArrayList;


/**
 * 〈一句话功能简述〉 〈功能详细描述〉
 * 
 * @author [作者]（必须）
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnFileApiImpl implements SvnFileApi
{
    /**
     * 加载so
     */
    static
    {
        System.loadLibrary("svnapi");
        System.loadLibrary("anyofficesdk");
        System.loadLibrary("jniapi");
    }
    
    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#setFileEncSteadyKey(java.lang.String, java.lang.String)
     */
    public native int setFileEncSteadyKey(String strUserName,
            String deviceId);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#initFileEncEnv(java.lang.String)
     */
    public native int initFileEncEnv(String workPath);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#cleanFileEncEnv()
     */
    public native void cleanFileEncEnv();

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#encPathname(java.lang.String)
     */
    public native String encPathname(String decPath);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#createDir(java.lang.String)
     */
    public native boolean createDir(String pathName);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#renameDir(java.lang.String, java.lang.String)
     */
    public native int renameDir(String oldName, String newName);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#getFileLength(java.lang.String)
     */
    public native int getFileLength(String fileName);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#access(java.lang.String, int)
     */
    public native boolean access(String path, int mode);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#openFile(java.lang.String, java.lang.String)
     */
    public native int openFile(String path, String mode);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#closeFile(int)
     */
    public native boolean closeFile(int fd);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#remove(java.lang.String)
     */
    public native boolean remove(String path);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#getLastModiTime(java.lang.String)
     */
    public native long getLastModiTime(String path);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#list(java.lang.String)
     */
    public native ArrayList<String> list(String filePath);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#readFile(byte[], int, int, int)
     */
    public native int readFile(byte[] b, int offset, int len, int fileDesc);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#seek(int, long)
     */
    public native long seek(int fileDesc, long n);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#available(int)
     */
    public native int available(int fileDesc);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#writeFile(byte[], int)
     */
    public native int writeFile(byte[] b, int fd);
   
    
    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.fsm.SvnFileApi#isEncFile(java.lang.String)
     */
    public native boolean isEncFile(String fileName);

}
