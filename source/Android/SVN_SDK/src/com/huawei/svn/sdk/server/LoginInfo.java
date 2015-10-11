/*
 * 
 */
package com.huawei.svn.sdk.server;

import com.huawei.svn.sdk.SvnConstants;

/**
 * 环境所需的参数实体
 * 
 * @author cKF63593
 */
public class LoginInfo
{

    /**
     * 网关IP
     */
    private String gatewayUrl;

    /**
     * 网关端口
     */
    private String gatewayPort = SvnConstants.UDP_SERVER_PORT;

    /**
     * 登录模式
     */
    private short tunnelMode = SvnConstants.SVN_TUNNEL_MODE_UDPS;

    /**
     * 登录用户名
     */
    private String userName;

    /**
     * 登录密码
     */
    private String password;

    /**
     * 代理类型
     */
    private short proxyType = 0;

    /**
     * 代理IP
     */
    private String proxyUrl = "";

    /**
     * 代理端口
     */
    private short proxyPort = 0;

    /**
     * 代理登录用户名
     */
    private String proxyUsername = "";

    /**
     * 代理登录密码
     */
    private String proxyPassword = "";

    /**
     * 代理域
     */
    private String proxyDomain = "";

    /**
     * 工作目录
     */
    private String strPackagePath;

    /**
     * 是否需要登录网关
     */
    private boolean loginGatewayFlag = true;

    /**
     * 是否使用http服务
     */
    private boolean enableHttpFlag = true;

    /**
     * 是否使用文件加解密服务
     */
    private boolean enableFsmFlag = false;

    /**
     * 设备Id
     */
    private String deviceId = "";

    /**
     * 文件加解密工作目录
     */
    private String fileEncDir = "";

    /**
     * 是否使用在线模式
     */
    private boolean onlineModeFlag = false;

    /**
     * 证书路径，用于校验设备证书
     */
    private String certPath = "";

    /**
     * 证书内容，用于校验设备证书，只支持传输BASE64编码的证书
     */
    private byte[] certContent = null;

    private short caChecking = 1;

    /**
     * 连接类型，0：表示SDK连接、 1：表示anyoffice连接、 2：表示MTM连接
     */
    private int connectType = 2;

    /**
     * 应用名,用于SDK连接
     */
    private String appName = "";

    /**
     * 设备的ID，用于SDK连接
     */
    private String authId = "";

    private String clientCert = "";

    private String clientKey = "";

    private String clientPassword = "";

    /**
     * 缓存本地文件路径
     */
    private String cachePath;

    /**
     * @return the clientCert
     */
    public String getClientCert()
    {
        return clientCert;
    }

    /**
     * @param clientCert
     *            the clientCert to set
     */
    public void setClientCert(String clientCert)
    {
        this.clientCert = clientCert;
    }

    /**
     * @return the clientKey
     */
    public String getClientKey()
    {
        return clientKey;
    }

    /**
     * @param clientKey
     *            the clientKey to set
     */
    public void setClientKey(String clientKey)
    {
        this.clientKey = clientKey;
    }

    /**
     * @return the clientPassword
     */
    public String getClientPassword()
    {
        return clientPassword;
    }

    /**
     * @param clientPassword
     *            the clientPassword to set
     */
    public void setClientPassword(String clientPassword)
    {
        this.clientPassword = clientPassword;
    }

    /**
     * 获取是否校验证书
     * 
     * @return 是否校验证书
     */
    public short getCAChecking()
    {
        return caChecking;
    }

    /**
     * 设置是否校验证书
     * 
     * @param cAChecking
     *            是否校验证书
     */
    public void setCAChecking(short cAChecking)
    {
        caChecking = cAChecking;
    }

    /**
     * 获取网关IP
     * 
     * @return 网关IP
     */
    public String getGatewayUrl()
    {
        return gatewayUrl;
    }

    /**
     * 设置网关IP
     * 
     * @param serverURL
     *            网关IP
     */
    public void setGatewayUrl(String serverURL)
    {
        this.gatewayUrl = serverURL;
    }

    /**
     * 获取网关端口
     * 
     * @return 网关端口
     */
    public String getGatewayPort()
    {
        return gatewayPort;
    }

    /**
     * 设置网关端口
     * 
     * @param gatewayPort
     *            网关端口
     */
    public void setGatewayPort(String gatewayPort)
    {
        this.gatewayPort = gatewayPort;
    }

    /**
     * 获取登录模式
     * 
     * @return 登录模式
     */
    public short getTunnelMode()
    {
        return tunnelMode;
    }

    /**
     * 设置登录模式
     * 
     * @param tunnelMode
     *            登录模式
     */
    public void setTunnelMode(short tunnelMode)
    {
        this.tunnelMode = tunnelMode;
    }

    /**
     * 获取登录用户名
     * 
     * @return 登录用户名
     */
    public String getUserName()
    {
        return userName;
    }

    /**
     * 设置登录用户名
     * 
     * @param userName
     *            登录用户名
     */
    public void setUserName(String userName)
    {
        this.userName = userName;
    }

    /**
     * 获取登录密码
     * 
     * @return 登录密码
     */
    public String getPassword()
    {
        return password;
    }

    /**
     * 设置登录密码
     * 
     * @param password
     *            登录密码
     */
    public void setPassword(String password)
    {
        this.password = password;
    }

    /**
     * 获取代理IP
     * 
     * @return 代理IP
     */
    public String getProxyUrl()
    {
        return proxyUrl;
    }

    /**
     * 设置代理IP
     * 
     * @param proxyUrl
     *            代理IP
     */
    public void setProxyUrl(String proxyUrl)
    {
        this.proxyUrl = proxyUrl;
    }

    /**
     * 获取代理端口
     * 
     * @return 代理端口
     */
    public short getProxyPort()
    {
        return proxyPort;
    }

    /**
     * 设置代理端口
     * 
     * @param proxyPort
     *            代理端口
     */
    public void setProxyPort(short proxyPort)
    {
        this.proxyPort = proxyPort;
    }

    /**
     * 获取代理类型
     * 
     * @return 代理类型
     */
    public short getProxyType()
    {
        return proxyType;
    }

    /**
     * 设置代理类型
     * 
     * @param proxyType
     *            代理类型
     */
    public void setProxyType(short proxyType)
    {
        this.proxyType = proxyType;
    }

    /**
     * 获取代理登录用户名
     * 
     * @return 代理登录用户名
     */
    public String getProxyUsername()
    {
        return proxyUsername;
    }

    /**
     * 设置代理登录用户名
     * 
     * @param proxyUserName
     *            代理登录用户名
     */
    public void setProxyUsername(String proxyUserName)
    {
        this.proxyUsername = proxyUserName;
    }

    /**
     * 获取代理登录密码
     * 
     * @return 代理登录密码
     */
    public String getProxyPwd()
    {
        return proxyPassword;
    }

    /**
     * 设置代理登录密码
     * 
     * @param proxyPassword
     *            代理登录密码
     */
    public void setProxyPwd(String proxyPassword)
    {
        this.proxyPassword = proxyPassword;
    }

    /**
     * 获取代理域
     * 
     * @return 代理域
     */
    public String getAttestDomain()
    {
        return proxyDomain;
    }

    /**
     * 设置代理域
     * 
     * @param proxyDomain
     *            代理域
     */
    public void setAttestDomain(String proxyDomain)
    {
        this.proxyDomain = proxyDomain;
    }

    /**
     * 获取工作目录
     * 
     * @return 工作目录
     */
    public String getPackagePath()
    {
        return strPackagePath;
    }

    /**
     * 设置工作目录
     * 
     * @param strPackagePath
     *            工作目录
     */
    public void setPackagePath(String strPackagePath)
    {
        this.strPackagePath = strPackagePath;
    }

    /**
     * 设置设备Id
     * 
     * @param deviceId
     *            设备Id
     */
    public void setDeviceId(String deviceId)
    {
        this.deviceId = deviceId;
    }

    /**
     * 获取设备Id
     * 
     * @return 设备Id
     */
    public String getDeviceId()
    {
        return deviceId;
    }

    /**
     * 设置文件加解密工作目录
     * 
     * @param fileEncDir
     *            文件加解密工作目录
     */
    public void setFileEncDir(String fileEncDir)
    {
        if (null != fileEncDir && fileEncDir.endsWith("/"))
        {
            fileEncDir = fileEncDir.substring(0, fileEncDir.length() - 1);
        }
        this.fileEncDir = fileEncDir;
    }

    /**
     * 获取文件加解密工作目录
     * 
     * @return 文件加解密工作目录
     */
    public String getFileEncDir()
    {
        return fileEncDir;
    }

    /**
     * 获取是否需要登录网关
     * 
     * @return 是否需要登录网关
     */
    public boolean getLoginGatewayFlag()
    {
        return loginGatewayFlag;
    }

    /**
     * 设置是否需要登录网关
     * 
     * @param loginGatewayFlag
     *            是否需要登录网关
     */
    public void setLoginGatewayFlag(boolean loginGatewayFlag)
    {
        this.loginGatewayFlag = loginGatewayFlag;
    }

    /**
     * 获取是否使用http服务
     * 
     * @return 是否使用http服务
     */
    public boolean getEnableHttpFlag()
    {
        return enableHttpFlag;
    }

    /**
     * 设置是否使用http服务
     * 
     * @param enableHttpFlag
     *            是否使用http服务
     */
    public void setEnableHttpFlag(boolean enableHttpFlag)
    {
        this.enableHttpFlag = enableHttpFlag;
    }

    /**
     * 获取是否使用文件加解密服务
     * 
     * @return 是否使用文件加解密服务
     */
    public boolean getEnableFsmFlag()
    {
        return enableFsmFlag;
    }

    /**
     * 设置是否使用文件加解密服务
     * 
     * @param enableFsmFlag
     *            是否使用文件加解密服务
     */
    public void setEnableFsmFlag(boolean enableFsmFlag)
    {
        this.enableFsmFlag = enableFsmFlag;
    }

    /**
     * 获取是否使用在线模式
     * 
     * @return 是否使用在线模式
     */
    public boolean getOnlineModeFlag()
    {
        return onlineModeFlag;
    }

    /**
     * 设置是否使用在线模式
     * 
     * @param onlineModeFlag
     *            是否使用在线模式
     */
    public void setOnlineModeFlag(boolean onlineModeFlag)
    {
        this.onlineModeFlag = onlineModeFlag;
    }

    /**
     * 获取证书路径，用于校验设备证书
     * 
     * @return 证书路径，用于校验设备证书
     */
    public String getCertPath()
    {
        return certPath;
    }

    /**
     * 设置证书路径，用于校验设备证书
     * 
     * @param certPath
     *            证书路径，用于校验设备证书
     */
    public void setCertPath(String certPath)
    {
        this.certPath = certPath;
    }

    /**
     * 获取证书内容，用于校验设备证书，只支持传输BASE64编码的证书
     * 
     * @return 证书内容，用于校验设备证书，只支持传输BASE64编码的证书
     */
    public byte[] getCertContent()
    {
        if (certContent != null && certContent.length > 0)
        {
            byte[] result = new byte[certContent.length];
            System.arraycopy(certContent, 0, result, 0, certContent.length);
            return result;
        }
        return new byte[]{};
    }

    /**
     * 设置证书内容，用于校验设备证书，只支持传输BASE64编码的证书
     * 
     * @param certContent
     *            证书内容，用于校验设备证书，只支持传输BASE64编码的证书
     */
    public void setCertContent(byte[] certContent)
    {
        if (certContent != null && certContent.length > 0)
        {
            this.certContent = new byte[certContent.length];
            System.arraycopy(certContent, 0, this.certContent, 0,
                    certContent.length);
        }
        else
        {
            this.certContent = null;
        }
    }

    public String getAuthId()
    {
        return authId;
    }

    public void setAuthId(String authId)
    {
        this.authId = authId;
    }

    public String getAppName()
    {
        return appName;
    }

    public void setAppName(String appName)
    {
        this.appName = appName;
    }

    public int getConnectType()
    {
        return connectType;
    }

    public void setConnectType(int connectType)
    {
        this.connectType = connectType;
    }

    public String getCachePath()
    {
        if (null != fileEncDir)
        {
            return fileEncDir + "/" + cachePath;
        }
        else
        {
            return null;
        }
    }

    public void setCacheName(String cacheName)
    {
        this.cachePath = cacheName;
    }
}
