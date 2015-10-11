package com.huawei.esdk.demo.common;

import android.widget.ImageView;
import android.widget.TextView;

public class CommonItemHolder
{
    private TextView itemContent;
    private ImageView itemIcon;

    public TextView getItemContent()
    {
        return itemContent;
    }

    public void setItemContent(TextView itemContent)
    {
        this.itemContent = itemContent;
    }

    public ImageView getItemIcon()
    {
        return itemIcon;
    }

    public void setItemIcon(ImageView itemIcon)
    {
        this.itemIcon = itemIcon;
    }
}
