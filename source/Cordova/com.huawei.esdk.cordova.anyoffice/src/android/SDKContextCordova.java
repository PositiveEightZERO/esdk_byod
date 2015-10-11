package com.huawei.esdk.anyoffice.cordova;

import java.io.File;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;

import com.huawei.anyoffice.sdk.SDKContext;
import com.huawei.svn.sdk.webview.SvnWebViewProxy;

public class SDKContextCordova extends CordovaPlugin {
    
    
    
	
	@Override
    public void initialize(CordovaInterface cordova, CordovaWebView webView)
    {
        // TODO Auto-generated method stub
        super.initialize(cordova, webView);
        System.loadLibrary("svnapi");
        System.loadLibrary("anyofficesdk");
        System.loadLibrary("jniapi");
        
        SvnWebViewProxy.getInstance().setWebViewUseSVN(webView);
    }


	
	@Override
	public boolean execute(String action, JSONArray param, CallbackContext callbackContext) throws JSONException {
		if ("init".equals(action)) {
			init(param, callbackContext, cordova);
		} 
		return true;
	}
	
	public void init(JSONArray param, CallbackContext callbackContext, CordovaInterface cordova) {
		try {
		    
		    
			String workPath = param.getString(0);
			String username = param.getString(1);
			
			
			boolean res = false;
			
			File folder = new File(workPath);
			if (!folder.exists()) {
				folder.mkdirs();
			}
			
			if (username != null && !username.equals("")) {
				res = SDKContext.getInstance().init(cordova.getActivity(), workPath);
			} else {
				res = SDKContext.getInstance().init(cordova.getActivity(), username, workPath);
			}
			
			
			if(res)
			{
			    
			    callbackContext.sendPluginResult(new PluginResult(PluginResult.Status.OK, res));
			}
			else 
			{
			    callbackContext.sendPluginResult(new PluginResult(PluginResult.Status.ERROR, res));
            }
			
			
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
}
