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
 * An HTTP payload terminated by the end of the socket stream.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */

public final class UnknownLengthHttpInputStream extends AbstractHttpInputStream
{
    
    /** The input exhausted. */
    private boolean inputExhausted;

    /**
     * Instantiates a new unknown length http input stream.
     * 
     * @param is
     *            the is
     * @param cacheRequest
     *            the cache request
     * @param httpURLConnection
     *            the http url connection
     * @throws IOException
     *              I/O异常
     */
    UnknownLengthHttpInputStream(InputStream is, CacheRequest cacheRequest,
            SvnHttpURLConnection httpURLConnection) throws IOException
    {
        super(is, httpURLConnection, cacheRequest);
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#read(byte[], int, int)
     */
    @Override
    public int read(byte[] buffer, int offset, int count) throws IOException
    {
        checkBounds(buffer, offset, count);
        checkNotClosed();
        if (null == in)
        {
            return -1;
        }
        int read = in.read(buffer, offset, count);
        if (-1 == read )
        {
            inputExhausted = true;
            endOfInput(false);
            return -1;
        }
        cacheWrite(buffer, offset, read);
        return read;
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#available()
     */
    @Override
    public int available() throws IOException
    {
        checkNotClosed();
        return null == in ? 0 : in.available();
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
        if (!inputExhausted)
        {
            unexpectedEndOfInput();
        }
    }
}