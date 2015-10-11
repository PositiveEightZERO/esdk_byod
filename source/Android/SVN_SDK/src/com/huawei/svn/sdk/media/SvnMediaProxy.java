// Copyright 2010 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package com.huawei.svn.sdk.media;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.net.URLDecoder;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

import org.apache.http.Header;
import org.apache.http.HttpResponse;
import org.apache.http.HttpResponseFactory;
import org.apache.http.ParseException;
import org.apache.http.ProtocolVersion;
import org.apache.http.StatusLine;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.conn.ClientConnectionOperator;
import org.apache.http.conn.OperatedClientConnection;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.impl.conn.DefaultClientConnection;
import org.apache.http.impl.conn.DefaultClientConnectionOperator;
import org.apache.http.impl.conn.DefaultResponseParser;
import org.apache.http.impl.conn.SingleClientConnManager;
import org.apache.http.io.HttpMessageParser;
import org.apache.http.io.SessionInputBuffer;
import org.apache.http.message.BasicHttpResponse;
import org.apache.http.message.BasicLineParser;
import org.apache.http.message.ParserCursor;
import org.apache.http.params.HttpParams;
import org.apache.http.protocol.HTTP;
import org.apache.http.util.CharArrayBuffer;

import com.huawei.svn.sdk.fsm.SvnFileInputStream;
import com.huawei.svn.sdk.fsm.SvnFileTool;
import com.huawei.svn.sdk.thirdpart.SvnHttpClient;

import android.util.Log;

public class SvnMediaProxy implements Runnable {
	private static final String TAG = "SvnMediaProxy";

	private static final int BUFFER_SIZE = 1024 * 1024;

	private int port = 0;

	public int getPort() {
		return port;
	}

	private boolean isRunning = true;
	private ServerSocket socket;
	private Thread thread;

	private SvnMediaProxy() {
		init();

	}

	private static SvnMediaProxy instance = null;

	public static SvnMediaProxy getInstance() {
		if (instance == null) {
			instance = new SvnMediaProxy();
		}

		return instance;
	}

	public void init() {
		try {
			socket = new ServerSocket(port, 0, InetAddress.getByAddress(new byte[] { 127, 0, 0, 1 }));
			socket.setSoTimeout(5000);
			port = socket.getLocalPort();
			Log.d(TAG, "port " + port + " obtained");
		} catch (UnknownHostException e) {
			Log.e(TAG, "Error initializing server", e);
		} catch (IOException e) {
			Log.e(TAG, "Error initializing server", e);
		}

		start();
	}

	private void start() {

		if (socket == null) {
			throw new IllegalStateException("Cannot start proxy; it has not been initialized.");
		}

		thread = new Thread(this);
		thread.start();
	}

	public void stop() {
		isRunning = false;

		if (thread == null) {
			throw new IllegalStateException("Cannot stop proxy; it has not been started.");
		}

		thread.interrupt();
		try {
			thread.join(5000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void run() {
		Log.d(TAG, "running");
		while (isRunning) {
			try {
				final Socket client = socket.accept();
				if (client == null) {
					continue;
				}
				Log.d(TAG, "client connected");

				Thread t = new Thread(new Runnable() {

					@Override
					public void run() {
						// TODO Auto-generated method stub

						try {
							List<String> headers = readRequest(client);
							if (headers != null && headers.size() >= 1) {
								String requestURL = getRequestURL(headers.get(0));
								Log.d(TAG, requestURL);

								if (requestURL.startsWith("http://")) {

									// HttpRequest request = new
									// BasicHttpRequest("GET", requestURL);

									processHttpRequest(requestURL, client, headers);
								} else {
									processFileRequest(requestURL, client, headers);
								}
							}

						} catch (IllegalStateException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						} catch (IOException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}

				});

				t.start();

			} catch (SocketTimeoutException e) {
				// Do nothing
			} catch (IOException e) {
				Log.e(TAG, "Error connecting to client", e);
			}
		}
		Log.d(TAG, "Proxy interrupted. Shutting down.");
	}

	private String getRequestURL(String firstLine) throws UnsupportedEncodingException {
		if (firstLine == null) {
			Log.i(TAG, "Proxy client closed connection without a request.");
			return null;
		}
		Log.i(TAG, "firstLine: " + firstLine);

		StringTokenizer st = new StringTokenizer(firstLine);
		String method = st.nextToken();
		String uri = st.nextToken();
		Log.d(TAG, uri);
		String realUri =  uri.substring(1);
		
		if(realUri != null)
		{
			realUri = URLDecoder.decode(realUri, "UTF-8");
		}
		
		Log.i(TAG, "realUri: " + realUri);

		// realUri = "http://172.22.8.206:8180/" + realUri;
		return realUri;
	}

	private List<String> readRequest(Socket client) {
		InputStream is = null;
		String line = null;

		List<String> headers = new ArrayList<String>();
		try {
			is = client.getInputStream();
			BufferedReader reader = new BufferedReader(new InputStreamReader(is), 8192);

			while ((line = reader.readLine()) != null) {
				if (line.equals("")) { // last line of request message
										// header is a
										// blank line (\r\n\r\n)
					break; // quit while loop when last line of header is
							// reached
				}

				headers.add(line);
			}

		} catch (IOException e) {
			Log.e(TAG, "Error parsing request", e);
			return null;
		}

		return headers;
	}

	private HttpResponse download(String url, List<String> headers) {
		// DefaultHttpClient seed = new DefaultHttpClient();
		// SchemeRegistry registry = new SchemeRegistry();
		// registry.register(
		// new Scheme("http", PlainSocketFactory.getSocketFactory(), 80));
		// SingleClientConnManager mgr = new
		// MyClientConnManager(seed.getParams(),
		// registry);
		// DefaultHttpClient http = new DefaultHttpClient(mgr,
		// seed.getParams());
		SvnHttpClient http = new SvnHttpClient();
		HttpGet getMethod = new HttpGet(url);
		if (headers != null && headers.size() > 1) {
			for (int i = 1; i < headers.size(); i++) {
				String[] parts = headers.get(i).split(":");
				if (parts != null && parts.length == 2) {
					getMethod.addHeader(parts[0], parts[1]);
				}

			}
		}

		HttpResponse response = null;
		try {
			Log.d(TAG, "starting download");
			response = http.execute(getMethod);
			Log.d(TAG, "downloaded");
		} catch (ClientProtocolException e) {
			Log.e(TAG, "Error downloading", e);
		} catch (IOException e) {
			Log.e(TAG, "Error downloading", e);
		}
		return response;
	}

	private void processFileRequest(String requestURL, Socket client, List<String> headers) throws IOException {
		// TODO Auto-generated method stub
		if (requestURL == null) {
			return;
		}
		Log.d(TAG, "processing file request:" + requestURL + "headers:" + headers);
		int startRange = -1;
		int endRange = -1;

		if (headers != null && headers.size() > 1) {
			// String header = headers.get(i);
			// if(header.contains("C"))
			for (int i = 1; i < headers.size(); i++) {
				String[] parts = headers.get(i).split(":");
				if (parts != null && parts.length == 2) {
					if ("Range".equals(parts[0].trim())) {
						// bytes=0-1024
						int index = parts[1].indexOf('=');
						if (index > 0 && index < parts[1].length() - 1) {
							String range = parts[1].substring(index + 1).trim();
							parts = range.split("-");
							if (parts != null && parts.length >= 1) {
								if (parts.length == 1) {
									if (range.startsWith("-")) {
										endRange = Integer.parseInt(parts[0]);
										startRange = 0;
									} else {
										startRange = Integer.parseInt(parts[0]);
									}
								} else if (parts.length == 2) {
									startRange = Integer.parseInt(parts[0]);

									endRange = Integer.parseInt(parts[1]);

								}

							}
						}
					}
				}

			}
		}

		InputStream data = new SvnFileInputStream(requestURL);
		int fileLength = SvnFileTool.getFileLength(requestURL);

		int requestLength = fileLength;

		Log.d(TAG, "reading headers");
		StringBuilder httpString = new StringBuilder();

		if (startRange >= 0) {
			if (endRange >= fileLength || endRange < startRange) {
				endRange = fileLength - 1;
			}
			data.skip(startRange);

			requestLength = endRange - startRange + 1;
			httpString.append("HTTP/1.1 206 Partial Content\n");

			httpString.append("Content-Range").append(": bytes ").append("" + startRange).append("-").append("" + endRange)
					.append("/" + fileLength).append("\n");
		} else {
			httpString.append("HTTP/1.1 200 OK\n");
		}

		// for (Header h : response.getAllHeaders()) {
		// if(!"Transfer-Encoding".equalsIgnoreCase(h.getName()))
		// {
		// httpString.append(h.getName()).append(": ").append(h.getValue())
		// .append("\n");
		// }
		//
		// }

		httpString.append("Content-Length").append(": ").append("" + requestLength).append("\n");

		httpString.append("\n");
		Log.e(TAG, "headers done:" + httpString.length() + "\n" + httpString.toString());
		int totalReadBytes = 0;
		
		
		try {
			byte[] buffer = httpString.toString().getBytes();
			int readBytes;
			Log.d(TAG, "writing to client");
			client.getOutputStream().write(buffer, 0, buffer.length);

			// Start streaming content.
			byte[] buff = new byte[BUFFER_SIZE];

			while (isRunning && totalReadBytes < requestLength) {
				
				
				int toRead = Math.min(requestLength - totalReadBytes, BUFFER_SIZE);
				Log.e(TAG, "toRead:" + toRead);
				

				readBytes = data.read(buff, 0, toRead);
				
				if(readBytes < 0)
				{
					break;
				}

				
				// System.out.println("totalReadBytes:" + totalReadBytes);
				if(readBytes > 0)
				{
					totalReadBytes += readBytes;
					client.getOutputStream().write(buff, 0, readBytes);
				}
				
			}

		} catch (Exception e) {
			Log.e("", e.getMessage(), e);
		} finally {

			Log.e(TAG, "totalReadBytes:" + totalReadBytes);
			if (data != null) {
				data.close();
			}
			client.close();

		}
	}

	private void processHttpRequest(String url, Socket client, List<String> headers)
			throws IllegalStateException, IOException {
		if (url == null) {
			return;
		}
		Log.d(TAG, "processing");
		HttpResponse realResponse = download(url, headers);
		if (realResponse == null) {
			return;
		}

		Log.d(TAG, "downloading...");

		InputStream data = realResponse.getEntity().getContent();
		StatusLine line = realResponse.getStatusLine();
		HttpResponse response = new BasicHttpResponse(line);
		response.setHeaders(realResponse.getAllHeaders());

		Log.d(TAG, "reading headers");
		StringBuilder httpString = new StringBuilder();
		httpString.append(response.getStatusLine().toString());

		httpString.append("\n");
		for (Header h : response.getAllHeaders()) {
			if (!"Transfer-Encoding".equalsIgnoreCase(h.getName())) {
				httpString.append(h.getName()).append(": ").append(h.getValue()).append("\n");
			}

		}
		httpString.append("\n");
		Log.d(TAG, "headers done:" + httpString.length() + "\n" + httpString.toString());
		long totalReadBytes = 0;
		try {
			byte[] buffer = httpString.toString().getBytes();
			int readBytes;
			Log.d(TAG, "writing to client");
			client.getOutputStream().write(buffer, 0, buffer.length);

			// Start streaming content.
			byte[] buff = new byte[1024 * 50];
			while (isRunning && (readBytes = data.read(buff, 0, buff.length)) != -1) {

				totalReadBytes += readBytes;
				// System.out.println("totalReadBytes:" + totalReadBytes);
				client.getOutputStream().write(buff, 0, readBytes);
			}

		} catch (Exception e) {
			Log.e("", e.getMessage(), e);
		} finally {

			System.out.println("totalReadBytes:" + totalReadBytes);
			if (data != null) {
				data.close();
			}
			client.close();

		}
	}

	private class IcyLineParser extends BasicLineParser {
		private static final String ICY_PROTOCOL_NAME = "ICY";

		private IcyLineParser() {
			super();
		}

		@Override
		public boolean hasProtocolVersion(CharArrayBuffer buffer, ParserCursor cursor) {
			boolean superFound = super.hasProtocolVersion(buffer, cursor);
			if (superFound) {
				return true;
			}
			int index = cursor.getPos();

			final int protolength = ICY_PROTOCOL_NAME.length();

			if (buffer.length() < protolength)
				return false; // not long enough for "HTTP/1.1"

			if (index < 0) {
				// end of line, no tolerance for trailing whitespace
				// this works only for single-digit major and minor version
				index = buffer.length() - protolength;
			} else if (index == 0) {
				// beginning of line, tolerate leading whitespace
				while ((index < buffer.length()) && HTTP.isWhitespace(buffer.charAt(index))) {
					index++;
				}
			} // else within line, don't tolerate whitespace

			return index + protolength <= buffer.length()
					&& buffer.substring(index, index + protolength).equals(ICY_PROTOCOL_NAME);

		}

		@Override
		public ProtocolVersion parseProtocolVersion(CharArrayBuffer buffer, ParserCursor cursor) throws ParseException {

			if (buffer == null) {
				throw new IllegalArgumentException("Char array buffer may not be null");
			}
			if (cursor == null) {
				throw new IllegalArgumentException("Parser cursor may not be null");
			}

			final int protolength = ICY_PROTOCOL_NAME.length();

			int indexFrom = cursor.getPos();
			int indexTo = cursor.getUpperBound();

			skipWhitespace(buffer, cursor);

			int i = cursor.getPos();

			// long enough for "HTTP/1.1"?
			if (i + protolength + 4 > indexTo) {
				throw new ParseException("Not a valid protocol version: " + buffer.substring(indexFrom, indexTo));
			}

			// check the protocol name and slash
			if (!buffer.substring(i, i + protolength).equals(ICY_PROTOCOL_NAME)) {
				return super.parseProtocolVersion(buffer, cursor);
			}

			cursor.updatePos(i + protolength);

			return createProtocolVersion(1, 0);
		}

		@Override
		public StatusLine parseStatusLine(CharArrayBuffer buffer, ParserCursor cursor) throws ParseException {
			return super.parseStatusLine(buffer, cursor);
		}
	}

	class MyClientConnection extends DefaultClientConnection {
		@Override
		protected HttpMessageParser createResponseParser(final SessionInputBuffer buffer,
				final HttpResponseFactory responseFactory, final HttpParams params) {
			return new DefaultResponseParser(buffer, new IcyLineParser(), responseFactory, params);
		}
	}

	class MyClientConnectionOperator extends DefaultClientConnectionOperator {
		public MyClientConnectionOperator(final SchemeRegistry sr) {
			super(sr);
		}

		@Override
		public OperatedClientConnection createConnection() {
			return new MyClientConnection();
		}
	}

	class MyClientConnManager extends SingleClientConnManager {
		private MyClientConnManager(HttpParams params, SchemeRegistry schreg) {
			super(params, schreg);
		}

		@Override
		protected ClientConnectionOperator createConnectionOperator(final SchemeRegistry sr) {
			return new MyClientConnectionOperator(sr);
		}
	}

}
