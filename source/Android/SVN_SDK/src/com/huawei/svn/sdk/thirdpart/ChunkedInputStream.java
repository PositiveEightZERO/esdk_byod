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
 * An HTTP body with alternating chunk sizes and chunk bodies.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public final class ChunkedInputStream extends AbstractHttpInputStream
{
    
    /** The Constant MIN_LAST_CHUNK_LENGTH. */
    private static final int MIN_LAST_CHUNK_LENGTH = "\r\n0\r\n\r\n".length();
    
    /** The Constant NO_CHUNK_YET. */
    private static final int NO_CHUNK_YET = -1;
    
    /** The bytes remaining in chunk. */
    private int bytesRemainingInChunk = NO_CHUNK_YET;
    
    /** The has more chunks. */
    private boolean hasMoreChunks = true;

    /**
     * Instantiates a new chunked input stream.
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
    ChunkedInputStream(InputStream is, CacheRequest cacheRequest,
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

        if (!hasMoreChunks)
        {
            return -1;
        }
        if (bytesRemainingInChunk == 0 || bytesRemainingInChunk == NO_CHUNK_YET)
        {
            readChunkSize();
            if (!hasMoreChunks)
            {
                return -1;
            }
        }
        int read = in.read(buffer, offset,
                Math.min(count, bytesRemainingInChunk));
        if (read == -1)
        {
            unexpectedEndOfInput(); // the server didn't supply the promised
                                    // chunk length
            throw new IOException("unexpected end of stream");
        }
        bytesRemainingInChunk -= read;
        cacheWrite(buffer, offset, read);

        /*
         * If we're at the end of a chunk and the next chunk size is readable,
         * read it! Reading the last chunk causes the underlying connection to
         * be recycled and we want to do that as early as possible. Otherwise
         * self-delimiting streams like gzip will never be recycled.
         * http://code.google.com/p/android/issues/detail?id=7059
         */
        if (bytesRemainingInChunk == 0
                && in.available() >= MIN_LAST_CHUNK_LENGTH)
        {
            readChunkSize();
        }

        return read;
    }

    /**
     * Read chunk size.
     * 
     * @throws IOException
     *              I/O异常
     */
    private void readChunkSize() throws IOException
    {
        // read the suffix of the previous chunk
        if (bytesRemainingInChunk != NO_CHUNK_YET)
        {
            SvnHttpURLConnection.readLine(in);
        }
        String chunkSizeString = SvnHttpURLConnection.readLine(in);
        int index = chunkSizeString.indexOf(";");
        if (index != -1)
        {
            chunkSizeString = chunkSizeString.substring(0, index);
        }
        try
        {
            bytesRemainingInChunk = Integer
                    .parseInt(chunkSizeString.trim(), 16);
        }
        catch (NumberFormatException e)
        {
            throw new IOException("Expected a hex chunk size, but was "
                    + chunkSizeString, e);
        }
        if (bytesRemainingInChunk == 0)
        {
            hasMoreChunks = false;
            httpURLConnection.readHeaders(); // actually trailers!
            endOfInput(true);
        }
    }

    /* (non-Javadoc)
     * @see java.io.InputStream#available()
     */
    @Override
    public int available() throws IOException
    {
        checkNotClosed();
        return hasMoreChunks ? Math.min(in.available(), bytesRemainingInChunk)
                : 0;
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
        if (hasMoreChunks)
        {
            unexpectedEndOfInput();
        }
    }
}