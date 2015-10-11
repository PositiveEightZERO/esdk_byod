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
import java.io.OutputStream;
import java.net.SocketImpl;


/**
 * 〈一句话功能简述〉 〈功能详细描述〉
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnSocketOutputStream extends OutputStream
{

    /** The socket*/
    private SvnPlainSocketImpl socket;

    /**
     * Constructs a SocketOutputStream for the <code>socket</code>. Write
     * operations are forwarded to the <code>socket</code>.
     * 
     * @param socket
     *            the socket to be written
     * @see Socket
     */
    public SvnSocketOutputStream(SocketImpl socket)
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
     * @see java.io.OutputStream#close()
     */
    @Override
    public void close() throws IOException
    {
        socket.close();
    }

    /* (non-Javadoc)
     * @see java.io.OutputStream#write(byte[])
     */
    @Override
    public void write(byte[] buffer) throws IOException
    {
        socket.write(buffer, 0, buffer.length);
    }

    /* (non-Javadoc)
     * @see java.io.OutputStream#write(byte[], int, int)
     */
    @Override
    public void write(byte[] buffer, int offset, int count) throws IOException
    {
        //System.out.println("svn write 1------- ");
        if (buffer == null)
        {
            throw new IOException("buffer == null");
        }
        // if (0 <= offset && offset <= buffer.length && 0 <= count && count <=
        // buffer.length - offset) { //�����ư��С��delete by wuxiaolong
        if (0 <= offset && offset <= buffer.length && 0 <= count)
        {
            int sentCount = 0;
            
            int iSend = 0;
            while (sentCount < count)
            {
                //System.out.println("buffer size:" + buffer.length + ", offset:" + (offset + sentCount) + ",count:" + (count - sentCount));
                iSend = socket.write(buffer, offset + sentCount, count - sentCount);
                sentCount += iSend;
                
                //System.out.println("sent count:" + sentCount);
            }
        }
        else
        {
            throw new ArrayIndexOutOfBoundsException();
        }
    }

    /* (non-Javadoc)
     * @see java.io.OutputStream#write(int)
     */
    @Override
    public void write(int oneByte) throws IOException
    {
        //System.out.println("svn write 2------- ");
        byte[] buffer = new byte[1];
        buffer[0] = (byte) (oneByte & 0xFF);
        socket.write(buffer, 0, 1);
    }
}
