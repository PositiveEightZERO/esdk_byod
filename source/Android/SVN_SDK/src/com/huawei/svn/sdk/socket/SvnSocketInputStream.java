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
import java.net.SocketImpl;


/**
 * SvnSocketInputStream
 * 
 * 支持从SvnSocket进行流式读入，在一个SvnSocket上可能打开多个流， 因此要注意打开流的管理和线程间的协调处理。.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnSocketInputStream extends InputStream
{

    /** SvnSocket实现*/
    private final SvnPlainSocketImpl socket;

    /**
     * 构造函数
     * 
     * @param socket
     *            SvnSocket实现
     * @see SvnSocket, SvnPlainSocketImpl
     */
    public SvnSocketInputStream(SocketImpl socket)
    {
        super();
        if(socket != null && socket instanceof SvnPlainSocketImpl)
        {
            this.socket = (SvnPlainSocketImpl) socket;
        }
        else 
        {
            this.socket = null;
        }
    }


    /* (non-Javadoc)
     * @see java.io.InputStream#available()
     */
    public int available() throws IOException
    {
        return socket.available();
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#close()
     */
    @Override
    public void close() throws IOException
    {
        socket.close();
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#read()
     */
    @Override
    public int read() throws IOException
    {
        //System.out.println("svn read 1------- ");
        byte[] buffer = new byte[1];
        int result = socket.read(buffer, 0, 1);
        // System.out.println("svn read without params offset is ");
        return (result == -1) ? result : buffer[0] & 0xFF;
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#read(byte[])
     */
    @Override
    public int read(byte[] buffer) throws IOException
    {
        return read(buffer, 0, buffer.length);
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#read(byte[], int, int)
     */
    @Override
    public int read(byte[] buffer, int offset, int count) throws IOException
    {
        //System.out.println("svn read 2------- " + count);
        if (buffer == null)
        {
            throw new IOException("buffer == null");
        }

        if (count == 0)
        {
            //System.out.println("svn read 2-------return " + 0);
            return 0;
        }
        // System.out.println("svn read with params offset is "k+ offset);
        if (0 > offset || offset >= buffer.length)
        {
            throw new ArrayIndexOutOfBoundsException("Offset out of bounds: "
                    + offset);
        }
        if (0 > count || offset + count > buffer.length)
        {
            throw new ArrayIndexOutOfBoundsException();
        }

        int ret =  socket.read(buffer, offset, count);
        
        //System.out.println("svn read 2-------ret: "+ ret);
        return ret;
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#skip(long)
     */
    @Override
    public long skip(long n) throws IOException
    {
        return (0 == n) ? 0 : super.skip(n);
    }
}
