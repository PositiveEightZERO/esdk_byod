package com.huawei.svn.sdk.socket;


/**
 * The Interface SvnSocketApi.
 * 
 * @author l00174413
 * @version 1.0
 * @see 
 * @since eSDK V1R2 
 */
public interface SvnSocketApi
{
    /* socket API */

    /**
     * Svn socket.
     * 
     * @param family
     *            the i family
     * @param type
     *            the i type
     * @param protocol
     *            the i protocol
     * @return the int
     */
    int svnSocket(int family, int type, int protocol);

    /**
     * Svn bind.
     * 
     * @param fd
     *            the i fd
     * @param objSockAddr
     *            the obj sock addr
     * @return the int
     */
    int svnBind(int fd, SvnSockaddrIn objSockAddr);

    /**
     * Svn connect.
     * 
     * @param fd
     *            the i fd
     * @param objSockAddr
     *            the obj sock addr
     * @param iTimeout
     *            the i timeout
     * @return the int
     */
    int svnConnect(int fd, SvnSockaddrIn objSockAddr, int iTimeout);

    /**
     * Svn close.
     * 
     * @param fd
     *            the i fd
     * @return the int
     */
    int svnClose(int fd);

    /**
     * Svn shutdown.
     * 
     * @param fd
     *            the i fd
     * @param iHow
     *            the i how
     * @return the int
     */
    int svnShutdown(int fd, int iHow);

    /**
     * Svn recv.
     * 
     * @param fd
     *            the i fd
     * @param byteBuf
     *            the byte buf
     * @param iLen
     *            the i len
     * @param iFlags
     *            the i flags
     * @return the int
     */
    int svnRecv(int fd, byte[] byteBuf, int iLen, int iFlags);

    /**
     * Svn recvfrom.
     * 
     * @param fd
     *            the i fd
     * @param byteBuf
     *            the byte buf
     * @param iLen
     *            the i len
     * @param iFlags
     *            the i flags
     * @param objFromAddr
     *            the obj from addr
     * @return the int
     */
    int svnRecvfrom(int fd, byte[] byteBuf, int iLen, int iFlags,
            SvnSockaddrIn objFromAddr);

    /**
     * Svn send.
     * 
     * @param fd
     *            the i fd
     * @param byteBuf
     *            the byte buf
     * @param iLen
     *            the i len
     * @param iFlags
     *            the i flags
     * @return the int
     */
    int svnSend(int fd, byte[] byteBuf, int iLen, int iFlags);

    /**
     * Svn sendto.
     * 
     * @param fd
     *            the i fd
     * @param byteBuf
     *            the byte buf
     * @param iLen
     *            the i len
     * @param iFlags
     *            the i flags
     * @param objToAddr
     *            the obj to addr
     * @return the int
     */
    int svnSendto(int fd, byte[] byteBuf, int iLen, int iFlags,
            SvnSockaddrIn objToAddr);

    /**
     * Svn getsockopt.
     * 
     * @param fd
     *            the i fd
     * @param iLevel
     *            the i level
     * @param iOptName
     *            the i opt name
     * @param iArrayOptVal
     *            the i array opt val
     * @param iArrayOptLen
     *            the i array opt len
     * @return the int
     */
    int svnGetsockopt(int fd, int iLevel, int iOptName, int[] iArrayOptVal,
            int[] iArrayOptLen);

    /**
     * Svn setsockopt.
     * 
     * @param fd
     *            the i fd
     * @param iLevel
     *            the i level
     * @param iOptName
     *            the i opt name
     * @param iOptVal
     *            the i opt val
     * @param iOptLen
     *            the i opt len
     * @return the int
     */
    int svnSetsockopt(int fd, int iLevel, int iOptName, int iOptVal,
            int iOptLen);

    /**
     * Svn getlocalport.
     * 
     * @param fd
     *            the i fd
     * @return the int
     */
    int svnGetlocalport(int fd);

}
