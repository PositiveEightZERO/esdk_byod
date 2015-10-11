package com.huawei.svn.sdk.server;


/**
 * 数据加解密
 * 
 * @author [作者]（必须）
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnBigStringOpterations
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
    /**
     * 数据加密
     * 
     * @param oriContent
     *            明文数据
     * @return 密文数据
     */
    public native static byte[] encryptLarge(byte[] oriContent);

    /**
     * 数据解密
     * 
     * @param encContent
     *            密文数据
     * @return 明文数据
     */
    public native static byte[] decryptLarge(byte[] encContent);

}
