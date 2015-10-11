/**
 * 
 */
package com.huawei.esdk.demo.common;

import com.huawei.anyoffice.sdk.sandbox.SDKClipboard;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.util.Log;

/**
 * @author cWX223941
 *
 */
public class BaseActivity extends FragmentActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        //		MyApplication.getInstance().addActivity(this);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        SDKClipboard.getInstance().onResume(this);
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        SDKClipboard.getInstance().onPause(this);
    }
//
//    public void startCommonThread(ThreadCommonIface threadCommon,
//            Handler handler, int type)
//    {
//        AsyncCommon asyncCommon = new AsyncCommon(threadCommon, handler, type);
//        asyncCommon.start();
//    }
//
//    public void startCommonThread(ThreadCommonIface threadCommon,
//            Handler handler, int type, Serializable object)
//    {
//        AsyncCommon asyncCommon = new AsyncCommon(threadCommon, handler, type,
//                object);
//        asyncCommon.start();
//    }
    
    @Override
    public void onLowMemory()
    {
        // TODO Auto-generated method stub
        super.onLowMemory();
        Log.e("Memory", "onLowMemory()");
    }
}
