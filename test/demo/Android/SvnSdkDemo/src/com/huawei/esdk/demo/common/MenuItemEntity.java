/**
 * 
 */
package com.huawei.esdk.demo.common;


/**
 * @author cWX223941
 *
 */
public class MenuItemEntity
{
   // private OnClickListener onClickListener;
    
    private String title;

    private int action;


    public MenuItemEntity(String title)
    {
        this.title = title;;
    }

    public MenuItemEntity(String title, int action)
    {
       this.title = title;
       this.action = action;
    }
    
    
    
    public String getTitle()
    {
        return title;
    }

    public void setTitle(String title)
    {
        this.title = title;
    }

    public int getAction()
    {
        return action;
    }

    public void setAction(int action)
    {
        this.action = action;
    }


//    public OnClickListener getOnClickListener()
//    {
//        return onClickListener;
//    }
//
//    public void setOnClickListener(OnClickListener onClickListener)
//    {
//        this.onClickListener = onClickListener;
//    }
}
