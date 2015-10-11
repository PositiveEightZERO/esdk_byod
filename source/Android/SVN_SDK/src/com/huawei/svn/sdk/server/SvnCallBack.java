package com.huawei.svn.sdk.server;


/**
 * 隧道状态及日志回调接口
 * 
 * 应用创建隧道时，设置好此回调对象后，用于接收隧道状态通知和日志记录通知
 * 
 * @author l00174413
 * @version 1.0
 * @see SvnApiService#setCallBack(SvnCallBack)
 * @since
 */
public interface SvnCallBack
{
    
    /**
     * 隧道状态回调通知
     * 
     * @param iStatus
     *            隧道状态
     * @param iErrorCode
     *            错误码
     */
    void statusNotify(int iStatus, int iErrorCode);
    
    /**
     * SDK日志回调通知
     * 
     * @param strLog
     *            日志信息
     * @param iLevel
     *            日志级别
     */
    void writeLog(String strLog, int iLevel);
}
