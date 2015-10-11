package com.huawei.svn.sdk.server;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.huawei.svn.sdk.SvnConstants;
import com.huawei.svn.sdk.fsm.SvnFileTool;
import com.huawei.svn.sdk.socket.SvnSocket;

/**
 * API接口类
 * 
 * @see 提供API编程接口的类
 * @author lWX80847
 */
public final class SvnApiService
{
    private static String TAG = "SvnApiService";
    /**
     * 回调对象
     */
    private static SvnCallBack iCallBack = null;
    /**
     * SvnClientApi接口实现
     */
    private static SvnClientApiImpl client = new SvnClientApiImpl();
    /**
     * 隧道创建参数
     */
    private static LoginInfo loginInfo = new LoginInfo();
    /**
     * 返回码
     */
    private static int iSvnErrorCode = SvnConstants.SVN_OK;
    
    
//    public static final String VERSION = "Version";
//    public static final String ENCCONTEXT = "EncContext";
//    public static final String SVNSERVER = "SvnServer";
//    public static final String SVNSERVERBACKUP = "SvnServerBackup";
//    public static final String URLENCODED = "UrlEncoded";
//    public static final String AUTH = "auth";
//    public static final String SOURCE = "source";
//    public static final String USERNAME = "user_name";
//    public static final String PASSWORD = "password";
//    public static final String SVNPARAMS = "SvnParams";
//    public static final String RETURNCODE = "ReturnCode";
//    public static final String PACKAGENAME = "PackageName";
//    public static final String HOMEACTIVITY = "HomeActivity";
//    public static final String TERMINALID = "TerminalID";
    
    
    /**
     * 单点登录传递过来的登录信息
     */
//    private static CacheLoginInfo mCacheLoginInfo;
//    private static Thread autoThread;
//    private static boolean autoConnect = true;
    private static Context mContext;
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
     * 私有构造函数
     */
    private SvnApiService()
    {
    }

    /**
     * 设置回调对象
     * 
     * @param paramCallBack
     *            回调对象
     */
    public static void setCallBack(SvnCallBack paramCallBack)
    {
        SvnApiService.iCallBack = paramCallBack;
    }

    /**
     * 设置工作路径
     * 
     * @param workPath
     *            工作路径 ，为"/data/data"+文件包名
     * @return int 0:成功
     */
    private static int setWorkingDir(String workPath)
    {
        if (null == workPath)
        {
            return SvnConstants.SVN_WORKPATH_ERROR;
        }
        File file = new File(workPath);
        if (!file.exists() || !file.canRead() || !file.canWrite())
        {
            return SvnConstants.SVN_WORKPATH_ERROR;
        }
        // 设置本地文件啊缓存路径
        return client.setWorkingDir(workPath);
    }

    /**
     * MTM连接模式登陆网关，初始化组件
     * 
     * @param username
     *            ： 用户名
     * @param passwd
     *            ：登陆的密码
     * @param gateway
     *            ：登陆的网关地址
     * @param strPackagePath
     *            ：工作路径 ，为"/data/data"+文件包名
     * @return 返回0:登陆成功 其他:失败
     */
    public static int login(String username, String passwd, String gateway,
            String strPackagePath)
    {
        setConnectType(SvnConstants.SVN_CONNECT_TYPE_MTM);
        setAppName("");
        setDeviceId("");
        /* BEGIN: Modified by zhaixianqi for DTS2013071101158 */
        return privateLogin(username, passwd, gateway, strPackagePath, null);
        /* END: Modified by zhaixianqi for DTS2013071101158 */
    }

    /**
     * SDK连接模式登陆网关，初始化组件 ,只用于建立安全SDK连接
     * 
     * @param username
     *            ： 用户名
     * @param passwd
     *            ：登陆的密码
     * @param gateway
     *            ：登陆的网关地址
     * @param strPackagePath
     *            ：工作路径 ，为"/data/data"+文件包名\
     * @param appName
     *            :应用名
     * @param deviceId
     *            ：设备ID 获取方法： ((TelephonyManager)
     *            getSystemService(TELEPHONY_SERVICE)).getDeviceId();
     * @return 返回0:登陆成功 其他:失败
     */
    public static int login(String username, String passwd, String gateway,
            String strPackagePath, String appName, String deviceId)
    {
        if (null == appName || null == deviceId)
        {
            return SvnConstants.SVN_PARAM_ERROR;
        }
        setConnectType(SvnConstants.SVN_CONNECT_TYPE_SDK);
        setAppName(appName);
        setDeviceId(deviceId);
        /* BEGIN: Modified by zhaixianqi for DTS2013071101158 */
        return privateLogin(username, passwd, gateway, strPackagePath, null);
        /* END: Modified by zhaixianqi for DTS2013071101158 */
    }

    /**
     * MTM连接模式登陆网关，初始化组件
     * 
     * @param username
     *            用户名
     * @param passwd
     *            登陆的密码
     * @param gateway
     *            登陆的网关地址
     * @param strPackagePath
     *            工作路径 ，为"/data/data"+文件包名
     * @param certFilename
     *            证书路径，用于校验设备证书
     * @return 返回0:登陆成功 其他:失败
     */
    public static int login(String username, String passwd, String gateway,
            String strPackagePath, String certFilename)
    {
        setConnectType(SvnConstants.SVN_CONNECT_TYPE_MTM);
        setAppName("");
        setDeviceId("");
        return privateLogin(username, passwd, gateway, strPackagePath,
                certFilename);
    }

    /**
     * 直接登陆网关，初始化组件
     * 
     * @param username
     *            用户名
     * @param passwd
     *            登陆的密码
     * @param gateway
     *            登陆的网关地址
     * @param strPackagePath
     *            工作路径 ，为"/data/data"+文件包名
     * @param certFilename
     *            证书路径，用于校验设备证书
     * @return 返回0:登陆成功 其他:失败
     */
    private static int privateLogin(String username, String passwd,
            String gateway, String strPackagePath, String certFilename)
    {
        int loginState = 0;
        int times = 0;
        iSvnErrorCode = SvnConstants.SVN_OK;
        if (0 != setWorkingDir(strPackagePath))
        {
            Log.e("SDK", "setWorkingDir error!");
            return SvnConstants.SVN_WORKPATH_ERROR;
        }
        if (0 != client.initEnv())
        {
            // System.out.println("HTTP initEnv error!");
            return SvnConstants.SVN_HTTPINIT_ERROR;
        }
        loginInfo.setUserName(username);
        loginInfo.setPassword(passwd);
        loginInfo.setGatewayUrl(gateway);
        if (null != certFilename)
        {
            loginInfo.setCertPath(certFilename);
        }
        client.login(loginInfo);
        for (;;)
        {
            loginState = client.getVPNStatus();
            if (SvnConstants.VPN_LOGIN_SUC_STATUS != loginState)
            {
                Log.i("SDK", String.valueOf(loginState));
                if (30 == times || SvnConstants.SVN_OK != iSvnErrorCode)//重试30次
                {
                    Log.e("SDK", "HTTP  login the SVN overtime!! !");
                    client.logout();
                    break;
                }
                times = times + 1;
                try
                {
                    Thread.sleep(1000);
                }
                catch (InterruptedException e)
                {
                    //                    Log.e("SDK", e.getMessage());
                    e.printStackTrace();
                }
                continue;
            }
            /*
             * BEGIN:Added by zhaixianqi 90006553 2013-08-12 for
             * 增加设置WebView使用VPN隧道
             */
            setWebViewUseVpn(true);
            /*
             * END:Added by zhaixianqi 90006553 2013-08-12 for
             * 增加设置WebView使用VPN隧道
             */
            break;
        }
        Log.i("SDK", "after login get vpn status is : " + loginState);
        if (loginInfo.getEnableFsmFlag())
        {
            int ret = SvnFileTool.initFsmEnv(loginInfo);
            if (0 != ret)
            {
                Log.e("SDK", "File Encrypt Init Error.");
                return SvnConstants.SVN_FILEINIT_ERROR;
            }
        }
        if (SvnConstants.VPN_LOGIN_SUC_STATUS == loginState)
        {
            return iSvnErrorCode;
        }
        else if (SvnConstants.SVN_OK == iSvnErrorCode)
        {
            return SvnConstants.SVN_DEFAULT_ERROR;
        }
        else
        {
            return iSvnErrorCode;
        }
    }

    // /**
    // * 设置代理
    // *
    // * @param proxyType
    // * 代理类型
    // * @param proxyUrl
    // * 代理URL地址
    // * @param proxyPort
    // * 代理端口
    // * @param proxyUserName
    // * 代理用户名
    // * @param proxyPwd
    // * 代理密码
    // * @param attestDomain
    // * 代理域
    // */
    // public static void setProxy(short proxyType, String proxyUrl,
    // short proxyPort, String proxyUserName, String proxyPwd,
    // String attestDomain)
    // {
    // loginInfo.setProxyType(proxyType);
    // loginInfo.setProxyUrl(proxyUrl);
    // loginInfo.setProxyUsername(proxyUserName);
    // loginInfo.setProxyPwd(proxyPwd);
    // loginInfo.setProxyPort(proxyPort);
    // loginInfo.setAttestDomain(attestDomain);
    // }
    /**
     * 设置文件加解密
     * 
     * @param deviceId
     *            设备ID
     * @param fileEncDir
     *            文件加密路径
     * @param loginGatewayFlag
     *            是否需要登录网关
     */
    public static void setFileEnable(String deviceId, String fileEncDir,
    /* boolean onlineModeFlag, */boolean loginGatewayFlag)
    {
        loginInfo.setEnableFsmFlag(true);
        loginInfo.setDeviceId(deviceId);
        loginInfo.setFileEncDir(fileEncDir);
        // loginInfo.setOnlineModeFlag(onlineModeFlag);
        loginInfo.setLoginGatewayFlag(loginGatewayFlag);
        // 设置本地文件啊缓存路径
        loginInfo.setCacheName(SvnConstants.CACHE_LOGININFO_NAME);
    }

    public static void resetFileEncPath(String fileEncPath)
    {
        if (loginInfo.getEnableFsmFlag())
        {
            SvnFileTool.cleanFileEncEnv();
            loginInfo.setFileEncDir(fileEncPath);
            SvnFileTool.initFsmEnv(loginInfo);
        }
    }

    public static void cleanFileEncEnv()
    {
        SvnFileTool.cleanFileEncEnv();
    }

    /**
     * 设置证书路径，支持DER和BASE64编码
     * 
     * @param certPath
     *            证书全路径
     */
    public static void setCertPath(String certPath)
    {
        loginInfo.setCertPath(certPath);
    }

    /**
     * 设置证书内容，进行设备证书校验，仅支持BASE64编码
     * 
     * @param certContent
     *            证书内容
     */
    public static void setCertContent(byte[] certContent)
    {
        loginInfo.setCertContent(certContent);
    }

    /**
     * 设置隧道模式
     * 
     * @param tunnelMode
     *            隧道模式
     */
    public static void setTunnelMode(short tunnelMode)
    {
        loginInfo.setTunnelMode(tunnelMode);
    }

    /**
     * 设置网关端口
     * 
     * @param port
     *            网关端口
     * 
     */
    public static void setGatewayPort(short port)
    {
        loginInfo.setGatewayPort("" + port);
    }

    /**
     * 不登录进行初始化
     * 
     * @param username
     *            用户名
     * @param passwd
     *            密码，采用离线模式时时可以为null
     * @param gateway
     *            SVN网关地址，采用离线模式时时可以为null
     * @param strPackagePath
     *            工作路径，为"/data/data"+文件包名
     * @return 返回0:初始化成功 其他:失败
     */
    public static int initWithoutLogin(String username, String passwd,
            String gateway, String strPackagePath)
    {
        if (loginInfo.getLoginGatewayFlag() || !loginInfo.getEnableFsmFlag())
        {
            Log.e("SDK", "Error Need Login, Please call login function.");
            return SvnConstants.SVN_NEEDLOGIN_ERROR;
        }
        if (null == username)
        {
            Log.e("SDK", "Error Param.");
            return SvnConstants.SVN_PARAM_ERROR;
        }
        if (0 != setWorkingDir(strPackagePath))
        {
            Log.e("SDK", "setWorkingDir error!");
            return SvnConstants.SVN_WORKPATH_ERROR;
        }
        if (0 != client.initEnv())
        {
            Log.e("SDK", "HTTP initEnv error!");
            return SvnConstants.SVN_HTTPINIT_ERROR;
        }
        loginInfo.setUserName(username);
        loginInfo.setPassword(passwd);
        loginInfo.setGatewayUrl(gateway);
        int ret = SvnFileTool.initFsmEnv(loginInfo);
        if (0 != ret)
        {
            Log.e("SDK", "File Encrypt Init Error.");
            return SvnConstants.SVN_FILEINIT_ERROR;
        }
        Log.d(TAG, "Encrypt Init OK.");
        return SvnConstants.SVN_OK;
    }




//    /**
//     * AnyOffice联动登录
//     * 
//     * @param context Android应用上下文
//     * @param bundle 单点登录传递过来的参数携带者
//     * @param fileEncDir
//     *            设置临时路径
//     * @param autologin
//     *            是否支持离线登录,true，支持，false不支持，默认false
//     * @return 成功返回 SvnConstants.SVN_OK;
//     *               离线登录返回 SvnConstants.SVN_OFFLINE_LOGIN;
//     *               正在连接返回SvnConstants.SVN_LOGIN_CONNECTING
//     *              登录失败返回其他负数错误码
//     */
//    public static int loginFromAnyOffice(final Context context, Bundle bundle,
//            String workPath, String loginTitle)
//    {
//        mContext = context;
//        Log.i(TAG, "login from anyoffice");
//        // 检查SVN的状态，如果在线，则校验单点登录传递的用 户名与本地缓存用户名是否一致；否则创建新的隧道。
//        int vpnStatus = getVPNStatus();
//        if(vpnStatus == 1)
//        {
//            Log.i(TAG, "login from anyoffice:tunnel already online");
//          
//            return 0;
//        }
//        
//        
//        File f = new File(workPath);
//        if (!f.exists())
//        {
//            if(!f.mkdir())
//            {
//                Log.e(TAG, "login from anyoffice:workPath error.");
//                return SvnConstants.SVN_PARAM_ERROR;
//            }
//        }
//       
//        String version = bundle.getString(VERSION);
//        String svnServer = bundle.getString(SVNSERVER);
//        String SvnServerBackup = bundle
//                .getString(SVNSERVERBACKUP);
//        String UrlEncoded = bundle.getString(URLENCODED);
//        String Auth = bundle.getString(AUTH);
//        String Source = bundle.getString(SOURCE);
//        String user_name = bundle.getString(USERNAME);
//        String password = bundle.getString(PASSWORD);
//        String SvnParams = bundle.getString(SVNPARAMS);
//        String ReturnCode = bundle.getString(RETURNCODE);
//        String PackageName = bundle.getString(PACKAGENAME);//com.huawei.svn.hiwork
//        String HomeActivity = bundle.getString(HOMEACTIVITY);//com.huawei.anyoffice.home.WelcomeActivity
//        String TerminalID = bundle.getString(TERMINALID);
//        Log.i(TAG, "login from anyoffice:" + TerminalID);
//        
//        if (user_name == null || password == null || svnServer == null)
//        {
//            Log.e(TAG, "user_name of GetLoginInfoFromAnyoffice is null !");
//            return SvnConstants.SVN_PARAM_ERROR;
//        }
//        if ("true".equals(UrlEncoded))
//        {
//            try
//            {
//                user_name = URLDecoder.decode(user_name, "UTF-8");
//                password = URLDecoder.decode(password, "UTF-8");
//            }
//            catch (UnsupportedEncodingException e)
//            {
//                Log.e(TAG, "UnsupportedEncodingException !");
//                return SvnConstants.SVN_PARAM_ERROR;
//            }
//        }
//        String[] svnServerIp = svnServer.split(":");
// 
//
//        Log.i(TAG, "Begin to init sdk envirenment");
//        boolean inited = SDKContext.getInstance().init(context, workPath);
//        
//        if(!inited)
//        {
//            return SvnConstants.INIT_ENV_FAILED;
//        }
//        
//        // 初始化登录参数
//        LoginParam loginParam = new LoginParam();
//        // 设置应用的业务类型，
//        loginParam.setServiceType("SDK");
//        loginParam.setLoginTitle(loginTitle);
//        loginParam.setAutoLoginType(AutoLoginType.auto_login_enable);
//        loginParam.setLoginBackground(true);
//        // 设置AnyOffice网关地址
//        loginParam.setInternetAddress(new InetSocketAddress(
//                svnServerIp[0], 443));
////        loginParam.setIntranetAddress(new InetSocketAddress(
////                "4.1.16.20", 443));
//        // 设置用户信息
//        LoginParam.UserInfo userInfo = loginParam.new UserInfo();
//        userInfo.userName = user_name;
//        userInfo.password = password;
//        loginParam.setUserInfo(userInfo);
//        loginParam.setUseSecureTransfer(true);
//        Log.i(TAG,"Begin to login gateway");
//        int ret = LoginAgent.getInstance().loginSync(
//                context, loginParam);
//        Log.i(TAG, "login result " + ret);
//        return ret;
//    }

   
    /**
     * 获取登录状态
     * 
     * @return SVN登录状态
     */
    public static int getVPNStatus()
    {
        return client.getVPNStatus();
    }

    /**
     * 获取用户IP地址
     * 
     * @return 虚拟地址
     */
    public static String getIpAddress()
    {
        return client.getIpAddress();
    }

    /**
     * 拉起AnyOffice登录页面
     * 
     * @param context
     *            上下文
     * @param cacheLoginInfo
     *            单点登录传递过来的登录是数据
     * @return 成功返回 SvnConstants.SVN_STATRT_ANYOFFICE_SUCCESS，失败返回
     *         SvnConstants.SVN_STATRT_ANYOFFICE_FAIL.
     */
    public static void startAnyoffice(Context context)
    {
        if (null == context)
        {
            return;
        }
        String packageName = "com.huawei.svn.hiwork";
        String homeActivity = "com.huawei.anyoffice.home.WelcomeActivity";
        Intent intent = new Intent();
        intent.setClassName(packageName, homeActivity);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
    }

    /**
     * 退出登录网关
     * 
     * @return SVN登录状态
     */
    public static int logout()
    {
        /* BEGIN: Modified by zhaixianqi 2013-08-12 for 销毁隧道时设置WebView不使用VPN */
        setWebViewUseVpn(false);
        /* END: Modified by zhaixianqi 2013-08-12 for 销毁隧道时设置WebView不使用VPN */
        /* BEGIN: Modified by zhaixianqi for DTS2013071000834 */
        return client.logout();
        /* END: Modified by zhaixianqi for DTS2013071000834 */
    }

    /**
     * SDK环境去初始化
     */
    public static void exitEnv()
    {
        if (loginInfo.getEnableFsmFlag())
        {
            cleanFsm();
        }
        if (loginInfo.getEnableHttpFlag())
        {
            cleanup();
        }
        client.exitEnv();
    }

    /**
     * 文件加解密环境去初始化
     */
    private static void cleanFsm()
    {
        SvnFileTool.cleanFileEncEnv();
    }

    /**
     * 设置写日志的路径和级别
     * 
     * @param savePath
     *            日志保存的路径
     * @param level
     *            保存日志的级别
     * @return 成功:0，失败 ：非0
     * @deprecated
     */
    public static int setLogParam(String savePath, int level)
    {
        return client.setLogParam(savePath, level);
    }

    /**
     * 去http初始化
     */
    private static void cleanup()
    {
        //HttpApi.cleanup();
    }

    /**
     * 加密数据内容
     * 
     * @param oriContent
     *            未加密内容
     * @return 返回加密后的内容
     */
    public static byte[] encryptContent(byte[] oriContent)
    {
        return SvnBigStringOpterations.encryptLarge(oriContent);
    }

    /**
     * 解密数据内容
     * 
     * @param encContent
     *            已加密数据
     * @return 返回解密后的内容
     */
    public static byte[] decryptContent(byte[] encContent)
    {
        return SvnBigStringOpterations.decryptLarge(encContent);
    }

    /**
     * jni层回调使用
     * 
     * @param strLog
     *            日志信息
     * @param iLevel
     *            日志级别
     * @param iStatus
     *            SVN隧道状态
     * @param iErrorCode
     *            SVN隧道错误码
     * @param flag
     *            常量WRITE_FLAG表示日志回调，STATUS_FLAG表示状态回调
     */
    protected static void callBack(String strLog, int iLevel, int iStatus,
            int iErrorCode, int flag)
    {
        if (SvnConstants.WRITE_FLAG == flag)
        {
            if (null != iCallBack && null != strLog)
            {
                iCallBack.writeLog(strLog, iLevel);
            }
          
        }
        else if (SvnConstants.STATUS_FLAG == flag)
        {
            iSvnErrorCode = iErrorCode;
          
            if (null != iCallBack)
            {
                iCallBack.statusNotify(iStatus, iErrorCode);
            }
        }
    }

    /**
     * 设置SDK校验设备证书
     * 
     * @return 设置成功(0) 或失败(1)
     */
    public static int doCAChecking()
    {
        loginInfo.setCAChecking((short) 1);
        return 0;
    }

    /**
     * 设置SDK不校验设备证书
     * 
     * @return 设置成功(0) 或失败(1)
     */
    public static int undoCAChecking()
    {
        loginInfo.setCAChecking((short) 0);
        return 0;
    }

    /**
     * 设置应用名。用于SDK连接
     * 
     * @param appName
     *            :应用名
     */
    private static void setAppName(String appName)
    {
        loginInfo.setAppName(appName);
    }

    /**
     * 设置设备ID，用于SDK连接
     * 
     * @param authId
     */
    private static void setDeviceId(String deviceId)
    {
        loginInfo.setAuthId(deviceId);
    }

    /**
     * 连接类型， 0：表示SDK连接、 2：表示MTM连接
     */
    private static void setConnectType(int connectType)
    {
        loginInfo.setConnectType(connectType);
    }

    /**
     * 设置WebView是否使用VPN
     * 
     * @param isUse
     *            ：true:使用VPN， false:不使用VPN
     * @deprecated
     */
    public static void setWebViewUseVpn(boolean isUse)
    {
        //        if (isUse)
        //        {
        //            SvnWebView.registerUseL4VPN();
        //        }
        //        else
        //        {
        //            SvnWebView.unregisterUseL4VPN();
        //        }
    }

    public static void importClientCert(String clientCertPath,
            String clientKeyPath, String clientPassword)
    {
        //if(null == clientCertPath || null == clientKeyPath || null == clientPassword )
        if (null == clientCertPath || null == clientKeyPath)
        {
            return;
        }
        String clientCert = "";
        String clientKey = "";
        FileInputStream fi = null;
        FileInputStream keyFi = null;
        try
        {
            fi = new FileInputStream(clientCertPath);
            int length = fi.available();
            if (length > 0)
            {
                byte[] buffer = new byte[length];
                int count = 0;
                while (count < length)
                {
                    int rc = fi.read(buffer, count, length - count);
                    if (rc > 0)
                    {
                        count += rc;
                    }
                    else if (rc < 0)
                    {
                        break;
                    }
                }
                if (count == length)
                {
                    clientCert = new String(buffer, "ISO_8859_1");
                }
            }
            //            fi.close();
            //            fi = null;
        }
        catch (IOException e)
        {
            Log.e("SDK", "Read client cert error !!!");
            return;
        }
        finally
        {
            if (fi != null)
            {
                try
                {
                    fi.close();
                }
                catch (IOException e1)
                {
                    Log.e("SDK", "close fileinputStream fi error!!");
                }
                fi = null;
            }
        }
        try
        {
            keyFi = new FileInputStream(clientKeyPath);
            int keylenth = keyFi.available();
            if (keylenth > 0)
            {
                byte[] keybuffer = new byte[keylenth];
                int count = 0;
                while (count < keylenth)
                {
                    int rc = keyFi.read(keybuffer, count, keylenth - count);
                    if (rc > 0)
                    {
                        count += rc;
                    }
                    else if (rc < 0)
                    {
                        break;
                    }
                }
                if (count == keylenth)
                {
                    clientKey = new String(keybuffer, "ISO_8859_1");
                }
            }
            //            keyFi.close();
            //            keyFi = null;
        }
        catch (IOException e)
        {
            Log.e("SDK", "Read client cert error !!!");
            return;
        }
        finally
        {
            if (keyFi != null)
            {
                try
                {
                    keyFi.close();
                }
                catch (IOException e1)
                {
                    Log.e("SDK", "close fileinputStream keyFi error!!");
                }
                keyFi = null;
            }
        }
        loginInfo.setClientCert(clientCert);
        loginInfo.setClientKey(clientKey);
        loginInfo.setClientPassword(clientPassword);
    }

    /**
     * 初始化应用使用文件沙盒
     * 
     * @param appIdentifier
     *            应用ID
     */
    public static int initSandbox(String appIdentifier)
    {
        return client.initSandbox(appIdentifier);
    }

    /**
     * 清除所有沙盒文件
     * 
     * @param appIdentifier
     *            应用ID
     */
    public static int clearSandbox()
    {
        return client.clearSandbox();
    }

    /**
     * 清除某一应用沙盒文件
     * 
     * @param appIdentifier
     *            应用ID
     */
    public static int eraseSandboxFile(String appIdentifier)
    {
        return client.eraseSandboxFile(appIdentifier);
    }

    /**
     * 设置是否有可用网络连接
     * @param isAvailable 是否有可用网络连接
     */
    public static void setNetworkAvailable(boolean isAvailable)
    {
        client.setNetState(isAvailable ? SvnConstants.USEFUL_NETWORK
                : SvnConstants.UNUSEFUL_NETWORK);
    }

    /**
     * 解析URL，获得对应的IP地址列表
     * @param strURL 域名URL
     * @return IP地址列表
     */
    public static String[] parseURL(String strURL)
    {
        String[] hostNotFound =
        {};
        if (null == strURL)
        {
            return hostNotFound;
        }
        int[] ipArray = client.parseURL(strURL);
        if (ipArray != null && ipArray.length > 0)
        {
            String[] result = new String[ipArray.length];
            for (int i = 0; i < ipArray.length; i++)
            {
                result[i] = SvnSocket.getIpAddrString(ipArray[i]);
            }
            return result;
        }
        return hostNotFound;
    }
    
    
    public static CertificateInfo getCertificate(String username)
    {
        if(username == null)
        {
            username = "";
        }
        return client.getCertificate(username);
    }
    
    
    public static String getAccountName()
    {
        return client.getAccountName();
    }
}
