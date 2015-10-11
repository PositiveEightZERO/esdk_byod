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
 * An HTTP body with a fixed length known in advance.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public final class FixedLengthOutputStream extends AbstractHttpOutputStream
{
    
    /** The socket out. */
    private final OutputStream socketOut;
    
    /** The bytes remaining. */
    private int bytesRemaining;

    /**
     * Instantiates a new fixed length output stream.
     * 
     * @param socketOut
     *            the socket out
     * @param bytesRemaining
     *            the bytes remaining
     */
    public FixedLengthOutputStream(OutputStream socketOut, int bytesRemaining)
    {
        this.socketOut = socketOut;
        this.bytesRemaining = bytesRemaining;
    }

    /* (non-Javadoc)
     * @see java.io.OutputStream#write(byte[], int, int)
     */
    @Override
    public void write(byte[] buffer, int offset, int count) throws IOException
    {
        checkNotClosed();
        checkBounds(buffer, offset, count);
        if (count > bytesRemaining)
        {
            throw new IOException("expected " + bytesRemaining
                    + " bytes but received " + count);
        }
        socketOut.write(buffer, offset, count);
        bytesRemaining -= count;
    }

    /* (non-Javadoc)
     * @see java.io.OutputStream#flush()
     */
    @Override
    public void flush() throws IOException
    {
        if (closed)
        {
            return; // don't throw; this stream might have been closed on the
                    // caller's behalf
        }
        socketOut.flush();
    }

    /* (non-Javadoc)
     * @see java.io.OutputStream#close()
     */
    @Override
    public void close() throws IOException
    {
        if (closed)
        {
            return;
        }
        closed = true;
        if (bytesRemaining > 0)
        {
            throw new IOException("unexpected end of stream");
        }
    }
}