/*
 * 
 */
package com.huawei.svn.sdk.thirdpart;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.CookieHandler;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.ProtocolException;
import java.net.Socket;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.security.AccessController;
import java.util.List;
import java.util.Map;
import java.util.zip.GZIPInputStream;

import android.util.Log;

import com.huawei.svn.sdk.socket.SvnSocket;

/**
 * SvnHttpURLConnection
 * 
 * SVN隧道上HTTP连接实现，发送HTTP请求，处理HTTP响应。
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnHttpURLConnection extends HttpURLConnection
{

    /** The Constant OPTIONS. */
    public static final String OPTIONS = "OPTIONS";

    /** The Constant GET. */
    public static final String GET = "GET";

    /** The Constant HEAD. */
    public static final String HEAD = "HEAD";

    /** The Constant POST. */
    public static final String POST = "POST";

    /** The Constant PUT. */
    public static final String PUT = "PUT";

    /** The Constant DELETE. */
    public static final String DELETE = "DELETE";

    /** The Constant TRACE. */
    public static final String TRACE = "TRACE";

    /** The Constant CONNECT. */
    public static final String CONNECT = "CONNECT";

    /** The Constant HTTP_CONTINUE. */
    public static final int HTTP_CONTINUE = 100;

    /**
     * HTTP 1.1 doesn't specify how many redirects to follow, but HTTP/1.0
     * recommended 5. http://www.w3.org/Protocols/HTTP/1.0/spec.html#Code3xx
     */
    public static final int MAX_REDIRECTS = 5;

    /**
     * The subset of HTTP methods that the user may select via.
     * {@link #setRequestMethod}.
     */
    private static final String[] PERMITTED_USER_METHODS = {OPTIONS, GET, HEAD,
            POST, PUT, DELETE, TRACE
    // Note: we don't allow users to specify "CONNECT"
    };

    /** The Constant DEFAULT_CHUNK_LENGTH. */
    private static final int DEFAULT_CHUNK_LENGTH = 1024;

    /** The default port. */
    private int defaultPort;

    /**
     * The version this client will use. Either 0 for HTTP/1.0, or 1 for
     * HTTP/1.1. Upon receiving a non-HTTP/1.1 response, this client
     * automatically sets its version to HTTP/1.0.
     */
    private int httpVersion = 1; // Assume HTTP/1.1

    // protected HttpConnection httpConnection;
    /** The socket in. */
    private InputStream socketIn;

    /** The socket out. */
    private OutputStream socketOut;

    /** The response body in. */
    private InputStream responseBodyIn;

    /** The request body out. */
    private AbstractHttpOutputStream requestBodyOut;

    /** The default request header. */
    private static Header defaultRequestHeader = new Header();

    /** The request header. */
    private Header requestHeader;

    /** Null until a response is received from the network or the cache. */
    private Header responseHeader;

    /** The redirection count. */
    private int redirectionCount;

    /**
     * Intermediate responses are always followed by another request for the
     * same content, possibly from a different URL or with different headers.
     */
    protected boolean intermediateResponse = false;

    /** The connection. */
    private Socket connection;

    /** The sent request headers. */
    private boolean sentRequestHeaders;

    /** The send chunked. */
    private boolean sendChunked;

    /** The uri. */
    private URI uri;

    /**
     * True if this client added an "Accept-Encoding: gzip" header and is
     * therefore responsible for also decompressing the transfer stream.
     */
    private boolean transparentGzip = false;

    /**
     * Instantiates a new svn http url connection.
     * 
     * @param url
     *            the url
     */
    protected SvnHttpURLConnection(URL url)
    {
        super(url);
        defaultPort = url.getPort();
        // System.out.println("defualtPort 1:" + defaultPort);
        if (defaultPort == -1)
        {
            defaultPort = 80;
        }
        // System.out.println("defualtPort 2:" + defaultPort);

        try
        {
            uri = url.toURI();
            // uri.getHost();
        }
        catch (URISyntaxException e)
        {
            Log.e("SDK",
                    "SvnHttpURLConnection URISyntaxException:" + e.getMessage());
            // e.printStackTrace();
        }

        initRequestHeader();
    }

    /**
     * 初始化 request header.
     */
    private synchronized void initRequestHeader()
    {
        Header clonedHeader = null;

        try
        {
            clonedHeader = (Header) defaultRequestHeader.clone();
        }
        catch (CloneNotSupportedException e)
        {
            Log.e("SDK", e.getMessage());
        }

        requestHeader = clonedHeader;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.HttpURLConnection#disconnect()
     */
    @Override
    public synchronized void disconnect()
    {
        if (connected)
        {
            try
            {
                this.connection.close();
            }
            catch (IOException e)
            {
                Log.e("SDK",
                        "SvnHttpURLConnection disconnect:" + e.getMessage());
            }
            connected = false;
        }
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.HttpURLConnection#usingProxy()
     */
    @Override
    public boolean usingProxy()
    {
        return false;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.URLConnection#connect()
     */
    @Override
    public void connect() throws IOException
    {
        if (!connected)
        {
            makeConnection();
        }

    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.URLConnection#getInputStream()
     */
    @Override
    public synchronized InputStream getInputStream() throws IOException
    {
        if (!doInput)
        {
            throw new ProtocolException("This protocol does not support input");
        }

        retrieveResponse();

        /*
         * if the requested file does not exist, throw an exception formerly the
         * Error page from the server was returned if the requested file was
         * text/html this has changed to return FileNotFoundException for all
         * file types
         */
        if (responseCode >= HTTP_BAD_REQUEST)
        {
            throw new FileNotFoundException("file not found:" + url.toString());
        }

        if (responseBodyIn == null)
        {
            throw new IOException("No response body exists; responseCode="
                    + responseCode);
        }

        return responseBodyIn;

    }

    /**
     * Internal method to open a connection to the server. Unlike connect(),
     * this method may be called multiple times for a single response. This may
     * be necessary when following redirects.
     * 
     * <p> Request parameters may not be changed after this method has been
     * called.
     * 
     * @throws IOException
     *             I/O异常
     */
    public synchronized void makeConnection() throws IOException
    {
        // connected = true;

        if (connection != null)
        {
            return;
        }

        try
        {
            uri = new URI(url.getProtocol(), null, url.getHost(),
                    url.getPort(), url.getPath(), null, null);
        }
        catch (URISyntaxException e1)
        {
            Log.e("SDK", "URISyntaxException:" + e1.getMessage());
            throw new IOException("URISyntaxException:" + e1.getMessage(), e1);
        }

        int port = url.getPort();
        if (port < 0)
        {
            port = defaultPort;
        }
        
        
        int connTimeout = getConnectTimeout();
        int readTimeout = getReadTimeout();
        
        connection = new SvnSocket();
        
        InetAddress address = SvnSocket.getHostbyName(url.getHost());
        
        InetSocketAddress remoteAddress = new InetSocketAddress(address, port);
       
        connection.setSoTimeout(readTimeout);
        connection.connect(remoteAddress, connTimeout);
 
        connected = connection.isConnected();

        setUpTransportIO(connection);
    }
    /**
     * Sets up the data streams used to send requests and read responses.
     * 
     * @param connection
     *            the new up transport io
     * @throws IOException
     *             I/O异常
     */
    protected synchronized void setUpTransportIO(Socket connection) throws IOException
    {
        socketOut = connection.getOutputStream();
        socketIn = connection.getInputStream();
    }

    /**
     * Populates requestHeader with the HTTP headers to be sent. Header values
     * are derived from the request itself and the cookie manager.
     * 
     * <p> This client doesn't specify a default {@code Accept} header because
     * it doesn't know what content types the application is interested in.
     * 
     * @return the header
     * @throws IOException
     *             I/O异常
     */
    private synchronized Header prepareRequestHeaders() throws IOException
    {
        // /*
        // * If we're establishing an HTTPS tunnel with CONNECT (RFC 2817 5.2),
        // * send only the minimum set of headers. This avoids sending
        // potentially
        // * sensitive data like HTTP cookies to the proxy unencrypted.
        // */
        // if (method == CONNECT) {
        // Header proxyHeader = new Header();
        // proxyHeader.setStatusLine(getStatusLine());
        //
        // // always set Host and User-Agent
        // String host = requestHeader.get("Host");
        // if (host == null) {
        // host = getOriginAddress(url);
        // }
        // proxyHeader.set("Host", host);
        //
        // String userAgent = requestHeader.get("User-Agent");
        // if (userAgent == null) {
        // userAgent = getDefaultUserAgent();
        // }
        // proxyHeader.set("User-Agent", userAgent);
        //
        // // copy over the Proxy-Authorization header if it exists
        // String proxyAuthorization = requestHeader.get("Proxy-Authorization");
        // if (proxyAuthorization != null) {
        // proxyHeader.set("Proxy-Authorization", proxyAuthorization);
        // }
        //
        // // Always set the Proxy-Connection to Keep-Alive for the benefit of
        // // HTTP/1.0 proxies like Squid.
        // proxyHeader.set("Proxy-Connection", "Keep-Alive");
        // return proxyHeader;
        // }

        requestHeader.setStatusLine(getStatusLine());

        if (requestHeader.get("User-Agent") == null)
        {
            requestHeader.add("User-Agent", getDefaultUserAgent());
        }

        if (requestHeader.get("Host") == null)
        {
            requestHeader.add("Host", getOriginAddress(url));
        }

        if (httpVersion > 0)
        {
            requestHeader.addIfAbsent("Connection", "Keep-Alive");
        }

        if (fixedContentLength != -1)
        {
            requestHeader.addIfAbsent("Content-Length",
                    Integer.toString(fixedContentLength));
        }
        else if (sendChunked)
        {
            requestHeader.addIfAbsent("Transfer-Encoding", "chunked");
        }
        else if (requestBodyOut instanceof RetryableOutputStream)
        {
            int size = ((RetryableOutputStream) requestBodyOut).contentLength();
            // System.out.println("RetryableOutputStream size:" + size);
            requestHeader.addIfAbsent("Content-Length", Integer.toString(size));
        }

        if (requestBodyOut != null)
        {
            requestHeader.addIfAbsent("Content-Type",
                    "application/x-www-form-urlencoded");
        }

        if (requestHeader.get("Accept-Encoding") == null)
        {
            transparentGzip = true;
            requestHeader.set("Accept-Encoding", "gzip");
        }

        CookieHandler cookieHandler = CookieHandler.getDefault();
        if (cookieHandler != null)
        {
            Map<String, List<String>> allCookieHeaders = cookieHandler.get(uri,
                    requestHeader.getFieldMap());
            String key = null;
            for (Map.Entry<String, List<String>> entry : allCookieHeaders
                    .entrySet())
            {
                key = entry.getKey();
                if ("Cookie".equalsIgnoreCase(key)
                        || "Cookie2".equalsIgnoreCase(key))
                {
                    requestHeader.addAll(key, entry.getValue());
                }
            }
        }

        return requestHeader;
    }

    /**
     * Gets the status line.
     * 
     * @return the status line
     */
    private String getStatusLine()
    {
        String protocol = (httpVersion == 0) ? "HTTP/1.0" : "HTTP/1.1";
        return method + " " + requestString() + " " + protocol;
    }

    /**
     * Gets the default user agent.
     * 
     * @return the default user agent
     */
    private String getDefaultUserAgent()
    {
        String agent = getSystemProperty("http.agent");
        return agent != null ? agent
                : ("Java" + getSystemProperty("java.version"));
    }

    /**
     * Gets the system property.
     * 
     * @param property
     *            the property
     * @return the system property
     */
    private String getSystemProperty(final String property)
    {
        return AccessController.doPrivileged(new PriviAction<String>(property));
    }

    /**
     * Gets the origin address.
     * 
     * @param url
     *            the url
     * @return the origin address
     */
    private String getOriginAddress(URL url)
    {
        int port = url.getPort();
        String result = url.getHost();
        if (port > 0)
        {
            result = result + ":" + port;
        }
        return result;
    }

    /**
     * Request string.
     * 
     * @return the string
     */
    protected String requestString()
    {
        // if (usingProxy()) {
        // return url.toString();
        // }
        String file = url.getFile();
        if (file == null || file.length() == 0)
        {
            file = "/";
        }
        return file;
    }

    /**
     * Prepares the HTTP headers and sends them to the server.
     * 
     * <p> For streaming requests with a body, headers must be prepared
     * <strong>before</strong> the output stream has been written to. Otherwise
     * the body would need to be buffered!
     * 
     * <p> For non-streaming requests with a body, headers must be prepared
     * <strong>after</strong> the output stream has been written to and closed.
     * This ensures that the {@code Content-Length} header receives the proper
     * value.
     * 
     * @param out
     *            the out
     * @throws IOException
     *             I/O异常
     */
    private synchronized void writeRequestHeaders(OutputStream out) throws IOException
    {

        prepareRequestHeaders();

        StringBuilder result = new StringBuilder(256);
        result.append(requestHeader.getStatusLine()).append("\r\n");
        int headerLen = requestHeader.length();
        String key = null;
        String value = null;
        for (int i = 0; i < headerLen; i++)
        {
            key = requestHeader.getKey(i);
            value = requestHeader.get(i);
            if (key != null)
            {
                result.append(key).append(": ").append(value).append("\r\n");
            }
        }
        result.append("\r\n");
        out.write(result.toString().getBytes("ISO_8859_1"));
        sentRequestHeaders = true;
    }

    /**
     * 〈一句话功能简述〉 〈功能详细描述〉.
     * 
     * @author [作者]（必须）
     * @version 1.0
     * @see [相关类/方法]（可选）
     * @since [产品/模块版本] （必须）
     */
    enum Retry
    {

        /** The NONE. */
        NONE,
        /** The SAM e_ connection. */
        SAME_CONNECTION,
        /** The NE w_ connection. */
        NEW_CONNECTION
    }

    /**
     * Returns the retry action to take for the current response headers. The
     * headers, proxy and target URL or this connection may be adjusted to
     * prepare for a follow up request.
     * 
     * @return the retry
     * @throws IOException
     *             I/O异常
     */
    private synchronized Retry processResponseHeaders() throws IOException
    {
        switch (responseCode)
        {
        // case HTTP_PROXY_AUTH: // proxy authorization failed ?
        // if (!usingProxy()) {
        // throw new IOException(
        // "Received HTTP_PROXY_AUTH (407) code while not using proxy");
        // }
        // return processAuthHeader("Proxy-Authenticate",
        // "Proxy-Authorization");
        //
        // case HTTP_UNAUTHORIZED: // HTTP authorization failed ?
        // return processAuthHeader("WWW-Authenticate", "Authorization");

        case HTTP_MULT_CHOICE:
        case HTTP_MOVED_PERM:
        case HTTP_MOVED_TEMP:
        case HTTP_SEE_OTHER:
        case HTTP_USE_PROXY:
            if (!getInstanceFollowRedirects())
            {
                return Retry.NONE;
            }
            // if (requestBodyOut != null)
            // {
            // // TODO: follow redirects for retryable output streams...
            // return Retry.NONE;
            // }
            redirectionCount++;
            if (redirectionCount > MAX_REDIRECTS)
            {
                throw new ProtocolException("Too many redirects");
            }
            String location = getHeaderField("Location");
            if (location == null)
            {
                return Retry.NONE;
            }

            // if (responseCode == HTTP_USE_PROXY) {
            // int start = 0;
            // if (location.startsWith(url.getProtocol() + ':')) {
            // start = url.getProtocol().length() + 1;
            // }
            // if (location.startsWith("//", start)) {
            // start += 2;
            // }
            // setProxy(location.substring(start));
            // return Retry.NEW_CONNECTION;
            // }
            URL previousUrl = url;
            url = new URL(previousUrl, location);
            method = "GET";
            if (!previousUrl.getProtocol().equals(url.getProtocol()))
            {
                return Retry.NONE; // the scheme changed; don't retry.
            }
            if (previousUrl.getHost().equals(url.getHost())
                    && (url.getPort() == -1 || previousUrl.getPort() == url
                            .getPort()))
            {
                return Retry.SAME_CONNECTION;
            }
            else
            {
                // TODO: strip cookies?
                requestHeader.removeAll("Host");
                return Retry.NEW_CONNECTION;
            }

        default:
            return Retry.NONE;
        }
    }

    /**
     * Aggressively tries to get the final HTTP response, potentially making
     * many HTTP requests in the process in order to cope with redirects and
     * authentication.
     * 
     * @throws IOException
     *             I/O异常
     */
    protected synchronized final void retrieveResponse() throws IOException
    {
        if (responseHeader != null)
        {
            return;
        }

        redirectionCount = 0;
        Retry retry = Retry.NONE;
        while (true)
        {
            makeConnection();

            // if we can get a response from the cache, we're done
            // if (cacheResponse != null) {
            // // TODO: how does this interact with redirects? Consider
            // processing the headers so
            // // that a redirect is never returned.
            // return;
            // }

            if (!sentRequestHeaders)
            {
                writeRequestHeaders(socketOut);
            }

            if (requestBodyOut != null)
            {
                requestBodyOut.close();
                if (requestBodyOut instanceof RetryableOutputStream)
                {
                    ((RetryableOutputStream) requestBodyOut)
                            .writeToSocket(socketOut);
                }
            }

            socketOut.flush();

            // System.out.println("=============to readResponseHeaders ================");
            readResponseHeaders();

            // if (hasResponseBody())
            // {
            // maybeCache(); // reentrant. this calls into user code which may
            // // call back into this!
            // }

            initContentStream();

            retry = processResponseHeaders();

            // System.out.println("processResponseHeaders result:" + retry);

            if (Retry.NONE.equals(retry))
            {
                return;
            }

            /*
             * The first request wasn't sufficient. Prepare for another...
             */

            // if (requestBodyOut != null
            // && !(requestBodyOut instanceof RetryableOutputStream))
            // {
            // throw new HttpRetryException("Cannot retry streamed HTTP body",
            // responseCode);
            // }

            if (Retry.SAME_CONNECTION.equals(retry)
                    && hasConnectionCloseHeader())
            {
                retry = Retry.NEW_CONNECTION;
            }

            discardIntermediateResponse();

            if (Retry.NEW_CONNECTION.equals(retry))
            {
                releaseSocket(true);
            }
        }
    }

    /**
     * Discard intermediate response.
     * 
     * @throws IOException
     *             I/O异常
     */
    private synchronized void discardIntermediateResponse() throws IOException
    {
        boolean oldIntermediateResponse = intermediateResponse;
        intermediateResponse = true;
        try
        {
            if (responseBodyIn != null)
            {
                // if (!(responseBodyIn instanceof
                // UnknownLengthHttpInputStream)) {
                // // skip the response so that the connection may be reused for
                // the retry
                // Streams.skipAll(responseBodyIn);
                // }
                responseBodyIn.close();
                responseBodyIn = null;
            }
            // new request for redirect
            initRequestHeader();
            sentRequestHeaders = false;
            responseHeader = null;
            responseCode = -1;
            responseMessage = null;
            // cacheRequest = null;

        }
        finally
        {
            intermediateResponse = oldIntermediateResponse;
        }

    }

    /**
     * 是否 connection close header.
     * 
     * @return true, if successful
     */
    private synchronized boolean hasConnectionCloseHeader()
    {
        return (responseHeader != null && "close"
                .equalsIgnoreCase(responseHeader.get("Connection")))
                || (requestHeader != null && "close"
                        .equalsIgnoreCase(requestHeader.get("Connection")));
    }

    /**
     * 初始化 content stream.
     * 
     * @return the input stream
     * @throws IOException
     *             I/O异常
     */
    private synchronized InputStream initContentStream() throws IOException
    {
        InputStream transferStream = getTransferStream();
        if (transparentGzip
                && "gzip".equalsIgnoreCase(responseHeader
                        .get("Content-Encoding")))
        {
            /*
             * If the response was transparently gzipped, remove the gzip header
             * so clients don't double decompress. http://b/3009828
             */
            responseHeader.removeAll("Content-Encoding");
            responseBodyIn = new GZIPInputStream(transferStream);
        }
        else
        {
            responseBodyIn = transferStream;
        }
        return responseBodyIn;
    }

    /**
     * Gets the transfer stream.
     * 
     * @return the transfer stream
     * @throws IOException
     *             I/O异常
     */
    private synchronized InputStream getTransferStream() throws IOException
    {
        if (!hasResponseBody())
        {
            // return new FixedLengthInputStream(socketIn, cacheRequest, this,
            // 0);
            return new FixedLengthInputStream(socketIn, null, this, 0);
        }

        if ("chunked".equalsIgnoreCase(responseHeader.get("Transfer-Encoding")))
        {
            // return new ChunkedInputStream(socketIn, cacheRequest, this);
            return new ChunkedInputStream(socketIn, null, this);
        }

        String contentLength = responseHeader.get("Content-Length");
        if (contentLength != null)
        {
            try
            {
                int length = Integer.parseInt(contentLength);
                return new FixedLengthInputStream(socketIn, null, this, length);
            }
            catch (NumberFormatException ignored)
            {
                Log.e("SDK", ignored.getMessage());
            }
        }

        /*
         * Wrap the input stream from the HttpConnection (rather than just
         * returning "socketIn" directly here), so that we can control its use
         * after the reference escapes.
         */
        return new UnknownLengthHttpInputStream(socketIn, null, this);
    }

    /**
     * Returns true if the response must have a (possibly 0-length) body. See
     * RFC 2616 section 4.3.
     * 
     * @return true, if successful
     */
    private synchronized boolean hasResponseBody()
    {
        if (!HEAD.equals(method) && !CONNECT.equals(method)
                && (responseCode < HTTP_CONTINUE || responseCode >= 200)
                && responseCode != HTTP_NO_CONTENT
                && responseCode != HTTP_NOT_MODIFIED)
        {
            return true;
        }
        /*
         * If the Content-Length or Transfer-Encoding headers disagree with the
         * response code, the response is malformed. For best compatibility, we
         * honor the headers.
         */
        String contentLength = responseHeader.get("Content-Length");
        if (contentLength != null && Integer.parseInt(contentLength) > 0)
        {
            return true;
        }
        if ("chunked".equalsIgnoreCase(responseHeader.get("Transfer-Encoding")))
        {
            return true;
        }

        return false;
    }

    /**
     * Read response headers.
     * 
     * @throws IOException
     *             I/O异常
     */
    private synchronized void readResponseHeaders() throws IOException
    {
        do
        {
            responseCode = -1;
            responseMessage = null;
            responseHeader = new Header();
            responseHeader.setStatusLine(readLine(socketIn).trim());
            // System.out.println("getStatusLine():" +
            // responseHeader.getStatusLine());
            readHeaders();
        }
        while (parseResponseCode() == HTTP_CONTINUE);
    }

    /**
     * 解析 response code.
     * 
     * @return the int
     */
    private synchronized int parseResponseCode()
    {
        // Response Code Sample : "HTTP/1.0 200 OK"
        String response = responseHeader.getStatusLine();
        if (response == null || !response.startsWith("HTTP/"))
        {
            return -1;
        }
        response = response.trim();
        int mark = response.indexOf(" ") + 1;
        if (mark == 0)
        {
            return -1;
        }
        if (response.charAt(mark - 2) != '1')
        {
            httpVersion = 0;
        }
        int last = mark + 3;
        if (last > response.length())
        {
            last = response.length();
        }
        responseCode = Integer.parseInt(response.substring(mark, last));
        if (last + 1 <= response.length())
        {
            responseMessage = response.substring(last + 1);
        }
        return responseCode;
    }

    /**
     * Returns the characters up to but not including the next "\r\n", "\n", or
     * the end of the stream, consuming the end of line delimiter.
     * 
     * @param is
     *            the is
     * @return the string
     * @throws IOException
     *             I/O异常
     */
    static String readLine(InputStream is) throws IOException
    {
        StringBuilder result = new StringBuilder(80);

        int c;
        while (true)
        {
            c = is.read();
            if (c == -1 || c == '\n')
            {
                break;
            }

            result.append((char) c);
        }
        int length = result.length();
        if (length > 0 && result.charAt(length - 1) == '\r')
        {
            result.setLength(length - 1);
        }
        return result.toString();
    }

    /**
     * Read headers.
     * 
     * @throws IOException
     *             I/O异常
     */
    synchronized void readHeaders() throws IOException
    {
        // parse the result headers until the first blank line
        String line = readLine(socketIn);
        int index = 0;
        while (line.length() > 1)
        {
            // System.out.println("header:" + line);
            // Header parsing
            index = line.indexOf(":");
            if (index == -1)
            {
                responseHeader.add("", line.trim());
            }
            else
            {
                responseHeader.add(line.substring(0, index),
                        line.substring(index + 1).trim());
            }
            line = readLine(socketIn);
        }

        //
        // BufferedReader reader = new BufferedReader(new InputStreamReader(
        // socketIn));
        // System.out.println("=============after readHeaders================");
        //
        // String lines;
        // while ((lines = reader.readLine()) != null)
        // {
        // System.out.println(lines);
        // }
        // reader.close();

        CookieHandler cookieHandler = CookieHandler.getDefault();
        if (cookieHandler != null)
        {
            cookieHandler.put(uri, responseHeader.getFieldMap());
        }
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.URLConnection#getOutputStream()
     */
    @Override
    public synchronized OutputStream getOutputStream() throws IOException
    {
        if (!doOutput)
        {
            throw new ProtocolException("Does not support output");
        }

        if (requestBodyOut != null)
        {
            return requestBodyOut;
        }

        // you can't write after you read
        if (sentRequestHeaders)
        {
            // TODO: just return 'requestBodyOut' if that's non-null?
            throw new ProtocolException(
                    "OutputStream unavailable because request headers have already been sent!");
        }

        // they are requesting a stream to write to. This implies a POST method
        if (GET.equals(method))
        {
            method = POST;
        }

        // If the request method is neither PUT or POST, then you're not writing
        if (!PUT.equals(method) && !POST.equals(method))
        {
            throw new ProtocolException(method + " does not support writing");
        }

        int contentLength = -1;

        String contentLengthString = requestHeader.get("Content-Length");
        if (contentLengthString != null)
        {
            contentLength = Integer.parseInt(contentLengthString);
        }

        String encoding = requestHeader.get("Transfer-Encoding");
        if (chunkLength > 0 || "chunked".equalsIgnoreCase(encoding))
        {
            sendChunked = true;
            contentLength = -1;
            if (chunkLength == -1)
            {
                chunkLength = DEFAULT_CHUNK_LENGTH;
            }
        }

        connect();

        if (socketOut == null)
        {
            // TODO: what should we do if a cached response exists?
            throw new IOException("No socket to write to; was a POST cached?");
        }

        if (httpVersion == 0)
        {
            sendChunked = false;
        }

        if (fixedContentLength != -1)
        {
            requestBodyOut = new FixedLengthOutputStream(socketOut,
                    fixedContentLength);
            writeRequestHeaders(socketOut);
        }
        else if (sendChunked)
        {
            requestBodyOut = new ChunkedOutputStream(socketOut, chunkLength);
            writeRequestHeaders(socketOut);
        }
        else if (contentLength != -1)
        {
            requestBodyOut = new RetryableOutputStream(contentLength);
        }
        else
        {
            requestBodyOut = new RetryableOutputStream();
        }

        return requestBodyOut;
    }

    /**
     * Returns the value of the field at position <code>pos<code>. Returns
     * <code>null</code> if there is fewer than <code>pos</code> fields in the
     * response header.
     * 
     * @param pos
     *            int the position of the field from the top
     * @return java.lang.String The value of the field
     * @see #getHeaderField(String)
     * @see #getHeaderFieldKey
     */
    @Override
    public synchronized String getHeaderField(int pos)
    {
        try
        {
            getInputStream();
        }
        catch (IOException e)
        {
            Log.e("SDK", e.getMessage());
            // ignore
        }
        if (null == responseHeader)
        {
            return null;
        }
        return responseHeader.get(pos);
    }

    /**
     * Returns the value of the field corresponding to the <code>key</code>
     * Returns <code>null</code> if there is no such field.
     * 
     * If there are multiple fields with that key, the last field value is
     * returned.
     * 
     * @param key
     *            java.lang.String the name of the header field
     * @return java.lang.String The value of the header field
     * @see #getHeaderField(int)
     * @see #getHeaderFieldKey
     */
    @Override
    public synchronized String getHeaderField(String key)
    {
        try
        {
            getInputStream();
        }
        catch (IOException e)
        {
            Log.e("SDK", e.getMessage());
            // ignore
        }
        if (null == responseHeader)
        {
            return null;
        }
        return responseHeader.get(key);
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.URLConnection#getHeaderFieldKey(int)
     */
    @Override
    public synchronized String getHeaderFieldKey(int pos)
    {
        try
        {
            getInputStream();
        }
        catch (IOException e)
        {
            Log.e("SDK", e.getMessage());
            // ignore
        }
        if (null == responseHeader)
        {
            return null;
        }
        return responseHeader.getKey(pos);
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.URLConnection#getHeaderFields()
     */
    @Override
    public synchronized Map<String, List<String>> getHeaderFields()
    {
        try
        {
            retrieveResponse();
        }
        catch (IOException ignored)
        {
            Log.e("SDK", ignored.getMessage());
        }
        return responseHeader != null ? responseHeader.getFieldMap() : null;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.URLConnection#getRequestProperties()
     */
    @Override
    public synchronized Map<String, List<String>> getRequestProperties()
    {
        if (connected)
        {
            throw new IllegalStateException(
                    "Cannot access request header fields after connection is set");
        }
        return requestHeader.getFieldMap();
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.URLConnection#setRequestProperty(java.lang.String,
     * java.lang.String)
     */
    @Override
    public synchronized void setRequestProperty(String field, String newValue)
    {
        if (connected)
        {
            throw new IllegalStateException(
                    "Cannot set request property after connection is made");
        }
        // if (field == null)
        // {
        // throw new NullPointerException();
        // }
        requestHeader.set(field, newValue);
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.net.URLConnection#addRequestProperty(java.lang.String,
     * java.lang.String)
     */
    @Override
    public synchronized void addRequestProperty(String field, String value)
    {
        if (connected)
        {
            throw new IllegalStateException(
                    "Cannot set request property after connection is made");
        }
        // if (field == null)
        // {
        // throw new NullPointerException();
        // }
        requestHeader.add(field, value);
    }

    /**
     * Releases this connection so that it may be either reused or closed.
     * 
     * @param reuseSocket
     *            the reuse socket
     */
    protected synchronized void releaseSocket(boolean reuseSocket)
    {
        // // we cannot recycle sockets that have incomplete output.
        if (requestBodyOut != null && !requestBodyOut.closed)
        {
            reuseSocket = false;
        }

        // if the headers specify that the connection shouldn't be reused, don't
        // reuse it
        if (hasConnectionCloseHeader())
        {
            reuseSocket = false;
        }

        /*
         * Don't return the socket to the connection pool if this is an
         * intermediate response; we're going to use it again right away.
         */
        if (intermediateResponse && reuseSocket)
        {
            return;
        }

        if (connection != null)
        {
            // if (reuseSocket) {
            // HttpConnectionPool.INSTANCE.recycle(connection);
            // } else {
            // connection.closeSocketAndStreams();
            // }
            try
            {
                connection.close();
            }
            catch (IOException e)
            {
                Log.e("SDK", e.getMessage());
                // e.printStackTrace();
            }
            finally
            {
                connection = null;
                connected = false;
            }

        }

        /*
         * Clear "socketIn" and "socketOut" to ensure that no further I/O
         * attempts from this instance make their way to the underlying
         * connection (which may get recycled).
         */
        socketIn = null;
        socketOut = null;
    }
}
