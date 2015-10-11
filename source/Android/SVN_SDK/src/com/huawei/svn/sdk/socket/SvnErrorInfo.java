/*
 * 
 */
package com.huawei.svn.sdk.socket;



/**
 * 〈一句话功能简述〉
 * 
 * 〈功能详细描述〉.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public interface SvnErrorInfo
{
    
    /**
     * 用户信息错误
     */
    String SVN_USER_INFO_ERR = "client infomation error,check the name and password";
    
    /** 
     * Socket创建失败
     */
    String SVN_SOCKET_ERROR = "socket create error";
    
    /** 
     * 用户信息验证失败，用户锁定打开
     */
    String SVN_USER_LOCK = "user info check error,user lock is opened";
    
    /** 
     * 用户被锁定
     */
    String SVN_USER_LOCKED = "user is locked,waiting for a moment";
    
    /** 
     * 用户锁定错误码
     */
    String SVN_ERRNO_LOCKED = "-1015";
    
    /** 
     * 用户信息错误码
     */
    String SVN_ERRNO_WRONG_INFO = "-1005";

}
