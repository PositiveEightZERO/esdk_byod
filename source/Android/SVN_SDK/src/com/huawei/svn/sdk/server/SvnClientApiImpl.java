package com.huawei.svn.sdk.server;

/**
 * 〈一句话功能简述〉 〈功能详细描述〉
 * 
 * @author [作者]（必须）
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnClientApiImpl implements SvnClientApi
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
    /*
     * (non-Javadoc)
     * 
     * @see com.huawei.svn.sdk.server.SvnClientApi#exitEnv()
     */
    @Override
    public native int exitEnv();

    /*
     * (non-Javadoc)
     * 
     * @see com.huawei.svn.sdk.server.SvnClientApi#getIpAddress()
     */
    @Override
    public native String getIpAddress();

    /*
     * (non-Javadoc)
     * 
     * @see com.huawei.svn.sdk.server.SvnClientApi#getVPNStatus()
     */
    @Override
    public native int getVPNStatus();

    /*
     * (non-Javadoc)
     * 
     * @see com.huawei.svn.sdk.server.SvnClientApi#initEnv()
     */
    @Override
    public native int initEnv();

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.huawei.svn.sdk.server.SvnClientApi#login(com.huawei.svn.sdk.server
     * .LoginInfo)
     */
    @Override
    public native int login(LoginInfo stLoginInfo);

    /*
     * (non-Javadoc)
     * 
     * @see com.huawei.svn.sdk.server.SvnClientApi#logout()
     */
    @Override
    public native int logout();

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.huawei.svn.sdk.server.SvnClientApi#setWorkingDir(java.lang.String)
     */
    @Override
    public native int setWorkingDir(String name);

    /*
     * (non-Javadoc)
     * 
     * @see com.huawei.svn.sdk.server.SvnClientApi#setLogParam(java.lang.String,
     * long)
     */
    @Override
    public native int setLogParam(String savepath, long level);

    /*
     * (non-Javadoc)
     * 
     * @see com.huawei.svn.sdk.server.SvnClientApi#parseURL(java.lang.String)
     */
    @Override
    public native int[] parseURL(String strURL);

    @Override
    public native int checkBind(String checkParam);

    @Override
    public native String getMdmViolationResult();

    @Override
    public native int initSandbox(String appIdentifier);

    @Override
    public native int clearSandbox();

    @Override
    public native int eraseSandboxFile(String appIdentifier);

    @Override
    public native void setNetState(int iNetState);
    
    
    
    @Override
    public native CertificateInfo getCertificate(String username);

    @Override
    public native String getAccountName();

    // /* (non-Javadoc)
    // * @see com.huawei.svn.sdk.server.SvnClientApi#doCAChecking()
    // */
    // @Override
    // public native int doCAChecking();
    // /* (non-Javadoc)
    // * @see com.huawei.svn.sdk.server.SvnClientApi#undoCAChecking()
    // */
    // @Override
    // public native int undoCAChecking();
    //
    /**
     * Call back.
     * 
     * @param strLog
     *            the str log
     * @param iLevel
     *            the i level
     * @param iStatus
     *            the i status
     * @param iErrorCode
     *            the i error code
     * @param flag
     *            the flag
     */
    private void callBack(String strLog, int iLevel, int iStatus,
            int iErrorCode, int flag)
    {
        SvnApiService.callBack(strLog, iLevel, iStatus, iErrorCode, flag);
    }

}
