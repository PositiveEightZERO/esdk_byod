package com.huawei.svn.sdk.socket;


/**
 * 〈一句话功能简述〉 〈功能详细描述〉
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnSocketApiImpl implements SvnSocketApi
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
    /* socket API */

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnSocket(int, int, int)
     */
    public native int svnSocket(int iFamily, int iType, int iProtocol);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnBind(int, com.huawei.svn.sdk.socket.SvnSockaddrIn)
     */
    public native int svnBind(int iFd, SvnSockaddrIn objSockAddr);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnConnect(int, com.huawei.svn.sdk.socket.SvnSockaddrIn, int)
     */
    public native int svnConnect(int iFd, SvnSockaddrIn objSockAddr,
            int iTimeout);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnClose(int)
     */
    public native int svnClose(int iFd);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnShutdown(int, int)
     */
    public native int svnShutdown(int iFd, int iHow);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnRecv(int, byte[], int, int)
     */
    public native int svnRecv(int iFd, byte[] byteBuf, int iLen, int iFlags);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnRecvfrom(int, byte[], int, int, com.huawei.svn.sdk.socket.SvnSockaddrIn)
     */
    public native int svnRecvfrom(int iFd, byte[] byteBuf, int iLen,
            int iFlags, SvnSockaddrIn objFromAddr);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnSend(int, byte[], int, int)
     */
    public native int svnSend(int iFd, byte[] byteBuf, int iLen, int iFlags);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnSendto(int, byte[], int, int, com.huawei.svn.sdk.socket.SvnSockaddrIn)
     */
    public native int svnSendto(int iFd, byte[] byteBuf, int iLen, int iFlags,
            SvnSockaddrIn objToAddr);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnGetsockopt(int, int, int, int[], int[])
     */
    public native int svnGetsockopt(int iFd, int iLevel, int iOptName,
            int[] iArrayOptVal, int[] iArrayOptLen);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnSetsockopt(int, int, int, int, int)
     */
    public native int svnSetsockopt(int iFd, int iLevel, int iOptName,
            int iOptVal, int iOptLen);

    /* (non-Javadoc)
     * @see com.huawei.svn.sdk.socket.SvnSocketApi#svnGetlocalport(int)
     */
    public native int svnGetlocalport(int iFd);

}
