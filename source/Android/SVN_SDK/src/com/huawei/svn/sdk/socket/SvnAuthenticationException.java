/*
 * 
 */
package com.huawei.svn.sdk.socket;

import java.io.IOException;

/**
 * 〈一句话功能简述〉 〈功能详细描述〉.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnAuthenticationException extends IOException
{

    /** 
     * 唯一标识
     */
    static final long serialVersionUID = 111111L;

    /** 
     * 解锁次数
     */
    private int freeLockTimes = -1;

    /**
     * 构造函数
     */
    public SvnAuthenticationException()
    {
        super();
    }

    /**
     * 构造函数
     * 
     * @param message
     *            异常信息
     */
    public SvnAuthenticationException(String message)
    {
        super(message);
    }

    /**
     * 获取解锁次数
     * 
     * @return 解锁次数
     */
    public int getFreeLockTimes()
    {
        return freeLockTimes;
    }


    /**
     * 设置解锁次数
     * 
     * @param iFreeLockTimes
     *            the new 解锁次数
     */
    public void setFreeLockTimes(int iFreeLockTimes)
    {
        this.freeLockTimes = iFreeLockTimes;
    }

}
