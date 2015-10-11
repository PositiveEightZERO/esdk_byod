package com.huawei.svn.sdk.mdm;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.List;

import org.json.JSONObject;

import android.R.bool;
import android.app.ActivityManager;
import android.app.admin.DevicePolicyManager;
import android.content.ComponentName;
import android.content.Context;
import android.util.Log;

import com.huawei.svn.sdk.SvnConstants;
import com.huawei.svn.sdk.server.SvnClientApiImpl;

/**
 * MDM检查-检查AnyOffice是否已安装并正在运行，检查设备绑定信息
 * 
 */
public class MdmCheck
{
    /**
     * 日志tag
     */
    private static final String TAG = "MDMCheck";

    /**
     * 未安装MDM ,前面检测进程或管理员权限只要返回false，都返回0
     */
    public static final int NULL_INSTALL_MDM = 0;

    /**
     * 已经被当前登录用户绑定 （不区分是否开启多用户功能 ，已阅读保密协议：合法用户，可以进入）
     */
    public static final int ASSERT_BINDED_BY_USER = 1;
    /**
     * 不区分是否开启多用户功能：未绑定&正在审批 （待审批设备，不允许进入）
     */
    public static final int ASSERT_APPROVING = 2;
    /**
     * 不区分是否开启多用户功能：超过允许绑定设备数量 （不允许进入）
     */
    public static final int ASSERT_BYOND_LIMINT = 3;
    /**
     * 该设备已被其他用户绑定 （未开启多用户功能： 登录用户和注册用户为不同用户，不允许进入）
     */
    public static final int ASSERT_BINDED_BY_OTHERS = 4;
    /**
     * 不区分是否开启多用户功能：该设备已被其他用户注册, 正在审批中 （待审批设备，不允许进入）
     */
    public static final int ASSERT_REG_BY_OTHERS = 5;
    /**
     * 未绑定&可绑定 （需要进行注册）
     */
    public static final int ASSERT_NONBINDED = 6;
    /**
     * 不区分是否开启多用户功能：资产绑定失败，请联系管理员 （数据库操作失败）
     */
    public static final int ASSERT_BINDED_FAILED = 7;
    /**
     * 注册通过但是未查看过保密协议 （当前登录用户为资产注册人 ，需要阅读保密协议）
     */
    public static final int ASSERT_AGREEMENT_NOREAD = 8;
    /**
     * 注册通过但是未查看过保密协议 （已开启多用户: 当前登录用户非资产注册人 ，不允许进入）
     */
    public static final int ASSERT_AGREEMENT_NOREAD_OTHERS = 9;
    /**
     * 已经被其它用户绑定----- 与ASSERT_BINDED_BY_OTHERS对应 （ 开启多用户功能：合法用户，已阅读保密协议可以进入 ）
     */
    public static final int ASSERT_BINDED_MULTIUSER = 10;
    /**
     * 该设备超期未登录:对于多用户， 没有任何一个用户登录过该设备
     */
    public static final int ASSERT_LOGON_TIMEOUT = 12;

    /**
     * 该设备超期未登录:对于多用户， 没有任何一个用户登录过该设备
     */
    public static final int ASSERT_UNKNOWN = 65535;

    private static SvnClientApiImpl client = new SvnClientApiImpl();

    /**
     * 加载so
     */
    static
    {
        System.loadLibrary("svnapi");
    }

    /**
     * 获取某一进程是否正在运行
     * 
     * @param context
     *            应用上下文
     * @param procname
     *            进程名
     * @return 进程是否正在运行
     */
    private static boolean getRunningAppProcessInfo(Context context,
            String procname)
    {
        // 获得ActivityManager服务的对象
        ActivityManager mActivityManager = (ActivityManager) context
                .getSystemService(Context.ACTIVITY_SERVICE);

        // 通过调用ActivityManager的getRunningAppProcesses()方法获得系统里所有正在运行的进程
        List<ActivityManager.RunningAppProcessInfo> appProcessList = mActivityManager
                .getRunningAppProcesses();

        for (ActivityManager.RunningAppProcessInfo appProcessInfo : appProcessList)
        {
            if (procname.equalsIgnoreCase(appProcessInfo.processName))
            {
                return true;
            }
        }

        return false;
    }

    /**
     * 检查是否已在设备管理器中激活
     * 
     * @param context
     *            应用上下文
     * @param pkg
     *            包名
     * @param cls
     *            类名
     * @return 是否已在设备管理器中激活
     */
    private static boolean getCheckDeviceManager(Context context, String pkg,
            String cls)
    {
        DevicePolicyManager devicePolicyManager = (DevicePolicyManager) context
                .getSystemService(Context.DEVICE_POLICY_SERVICE);// 实例化DevicePolicyManager
        
//        List<ComponentName> admins = devicePolicyManager.getActiveAdmins();
//        if(admins != null && admins.size() > 0)
//        {
//            for (ComponentName componentName : admins)
//            {
//                Log.i("SDK", componentName.getClassName() + ":" + componentName.getPackageName());
//            }
//        }

        ComponentName componentName = new ComponentName(pkg, cls);

        boolean result =  devicePolicyManager.isAdminActive(componentName);
        
        Log.i("SDK", "getCheckDeviceManager returns:" + result);
        return result;
    }

    /**
     * 检查AnyOffice是否已安装并正在运行，并且设备已被当前登录用户绑定
     * 
     * @param context
     *            应用上下文
     * @param pkg
     *            包名
     * @param cls
     *            类名
     * @return AnyOffice是否已安装并正在运行
     */
    public static int checkMdm(Context context, String pkg, String cls)
    {
        if (!getCheckDeviceManager(context, pkg, cls)
                || !getRunningAppProcessInfo(context, pkg))
        {
            Log.i(TAG,
                    "AnyOffice not activated as device manager or not running");
            return NULL_INSTALL_MDM;
        }

        int result = checkBind("mdm");
        Log.i(TAG, "device bind check result:" + result);

        return result;
    }

    /**
     * 检查设备绑定信息
     * 
     * @param checkParam
     * @return
     */
    public static int checkBind(String checkParam)
    {
        return client.checkBind(checkParam);
    }

    public static MDMCheckResult checkMdmSpecific(Context context, String pkg,
            String cls)
    {
        MDMCheckResult result = new MDMCheckResult();
        result.setIsSuccess(SvnConstants.SVN_OK);

        int iRet = checkMdm(context, pkg, cls);

        if (NULL_INSTALL_MDM == iRet)
        {
            result.setMDMEnabled(false);
            Log.i(TAG, "checkMdmSpecific success:mdm not enabled");
            return result;
        }

        if (ASSERT_UNKNOWN == iRet)
        {
            Log.i(TAG, "checkMdmSpecific failed:ASSERT_UNKNOWN");
            result.setIsSuccess(SvnConstants.SVN_ERR);
            return result;
        }

        result.setMDMEnabled(true);
        result.setBindResult(iRet);
        
        if(ASSERT_BINDED_BY_USER != iRet && ASSERT_BINDED_MULTIUSER != iRet)
        {
            return result;
        }

        String violationResult = client.getMdmViolationResult();
        if (null == violationResult)
        {
            result.setIsSuccess(SvnConstants.SVN_ERR);
            Log.i(TAG, "checkMdmSpecific failed:violationResult null");
            return result;
        }

        try
        {
            Log.i(TAG, "checkMdmSpecific violationResult:" + violationResult);
            JSONObject jsonObject = new JSONObject(violationResult);
            if (jsonObject != null)
            {
                JSONObject vgInfo = jsonObject.getJSONObject("vgInfo");
                JSONObject webInfo = jsonObject.getJSONObject("webInfo");
                if (vgInfo != null && webInfo != null)
                {
                    String errorCode = vgInfo.getString("errorCode");
                    if ("0".equals(errorCode))
                    {

                        boolean osverFlag = webInfo.getBoolean("osverFlag");
                        boolean nopasswdFlag = webInfo
                                .getBoolean("nopasswdFlag");
                        boolean decryptFlag = webInfo.getBoolean("decryptFlag");
                        boolean usbFlag = webInfo.getBoolean("usbFlag");
                        boolean appNormalFlag = webInfo
                                .getBoolean("appNormalFlag");
                        boolean rootFlag = webInfo.getBoolean("rootFlag");
                        boolean appNeedFlag = webInfo.getBoolean("appNeedFlag");
                        //boolean loginFlag = webInfo.getBoolean("loginFlag");
                        // boolean mdmcfgFlag =
                        // webInfo.getBoolean("mdmcfgFlag");

                        result.setRoot(rootFlag);
                        result.setPwdCheckOK(!nopasswdFlag);
                        result.setAppCheckOK(!appNormalFlag && !appNeedFlag);
                        //result.setLongTimeNoLogin(loginFlag);
                        result.setOtherCheckOK(!osverFlag && !decryptFlag
                                && !usbFlag);

                        return result;
                    }
                }
            }

            result.setIsSuccess(SvnConstants.SVN_ERR);
            Log.i(TAG, "checkMdmSpecific failed:jsonObject error");
            return result;

            // {"webInfo":{"osverFlag":"","nopasswdFlag":"","decryptFlag":"","usbFlag":"","appNormalFlag":"","rootFlag":"","appNeedFlag":"","loginFlag":"","mdmcfgFlag":""},"vgInfo":{"errorCode":"2","dc":null}}

            // {"webInfo":{"osverFlag":false,"nopasswdFlag":false,"decryptFlag":false,"usbFlag":false,"appNormalFlag":false,"rootFlag":false,"appNeedFlag":false,"loginFlag":false,"mdmcfgFlag":false},"vgInfo":{"errorCode":"0","dc":null}}

        }
        catch (Exception e)
        {
            StringWriter sw = new StringWriter();
            e.printStackTrace(new PrintWriter(sw));
            String exceptionAsString = sw.toString();
            Log.i(TAG, "" + e.getMessage());
            Log.i(TAG, exceptionAsString);

            result.setIsSuccess(SvnConstants.SVN_ERR);
            Log.i(TAG, "checkMdmSpecific failed:Exception");
            return result;
        }

    }
}
