/*
 * 
 */
package com.huawei.svn.sdk.thirdpart;

import java.io.IOException;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLStreamHandler;


/**
 * SvnURLStreamHandler
 * 用来处理HTTP连接，负责建立SVN隧道上的HTTP连接
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnURLStreamHandler extends URLStreamHandler
{

    /* (non-Javadoc)
     * @see java.net.URLStreamHandler#openConnection(java.net.URL)
     */
    @Override
    protected URLConnection openConnection(URL url) throws IOException
    {
        return new SvnHttpURLConnection(url);
    }

}
