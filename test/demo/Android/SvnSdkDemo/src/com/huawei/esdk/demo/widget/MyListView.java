/**
 * 
 */
package com.huawei.esdk.demo.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.ListView;

/**
 * @author cWX223941
 *
 */
public class MyListView extends ListView
{
    //    private static final String TAG = "MyListView";
    public MyListView(Context context)
    {
        super(context);
    }

    public MyListView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    }

    public MyListView(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
    }

    @Override
    public void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
    {
        //calculate all childs's height
        int expandSpec = MeasureSpec.makeMeasureSpec(Integer.MAX_VALUE >> 2,
                MeasureSpec.AT_MOST);
        super.onMeasure(widthMeasureSpec, expandSpec);
    }
}
