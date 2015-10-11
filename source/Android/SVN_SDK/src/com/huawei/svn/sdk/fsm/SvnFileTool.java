package com.huawei.svn.sdk.fsm;

import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.huawei.svn.sdk.SvnConstants;
import com.huawei.svn.sdk.server.LoginInfo;


/**
 * 文件加解密接口工具类
 * 封装文件加解密功能
 * 
 * @author [作者]（必须）
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public final class SvnFileTool
{

    /** 
     * 日志记录 
     */
    private static final Logger logger = Logger.getLogger(SvnFileTool.class
            .getSimpleName());

    /**
     * 文件加解密接口实现
     */
    private static SvnFileApi svnFile = new SvnFileApiImpl();

    /**
     * 构造函数私有
     */
    private SvnFileTool()
    {
    }


    /**
     * 设置固定密钥
     * 
     * @param strUserName
     *            用户id
     * @param deviceId
     *            设备id
     * @return 设置结果
     */
    private static int setFileEncSteadyKey(String strUserName,
            String deviceId)
    {
        return svnFile.setFileEncSteadyKey(strUserName, deviceId);
    }

    /**
     * 初始化文件加解密的环境
     * 
     * @param workPath
     *            文件加解密工作路径
     * @return 初始化结果
     */
    private static int initFileEncEnv(String workPath)
    {
        return svnFile.initFileEncEnv(workPath);
    }

    /**
     * 清理文件加解密的环境
     */
    public static void cleanFileEncEnv()
    {
        svnFile.cleanFileEncEnv();
    }

    /**
     * 获取指定明文路径所对应的密文路径
     * 
     * @param decPath
     *            明文路径
     * @return 密文路径
     */
    public static String encPathname(String decPath)
    {
        return svnFile.encPathname(decPath);
    }

    /**
     * 创建目录
     * 
     * @param pathName
     *            路径名
     * @return 是否成功
     */
    public static boolean createDir(String pathName)
    {
        return svnFile.createDir(pathName);
    }

    /**
     * 文件或文件夹重命名
     * 
     * @param oldName
     *            旧文件名
     * @param newName
     *            新文件名
     * @return 操作结果
     */
    public static int renameDir(String oldName, String newName)
    {
        return svnFile.renameDir(oldName, newName);
    }

    /**
     * 获取文件长度
     * 
     * @param fileName
     *            文件名
     * @return 文件长度
     */
    public static int getFileLength(String fileName)
    {
        return svnFile.getFileLength(fileName);
    }

    // // 打开目录
    // public static int openDir(String filePath);
    //
    // // 读取目录
    // public static String readDir(String filePath, int dirDesc);
    //
    // // 关闭目录
    // public static boolean closeDir(int dirDesc);

    /**
     * 访问文件或目录
     * 
     * @param path
     *            路径
     * @param mode
     *            访问模式
     * @return 是否成功
     */
    public static boolean access(String path, int mode)
    {
        return svnFile.access(path, mode);
    }

    /**
     * 打开文件
     * 
     * @param path
     *            文件路径
     * @param mode
     *            打开模式
     * @return 文件句柄
     */
    public static int openFile(String path, String mode)
    {
        return svnFile.openFile(path, mode);
    }

    /**
     * 关闭文件
     * 
     * @param fd
     *           文件句柄
     * @return 是否成功
     */
    public static boolean closeFile(int fd)
    {
        return svnFile.closeFile(fd);
    }

    /**
     * 移除一个文件或文件夹
     * 
     * @param path
     *            文件路径
     * @return 是否成功
     */
    public static boolean remove(String path)
    {
        return svnFile.remove(path);
    }

    /**
     * 获取上次修改时间
     * 
     * @param path
     *            文件路径
     * @return 上次修改时间
     */
    public static long getLastModiTime(String path)
    {
        return svnFile.getLastModiTime(path);
    }

    /**
     * 获取目录下所有文件的文件名
     * 
     * @param filePath
     *            目录路径
     * @return 文件名列表
     */
    public static ArrayList<String> list(String filePath)
    {
        return svnFile.list(filePath);
    }

    /**
     * 读取文件
     * 
     * @param b
     *            数据缓冲区
     * @param offset
     *            偏移量
     * @param len
     *            读入字节
     * @param fileDesc
     *            文件句柄
     * @return 实际读入字节
     */
    public static int readFile(byte[] b, int offset, int len, int fileDesc)
    {
        return svnFile.readFile(b, offset, len, fileDesc);
    }

    /**
     * 将文件当前位置向后移动指定长度
     * 
     * @param fileDesc
     *            文件句柄
     * @param n
     *            指定长度
     * @return 实际移动长度
     */
    public static long seek(int fileDesc, long n)
    {
        return svnFile.seek(fileDesc, n);
    }

    /**
     * 获取输入流中可用字节数
     * 
     * @param fileDesc
     *            文件句柄
     * @return 可用字节数
     */
    public static int available(int fileDesc)
    {
        return svnFile.available(fileDesc);
    }

    /**
     * 写文件
     * 
     * @param b
     *            写入缓冲区
     * @param fd
     *            文件描述符
     * @return 写入字节数
     */
    public static int writeFile(byte[] b, int fd)
    {
        return svnFile.writeFile(b, fd);
    }

    /**
     * 初始化文件加解密环境
     * 
     * @param fileEncInfo
     *            文件加解密服务信息
     * @return 是否成功，0表示成功，非0表示失败
     */
    public static int initFsmEnv(LoginInfo fileEncInfo)
    {
        int ret = -1;
        if (fileEncInfo != null)
        {
            // 文件加解密工作目录为空
            if (null == fileEncInfo.getFileEncDir()
                    || fileEncInfo.getFileEncDir().trim().length() <= 0)
            {
                ret = SvnConstants.INIT_ENV_FAILED;
                logger.log(Level.SEVERE, "Incorrect file encryption work dir !");
                return ret;
            }
            logger.log(Level.INFO,
                    "Beginning to initialize file encryption work dir!");
            // 初始化文件加解密工作目录并初始化HTTP服务
            ret = initFileEncEnv(fileEncInfo.getFileEncDir());
            if (ret != SvnConstants.INIT_ENV_OK)
            {
                logger.log(Level.SEVERE,
                        "Initializing file encryption work dir failed,errorcode is ："
                                + ret);
                return ret;
            }
            logger.log(Level.INFO,
                    "Initialized file encryption work dir successfully!");

            // 设置固定密钥时用户名或设备Id为空
            if (fileEncInfo.getDeviceId() == null
                    || fileEncInfo.getDeviceId().trim().length() <= 0
                    || fileEncInfo.getUserName() == null
                    || fileEncInfo.getUserName().trim().length() <= 0)
            {
                ret = SvnConstants.INIT_ENV_FAILED;
                logger.log(Level.SEVERE,
                        "Incorrect parameters to create steady encryption!");
                return ret;
            }

            logger.log(Level.INFO, "Beginning to create steady encryption!");
            // 创建固定密钥，并设置
            ret = setFileEncSteadyKey(fileEncInfo.getUserName(),
                    fileEncInfo.getDeviceId());
            if (ret != SvnConstants.INIT_ENV_OK)
            {
                logger.log(Level.SEVERE,
                        "Create steady encryption failed,errorcode is ：" + ret);
                return ret;
            }
            logger.log(Level.INFO, "Create steady encryption successfully!");

        }

        return ret;

    }
    
    
    /**
     * 检查是否是加密文件.
     *
     * @param fileName 文件名
     * @return true:加密文件, false：非加密文件
     */
    public static boolean isEncFile(String fileName)
    {
        return svnFile.isEncFile(fileName);
    }

}
