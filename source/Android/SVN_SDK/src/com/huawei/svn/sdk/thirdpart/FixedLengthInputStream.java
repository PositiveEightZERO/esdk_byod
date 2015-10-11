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
import java.io.InputStream;
import java.net.CacheRequest;


/**
 * An HTTP body with a fixed length specified in advance.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public final class FixedLengthInputStream extends AbstractHttpInputStream
{
    
    /** The bytes remaining. */
    private int bytesRemaining;

    /**
     * Instantiates a new fixed length input stream.
     * 
     * @param is
     *            the is
     * @param cacheRequest
     *            the cache request
     * @param httpURLConnection
     *            the http url connection
     * @param length
     *            the length
     * @throws IOException
     *              I/O异常
     */
    public FixedLengthInputStream(InputStream is, CacheRequest cacheRequest,
            SvnHttpURLConnection httpURLConnection, int length)
            throws IOException
    {
        super(is, httpURLConnection, cacheRequest);
        bytesRemaining = length;
        if (bytesRemaining == 0)
        {
            endOfInput(true);
        }
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#read(byte[], int, int)
     */
    @Override
    public int read(byte[] buffer, int offset, int count) throws IOException
    {
        checkBounds(buffer, offset, count);
        checkNotClosed();
        if (bytesRemaining == 0)
        {
            return -1;
        }
        int read = in.read(buffer, offset, Math.min(count, bytesRemaining));
        if (read == -1)
        {
            unexpectedEndOfInput(); // the server didn't supply the promised
                                    // content length
            throw new IOException("unexpected end of stream");
        }
        bytesRemaining -= read;
        cacheWrite(buffer, offset, read);
        if (bytesRemaining == 0)
        {
            endOfInput(true);
        }
        return read;
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#available()
     */
    @Override
    public int available() throws IOException
    {
        checkNotClosed();
        return bytesRemaining == 0 ? 0 : Math.min(in.available(),
                bytesRemaining);
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#close()
     */
    @Override
    public void close() throws IOException
    {
        if (closed)
        {
            return;
        }
        closed = true;
        if (bytesRemaining != 0)
        {
            unexpectedEndOfInput();
        }
    }
}