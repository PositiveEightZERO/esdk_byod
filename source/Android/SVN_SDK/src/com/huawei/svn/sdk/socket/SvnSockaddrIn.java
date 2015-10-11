/*
 * 
 */
package com.huawei.svn.sdk.socket;


/**
 * Svn SockaddrIn封装
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnSockaddrIn
{
    
    /** 协议族 */
    private int sinFamily;
    
    /** 地址 */
    private String sinAddr;
    
    /** 端口 */
    private int sinPort;
    
    /** 地址长度 */
    private int sinAddrlen;

    /**
     * 获取协议族
     * 
     * @return 协议族
     */
    public int getSinFamily()
    {
        return sinFamily;
    }

    /**
     * 设置协议族
     * 
     * @param family
     *            协议族
     */
    public void setSinFamily(int family)
    {
        this.sinFamily = family;
    }

    /**
     * 获取地址
     * 
     * @return 地址
     */
    public String getSinAddr()
    {
        return sinAddr;
    }

    /**
     * 设置地址
     * 
     * @param addr
     *            地址
     */
    public void setSinAddr(String addr)
    {
        this.sinAddr = addr;
    }

    /**
     * 获取端口
     * 
     * @return 端口
     */
    public int getSinPort()
    {
        return sinPort;
    }

    /**
     * 设置端口
     * 
     * @param port
     *            端口
     */
    public void setSinPort(int port)
    {
        this.sinPort = port;
    }
    
    /**
     * 获取地址长度
     * 
     * @return 地址长度
     */
    public int getSinAddrlen()
    {
        return sinAddrlen;
    }

    /**
     * 设置地址长度
     * 
     * @param addrlen
     *            地址长度
     */
    public void setSinAddrlen(int addrlen)
    {
        this.sinAddrlen = addrlen;
    }
}
