package com.huawei.esdk.anyoffice.cordova;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.huawei.anyoffice.sdk.doc.SecReader;
import com.huawei.anyoffice.sdk.exception.NoRMSAppFoundException;
import com.huawei.anyoffice.sdk.exception.NoRecommendedAppException;
import com.huawei.svn.sdk.fsm.SvnFile;
import com.huawei.svn.sdk.fsm.SvnFileTool;

public class FilePluginCordova extends CordovaPlugin {
	
	public FilePluginCordova() {
        System.loadLibrary("svnapi");
        System.loadLibrary("anyofficesdk");
        System.loadLibrary("jniapi");
	}
	
	@Override
	public boolean execute(String action, JSONArray param, CallbackContext callbackContext) throws JSONException {
		if ("listFile".equals(action)) {
			listFile(param, callbackContext, cordova);
		} else if ("readFile".equals(action)) {
			readFile(param, callbackContext, cordova);
		}
		return true;
	}
	
	private void listFile(JSONArray param, CallbackContext callbackContext, CordovaInterface cordova) {
		try {
			String dirName = param.getString(0);
			SvnFile rootFile = new SvnFile(dirName);
			
			JSONArray ret = new JSONArray();
			JSONObject obj = null;
			if (rootFile.isDirectory()) {
				SvnFile[] fileChildList = rootFile.listFiles();
				for (SvnFile childFile : fileChildList) {
					obj = new JSONObject();
					obj.put("name", childFile.getName());
					obj.put("fullPath", childFile.getPath());
		            obj.put("isDirectory", childFile.isDirectory());
		            obj.put("parentPath", childFile.getParentFile().getPath());
		            if (!childFile.isDirectory())
		            {
		            	obj.put("isEnryptedFile",SvnFileTool.isEncFile(childFile
		                        .getPath()));
		            }
		            ret.put(obj);
				}
			}
			
			callbackContext.success(ret);
		} catch (JSONException e) {
			callbackContext.error("fail to list files");
		}
	}
	
	private void readFile(JSONArray param, CallbackContext callbackContext, CordovaInterface cordova) {
		try {
			String filePath = param.getString(0);
			String openMode = param.getString(1);
			
			SecReader reader = new SecReader();
	        
	        reader.setRecommendedApp("com.kingsoft.moffice_pro_hw", SecReader.SDK_MIMETYPE_DOCUMENT);
	        
	        boolean ret = false;
	        try
	        {
	            //打开文件
	            ret = reader.openDocWithSDK(cordova.getActivity(), filePath, cordova.getActivity().getPackageName(), "".equals(openMode) ? null : openMode);
	            if (ret) {
	            	callbackContext.success();
	            } else {
	            	callbackContext.error("fail to read");
	            }
	        }
	        catch (NoRMSAppFoundException e)
	        {
	            callbackContext.error("NoRMSAppFoundException");
	        }
	        catch (NoRecommendedAppException e)
	        {
	        	callbackContext.error("NoRecommendedAppException");
	        }
		} catch (JSONException e) {
			callbackContext.error("NoRecommendedAppException");
		}
	}
}
