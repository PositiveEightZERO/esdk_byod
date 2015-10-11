/**
 * 
 */
package com.huawei.esdk.demo.mdm;


/**
 * @author cWX223941
 *
 */
public class MdmCheckResultEntity
{
    
    private String content;
    
    private boolean isCheckOk;
    


    public String getContent()
    {
        return content;
    }

    public void setContent(String content)
    {
        this.content = content;
    }

    public MdmCheckResultEntity()
    {
    }

    public MdmCheckResultEntity(String content, boolean isCheckOk)
    {
        this.setContent(content);
        this.isCheckOk = isCheckOk;
    }

    public boolean isCheckOk()
    {
        return isCheckOk;
    }

    public void setCheckOk(boolean isCheckOk)
    {
        this.isCheckOk = isCheckOk;
    }
}
