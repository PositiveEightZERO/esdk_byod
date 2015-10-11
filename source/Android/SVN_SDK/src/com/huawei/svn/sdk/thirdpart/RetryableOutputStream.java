/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.huawei.svn.sdk.thirdpart;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;


/**
 * An HTTP request body that's completely buffered in memory. This allows the
 * post body to be transparently re-sent if the HTTP request must be sent
 * multiple times.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public final class RetryableOutputStream extends AbstractHttpOutputStream
{
    
    /** The limit. */
    private final int limit;
    
    /** The content. */
    private final ByteArrayOutputStream content;

    /**
     * Instantiates a new retryable output stream.
     * 
     * @param limit
     *            the limit
     */
    public RetryableOutputStream(int limit)
    {
        this.limit = limit;
        this.content = new ByteArrayOutputStream(limit);
    }

    /**
     * Instantiates a new retryable output stream.
     */
    public RetryableOutputStream()
    {
        this.limit = -1;
        this.content = new ByteArrayOutputStream();
    }

    /* (non-Javadoc)
     * @see java.io.OutputStream#close()
     */
    @Override
    public synchronized void close() throws IOException
    {
        if (closed)
        {
            return;
        }
        closed = true;
        if (content.size() < limit)
        {
            throw new IOException("content-length promised " + limit
                    + " bytes, but received " + content.size());
        }
    }

    /* (non-Javadoc)
     * @see java.io.OutputStream#write(byte[], int, int)
     */
    @Override
    public synchronized void write(byte[] buffer, int offset, int count)
            throws IOException
    {
        checkNotClosed();
        checkBounds(buffer, offset, count);
        if (limit != -1 && content.size() > limit - count)
        {
            throw new IOException("exceeded content-length limit of " + limit
                    + " bytes");
        }
        content.write(buffer, offset, count);
    }

    /**
     * Content length.
     * 
     * @return the int
     * @throws IOException
     *              I/O异常
     */
    public synchronized int contentLength() throws IOException
    {
        //close();
        return content.size();
    }

    /**
     * Write to socket.
     * 
     * @param socketOut
     *            the socket out
     * @throws IOException
     *              I/O异常
     */
    public synchronized void writeToSocket(OutputStream socketOut) throws IOException
    {
        //close();
        content.writeTo(socketOut);
        socketOut.flush();
    }
}