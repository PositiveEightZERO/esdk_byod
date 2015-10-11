package com.huawei.esdk.anyoffice.cordova;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Iterator;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import com.huawei.esdk.anyoffice.cordova.util.FileProgressResult;
import com.huawei.esdk.anyoffice.cordova.util.FileUtil;
import com.huawei.svn.sdk.fsm.SvnFile;
import com.huawei.svn.sdk.fsm.SvnFileInputStream;
import com.huawei.svn.sdk.fsm.SvnFileOutputStream;
import com.huawei.svn.sdk.fsm.SvnFileTool;
import com.huawei.svn.sdk.thirdpart.SvnHttpClient;

public class FileEncryptionCordova extends CordovaPlugin {
	
	private static final int MAX_BUFFER_SIZE = 16 * 1024;
	
	private static final String LOG_TAG = "FileEncryptionCordova";
	
	public static int FILE_NOT_FOUND_ERR = 1;
    public static int INVALID_URL_ERR = 2;
    public static int CONNECTION_ERR = 3;
    public static int ABORTED_ERR = 4;
    public static int NOT_MODIFIED_ERR = 5;
	
	public FileEncryptionCordova() {
        System.loadLibrary("svnapi");
        System.loadLibrary("anyofficesdk");
        System.loadLibrary("jniapi");
	}
	
	private static HashMap<String, RequestContext> activeRequests = new HashMap<String, RequestContext>();
	
	private static final class RequestContext {
        String source;
        String target;
        SvnFile targetFile;
        CallbackContext callbackContext;
        HttpGet connection;
        boolean aborted;
        RequestContext(String source, String target, CallbackContext callbackContext) {
            this.source = source;
            this.target = target;
            this.callbackContext = callbackContext;
        }
        void sendPluginResult(PluginResult pluginResult) {
            synchronized (this) {
                if (!aborted) {
                    callbackContext.sendPluginResult(pluginResult);
                }
            }
        }
    }
	
	/**
	 * Adds an interface method to an InputStream to return the number of bytes
	 * read from the raw stream. This is used to track total progress against
	 * the HTTP Content-Length header value from the server.
	 */
	private static abstract class TrackingInputStream extends FilterInputStream {
	  public TrackingInputStream(final InputStream in) {
	    super(in);
	  }
	    public abstract long getTotalRawBytesRead();
	}
	
	private static class SimpleTrackingInputStream extends TrackingInputStream {
        private long bytesRead = 0;
        public SimpleTrackingInputStream(InputStream stream) {
            super(stream);
        }

        private int updateBytesRead(int newBytesRead) {
          if (newBytesRead != -1) {
            bytesRead += newBytesRead;
          }
          return newBytesRead;
        }

        @Override
        public int read() throws IOException {
            return updateBytesRead(super.read());
        }

        // Note: FilterInputStream delegates read(byte[] bytes) to the below method,
        // so we don't override it or else double count (CB-5631).
        @Override
        public int read(byte[] bytes, int offset, int count) throws IOException {
            return updateBytesRead(super.read(bytes, offset, count));
        }

        public long getTotalRawBytesRead() {
          return bytesRead;
        }
    }

	@Override
	public boolean execute(String action, JSONArray param, CallbackContext callbackContext) throws JSONException {
		if ("fileEncrypt".equals(action)) {
			fileEncrypt(param, callbackContext);
		} else if ("fileDecrypt".equals(action)) {
			fileDecrypt(param, callbackContext);
		} else if ("fileEncryptDownload".equals(action)) {
			download(param, callbackContext);
		} else if ("abortDownload".equals(action)) {
			abortDownload(param, callbackContext);
		}
		return true;
	}
	
	private void abortDownload(final JSONArray param, final CallbackContext callbackContext) throws JSONException {
		final RequestContext context;
		String objectId = param.getString(0);
        synchronized (activeRequests) {
            context = activeRequests.remove(objectId);
        }
        if (context != null) {
            // Closing the streams can block, so execute on a background thread.
            cordova.getThreadPool().execute(new Runnable() {
                public void run() {
                    synchronized (context) {
                        SvnFile file = context.targetFile;
                        if (file != null) {
                            file.delete();
                        }
                        // Trigger the abort callback immediately to minimize latency between it and abort() being called.
                        JSONObject error = createFileTransferError(ABORTED_ERR, context.source, context.target, null, -1, null);
                        context.sendPluginResult(new PluginResult(PluginResult.Status.ERROR, error));
                        context.aborted = true;
                        if (context.connection != null) {
                            context.connection.abort();
                        }
                    }
                }
            });
        }
	}
	
	private void download(final JSONArray param, final CallbackContext callbackContext) throws JSONException {
		
		final String source = param.getString(0);
		final String target = param.getString(1);
		final String objectId = param.getString(2);
        final JSONObject headers = param.optJSONObject(3);
        
        final RequestContext context = new RequestContext(source, target, callbackContext);
        synchronized (activeRequests) {
            activeRequests.put(objectId, context);
        }
		
		cordova.getThreadPool().execute(new Runnable() {
			@Override
			public void run() {
				if (context.aborted) {
                    return;
                }
				FileProgressResult progress = new FileProgressResult();
				TrackingInputStream is = null;
				SvnFileOutputStream os = null;
				JSONObject fileObj = new JSONObject();
				PluginResult result = null;
				HttpResponse response = null;
				SvnFile file = null;
				try {
					File targetFolder = new File(target.substring(0, target.lastIndexOf("/")));
					if (!targetFolder.exists()) {
						targetFolder.mkdirs();
					}
					
					file = new SvnFile(target);
					context.targetFile = file;
					
					HttpClient client = new SvnHttpClient();
					HttpGet get = new HttpGet(source);
					get.setHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.79 Safari/537.1");  
					get.setHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");  
					
					// Handle the other headers
                    if (headers != null) {
                        addHeadersToRequest(get, headers);
                    }
					
				    response = client.execute(get);
				    
				    int status = response.getStatusLine().getStatusCode();
				    if (status == 200) {
				    	HttpEntity entity = response.getEntity();
//						long len = entity.getContentLength();
						
						if (entity.getContentLength() != -1) {
			                progress.setLengthComputable(true);
			                progress.setTotal(entity.getContentLength());
			            }
						
						is = new SimpleTrackingInputStream(entity.getContent());
						
						try {
							synchronized (context) {
	                            if (context.aborted) {
	                                return;
	                            }
	                            context.connection = get;
	                        }
							
							// write bytes to file
				            byte[] buffer = new byte[MAX_BUFFER_SIZE];
				            int bytesRead = 0;
				            os = new SvnFileOutputStream(target);
				            while ((bytesRead = is.read(buffer)) > 0) {
				                os.write(buffer, 0, bytesRead);
				                // Send a progress event.
				                progress.setLoaded(is.getTotalRawBytesRead());
				                PluginResult progressResult = new PluginResult(PluginResult.Status.OK, progress.toJSONObject());
				                progressResult.setKeepCallback(true);
				                context.sendPluginResult(progressResult);
				            }
							
				            Log.i("FileEncryptDownload", "download success!");
						}  finally {
							synchronized (context) {
	                            context.connection = null;
	                        }
							safeClose(is);
							safeClose(os);
						}
						
						file = new SvnFile(target);
						if (file != null && file.exists()) {
							fileObj.put("name", file.getName());
							fileObj.put("fullPath", file.getPath());
					        fileObj.put("isEnryptedFile",SvnFileTool.isEncFile(file.getPath()));
					        
					        result = new PluginResult(PluginResult.Status.OK, fileObj);
						} else {
							JSONObject error = createFileTransferError(CONNECTION_ERR, source, target, response, null);
	                        Log.e(LOG_TAG, "File plugin cannot represent download path");
	                        result = new PluginResult(PluginResult.Status.IO_EXCEPTION, error);
						}
				    } else {
				    	throw new FileNotFoundException();
				    }
				} catch (FileNotFoundException e) {
					JSONObject error = createFileTransferError(FILE_NOT_FOUND_ERR, source, target, null, e);
                    Log.e(LOG_TAG, error.toString(), e);
                    result = new PluginResult(PluginResult.Status.IO_EXCEPTION, error);
				} catch (IOException e) {
					JSONObject error = createFileTransferError(CONNECTION_ERR, source, target, null, e);
                    Log.e(LOG_TAG, error.toString(), e);
                    result = new PluginResult(PluginResult.Status.IO_EXCEPTION, error);
				} catch (JSONException e) {
					Log.e(LOG_TAG, e.getMessage(), e);
                    context.sendPluginResult(new PluginResult(PluginResult.Status.JSON_EXCEPTION));
				} finally {
                    synchronized (activeRequests) {
                        activeRequests.remove(objectId);
                    }

                    if (result == null) {
                        result = new PluginResult(PluginResult.Status.ERROR, createFileTransferError(CONNECTION_ERR, source, target, response, null));
                    }
                    // Remove incomplete download.
                    if (result.getStatus() != PluginResult.Status.OK.ordinal() && file != null) {
                        file.delete();
                    }
                    context.sendPluginResult(result);
                }
			}
		});
	}
	
	private static JSONObject createFileTransferError(int errorCode, String source, String target,HttpResponse response, Throwable throwable) {

        int httpStatus = 0;
        StringBuilder bodyBuilder = new StringBuilder();
        String body = null;
        
        if (response != null) {
            try {
                    httpStatus = response.getStatusLine().getStatusCode();
                    InputStream err = response.getEntity().getContent();
                    if(err != null)
                    {
                        BufferedReader reader = new BufferedReader(new InputStreamReader(err, "UTF-8"));
                        try {
                            String line = reader.readLine();
                            while(line != null) {
                                bodyBuilder.append(line);
                                line = reader.readLine();
                                if(line != null) {
                                    bodyBuilder.append('\n');
                                }
                            }
                            body = bodyBuilder.toString();
                        } finally {
                            reader.close();
                        }
                    }
            // IOException can leave connection object in a bad state, so catch all exceptions.
            } catch (Throwable e) {
                Log.w(LOG_TAG, "Error getting HTTP status code from connection.", e);
            }
        }

        return createFileTransferError(errorCode, source, target, body, httpStatus, throwable);
    }

        /**
        * Create an error object based on the passed in errorCode
        * @param errorCode      the error
        * @return JSONObject containing the error
        */
    private static JSONObject createFileTransferError(int errorCode, String source, String target, String body, Integer httpStatus, Throwable throwable) {
        JSONObject error = null;
        try {
            error = new JSONObject();
            error.put("code", errorCode);
            error.put("source", source);
            error.put("target", target);
            if(body != null)
            {
                error.put("body", body);
            }   
            if (httpStatus != null) {
                error.put("http_status", httpStatus);
            }
            if (throwable != null) {
                String msg = throwable.getMessage();
                if (msg == null || "".equals(msg)) {
                    msg = throwable.toString();
                }
                error.put("exception", msg);
            }
        } catch (JSONException e) {
            Log.e(LOG_TAG, e.getMessage(), e);
        }
        return error;
    }
	
    private static void addHeadersToRequest(HttpGet get, JSONObject headers) {
        try {
            for (Iterator<?> iter = headers.keys(); iter.hasNext(); ) {
                String headerKey = iter.next().toString();
                JSONArray headerValues = headers.optJSONArray(headerKey);
                if (headerValues == null) {
                    headerValues = new JSONArray();
                    headerValues.put(headers.getString(headerKey));
                }
                get.setHeader(headerKey, headerValues.getString(0));
                for (int i = 1; i < headerValues.length(); ++i) {
                	get.setHeader(headerKey, headerValues.getString(i));
                }
            }
        } catch (JSONException e1) {
          // No headers to be manipulated!
        }
    }
	
	private static void safeClose(Closeable stream) {
        if (stream != null) {
            try {
                stream.close();
            } catch (IOException e) {
            }
        }
    }


	private void fileEncrypt(JSONArray param, CallbackContext callbackContext) {
		try {
			SvnFileOutputStream fileOutStream = null;
	        FileInputStream fileInStream = null;
	        
			String srcFileName = param.getString(0);
			String dstFileName = param.getString(1);
			
			String dstDir = dstFileName.substring(0, dstFileName.lastIndexOf("/"));
			File dir = new File(dstDir);
			boolean flag = true;
			if (!dir.exists()) {
				flag = dir.mkdirs();
			}
			if (flag) {
				File originFile = new File(srcFileName);
	            if (originFile.exists() && originFile.isFile())
	            {
	                //原始文件
	                fileInStream = new FileInputStream(originFile);
	                
	                //加密文件
	                fileOutStream = new SvnFileOutputStream(dstFileName);
	                //加密写入，使用和OutputStream一致
	                FileUtil.streamCopy(fileInStream, fileOutStream);
	                Log.i("FileEncrypt", "encrypt success!");
	                callbackContext.success();
	            }
			} else {
				callbackContext.error("目标文件夹不能创建");
			}
		} catch (JSONException e) {
			callbackContext.error("param error");
		} catch (FileNotFoundException e) {
			callbackContext.error("File not found");
		} catch (Exception e) {
			callbackContext.error("inner error");
		}
	}
	
	private void fileDecrypt(JSONArray param, CallbackContext callbackContext) {
		try
        {
			SvnFileInputStream fileInStream = null;
	        FileOutputStream fileOutStream = null;
	        
	        String srcFileName = param.getString(0);
			String dstFileName = param.getString(1);
			
			String dstDir = dstFileName.substring(0, dstFileName.lastIndexOf("/"));
			File dir = new File(dstDir);
			boolean flag = true;
			if (!dir.exists()) {
				flag = dir.mkdirs();
			}
			if (flag) {
	            SvnFile originFile = new SvnFile(srcFileName);
	            if (originFile.exists() && originFile.isFile() && SvnFileTool.isEncFile(originFile.getPath()))
	            {
	                //加密文件
	                fileInStream = new SvnFileInputStream(originFile);
	                
	                //解密文件
	                fileOutStream = new FileOutputStream(new File(dstFileName));
	                
	                //解密读，使用和InputStream一致
	                FileUtil.write(fileInStream, fileOutStream);
	                Log.i("DecryptFile", "decipher success!");
	                callbackContext.success();
	            } else {
	            	callbackContext.error("wrong originFile");
	            }
		        
			} else {
				callbackContext.error("目标文件夹不能创建");
			}
        }
        catch (Exception e)
        {
            e.printStackTrace();
            Log.e("DecryptFile", "decipher error:" + e.getMessage());
        }
	}
	
}
