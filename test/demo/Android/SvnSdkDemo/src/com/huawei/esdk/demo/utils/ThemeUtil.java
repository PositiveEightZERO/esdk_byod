/**
 * 
 */
package com.huawei.esdk.demo.utils;

import android.content.Context;
import android.widget.TextView;

import com.huawei.esdk.demo.R;

/**
 * @author cWX223941
 *
 */
public class ThemeUtil
{
//    public static void setListItemChildTheme(MyListItem childItem)
//    {
//        if (null != childItem)
//        {
//            //            childItem.setItemBackgroundColor(R.color.list_item_child_bg);
//            childItem
//                    .setItemBackgroundRes(R.drawable.bg_list_item_child_selected);
//            childItem.setArrowRes(R.drawable.icon_arrow_right);
//        }
//    }

//    public static void setMDMCheckingTheme(MyCheckListItem checkItem)
//    {
//        if (null != checkItem)
//        {
//            checkItem.setCheckBlockVisiable(View.VISIBLE);
//            checkItem.setCheckTitle(R.string.checking);
//        }
//    }

    //    public static void setHttpLoginResultTheme(HttpLoginFragment loginFragment){
    //        if(null != loginFragment){
    //            return;
    //        }
    //        loginFragment.
    //    }
    public static void setBtnToEnable(TextView button, Context context)
    {
        if (null != button && null != context)
        {
            if (!button.isEnabled())
            {
                button.setEnabled(true);
                button.setTextColor(context.getResources().getColor(
                        R.color.btn_normal_withbg));
                button.setBackgroundResource(R.drawable.button_withbg_enable_nor);
            }
        }
    }

    public static void setBtnToUnable(TextView button, Context context)
    {
        if (null != button && null != context)
        {
            if (button.isEnabled())
            {
                button.setEnabled(false);
                button.setTextColor(context.getResources().getColor(
                        R.color.btn_content_unable));
                button.setBackgroundResource(R.drawable.button_withbg_unable);
            }
        }
    }
}
