package com.huawei.esdk.anyoffice.cordova;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import com.huawei.anyoffice.sdk.network.NetChangeCallback;
import com.huawei.anyoffice.sdk.network.NetStatusManager;

public class NetStatusManagerCordova extends CordovaPlugin {
	
    private CallbackContext statusCallbackContext = null;
	
	public NetStatusManagerCordova() {
		System.loadLibrary("svnapi");
		System.loadLibrary("anyofficesdk");
		System.loadLibrary("jniapi");
		
	}
	
	@Override
	public boolean execute(String action, JSONArray param, CallbackContext callbackContext) throws JSONException {
		if ("getNetStatus".equals(action)) {
			int res = NetStatusManager.getInstance().getNetStatus();
			callbackContext.success(res);
		} else if ("start".equals(action)) {
			if (this.statusCallbackContext != null) {
				callbackContext.error("Status listener already running.");
				return true;
			}
			this.statusCallbackContext = callbackContext;
			
			NetStatusManager.getInstance().setNetChangeCallback(callback);
		}
		return true;
	}
	
    private NetChangeCallback callback = new NetChangeCallback()
    {
        @Override
        public void onNetChanged(int oldState, int newState, int errorCode)
        {
            Log.e("ChangeStatusCallBack", String.format(
                    "oldState:%d, newState:%d, errorCode:%d, ", oldState,
                    newState, errorCode));
            JSONObject obj = new JSONObject();
            try {
				obj.put("oldStatus", oldState);
				obj.put("newStatus", newState);
				obj.put("errorCode", errorCode);
			} catch (JSONException e) {
				statusCallbackContext.error("callback error");
			}
            PluginResult result = new PluginResult(errorCode == 0 ? PluginResult.Status.OK : PluginResult.Status.ERROR, obj);
            result.setKeepCallback(true);
            statusCallbackContext.sendPluginResult(result);
        }
    };
}
