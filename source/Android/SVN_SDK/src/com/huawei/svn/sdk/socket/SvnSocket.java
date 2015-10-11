/*
 * 
 */
package com.huawei.svn.sdk.socket;

/*
 * @(#)Socket.java    1.115 07/09/05
 *
 * Copyright 2006 Sun Microsystems, Inc. All rights reserved
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms
 */
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.SocketException;
import java.net.SocketImplFactory;
import java.net.SocketOptions;
import java.net.UnknownHostException;
import java.nio.channels.SocketChannel;
import java.security.AccessController;
import java.security.PrivilegedExceptionAction;

import android.util.Log;

import com.huawei.shield.ProxyConstruct;
import com.huawei.svn.sdk.server.SvnClientApiImpl;

/**
 * SvnSocket
 * 
 * @author l00174413
 * @see java.net.Socket#setSocketImplFactory(java.net.SocketImplFactory)
 * @see java.net.SocketImpl
 * @see java.nio.channels.SocketChannel
 */
@ProxyConstruct(value="Ljava/net/Socket;")
public class SvnSocket extends Socket
{
    /** 是否已创建 */
    private boolean created = false;

    /** 是否已绑定 */
    private boolean bound = false;

    /** 是否已连接 */
    private boolean connected = false;

    /** 是否已关闭 */
    private boolean closed = false;

    /** 关闭锁对象 */
    private final Object closeLock = new Object();

    /** 是否关闭输入 */
    private boolean shutIn = false;

    /** 是否关闭输出 */
    private boolean shutOut = false;

    /** SocketImpl实现对象 */
    private SvnPlainSocketImpl impl;

    // /** Are we using an older SocketImpl?. */
    // private boolean oldImpl = false;

    /**
     * 生成一个新的SvnSocket对象
     */
    public SvnSocket()
    {
        setImpl();
    }

    /**
     * 创建连接至指定主机名和端口的SvnSocket
     * 
     * @param host
     *            主机名, 传入null表示本机回环地址
     * @param port
     *            端口
     * @throws IOException
     *             发生I/O异常
     */
    public SvnSocket(String host, int port) throws IOException
    {
        this(new InetSocketAddress(getHostbyName(host), port),
                (SocketAddress) null, true);

        // System.out.println("SvnSocket for host:"+ host + ", port:" + port);
    }

    /**
     * 获取IP地址的字符串表示
     * 
     * @param iAddr
     *            IP地址
     * @return IP地址的字符串表示
     */
    public static String getIpAddrString(int iAddr)
    {
        int iPart1 = (iAddr & 0x000000FF);
        int iPart2 = (iAddr & 0x0000FF00) >> 8;
        int iPart3 = (iAddr & 0x00FF0000) >> 16;
        int iPart4 = (iAddr & 0xFF000000) >> 24;
       
        if (iPart1 < 0)
        {
            iPart1 = 256 + iPart1;
        }

        if (iPart2 < 0)
        {
            iPart2 = 256 + iPart2;
        }

        if (iPart3 < 0)
        {
            iPart3 = 256 + iPart3;
        }

        if (iPart4 < 0)
        {
            iPart4 = 256 + iPart4;
        }

        return "" + iPart1 + "." + iPart2 + "." + iPart3 + "." + iPart4;
    }

    /**
     * 解析传入主机的iP地址
     * 
     * @param host
     *            主机名
     * @param port
     *            端口
     * @return IP地址
     * @throws UnknownHostException
     *             主机名无法解析
     */
    public static InetAddress getHostbyName(String host)
            throws UnknownHostException
    {
        // System.out.println("getHostbyName for host:"+ host + ", port:" +
        // port);

        if (host == null || host.isEmpty())
        {
            // throw new IllegalArgumentException("host=" + host);
            return InetAddress.getByName(host);
        }

        boolean bIncludeLetter = isInclueLetter(host);

        /* BEGIN: Added by xwei/xKF66393 for print */
        // System.out.println("######### Into function getHostbyName ");
        /* END: Added by xwei/xKF66393 for print */

//        InetAddress[] result = null;

        

        if (bIncludeLetter)
        {
            /* BEGIN: Added by xwei/xKF66393 for print */
            // System.out.println("######### Call DNS parse function ");
            // System.out.println(host);
            /* END: Added by xwei/xKF66393 for print */

            int[] iAddr = new SvnClientApiImpl().parseURL(host);
            if (iAddr != null && iAddr.length > 0)
            {
                //System.out.println("iAddr[0]=" + iAddr[0]);
                if(iAddr[0] != 0)
                {
                    String strhost = getIpAddrString(iAddr[0]);
                    Log.i("SDK", String.format("%d to ip is %s", iAddr[0], strhost));
                    return InetAddress.getByName(strhost);
                }
               
            }

            throw new UnknownHostException("unknown host:" + host);
            

            /* BEGIN: Added by xwei/xKF66393 for print */
            // System.out.println(strhost);
            // System.out.println("######### Call DNS parse function success ");
            /* END: Added by xwei/xKF66393 for print */

        }
        else
        {
            return InetAddress.getByName(host);
        }

        // System.out.println("Host: " + host);
        // System.out.println("Host OR URL: " + strhost);
        /* BEGIN: Added by xwei/xKF66393 for print */
        // System.out.println("######### Call function getHostbyName success ");
        /* END: Added by xwei/xKF66393 for print */
        // System.out.println("after parseURL, getHostbyName for host:"+ host +
        // ", port:" + port);

        // return InetAddress.getByName(strhost);
    }

    /**
     * 主机地址是否包含字母
     * 
     * @param host
     *            主机地址
     * @return 是否包含字母
     */
    private static boolean isInclueLetter(String host)
    {
        boolean bIncludeLetter = false;
        if (host == null)
        {
            return false;
        }
        int hostLen = host.length();

        char c;
        for (int i = 0; i < hostLen; i++)
        {
            c = host.charAt(i);

            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            {
                bIncludeLetter = true;
                break;
            }
        }
        return bIncludeLetter;
    }

    /* END: Added by lizongyu 20120323 for DNS prase */

    /**
     * 创建连接至指定IP地址和端口的SvnSocket
     * 
     * @param address
     *            IP地址
     * @param port
     *            端口
     * @throws IOException
     *             I/O异常
     */
    public SvnSocket(InetAddress address, int port) throws IOException
    {
        this(address != null ? new InetSocketAddress(address, port) : null,
                (SocketAddress) null, true);
    }

    /**
     * 创建连接至指定远端地址和端口的SvnSocket，同时绑定本地的地址和端口
     * 
     * @param host
     *            远端地址
     * @param port
     *            远端端口
     * @param localAddr
     *            本地地址
     * @param localPort
     *            本地端口
     * @throws IOException
     *             发生I/O异常
     */
    public SvnSocket(String host, int port, InetAddress localAddr, int localPort)
            throws IOException
    {
        this(new InetSocketAddress(getHostbyName(host), port),
                new InetSocketAddress(localAddr, localPort), true);
    }

    /**
     * 创建连接至指定远端IP地址和端口的SvnSocket，同时绑定本地的地址和端口
     * 
     * @param address
     *            远端IP地址
     * @param port
     *            远端端口
     * @param localAddr
     *            本地地址
     * @param localPort
     *            本地端口
     * @throws IOException
     *             发生I/O异常
     */
    public SvnSocket(InetAddress address, int port, InetAddress localAddr,
            int localPort) throws IOException
    {
        this(address != null ? new InetSocketAddress(address, port) : null,
                new InetSocketAddress(localAddr, localPort), true);
    }

    /**
     * 创建连接至指定远端地址和端口的SvnSocket，并指定连接类型（TCP或UDP）
     * 
     * @param host
     *            主机名, 传入null表示本机回环地址
     * @param port
     *            端口
     * @param stream
     *            是否是流式连接，true表示TCP，false表示UDP
     * @throws IOException
     *             发生I/O异常
     */
    public SvnSocket(String host, int port, boolean stream) throws IOException
    {
        this(new InetSocketAddress(getHostbyName(host), port),
                (SocketAddress) null, stream);
    }

    /**
     * 创建连接至指定远端IP地址和端口的SvnSocket，并指定连接类型（TCP或UDP）
     * 
     * @param host
     *            IP地址
     * @param port
     *            端口
     * @param stream
     *            是否是流式连接，true表示TCP，false表示UDP
     * @throws IOException
     *             发生I/O异常
     */
    public SvnSocket(InetAddress host, int port, boolean stream)
            throws IOException
    {
        this(host != null ? new InetSocketAddress(host, port) : null,
                new InetSocketAddress(0), stream);
    }

    /**
     * 初始化连接至指定地址的SvnSocket，并指定连接类型（TCP或UDP），
     * 
     * @param address
     *            IP地址
     * @param localAddr
     *            端口
     * @param stream
     *            是否是流式连接，true表示TCP，false表示UDP
     * @throws IOException
     *             发生I/O异常
     */
    private SvnSocket(SocketAddress address, SocketAddress localAddr,
            boolean stream) throws IOException
    {
        setImpl();

        // backward compatibility
        if (address == null)
        {
            throw new IOException("SocketAddress address is null");
            // throw new NullPointerException();
        }
        try
        {
            createImpl(stream);
            if (localAddr != null)
            {
                bind(localAddr);
            }

            connect(address);

        }
        catch (IOException e)
        {
            close();
            throwSvnExp(e);
            // throw e;
        }
    }

    /**
     * 初始化SocketImpl对象
     * 
     * @param stream
     *            是否是流式连接，true表示TCP，false表示UDP
     * @throws SocketException
     *             Socket异常
     */
    void createImpl(boolean stream) throws SocketException
    {
        if (impl == null)
        {
            setImpl();
        }

        try
        {
            // System.out.println("begin create socket when createImpl");
            impl.create(stream);
            created = true;
        }
        catch (IOException e)
        {
            throw new SocketException(e.getMessage());
        }
    }

    /**
     * 设置SocketImpl对象为默认实现
     */
    void setImpl()
    {
        if (null == impl)
        {
            impl = new SvnPlainSocketImpl();
        }
    }

    /**
     * 获取SocketImpl对象
     * 
     * @return SocketImpl对象
     */
    SvnPlainSocketImpl getImpl() throws SocketException
    {
        if (!created)
        {
            createImpl(true);
        }
        return impl;
    }

    /**
     * 连接至远端地址
     * 
     * @param endpoint
     *            服务器地址
     * @throws IOException
     *             发生I/O异常
     */
    public void connect(SocketAddress endpoint) throws IOException
    {
        if (null != endpoint)
        {
            connect(endpoint, 0);
        }
        else
        {
            throw new IOException("SocketAddress is null");
        }
    }

    /**
     * 抛出SvnExcetion
     * 
     * @param e
     *            异常对象
     * @throws IOException
     *             I/O异常
     */
    private void throwSvnExp(Exception e) throws IOException
    {
        if (e.getMessage().equals(SvnErrorInfo.SVN_SOCKET_ERROR))
        {
            throw new IOException(e.getMessage());
        }
        else if (e.getMessage().equals(SvnErrorInfo.SVN_ERRNO_WRONG_INFO))
        {
            throw new SvnAuthenticationException(SvnErrorInfo.SVN_USER_INFO_ERR);
        }
        else if (e.getMessage().equals(SvnErrorInfo.SVN_ERRNO_LOCKED))
        {
            SvnAuthenticationException objSvnExp = new SvnAuthenticationException(
                    SvnErrorInfo.SVN_USER_LOCKED);
            objSvnExp.setFreeLockTimes(0);
            throw objSvnExp;
        }

        if (10 < e.getMessage().length())
        {
            throw new IOException(e.getMessage());
        }
        if (-1200 > Integer.valueOf(e.getMessage()))
        {
            int iErrno = Integer.valueOf(e.getMessage());
            int iFreeLockTimes = Math.abs(iErrno) % 1200;
            SvnAuthenticationException objSvnExp = new SvnAuthenticationException(
                    SvnErrorInfo.SVN_USER_LOCK);
            objSvnExp.setFreeLockTimes(iFreeLockTimes);
            throw objSvnExp;
        }
    }

    /**
     * 连接至远端地址，并设置连接超时时间 超时时间设置为0表示无限大，连接将一直阻塞直到连接建立或发生超时错误
     * 
     * @param endpoint
     *            远端地址
     * @param timeout
     *            超时时间，单位毫秒
     * @throws IOException
     *             发生I/O异常
     */
    public void connect(SocketAddress endpoint, int timeout) throws IOException
    {
        if (endpoint == null)
        {
            throw new IllegalArgumentException(
                    "connect: The address can't be null");
        }

        if (timeout < 0)
        {
            throw new IllegalArgumentException(
                    "connect: timeout can't be negative");
        }

        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }

        // if (!oldImpl && isConnected())
        if (isConnected())
        {
            throw new SocketException("already connected");
        }

        if (!(endpoint instanceof InetSocketAddress))
        {
            throw new IllegalArgumentException("Unsupported address type");
        }

        InetSocketAddress epoint = (InetSocketAddress) endpoint;

        SecurityManager security = System.getSecurityManager();
        if (security != null)
        {
            if (epoint.isUnresolved())
            {
                security.checkConnect(epoint.getHostName(), epoint.getPort());
            }
            else
            {
                security.checkConnect(epoint.getAddress().getHostAddress(),
                        epoint.getPort());
            }
        }
        if (!created)
        {
            try
            {
                // System.out.println("begin to create socket when connecting");
                createImpl(true);
            }
            catch (Exception e)
            {
                throwSvnExp(e);
            }
        }

        // if (!oldImpl)
        // {
        impl.connect(epoint, timeout);
        // }
        // else if (timeout == 0)
        // {
        // if (epoint.isUnresolved())
        // {
        // impl.connect(epoint.getAddress().getHostName(),
        // epoint.getPort());
        // }
        // else
        // {
        // impl.connect(epoint.getAddress(), epoint.getPort());
        // }
        // }
        // else
        // {
        // throw new UnsupportedOperationException(
        // "SocketImpl.connect(addr, timeout)");
        // }
        connected = true;
        /*
         * If the socket was not bound before the connect, it is now because the
         * kernel will have picked an ephemeral port & a local address
         */
        bound = true;
    }

    /**
     * 获取任意本地IP地址
     */
    private static InetAddress anyLocalAddress()
    {
        return new InetSocketAddress("0.0.0.0", 65535).getAddress();
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.Socket#bind(java.net.SocketAddress)
     */
    public void bind(SocketAddress bindpoint) throws IOException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        // if (!oldImpl && isBound())
        if (isBound())
        {
            throw new SocketException("Already bound");
        }

        if (bindpoint != null && (!(bindpoint instanceof InetSocketAddress)))
        {
            throw new IllegalArgumentException("Unsupported address type");
        }
        InetSocketAddress epoint = (InetSocketAddress) bindpoint;
        if (epoint != null && epoint.isUnresolved())
        {
            throw new SocketException("Unresolved address");
        }
        if (bindpoint == null)
        {
            getImpl().bind(anyLocalAddress(), 0);
        }
        else
        {
            getImpl().bind(epoint.getAddress(), epoint.getPort());
        }
        bound = true;
    }

    /**
     * accept()调用后设置标志
     */
    final void postAccept()
    {
        connected = true;
        created = true;
        bound = true;
    }

    void setCreated()
    {
        created = true;
    }

    void setBound()
    {
        bound = true;
    }

    void setConnected()
    {
        connected = true;
    }

    /**
     * 获取连接的远端IP地址
     * 
     * @return 远端IP地址，未连接返回null
     */
    public InetAddress getInetAddress()
    {
        if (!isConnected())
        {
            return null;
        }
        try
        {
            return getImpl().getInetAddress();
        }
        catch (SocketException e)
        {
            return null;
        }
    }

    /**
     * 获取绑定的本地IP地址
     * 
     * @return 本地IP地址，未绑定返回“0.0.0.0”
     */
    public InetAddress getLocalAddress()
    {
        // This is for backward compatibility
        if (!isBound())
        {
            return anyLocalAddress();
        }
        InetAddress in = null;
        try
        {
            /*
             * in = (InetAddress)
             * getImpl().getOption(SocketOptions.SO_BINDADDR); if
             * (in.isAnyLocalAddress()) { in = anyLocalAddress(); }
             */
            in = getImpl().getLocalInetAddress();
        }
        catch (SocketException e)
        {
            in = anyLocalAddress(); // "0.0.0.0"
        }
        return in;
    }

    /**
     * 获取连接的远端端口
     * 
     * @return 远端端口，未连接返回0
     */
    public int getPort()
    {
        if (!isConnected())
        {
            return 0;
        }
        try
        {
            return getImpl().getPort();
        }
        catch (SocketException e)
        {
            return -1;
        }

    }

    /**
     * 获取连接绑定的本地端口
     * 
     * @return 本地端口，未绑定返回-1
     */
    public int getLocalPort()
    {
        if (!isBound())
        {
            return -1;
        }
        try
        {
            return getImpl().getLocalPort();
        }
        catch (SocketException e)
        {
            return -1;
        }

    }

    /**
     * 返回连接的远端地址
     * 
     * @return 远端地址，未连接返回null
     * @see #getInetAddress()
     * @see #getPort()
     * @see #connect(SocketAddress, int)
     * @see #connect(SocketAddress)
     */
    public SocketAddress getRemoteSocketAddress()
    {
        if (!isConnected())
        {
            return null;
        }
        return new InetSocketAddress(getInetAddress(), getPort());
    }

    /**
     * 返回连接绑定的本地地址
     * 
     * @return 绑定的本地地址，未绑定返回null
     * @see #getLocalAddress()
     * @see #getLocalPort()
     * @see #bind(SocketAddress)
     */
    public SocketAddress getLocalSocketAddress()
    {
        if (!isBound())
        {
            return null;
        }
        return new InetSocketAddress(getLocalAddress(), getLocalPort());
    }

    /**
     * 获取SocketChannel对象，暂未实现
     * 
     * @return 连接关联的SocketChannel对象
     */
    public SocketChannel getChannel()
    {
        return null;
    }

    /**
     * 获取连接对应的InputStream
     * 
     * @return 连接对应的InputStream
     * @throws IOException
     *             发送I/O异常
     */
    public InputStream getInputStream() throws IOException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        if (!isConnected())
        {
            throw new SocketException("Socket is not connected");
        }
        if (isInputShutdown())
        {
            throw new SocketException("Socket input is shutdown");
        }
        // final Socket s = this;
        InputStream is = null;
        try
        {
            is = (InputStream) AccessController
                    .doPrivileged(new PrivilegedExceptionAction<InputStream>()
                    {
                        public InputStream run() throws IOException
                        {
                            return impl.getInputStream();
                        }
                    });
        }
        catch (java.security.PrivilegedActionException e)
        {
        
             throw new IOException("Socket PrivilegedActionException", e);
            
        }
        return is;
    }

    /**
     * 获取连接对应的OutputStream
     * 
     * @return 连接对应的OutputStream
     * @throws IOException
     *             发送I/O异常
     */
    public OutputStream getOutputStream() throws IOException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        if (!isConnected())
        {
            throw new SocketException("Socket is not connected");
        }
        if (isOutputShutdown())
        {
            throw new SocketException("Socket output is shutdown");
        }
        // final Socket s = this;
        OutputStream os = null;
        try
        {
            os = (OutputStream) AccessController
                    .doPrivileged(new PrivilegedExceptionAction<OutputStream>()
                    {
                        public OutputStream run() throws IOException
                        {
                            return impl.getOutputStream();
                        }
                    });
        }
        catch (java.security.PrivilegedActionException e)
        {
            throw new IOException("Socket PrivilegedActionException", e);
            //throw (IOException) e.getException();
        }
        return os;
    }

    /**
     * 设置 TCP_NODELAY参数
     * 
     * @param on
     *            是否使能TCP_NODELAY
     * @throws SocketException
     *             发生Socket异常
     * @see #getTcpNoDelay()
     */
    public void setTcpNoDelay(boolean on) throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        getImpl().setOption(SocketOptions.TCP_NODELAY, Boolean.valueOf(on));
    }

    /**
     * 获取TCP_NODELAY参数
     * 
     * @return 是否使能TCP_NODELAY
     * @throws SocketException
     *             发生Socket异常
     * @see #setTcpNoDelay(boolean)
     */
    public boolean getTcpNoDelay() throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        return ((Boolean) getImpl().getOption(SocketOptions.TCP_NODELAY))
                .booleanValue();
    }

    /**
     * 设置SO_LINGER参数
     * 
     * @param on
     *            是否使能SO_LINGER
     * @param linger
     *            LINGER时间值，如果使能
     * @throws SocketException
     *             发生Socket异常
     * @see #getSoLinger()
     */
    public void setSoLinger(boolean on, int linger) throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        if (!on)
        {
            // getImpl().setOption(SocketOptions.SO_LINGER, new Boolean(on));
            getImpl().setOption(SocketOptions.SO_LINGER, Boolean.FALSE);
        }
        else
        {
            if (linger < 0)
            {
                throw new IllegalArgumentException(
                        "invalid value for SO_LINGER");
            }
            if (linger > 65535)
            {
                linger = 65535;
            }
            getImpl().setOption(SocketOptions.SO_LINGER,
                    Integer.valueOf(linger));
        }
    }

    /**
     * 获取SO_LINGER参数
     * 
     * @return SO_LINGER参数
     * @throws SocketException
     *             发生Socket异常
     * @see #setSoLinger(boolean, int)
     */
    public int getSoLinger() throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        Object o = getImpl().getOption(SocketOptions.SO_LINGER);
        if (o instanceof Integer)
        {
            return ((Integer) o).intValue();
        }
        else
        {
            return -1;
        }
    }

    /**
     * 发送一字节的紧急数据
     * 
     * @param data
     *            紧急数据
     * @throws IOException
     *             发生I/O异常
     */
    public void sendUrgentData(int data) throws IOException
    {
        if (!getImpl().supportsUrgentData())
        {
            throw new SocketException("Urgent data not supported");
        }
        getImpl().sendUrgentData(data);
    }

    /**
     * 设置OOBINLINE参数（TCP紧急数据收据）
     * 
     * @param on
     *            是否使能OOBINLINE
     * @throws SocketException
     *             发生Socket异常
     * @see #getOOBInline()
     */
    public void setOOBInline(boolean on) throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        getImpl().setOption(SocketOptions.SO_OOBINLINE, Boolean.valueOf(on));
    }

    /**
     * 获取OOBINLINE参数（TCP紧急数据收据）
     * 
     * @return 是否使能OOBINLINE
     * @throws SocketException
     *             发生Socket异常
     * @see #setOOBInline(boolean)
     */
    public boolean getOOBInline() throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        return ((Boolean) getImpl().getOption(SocketOptions.SO_OOBINLINE))
                .booleanValue();
    }

    /**
     * 设置SO_TIMEOUT参数，单位毫秒
     * 
     * @param timeout
     *            SO_TIMEOUT参数
     * @throws SocketException
     *             发生Socket异常
     * @see #getSoTimeout()
     */
    public synchronized void setSoTimeout(int timeout) throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        if (timeout < 0)
        {
            throw new IllegalArgumentException("timeout can't be negative");
        }
        getImpl().setOption(SocketOptions.SO_TIMEOUT, Integer.valueOf(timeout));
    }

    /**
     * 获取SO_TIMEOUT参数，单位毫秒
     * 
     * @return SO_TIMEOUT参数
     * @throws SocketException
     *             发生Socket异常
     * @see #setSoTimeout(int)
     */
    public synchronized int getSoTimeout() throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        Object o = getImpl().getOption(SocketOptions.SO_TIMEOUT);
        /* extra type safety */
        if (o instanceof Integer)
        {
            return ((Integer) o).intValue();
        }
        else
        {
            return 0;
        }
    }

    /**
     * 设置SO_SNDBUF参数
     * 
     * @param size
     *            发送缓冲区大小
     * @throws SocketException
     *             发生Socket异常
     * @see #getSendBufferSize()
     */
    public synchronized void setSendBufferSize(int size) throws SocketException
    {
        if (!(size > 0))
        {
            throw new IllegalArgumentException("negative send size");
        }
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        getImpl().setOption(SocketOptions.SO_SNDBUF, Integer.valueOf(size));
    }

    /**
     * 获取SO_SNDBUF参数
     * 
     * @return 发送缓冲区大小
     * @throws SocketException
     *             发生Socket异常
     * @see #setSendBufferSize(int)
     */
    public synchronized int getSendBufferSize() throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        int result = 0;
        Object o = getImpl().getOption(SocketOptions.SO_SNDBUF);
        if (o instanceof Integer)
        {
            result = ((Integer) o).intValue();
        }
        return result;
    }

    /**
     * 设置SO_RCVBUF参数
     * 
     * @param size
     *            接收缓冲区大小
     * @throws SocketException
     *             发生Socket异常
     * @see #getReceiveBufferSize()
     * @see ServerSocket#setReceiveBufferSize(int)
     */
    public synchronized void setReceiveBufferSize(int size)
            throws SocketException
    {
        if (size <= 0)
        {
            throw new IllegalArgumentException("invalid receive size");
        }
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        getImpl().setOption(SocketOptions.SO_RCVBUF, Integer.valueOf(size));
    }

    /**
     * 获取SO_RCVBUF参数
     * 
     * @return 接收缓冲区大小
     * @throws SocketException
     *             发生Socket异常
     * @see #setReceiveBufferSize(int)
     */
    public synchronized int getReceiveBufferSize() throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        int result = 0;
        Object o = getImpl().getOption(SocketOptions.SO_RCVBUF);
        if (o instanceof Integer)
        {
            result = ((Integer) o).intValue();
        }
        return result;
    }

    /**
     * 设置SO_KEEPALIVE参数
     * 
     * @param on
     *            是否使能SO_KEEPALIVE
     * @throws SocketException
     *             发生Socket异常
     * @see #getKeepAlive()
     */
    public void setKeepAlive(boolean on) throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        getImpl().setOption(SocketOptions.SO_KEEPALIVE, Boolean.valueOf(on));
    }

    /**
     * 获取SO_KEEPALIVE参数
     * 
     * @return 是否使能SO_KEEPALIVE
     * 
     * @throws SocketException
     *             发生Socket异常
     * @see #setKeepAlive(boolean)
     */
    public boolean getKeepAlive() throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        return ((Boolean) getImpl().getOption(SocketOptions.SO_KEEPALIVE))
                .booleanValue();
    }

    /**
     * 设置TrafficClass参数
     * 
     * 参数范围： 0 <= tc <= 255 - <p> <UL> <LI><CODE>IPTOS_LOWCOST
     * (0x02)</CODE></LI> <LI><CODE>IPTOS_RELIABILITY (0x04)</CODE></LI>
     * <LI><CODE>IPTOS_THROUGHPUT (0x08)</CODE></LI> <LI><CODE>IPTOS_LOWDELAY
     * (0x10)</CODE></LI> </UL>
     * 
     * @param tc
     *            TrafficClass参数
     * @throws SocketException
     *             发生Socket异常
     * @see #getTrafficClass
     */
    public void setTrafficClass(int tc) throws SocketException
    {
        if (tc < 0 || tc > 255)
        {
            throw new IllegalArgumentException("tc is not in range 0 -- 255");
        }
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        getImpl().setOption(SocketOptions.IP_TOS, Integer.valueOf(tc));
    }

    /**
     * 获取TrafficClass参数
     * 
     * @return TrafficClass参数
     * @throws SocketException
     *             发生Socket异常
     * @see #setTrafficClass(int)
     */
    public int getTrafficClass() throws SocketException
    {
        return ((Integer) (getImpl().getOption(SocketOptions.IP_TOS)))
                .intValue();
    }

    /**
     * 设置SO_REUSEADDR参数
     * 
     * @param on
     *            SO_REUSEADDR参数
     * @throws SocketException
     *             发生Socket异常
     * @see #getReuseAddress()
     * @see #bind(SocketAddress)
     * @see #isClosed()
     * @see #isBound()
     */
    public void setReuseAddress(boolean on) throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        getImpl().setOption(SocketOptions.SO_REUSEADDR, Boolean.valueOf(on));
    }

    /**
     * 获取SO_REUSEADDR参数
     * 
     * @return SO_REUSEADDR参数
     * @throws SocketException
     *             发生Socket异常
     * @see #setReuseAddress(boolean)
     */
    public boolean getReuseAddress() throws SocketException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        return ((Boolean) (getImpl().getOption(SocketOptions.SO_REUSEADDR)))
                .booleanValue();
    }

    /**
     * 关闭已经建立的连接
     * 
     * @throws IOException
     *             发生I/O异常
     * @see #isClosed
     */
    public synchronized void close() throws IOException
    {
        synchronized (closeLock)
        {
            boolean svnClosed = isClosed();
            if (svnClosed)
            {
                // System.out.println("svn socket is closed");
                return;
            }

            if (created)
            {
                // System.out.println("svn socket is closing");
                impl.close();
            }
            closed = true;
        }
    }

    /**
     * 关闭连接的接收数据功能
     * 
     * @throws IOException
     *             发生I/O异常
     * @see java.net.Socket#shutdownOutput()
     * @see java.net.Socket#close()
     * @see java.net.Socket#setSoLinger(boolean, int)
     * @see #isInputShutdown
     */
    public void shutdownInput() throws IOException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        if (!isConnected())
        {
            throw new SocketException("Socket is not connected");
        }
        if (isInputShutdown())
        {
            throw new SocketException("Socket input is already shutdown");
        }
        getImpl().shutdownInput();
        shutIn = true;
    }

    /**
     * 关闭连接的发送数据功能
     * 
     * @throws IOException
     *             发生I/O异常
     * @see java.net.Socket#shutdownInput()
     * @see java.net.Socket#close()
     * @see java.net.Socket#setSoLinger(boolean, int)
     * @see #isOutputShutdown
     */
    public void shutdownOutput() throws IOException
    {
        if (isClosed())
        {
            throw new SocketException("Socket is closed");
        }
        if (!isConnected())
        {
            throw new SocketException("Socket is not connected");
        }
        if (isOutputShutdown())
        {
            throw new SocketException("Socket output is already shutdown");
        }
        getImpl().shutdownOutput();
        shutOut = true;
    }

    /**
     * 获取SvnSocket的字符串描述
     * 
     * @return SvnSocket的字符串描述
     */
    public String toString()
    {
        try
        {
            if (isConnected())
            {
                return "Socket[addr=" + getImpl().getInetAddress() + ",port="
                        + getImpl().getPort() + ",localport="
                        + getImpl().getLocalPort() + "]";
            }
        }
        catch (SocketException e)
        {
            return "Socket[unconnected]";
        }
        return "Socket[unconnected]";
    }

    /**
     * 获取连接是否建立
     * 
     * @return 连接是否建立
     */
    public boolean isConnected()
    {
        // Before 1.3 Sockets were always connected during creation
        return connected;// || oldImpl;
    }

    /**
     * 获取连接是否绑定
     * 
     * @return 连接是否绑定
     * @see #bind
     */
    public boolean isBound()
    {
        // Before 1.3 Sockets were always bound during creation
        return bound;// || oldImpl;
    }

    /**
     * 获取连接是否关闭
     * 
     * @return 连接是否关闭
     * @see #close
     */
    public boolean isClosed()
    {
        synchronized (closeLock)
        {
            return closed;
        }
    }

    /**
     * 连接的接收功能是否关闭
     * 
     * @return 接收是否关闭
     * @see #shutdownInput
     */
    public boolean isInputShutdown()
    {
        return shutIn;
    }

    /**
     * 连接的发送功能是否关闭
     * 
     * @return 发送是否关闭
     * @see #shutdownOutput
     */
    public boolean isOutputShutdown()
    {
        return shutOut;
    }

    /**
     * The factory for all client sockets
     */
    private static SocketImplFactory factory = null;

    /**
     * 设置SokcetImpl工厂类，此方法只能被设置调用一次
     * 
     * @param fac
     *            SokcetImpl工厂类
     * @throws IOException
     *             发生I/O异常
     * @see java.net.SocketImplFactory#createSocketImpl()
     * @see SecurityManager#checkSetFactory
     */
    public static synchronized void setSocketImplFactory(SocketImplFactory fac)
            throws IOException
    {
        if (factory != null)
        {
            throw new SocketException("factory already defined");
        }
        SecurityManager security = System.getSecurityManager();
        if (security != null)
        {
            security.checkSetFactory();
        }
        factory = fac;
    }

    /**
     * 设置TCP性能参数，未实现
     * 
     * @param connectionTime
     *            连接时间参数
     * 
     * @param latency
     *            网络延迟参数
     * 
     * @param bandwidth
     *            网络带宽参数
     */
    public void setPerformancePreferences(int connectionTime, int latency,
            int bandwidth)
    {
        /* Not implemented yet */
    }
}
