package com.huawei.svn.sdk.fileview;

import android.content.Context;
import android.util.Log;

import com.huawei.anyoffice.sdk.doc.SecReader;
import com.huawei.anyoffice.sdk.exception.NoRMSAppFoundException;
import com.huawei.anyoffice.sdk.exception.NoWPSAppFoundException;
import com.huawei.anyoffice.sdk.sandbox.SDKClipboard;

// TODO: Auto-generated Javadoc
/**
 * 加密文件查看类，加密文档可能是通过文件加解密接口加密保存的文档，也可能是RMS加密的文档。.
 */
public class FileViewUtil
{
    static
    {
        System.loadLibrary("svnapi");
        System.loadLibrary("anyofficesdk");
        System.loadLibrary("jniapi");
    }

    /**
     * 加密文件查看，加密文档可能是通过文件加解密接口加密保存的文档，也可能是RMS加密的文档。
     * 通过文件加解密接口加密保存的文档通过WPS打开，RMS文档通过GigaTrust打开.
     *
     * @param context 应用上下文
     * @param filePath 文件明文路径
     * @param packageName 应用包名
     * @return true 打开成功
     * false 打开失败
     * @throws NoWPSAppFoundException 未安装WPS
     */
    public static boolean openEncryptedFile(Context context, String filePath, String packageName)
            throws NoWPSAppFoundException, NoRMSAppFoundException 
    {
        boolean isRMSFile = isRMSDoc(filePath);
        if (isRMSFile)
        {
            return openRMSFile(context, filePath);
           
        }
        else
        {
            return openEncryptedFileInWPS(context, filePath, packageName);
        }

    }

    /**
     * 加密文件查看，加密文档是通过文件加解密接口加密保存的文档。.
     *
     * @param context 应用上下文
     * @param filePath 文件明文路径
     * @param packageName 应用包名
     * @return true 打开成功
     * false 打开失败
     * @throws NoWPSAppFoundException 未安装WPS
     */
    public static boolean openEncryptedFileInWPS(Context context,
            String filePath, String packageName) throws NoWPSAppFoundException
    {
        try
        {
            boolean result = SecReader.openDocWithWPS(context, filePath,
                    "true", packageName);

            return result;
        }
        catch (NoWPSAppFoundException e)
        {
            Log.e("SDK", "WPS not installed!");
            throw e;
        }
    }

    /**
     * 检查是否是RMS加密的文档.
     *
     * @param filePath 文件路径
     * @return true 是RMS加密文件
     * false 非RMS加密文件
     */
    public static boolean isRMSDoc(String filePath)
    {
        return SecReader.isRMSDoc(filePath);
    }

    /**
     * 根据文件内容检查是否是RMS加密的文档.
     *
     * @param fileName the file name
     * @param content the content
     * @return true 是RMS加密文件
     * false 非RMS加密文件
     */
    public static boolean isRMSDoc(String fileName, byte[] content)
    {

        boolean result = false;

        try
        {
            result = SecReader.isRMSDoc(fileName, content);
        }
        catch (Exception e)
        {
            // TODO: handle exception
            result = false;
        }

        return result;

    }

    /**
     * RMS加密文件查看。.
     *
     * @param context 应用上下文
     * @param filePath 文件路径
     * @return true 打开成功
     * false 打开失败
     * @throws NoRMSAppFoundException 
     */
    public static boolean openRMSFile(Context context, String filePath)
            throws NoRMSAppFoundException
    {
        try
        {
            boolean result = SecReader.openRMSDoc(context, filePath, "ReadOnly");
            return result;
        }
        catch (NoRMSAppFoundException e)
        {
            Log.e("SDK", "File:" + filePath + "not RMSDoc!");
            throw e;
        }
    }
    
    
    /**
     * 前台切换到后台时，保存剪贴板内容到内存，清空剪贴板.
     *
     * @param context 应用上下文
     */
    public static void clearClipboard(Context context)
    {
        SDKClipboard.getInstance().onPause(context);
    }
    
    
    /**
     *  后台切换到前台时，恢复剪贴板内容.
     *
     * @param context 应用上下文
     */
    public static void restoreClipboard(Context context)
    {
        SDKClipboard.getInstance().onResume(context);
    }

}
