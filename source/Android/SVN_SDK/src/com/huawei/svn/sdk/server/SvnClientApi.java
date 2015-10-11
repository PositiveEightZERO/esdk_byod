package com.huawei.svn.sdk.server;



/**
 * SVN隧道环境接口.
 *
 * @author [作者]（必须）
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public interface SvnClientApi
{

    /**
     * 设置工作路径.
     *
     * @param name the name
     * @return 成功:0，失败 ：非0
     */
    int setWorkingDir(String name);

    /**
     * 退出SSL运行环境.
     *
     * @return 成功:0，失败 ：非0
     */
    int exitEnv();

    /**
     * 创建SSL隧道成功后，获取虚拟IP地址和子网掩码.
     *
     * @return 成功:0，失败 ：非0
     * @see
     */
    String getIpAddress();

    /**
     * 通过该接口返回当前的svn连接的状态，可确定是否能通过SVN的安全Socket接口进行隧道聚合和加密发送。.
     *
     * @return 连接成功：1，正在连接：2，
     * @see
     */
    int getVPNStatus();

    /**
     * 应用程序启动成功后调用此接口初始化SSL运行环境，只有在此方法返回成功后，后续的SSL接口才能正确执行。.
     *
     * @return 成功:0，失败 ：非0
     * @see
     */
    int initEnv();

    /**
     * 需要建立SSL隧道时使用，传入各类连接信息，例如SVN服务器信息、用户信息和代理信息。.
     *
     * @param stLoginInfo 连接信息
     * @return 成功:0，失败 ：非0
     * @see
     */
    int login(LoginInfo stLoginInfo);

    /**
     * 需要关闭SSL隧道时使用。.
     *
     * @return 成功:0，失败 ：非0
     * @see
     */
    int logout();

    /**
     * 设置日志保存路径和保存日志的级别.
     *
     * @param savePath 日志保存路径
     * @param level 保存日志的级别
     * @return 设置结果
     * @see
     */
    int setLogParam(String savePath, long level);

    /**
     * 解析域名并返回IP地址.
     *
     * @param strURL IP地址
     * @return 返回int 型IP地址,失败返回0，成功返回IP地址
     */
    int[] parseURL(String strURL);
    
	
    /**
     * 检查设备是否已绑定
     * @param checkParam 检查参数，目前未使用
     * @return
     */
    int checkBind(String checkParam);
    
    /**
     * 检查设备违规情况
     * @param 
     * @return
     */
    String getMdmViolationResult();
    
    /**
     * 初始化当前应用使用沙盒功能.
     *
     * @param appIdentifier the app identifier
     * @return the int
     */
    int initSandbox(String appIdentifier);
    
    
    /**
     * 清除当前应用的沙盒数据.
     *
     * @return the int
     */
    int clearSandbox();
    
    
    /**
     * 删除指定应用沙盒中的所有数据.
     *
     * @param appIdentifier 待删除沙盒的应用包名
     * @return the int
     */
    int eraseSandboxFile(String appIdentifier);
    
    
//    /**
//     * 设置证书校验
//     * 
//     * @return 成功:0，失败 ：非0
//     */
//    int doCAChecking();
//    
//    /**
//     * 取消设置证书校验
//     * 
//     * @return 成功:0，失败 ：非0
//     */
//    int undoCAChecking();
    
    //void SVN_API_SetNetState(unsigned long ulNetState);
    void setNetState(int iNetState);

    CertificateInfo getCertificate(String username);
    
    
    String getAccountName();
}
