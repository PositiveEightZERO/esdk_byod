package com.huawei.svn.sdk.mdm;

// TODO: Auto-generated Javadoc
/**
 * The Class MDMCheckResult.
 */
public class MDMCheckResult
{

    /**
     * MDM检查是否成功."0"：表示查询正常。 其他：表示查询失败.
     */
    private int isSuccess;

    /** MDM功能是否启用（AnyOffice MDM配置文件已安装）. */
    private boolean isMDMEnabled;

    /** 绑定检查结果. */
    private int bindResult;

    /** 是否root或者越狱. */
    private boolean isRoot;

    /** 锁屏密码是否符合安全策略要求. */
    private boolean isPwdCheckOK;

    /** 终端安装应用列表是否符合安全策略要求. */
    private boolean isAppCheckOK;

    /** 是否长期未登录. */
    private boolean isLongTimeNoLogin;

    /** 其他违规检查是否安全策略要求. */
    private boolean isOtherCheckOK;


    public int getIsSuccess()
    {
        return isSuccess;
    }


    public void setIsSuccess(int isSuccess)
    {
        this.isSuccess = isSuccess;
    }


    public boolean isMDMEnabled()
    {
        return isMDMEnabled;
    }


    public void setMDMEnabled(boolean isMDMEnabled)
    {
        this.isMDMEnabled = isMDMEnabled;
    }


    public int getBindResult()
    {
        return bindResult;
    }


    public void setBindResult(int bindResult)
    {
        this.bindResult = bindResult;
    }


    public boolean isRoot()
    {
        return isRoot;
    }


    public void setRoot(boolean isRoot)
    {
        this.isRoot = isRoot;
    }

  
    public boolean isPwdCheckOK()
    {
        return isPwdCheckOK;
    }


    public void setPwdCheckOK(boolean isPwdCheckOK)
    {
        this.isPwdCheckOK = isPwdCheckOK;
    }


    public boolean isAppCheckOK()
    {
        return isAppCheckOK;
    }

    public void setAppCheckOK(boolean isAppCheckOK)
    {
        this.isAppCheckOK = isAppCheckOK;
    }


    public boolean isLongTimeNoLogin()
    {
        return isLongTimeNoLogin;
    }


    public void setLongTimeNoLogin(boolean isLongTimeNoLogin)
    {
        this.isLongTimeNoLogin = isLongTimeNoLogin;
    }


    public boolean isOtherCheckOK()
    {
        return isOtherCheckOK;
    }

  
    public void setOtherCheckOK(boolean isOtherCheckOK)
    {
        this.isOtherCheckOK = isOtherCheckOK;
    }

    @Override
    public String toString()
    {
        StringBuilder sb = new StringBuilder();
        sb.append("isSuccess:" + isSuccess);
        sb.append(",isMDMEnabled:" + isMDMEnabled);
        sb.append(",bindResult:" + bindResult);
        sb.append(",isRoot:" + isRoot);
        sb.append(",isPwdCheckOK:" + isPwdCheckOK);
        sb.append(",isAppCheckOK:" + isAppCheckOK);
        sb.append(",isLongTimeNoLogin:" + isLongTimeNoLogin);
        sb.append(",isOtherCheckOK:" + isOtherCheckOK);
        return sb.toString();
    }

}
