/*
 * 
 */
package com.huawei.svn.sdk.thirdpart;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;

import org.apache.http.conn.ConnectTimeoutException;
import org.apache.http.conn.scheme.SocketFactory;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;

import com.huawei.svn.sdk.socket.SvnSocket;


/**
 * SvnHttpSocketFactory
 * 用来创建SvnSocket
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnHttpSocketFactory implements SocketFactory
{

    /* (non-Javadoc)
     * @see org.apache.http.conn.scheme.SocketFactory#connectSocket(java.net.Socket, java.lang.String, int, java.net.InetAddress, int, org.apache.http.params.HttpParams)
     */
    @Override
    public Socket connectSocket(Socket sock, String host, int port,
            InetAddress localAddress, int localPort, HttpParams params)
            throws IOException, ConnectTimeoutException
    {
        int connTimeout = HttpConnectionParams.getConnectionTimeout(params);
        int soTimeout = HttpConnectionParams.getSoTimeout(params);
        
        InetAddress address = SvnSocket.getHostbyName(host);
        
        InetSocketAddress remoteAddress = new InetSocketAddress(address, port);
        //InetSocketAddress remoteAddress = SvnSocket.getHostbyName(host, port);
        
        Socket oldSokcet = (sock != null) ? sock : createSocket();
        if(oldSokcet!= null && oldSokcet instanceof SvnSocket)
        {
            SvnSocket sslsock = (SvnSocket) (oldSokcet);
            if ((localAddress != null) || (localPort > 0))
            {
                // we need to bind explicitly
                if (localPort < 0)
                {
                    localPort = 0; // indicates "any"
                }
                InetSocketAddress isa = new InetSocketAddress(localAddress,
                        localPort);
                sslsock.bind(isa);
            }
            sslsock.connect(remoteAddress, connTimeout);
            sslsock.setSoTimeout(soTimeout);
            return sslsock;
        }
        return null;

    }

    /* (non-Javadoc)
     * @see org.apache.http.conn.scheme.SocketFactory#createSocket()
     */
    @Override
    public Socket createSocket() throws IOException
    {
        Socket socket = new SvnSocket();
        return socket;
    }

    /* (non-Javadoc)
     * @see org.apache.http.conn.scheme.SocketFactory#isSecure(java.net.Socket)
     */
    @Override
    public boolean isSecure(Socket arg0) throws IllegalArgumentException
    {
        return false;
    }

}
