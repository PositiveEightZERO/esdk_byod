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

import java.io.IOException;
import java.io.OutputStream;


/**
 * An output stream for the body of an HTTP request.
 * 
 * <p>Since a single socket's output stream may be used to write multiple HTTP
 * requests to the same server, subclasses should not close the socket stream.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public abstract class AbstractHttpOutputStream extends OutputStream
{
    
    /** The closed. */
    protected boolean closed;

    /* (non-Javadoc)
     * @see java.io.OutputStream#write(int)
     */
    @Override
    public final void write(int data) throws IOException
    {
        write(new byte[] {(byte) data});
    }

    /**
     * Check bounds.
     * 
     * @param buffer
     *            the buffer
     * @param offset
     *            the offset
     * @param count
     *            the count
     */
    protected final void checkBounds(byte[] buffer, int offset, int count)
    {
        if (offset < 0 || offset > buffer.length || count < 0
                || buffer.length - offset < count)
        {
            throw new ArrayIndexOutOfBoundsException("offset=" + offset
                    + ", buffer.length=" + buffer.length + ", count=" + count);
        }
    }

    /**
     * Check not closed.
     * 
     * @throws IOException
     *              I/O异常
     */
    protected final void checkNotClosed() throws IOException
    {
        if (closed)
        {
            throw new IOException("stream closed");
        }
    }
}