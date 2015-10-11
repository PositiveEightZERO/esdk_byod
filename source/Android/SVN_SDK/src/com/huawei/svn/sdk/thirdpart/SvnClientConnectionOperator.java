/*
 * ====================================================================
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 */

package com.huawei.svn.sdk.thirdpart;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import org.apache.http.HttpHost;
import org.apache.http.conn.ClientConnectionOperator;
import org.apache.http.conn.OperatedClientConnection;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.conn.scheme.SocketFactory;
import org.apache.http.impl.conn.DefaultClientConnection;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.protocol.HttpContext;

import android.util.Log;

import com.huawei.svn.sdk.socket.SvnSocket;


/**
 * 〈一句话功能简述〉 〈功能详细描述〉.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnClientConnectionOperator implements ClientConnectionOperator
{

    /** SchemeRegistry对象，用来查找所用的SocketFactory. */
    protected final SchemeRegistry schemeRegistry; // @ThreadSafe

    /**
     * 构造函数
     * 使用SchemeRegistry对象创建SvnClientConnectionOperator.
     * 
     * @param schemes
     *            the scheme registry
     * 
     * @since 4.2
     */
    public SvnClientConnectionOperator(final SchemeRegistry schemes)
    {

        this.schemeRegistry = schemes;

    }

    /**
     * 获取 SchemeRegistry对象.
     * 
     * @return the scheme registry for looking up socket factories
     */
    private SchemeRegistry getSchemeRegistry()
    {
        // SchemeRegistry reg = (SchemeRegistry) context
        // .getAttribute(ClientContext.);
        // if (reg == null)
        // {
        // reg = this.schemeRegistry;
        // }
        return this.schemeRegistry;
    }
    
    
    /* (non-Javadoc)
     * @see org.apache.http.conn.ClientConnectionOperator#createConnection()
     */
    @Override
    public OperatedClientConnection createConnection()
    {
        return new DefaultClientConnection();
    }


    
    /* (non-Javadoc)
     * @see org.apache.http.conn.ClientConnectionOperator#openConnection(org.apache.http.conn.OperatedClientConnection, org.apache.http.HttpHost, java.net.InetAddress, org.apache.http.protocol.HttpContext, org.apache.http.params.HttpParams)
     */
    @Override
    public void openConnection(final OperatedClientConnection conn,
            final HttpHost target, final InetAddress local,
            final HttpContext context, final HttpParams params)
            throws IOException
    {

        final SchemeRegistry registry = getSchemeRegistry();
        final Scheme schm = registry.getScheme(target.getSchemeName());
        final SocketFactory sf = schm.getSocketFactory();

        String hostUri = target.getHostName();
        
        int port = target.getPort();
        if(port == -1)
        {
            port = schm.getDefaultPort();
        }
        final InetSocketAddress[] addresses = resolveHostname(hostUri,
                port);
        //final int port = schm.resolvePort(target.getPort());
        InetSocketAddress address = null;
        boolean last = false;
        
        Socket connsock = null;
        Socket sock = null;
        for (int i = 0; i < addresses.length; i++)
        {
            address = addresses[i];
            last = (i == addresses.length - 1);

            try
            {
                sock = sf.createSocket();
                
                conn.opening(sock, target);
                
                connsock = sf.connectSocket(sock, address
                        .getAddress().getHostAddress(), address.getPort(),
                        local, 0, params);

                // final Socket connsock = sf.connectSocket(sock, remoteAddress,
                // localAddress, params);
                if (sock != connsock)
                {
                    if(sock != null)
                    {
                        try
                        {
                            sock.close();
                        }
                        catch (IOException e)
                        {
                            Log.i("SDK", "Socket close exception!");
                        }
                    }
                    sock = connsock;
                    conn.opening(sock, target);
                }
                prepareSocket(sock, context, params);
                conn.openCompleted(sf.isSecure(sock), params);
                return;
            }
            catch (IOException ex)
            {
                if(sock != null)
                {
                    try
                    {
                        sock.close();
                    }
                    catch (IOException e)
                    {
                        Log.i("SDK", "Socket close exception!");
                    }
                    sock = null;
                }
                
                if (last)
                {
                    throw ex;
                }
            }

        }
    }

    /* (non-Javadoc)
     * @see org.apache.http.conn.ClientConnectionOperator#updateSecureConnection(org.apache.http.conn.OperatedClientConnection, org.apache.http.HttpHost, org.apache.http.protocol.HttpContext, org.apache.http.params.HttpParams)
     */
    @Override
    public void updateSecureConnection(final OperatedClientConnection conn,
            final HttpHost target, final HttpContext context,
            final HttpParams params) throws IOException
    {
        // Args.notNull(conn, "Connection");
        // Args.notNull(target, "Target host");
        // Args.notNull(params, "Parameters");
        // Asserts.check(conn.isOpen(), "Connection must be open");

        // final SchemeRegistry registry = getSchemeRegistry(context);
        // final Scheme schm = registry.getScheme(target.getSchemeName());
        // Asserts.check(
        // schm.getSocketFactory() instanceof LayeredConnectionSocketFactory,
        // "Socket factory must implement SchemeLayeredSocketFactory");
        // final SchemeLayeredSocketFactory lsf = (SchemeLayeredSocketFactory)
        // schm
        // .getSocketFactory();
        // final Socket sock = lsf.createLayeredSocket(conn.getSocket(),
        // target.getHostName(), schm.resolvePort(target.getPort()),
        // params);
        // prepareSocket(sock, context, params);
        // conn.update(sock, target, lsf.isSecure(sock), params);
        
    }

    /**
     * 对新建的SvnSocket设置通用参数
     * 
     * @param sock
     *            Socket对象
     * @param context
     *            HTTP连接Context
     * @param params
     *            HTTP连接参数
     * 
     * @throws IOException
     *             发生I/O异常
     */
    protected void prepareSocket(final Socket sock, final HttpContext context,
            final HttpParams params) throws IOException
    {
        sock.setTcpNoDelay(HttpConnectionParams.getTcpNoDelay(params));
        sock.setSoTimeout(HttpConnectionParams.getSoTimeout(params));

        final int linger = HttpConnectionParams.getLinger(params);
        if (linger >= 0)
        {
            sock.setSoLinger(linger > 0, linger);
        }
    }

    /**
     * 将传入的主机名解析为IP地址
     * 此功能需要网络中配置有DNS服务器，如果未配置，将使用默认的解析结果.
     * 
     * @param host
     *            要解析的主机名
     * @param port
     *            端口
     * @return IP地址列表
     * @throws UnknownHostException
     *             无法解析出该主机名对应的IP地址
     */
    protected InetSocketAddress[] resolveHostname(final String host,
            final int port) throws UnknownHostException
    {
        InetSocketAddress[] result = new InetSocketAddress[1];
        InetAddress address = SvnSocket.getHostbyName(host);
        result[0] = new InetSocketAddress(address, port);

        return result;
    }

}
