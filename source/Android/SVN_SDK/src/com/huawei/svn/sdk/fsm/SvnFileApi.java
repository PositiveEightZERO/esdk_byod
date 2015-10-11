/*
 * 
 */
package com.huawei.svn.sdk.fsm;

import java.util.ArrayList;

// TODO: Auto-generated Javadoc
/**
 * SVN 文件加解密接口.
 *
 * @author zhaixianqi
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public interface SvnFileApi
{

    /**
     * 设置固定密钥.
     *
     * @param strUserName 用户名
     * @param deviceId 设备ID
     * @return 操作结果
     */
    int setFileEncSteadyKey(String strUserName, String deviceId);

    /**
     * 初始化文件加解密的环境.
     *
     * @param workPath 文件加解密工作路径
     * @return 操作结果
     */
    int initFileEncEnv(String workPath);

    /**
     * 清理文件加解密的环境.
     */
    void cleanFileEncEnv();

    /**
     * 获取指定明文路径所对应的密文路径.
     *
     * @param decPath 明文路径
     * @return 密文路径
     */
    String encPathname(String decPath);

    /**
     * ***************** SvnFile API START ***************************.
     *
     * @param pathName the path name
     * @return true, if successful
     */

    /**
     * 创建目录
     * 
     * @param pathName
     *            目录名称
     * @return 是否成功
     */
    boolean createDir(String pathName);

    /**
     * 文件或文件夹重命名.
     *
     * @param oldName 文件（文件夹）原名称
     * @param newName 文件（文件夹）新名称
     * @return 重命名结果
     */
    int renameDir(String oldName, String newName);

    /**
     * 获取文件长度.
     *
     * @param fileName 文件名称
     * @return 文件长度
     */
    int getFileLength(String fileName);

    /**
     * 访问文件或目录.
     *
     * @param path 文件路径
     * @param mode 文件权限模式
     * @return 操作结果
     */
    boolean access(String path, int mode);

    /**
     * 打开文件.
     *
     * @param path 文件路径
     * @param mode 打开方式
     * @return 打开结果
     */
    int openFile(String path, String mode);

    /**
     * 关闭文件.
     *
     * @param fd 文件fd
     * @return 关闭结果
     */
    boolean closeFile(int fd);

    /**
     * 移除一个文件或文件夹.
     *
     * @param path 删除的文件路径
     * @return 删除结果
     */
    boolean remove(String path);

    /**
     * 获取上次修改时间.
     *
     * @param path 文件路径
     * @return 上次修改时间
     */
    long getLastModiTime(String path);

    /**
     * 获取目录下所有文件的文件名.
     *
     * @param filePath 目录路径
     * @return 目录下文件名
     */
    ArrayList<String> list(String filePath);


    /**
     * 读取文件.
     *
     * @param b 字符数组，读取到的内容
     * @param offset 偏移量，从字符数组的该索引开始存放读取的内容
     * @param len 读取的内容长度
     * @param fileDesc 文件描述符
     * @return 实际读取长度
     */
    int readFile(byte[] b, int offset, int len, int fileDesc);

    /**
     * 将文件当前位置向后移动指定长度.
     *
     * @param fileDesc 文件描述符
     * @param n 向后移动的字节偏移量
     * @return 实际移动偏移量
     */
    long seek(int fileDesc, long n);

    /**
     * 获取输入流中可用字节数.
     *
     * @param fileDesc 文件描述符
     * @return 输入流中可用字节数
     */
    int available(int fileDesc);

    /**
     * 写文件.
     *
     * @param b 字符数组，待写入 的内容
     * @param fd 文件描述符
     * @return 实际写入长度
     */
    int writeFile(byte[] b, int fd);
    
    
    /**
     * 检查是否是加密文件.
     *
     * @param fileName 文件名
     * @return true:加密文件, false：非加密文件
     */
    boolean isEncFile(String fileName);

    /******************* SvnFileOutputStream API END ****************************/

}
