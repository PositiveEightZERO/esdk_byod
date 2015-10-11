/*
 * 
 */
package com.huawei.svn.sdk.socket;

/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.SocketException;
import java.net.SocketImpl;
import java.net.SocketTimeoutException;

import android.util.Log;

/**
 * 基于隧道连接的Socket实现
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnPlainSocketImpl extends SocketImpl
{

    // For SOCKS support. A SOCKS bind() uses the last
    // host connected to in its request.
    // @SuppressWarnings("unused")
    // static private InetAddress lastConnectedAddress;
    //
    // @SuppressWarnings("unused")
    // static private int lastConnectedPort;
    //
    // @SuppressWarnings("unused")
    // private static Field fdField;

    /**
     * Socket句柄
     */
    private int iSvnFd;

    /**
     * 是否基于流
     */
    private boolean streaming = true;

    /**
     * 是否关闭输入
     */
    private boolean bShutdownInput;

    /**
     * 是否已关闭连接
     */
    private boolean isSocketClosed = true;

    // private Proxy proxy;

    private InetAddress localAddress = null;
    /**
     * SvnSocketApi实现对象
     */
    private SvnSocketApi socketApi = new SvnSocketApiImpl();

    /**
     * 获取Socket句柄
     * 
     * @return the socket fd
     */
    public int getSocketFd()
    {
        return iSvnFd;
    }

    /* END: Added by xwei/xKF66393 20120322 */

    // public void PlainSocketImpl(FileDescriptor fd)
    // {
    // this.fd = fd;
    // }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#getPort()
     */
    public int getPort()
    {
        return port;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#getLocalPort()
     */
    public int getLocalPort()
    {
        return localport;
    }

    // public void PlainSocketImpl(Proxy proxy)
    // {
    //
    // }
    //
    // public void PlainSocketImpl()
    // {
    //
    // }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#getInetAddress()
     */
    public InetAddress getInetAddress()
    {
        return address;
    }

    public InetAddress getLocalInetAddress()
    {
        return localAddress;
    }

    // public void PlainSocketImpl(FileDescriptor fd, int localport,
    // InetAddress addr, int port)
    // {
    // this.fd = fd;
    // this.localport = localport;
    // this.address = addr;
    // this.port = port;
    // }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#accept(java.net.SocketImpl)
     */
    @Override
    protected void accept(SocketImpl newImpl) throws IOException
    {

    }

    // private boolean usingSocks()
    // {
    // //return proxy != null && proxy.type() == Proxy.Type.SOCKS;
    // return false;
    // }

    // /**
    // * gets SocketImpl field by reflection.
    // *
    // * @throws IOException
    // * I/O异常
    // */
    // private Field getSocketImplField(final String fieldName)
    // {
    // return AccessController.doPrivileged(new PrivilegedAction<Field>()
    // {
    // public Field run()
    // {
    // Field field = null;
    // try
    // {
    // field = SocketImpl.class.getDeclaredField(fieldName);
    // field.setAccessible(true);
    // }
    // catch (NoSuchFieldException e)
    // {
    // throw new Error(e);
    // }
    // return field;
    // }
    // });
    // }

    // public void initLocalPort(int localPort)
    // {
    // this.localport = localPort;
    // }
    //
    // public void initRemoteAddressAndPort(InetAddress remoteAddress,
    // int remotePort)
    // {
    // this.address = remoteAddress;
    // this.port = remotePort;
    // }

    /**
     * 检测是否关闭
     * 
     * @throws IOException
     *             发生I/O异常
     */
    private void checkNotClosed() throws IOException
    {
        if (isSocketClosed)
        {
            throw new SocketException("socket is closed");
        }
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#available()
     */
    @Override
    protected synchronized int available() throws IOException
    {
        checkNotClosed();
        // we need to check if the input has been shutdown. If so
        // we should return that there is no data to be read
        if (bShutdownInput)
        {
            return 0;
        }

        return iSvnFd;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#bind(java.net.InetAddress, int)
     */
    @Override
    protected void bind(InetAddress address, int port) throws IOException
    {
        if (null == address)
        {
            throw new IOException("socket bind exception,InetAddress is null");
        }

        SvnSockaddrIn sockaddr = new SvnSockaddrIn();
        sockaddr.setSinAddr(address.getHostAddress());
        sockaddr.setSinPort(port);
        sockaddr.setSinFamily(2);// must be AF_INET

        int iRet = socketApi.svnBind(iSvnFd, sockaddr);
        if (iRet != 0)
        {
            throw new IOException("socket bind exception:" + iRet);
        }
        // this.address = address;
        localAddress = address;
        if (port != 0)
        {
            this.localport = port;
        }
        else
        {
            this.localport = socketApi.svnGetlocalport(iSvnFd);
        }
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#close()
     */
    @Override
    protected void close() throws IOException
    {
        if (isSocketClosed || iSvnFd <= 0)
        {
            return;
        }

        int iRet = socketApi.svnClose(iSvnFd);

        if (iRet != 0)
        {
            throw new IOException("socket close exception:" + iRet);
        }

        iSvnFd = -1;
        isSocketClosed = true;

        // synchronized (fd)
        // {
        // if (fd.valid())
        // {
        // socketApi.svn_close(iSvnFd);
        // fd = new FileDescriptor();
        // }
        // }
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#connect(java.lang.String, int)
     */
    @Override
    protected void connect(String aHost, int aPort) throws IOException
    {
        // SvnSocket.getHostbyName(host, aPort)
        // connect(InetAddress.getByName(aHost), aPort);
        InetAddress address = SvnSocket.getHostbyName(aHost);
        connect(address, aPort);
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#connect(java.net.InetAddress, int)
     */
    @Override
    protected void connect(InetAddress anAddr, int aPort) throws IOException
    {
        connect(anAddr, aPort, 0);
    }

    /**
     * Connects this socket to the specified remote host address/port.
     * 
     * @param anAddr
     *            the remote host address to connect to
     * @param aPort
     *            the remote port to connect to
     * @param timeout
     *            a timeout where supported. 0 means no timeout
     * @throws IOException
     *             if an error occurs while connecting
     */
    private void connect(InetAddress anAddr, int aPort, int timeout)
            throws IOException
    {
        int iRet = 0;

        if (null == anAddr)
        {
            throw new IOException("InetAddress is null");
        }
        InetAddress normalAddr = anAddr.isAnyLocalAddress() ? InetAddress
                .getLocalHost() : anAddr;

        SvnSockaddrIn sockaddr = new SvnSockaddrIn();
        sockaddr.setSinAddr(normalAddr.getHostAddress());
        sockaddr.setSinPort(aPort);
        sockaddr.setSinFamily(2);// must be AF_INET

        if (streaming)
        {
            iRet = socketApi.svnConnect(iSvnFd, sockaddr, timeout);
            if (iRet != 0)
            {
                throw new IOException("socket connect error:" + iRet);
            }
        }
        super.address = normalAddr;
        super.port = aPort;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#create(boolean)
     */
    @Override
    protected void create(boolean streaming) throws IOException
    {
        this.streaming = streaming;

        int iFamily = 2; // must be AF_INET
        int iType = 0;
        int iProtocol = 0;

        if (streaming)
        {
            iType = 1; // SOCK_STREAM
            iProtocol = 6; // IPPROTO_TCP
        }
        else
        {
            iType = 2; // SOCK_DGRAM
            iProtocol = 17; // IPPROTO_UDP
        }

        iSvnFd = socketApi.svnSocket(iFamily, iType, iProtocol);
        
        if(iSvnFd <=0)
        {
            Log.i("SDK", "socket create returns:" + iSvnFd);
        }
        if (-1005 == iSvnFd || -1200 > iSvnFd || -1015 == iSvnFd)
        {
            throw new IOException(String.valueOf(iSvnFd));
        }
        else if (0 > iSvnFd)
        {
            
            throw new IOException(SvnErrorInfo.SVN_SOCKET_ERROR);
        }
        isSocketClosed = false;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.lang.Object#finalize()
     */
    @Override
    protected void finalize() throws Throwable
    {
        try
        {
            close();
        }
        finally
        {
            super.finalize();
        }
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#getInputStream()
     */
    @Override
    protected synchronized InputStream getInputStream() throws IOException
    {
        checkNotClosed();
        return new SvnSocketInputStream(this);
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketOptions#getOption(int)
     */
    @Override
    public Object getOption(int optID) throws SocketException
    {
        int[] iArrayOptVal = new int[1];
        int[] iArrayOptLen = new int[1];
        int iLevel = 65535; // SOL_SOCKET

        int iRet = socketApi.svnGetsockopt(iSvnFd, iLevel, optID, iArrayOptVal,
                iArrayOptLen);
        if (iRet != 0)
        {
            throw new SocketException("socket getOption error:" + iRet);
        }
        return Integer.valueOf(iArrayOptVal[0]);
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#getOutputStream()
     */
    @Override
    protected synchronized OutputStream getOutputStream() throws IOException
    {
        checkNotClosed();
        return new SvnSocketOutputStream(this);
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#listen(int)
     */
    @Override
    protected void listen(int backlog) throws IOException
    {

    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketOptions#setOption(int, java.lang.Object)
     */
    @Override
    public void setOption(int optID, Object val) throws SocketException
    {
        int iLevel = 65535; // SOL_SOCKET
        int iOptVal = 0;
        int iOptLen = 0;

        if (val == null)
        {
            return;
        }

        if (val instanceof Integer)
        {
            iOptVal = ((Integer) val).intValue();
            iOptLen = 4;
        }
        else
        {
            if (val instanceof Boolean)
            {
                iOptVal = ((Boolean) val).booleanValue() ? 1 : 0;
                iOptLen = 4;
            }
        }

        int iRet = socketApi.svnSetsockopt(iSvnFd, iLevel, optID, iOptVal,
                iOptLen);
        if (iRet != 0)
        {
            throw new SocketException("socket setOption error:" + iRet);
        }
    }

    /**
     * Gets the SOCKS proxy server port.
     * 
     * @throws IOException
     *             I/O异常
     */
    // private int socksGetServerPort()
    // {
    // // get socks server port from proxy. It is unnecessary to check
    // // "socksProxyPort" property, since proxy setting should only be
    // // determined by ProxySelector.
    // InetSocketAddress addr = (InetSocketAddress) proxy.address();
    // return addr.getPort();
    //
    // }

    /**
     * Gets the InetAddress of the SOCKS proxy server.
     */
    // private InetAddress socksGetServerAddress() throws UnknownHostException
    // {
    // String proxyName;
    // // get socks server address from proxy. It is unnecessary to check
    // // "socksProxyHost" property, since all proxy setting should be
    // // determined by ProxySelector.
    // InetSocketAddress addr = (InetSocketAddress) proxy.address();
    // proxyName = addr.getHostName();
    // if (null == proxyName)
    // {
    // proxyName = addr.getAddress().getHostAddress();
    // }
    // return InetAddress.getByName(proxyName);
    // }

    // /**
    // * Connect using a SOCKS server.
    // */
    // private void socksConnect(InetAddress applicationServerAddress,
    // int applicationServerPort, int timeout) throws IOException
    // {
    //
    // }
    //
    // /**
    // * Request a SOCKS connection to the application server given. If the
    // * request fails to complete successfully, an exception is thrown.
    // */
    // private void socksRequestConnection(InetAddress applicationServerAddress,
    // int applicationServerPort) throws IOException
    // {
    //
    // }

    /**
     * Perform an accept for a SOCKS bind.
     */
    public void socksAccept() throws IOException
    {

    }

    /**
     * 关闭Socket输入
     * 
     * @throws IOException
     *             I/O异常
     */
    @Override
    protected void shutdownInput() throws IOException
    {
        bShutdownInput = true;
        
        if(iSvnFd <= 0)
        {
            return;
        }
        
        int iRet = socketApi.svnShutdown(iSvnFd, 0);
        if (iRet != 0)
        {
            throw new IOException("socket shutdown input error:" + iRet);
        }

    }

    /**
     * 关闭Socket输出
     * 
     * @throws IOException
     *             I/O异常
     */
    @Override
    protected void shutdownOutput() throws IOException
    {
        if(iSvnFd <= 0)
        {
            return;
        }
        
        int iRet = socketApi.svnShutdown(iSvnFd, 1);

        if (iRet != 0)
        {
            throw new IOException("socket shutdown output error:" + iRet);
        }
    }

    // /**
    // * Bind using a SOCKS server.
    // */
    // private void socksBind() throws IOException
    // {
    //
    // }
    //
    // private static void intToBytes(int value, byte[] bytes, int start)
    // {
    // /*
    // * Shift the int so the current byte is right-most Use a byte mask of
    // * 255 to single out the last byte.
    // */
    // bytes[start] = (byte) ((value >> 24) & 255);
    // bytes[start + 1] = (byte) ((value >> 16) & 255);
    // bytes[start + 2] = (byte) ((value >> 8) & 255);
    // bytes[start + 3] = (byte) (value & 255);
    // }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#connect(java.net.SocketAddress, int)
     */
    @Override
    protected void connect(SocketAddress remoteAddr, int timeout)
            throws IOException
    {
        InetSocketAddress inetAddr = (InetSocketAddress) remoteAddr;
        connect(inetAddr.getAddress(), inetAddr.getPort(), timeout);
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#supportsUrgentData()
     */
    @Override
    protected boolean supportsUrgentData()
    {
        return false;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.SocketImpl#sendUrgentData(int)
     */
    @Override
    protected void sendUrgentData(int value) throws IOException
    {

    }

    // /**
    // * Gets the fD.
    // *
    // * @return the fD
    // */
    // FileDescriptor getFD()
    // {
    // return fd;
    // }

    /**
     * 读入数据至缓冲区
     * 
     * @param buffer
     *            缓冲区
     * @param offset
     *            偏移量
     * @param count
     *            读入字节数
     * @return 实际读入字节数
     * @throws IOException
     *             I/O异常
     */
    public int read(byte[] buffer, int offset, int count) throws IOException
    {
        if (bShutdownInput)
        {
            return -1;
        }

        int iRet = 0;
        // int iIndex = 0;
        byte[] arrayByteRecvBuf = new byte[count];

        iRet = socketApi.svnRecv(iSvnFd, arrayByteRecvBuf, count, 0);

        if (-61 == iRet)
        {
            throw new SocketTimeoutException();
        }
        
        if (iRet < -1)
        {
            throw new IOException("socket recv error:" + iRet);
        }
        
        // Return of -1 indicates the peer was closed
        if (-1 == iRet || 0 == iRet)
        {
            bShutdownInput = true;
            return iRet;
        }

//        if (0 == iRet)
//        {
//            throw new SocketTimeoutException();
//        }



        if (iRet > 0)
        {
            System.arraycopy(arrayByteRecvBuf, 0, buffer, offset, iRet);
            // for (iIndex = 0; iIndex < iRet; iIndex++)
            // {
            // buffer[offset++] = arrayByteRecvBuf[iIndex]; // modified by xwei
            // // 20120202
            // }
        }

        // for (iIndex = 0; iIndex < count; iIndex++)
        // {
        // buffer[offset++] = arrayByteRecvBuf[iIndex]; // modified by xwei
        // // 20120202
        // }
        return iRet;
    }

    /**
     * 发送缓冲区数据
     * 
     * @param buffer
     *            缓冲区
     * @param offset
     *            偏移量
     * @param count
     *            发送字节数
     * @return 实际发送字节数
     * @throws IOException
     *             I/O异常
     */
    public int write(byte[] buffer, int offset, int count) throws IOException
    {
        int iRet = 0;
        // int iIndex = 0;
        // int iBufLen = buffer.length;
        byte[] arrayByteSendBuf = new byte[count];

        System.arraycopy(buffer, offset, arrayByteSendBuf, 0, count);

        // for (iIndex = 0; iIndex < count; iIndex++)
        // {
        // arrayByteSendBuf[iIndex] = buffer[offset++]; // modified by xwei
        // // 20120202
        // }

        SvnSockaddrIn sockaddr = new SvnSockaddrIn();
        sockaddr.setSinAddr(address.getHostAddress());
        sockaddr.setSinPort(port);
        sockaddr.setSinFamily(2);// must be AF_INET

        if (streaming)
        {
            iRet = socketApi.svnSend(iSvnFd, arrayByteSendBuf, count, 0);
            if (0 > iRet)
            {
                throw new IOException(String.valueOf(iRet));
            }
        }
        else
        {
            iRet = socketApi.svnSendto(iSvnFd, arrayByteSendBuf, count, 0,
                    sockaddr);
            if (0 > iRet)
            {
                throw new IOException(String.valueOf(iRet));
            }
        }
        return iRet;
    }
}
