package com.huawei.esdk.anyoffice.cordova;

import java.net.InetSocketAddress;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;

import android.app.Activity;

import com.huawei.anyoffice.sdk.login.LoginAgent;
import com.huawei.anyoffice.sdk.login.LoginParam;
import com.huawei.anyoffice.sdk.login.LoginParam.AutoLoginType;
import com.huawei.anyoffice.sdk.login.LoginParam.UserInfo;

public class LoginAgentCordova extends CordovaPlugin {
	
	public LoginAgentCordova() {
        System.loadLibrary("svnapi");
        System.loadLibrary("anyofficesdk");
        System.loadLibrary("jniapi");
	}
	
	@Override
	public boolean execute(String action, JSONArray param, CallbackContext callbackContext) throws JSONException {
		if ("login".equals(action)) {
			login(param, callbackContext, cordova);
		} 
		return true;
	}
	
	public void login(JSONArray param, CallbackContext callbackContext, CordovaInterface cordova) {
		
		try {
			String username = param.getString(0);
			String password = param.getString(1);
	        String gateway = param.getString(2);
	        
//	        int res = SvnApiService.login(userName, password, gateway, strPackagePath, appName, devicedId);
	        
	        Activity activity = cordova.getActivity();
	        
	        LoginParam loginParam = new LoginParam(); 
	        loginParam.setServiceType(activity.getPackageName()); 
	        loginParam.setAutoLoginType(AutoLoginType.auto_login_disable); 
	        loginParam.setLoginBackground(true); 
	        loginParam.setInternetAddress(new InetSocketAddress(gateway, 443)); 
	        
	        UserInfo userInfo = loginParam.new UserInfo(); 
	        userInfo.userName = username; 
	        userInfo.password = password; 
	        
	        loginParam.setUserInfo(userInfo); 
	        loginParam.setUseSecureTransfer(true); 
	        System.out.println("Begin to login gateway"); 
	        int ret = LoginAgent.getInstance().loginSync(activity, loginParam); 

	        callbackContext.sendPluginResult(new PluginResult(ret == 0 ? PluginResult.Status.OK : PluginResult.Status.ERROR, ret));
	        
		} catch (JSONException e) {
			callbackContext.error(-101);
		}
        
	}
	
}
