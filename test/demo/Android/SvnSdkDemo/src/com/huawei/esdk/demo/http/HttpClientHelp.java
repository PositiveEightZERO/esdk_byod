package com.huawei.esdk.demo.http;

import org.apache.http.client.HttpClient;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.HttpConnectionParams;

import com.huawei.svn.sdk.thirdpart.SvnHttpClient;

public class HttpClientHelp
{
    static
    {
        java.util.logging.Logger.getLogger("org.apache.http.wire").setLevel(
                java.util.logging.Level.FINEST);
        java.util.logging.Logger.getLogger("org.apache.http.headers").setLevel(
                java.util.logging.Level.FINEST);
        System.setProperty("org.apache.commons.logging.Log",
                "org.apache.commons.logging.impl.SimpleLog");
        System.setProperty("org.apache.commons.logging.simplelog.showdatetime",
                "true");
        System.setProperty(
                "org.apache.commons.logging.simplelog.log.httpclient.wire",
                "debug");
        System.setProperty(
                "org.apache.commons.logging.simplelog.log.org.apache.http",
                "debug");
        System.setProperty(
                "org.apache.commons.logging.simplelog.log.org.apache.http.headers",
                "debug");
        //        adb shell setprop log.tag.org.apache.http VERBOSE
        //        adb shell setprop log.tag.org.apache.http.wire VERBOSE
        //        adb shell setprop log.tag.org.apache.http.headers VERBOSE
    }
    private static DefaultHttpClient httpClient = null;

    private HttpClientHelp()
    {
    }

    public synchronized static HttpClient getInstance()
    {
        if (null == httpClient)
        {
            
            httpClient = new SvnHttpClient();
            //httpClient = new SSLHttpClient();
            
            //httpClient = new DefaultHttpClient();
            
            httpClient.getParams().setIntParameter(  
                    HttpConnectionParams.SO_TIMEOUT, 2000); // 超时设置  
            httpClient.getParams().setIntParameter(  
                    HttpConnectionParams.CONNECTION_TIMEOUT, 5000);// 连接超时  
        }
        // if (cookie != null)
        // {
        // ((AbstractHttpClient)httpClient)
        // .setCookieStore(cookie);
        // Log.i("eSDK", "Cookie = " + cookie.toString());
        // }
        return httpClient;
    }
    // public static CookieStore getCookie()
    // {
    // return cookie;
    // }
    //
    // public static void setCookie(CookieStore cookie)
    // {
    // HttpClientHelp.cookie = cookie;
    // }
}
