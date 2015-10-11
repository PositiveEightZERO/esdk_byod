/*
 * 
 */
package com.huawei.svn.sdk;

/**
 * 全局常量.
 * 
 * @author l00174413
 */
public class SvnConstants
{

	public final static String CACHE_LOGININFO_NAME = "cacheLoginInfo.xml";
	
    /**
     * ************************************************************************
     * BEGIN: 隧道常量定义 *
     * ************************************************************************.
     */

    /**
     * 标志 VPN连接状态--成功
     */
    public final static Short VPN_LOGIN_SUC_STATUS = 1;

    /** 
     * 标志VPN隧道正在--连接
     */
    public final static Short VPN_LOGIN_CONNECTING_STATUS = 2;

    /** 
     * 网关端口
     */
    public static final String UDP_SERVER_PORT = "443";

    /** 
     * 初始化HTTP或文件加解密环境时，没有L4Vpn错误
     */
    public static final int INIT_ENV_WITH_NO_L4VPN = 100;

    /** 
     * 初始化环境成功
     */
    public static final int INIT_ENV_OK = 0;

    /** 
     * 初始化环境失败
     */
    public static final int INIT_ENV_FAILED = 1;

    /** 
     * 隧道模式：UDPS模式，默认模式
     */
    public final static short TUNNEL_MODE_UDPS = 3;

    /** 
     * 隧道模式：TLS模式
     */
    public final static short TUNNEL_MODE_TLS = 1;

    /**
     * ************************************************************************
     * END: 隧道常量定义 *
     * ************************************************************************.
     */

    /**************************************************************************
     * BEGIN: 文件常量定义 *
     **************************************************************************/

    /**
     * 读模式
     */
    public final static int OPER_READ = 4;

    /** 
     * 写模式
     */
    public final static int OPER_WRITE = 2;

    /** 
     * 执行模式
     */
    public final static int OPER_EXEC = 1;

    /** 
     * 文件存在
     */
    public final static int F_OK = 0;

    /** 
     * 读模式
     */
    public final static String OPER_READ_STR = "r";

    /** 
     * 写模式
     */
    public final static String OPER_WRITE_STR = "w";

    /** 
     * 执行模式
     */
    public final static String OPER_EXEC_STR = "x";

    /** 
     * 追加模式
     */
    public final static String OPER_APPEND = "a";

    /** 
     * 文件结尾
     */
    public final static int FILE_EOF = -1;

    /** 
     * 文件比较相差最大值
     */
    public final static int SVN_FILE_BIG = 1024;

    /**
     * ************************************************************************
     * END: 文件常量定义 *
     * ************************************************************************.
     */

    /**
     * 写日志回调标识
     */
    public static final int WRITE_FLAG = 1;

    /** 
     * 状态回调标识
     */
    public static final int STATUS_FLAG = 2;

    /** 
     * 返回成功
     */
    public static final int SVN_OK = 0;
    
    /** 
     * 返回失败
     */
    public static final int SVN_ERR = 1;

    /** 
     * 不使用加密传输
     */
    public static final int NONE_SVN_TRANS = 1;
    
    /** 
     * 使用加密传输
     */
    public static final int OPEN_SVN_TRANS = 0;

    /** 
     * 日志开关-关日志
     */
    public static final int NONE_LOG = 0;
    
    /** 
     * 日志开关-开日志
     */
    public static final int OPEN_LOG = 1;

    /** 
     * 网络状态-网络可用
     */
    public static final int USEFUL_NETWORK = 1;
    
    /** 
     * 网络状态-网络不可用
     */
    public static final int UNUSEFUL_NETWORK = 0;

    /** 
     * 清理标志-清理隧道
     */
    public static final int TUNNLE_CLEAN = 0;
    
    /** 
     * 清理标志-清理环境
     */
    public static final int ENV_CLEAN = 1;

    /** 
     * android平台标志
     */
    public static final int VER_ANDROID = 0;

    /** 
     * 代理设置-无代理
     */
    public static final short SVN_PROXY_NONE = 0;
    
    /** 
     * 代理设置-HTTP代理
     */
    public static final short SVN_PROXY_HTTP = 1;
    
    /** 
     * 代理设置-SOCKS5代理
     */
    public static final short SVN_PROXY_SOCKS5 = 2;
    
    /** 
     * 代理设置-其他类型
     */
    public static final short SVN_PROXY_UNKNOWN = 3;

    /** 
     * 隧道模式-DTLS
     */
    public static final short SVN_TUNNEL_MODE_DTLS = 0;
    
    /** 
     * 隧道模式-TLS
     */
    public static final short SVN_TUNNEL_MODE_TLS = 1; 
    
    /** 
     * 隧道模式-UDP
     */
    public static final short SVN_TUNNEL_MODE_UDP = 2; 
    
    /** 
     * 隧道模式-TLS + UDPS
     */
    public static final short SVN_TUNNEL_MODE_UDPS = 3; 

    /* 错误码 */    
    /** 
     * 创建TCP连接失败，请检查网络连接和网关的地址和端口是否正确。对于移动终端需要确保网络已激活
     */
    public static final int SVN_TCP_CONNECT_ERR = -1;      
    
    /** 
     * 与代理服务建立连接失败，请检查网络和代理服务器地址和端口是否正确
     */
    public static final int SVN_PROXY_CONNECT_ERR = -2;    
    
    /** 
     * 代理信息错误，请检查代理用户名、密码、域信息是否正确
     */
    public static final int SVN_PROXY_INFO_ERR = -3;      
    
    /** 
     * TLS握手失败
     */
    public static final int SVN_TLS_HANDSHAKE_ERR = -4;      
    
    /** 
     * 登录SVN的用户名、密码错误
     */
    public static final int SVN_USER_INFO_ERR = -5;      
    
    /** 
     * 无法获取虚拟IP
     */
    public static final int SVN_VIP_UNAVAILABLE = -6;    
    
    /** 
     * 用户数达到上线
     */
    public static final int SVN_USER_EXCEED_LIMIT = -7;  
    
    /** 
     * 用户IP受限
     */
    public static final int SVN_USER_IP_DENY = -8;      
    
    /** 
     * 多媒体隧道功能未开启
     */
    public static final int SVN_TUNNEL_DISABLED = -9;   
    
    /** 
     * User ID验证有误
     */
    public static final int SVN_USERID_INVALID = -10;     
    
    /** 
     * 隧道关闭，用户被踢下线
     */
    public static final int SVN_TUNNEL_CLOSED = -11;     
    
    /** 
     * 登录SVN网关时,UDP隧道探测超时失败,请检查网络状况
     */
    public static final int SVN_UDPS_TUNNEL_BLOCK = -12;    
    
    /** 
     * 登录SVN网关时,服务器证书的CA不匹配,校验失败
     */
    public static final int SVN_SERVER_VERIFY_FAILED = -13;     
    
    /** 
     * 登录SVN网关时,客户端证书不匹配,校验失败
     */
    public static final int SVN_VERIFY_CLIENT_CERT_ERR = -14;     
    
    /** 
     * 用户被锁定，无法登录
     */
    public static final int SVN_USER_LOCKED = -15;     
    
    /** 
     * auth id方式登录方式，用户名与auth id不匹配，无法登录
     */
    public static final int SVN_USER_AUTH_ID_ERR = -16;
    
    /** 
     * 网关运行异常
     */
    public static final int SVN_GATEWAY_EXCEPTION = -99;
    
    /** 
     * 组件运行异常
     */
    public static final int SVN_SYSTEM_EXCEPTION = -100;   
    
    /** 
     * 参数错误
     */
    public static final int SVN_PARAM_ERROR = -101;       
    
    /** 
     * sdk工作目录错误
     */
    public static final int SVN_WORKPATH_ERROR = -102;        
    
    /** 
     * HTTP初始化错误
     */
    public static final int SVN_HTTPINIT_ERROR = -103;      
    
    /** 
     * 文件加解密初始化错误
     */
    public static final int SVN_FILEINIT_ERROR = -104;     
    
    /** 
     * 需要登录网关
     */
    public static final int SVN_NEEDLOGIN_ERROR = -105;   
    
    /** 
     * 默认错误
     */
    public static final int SVN_DEFAULT_ERROR = -200; 


    

    

    /**
     * 连接类型，0：表示SDK连接、
     */
    public static final int SVN_CONNECT_TYPE_SDK = 0;
    
    /**
     * 连接类型，1：表示anyoffice连接、
     */
    public static final int SVN_CONNECT_TYPE_AYNOFFICE = 1;
    
    /**
     * 连接类型，2：表示MTM连接
     */
    public static final int SVN_CONNECT_TYPE_MTM = 2;
    
}
