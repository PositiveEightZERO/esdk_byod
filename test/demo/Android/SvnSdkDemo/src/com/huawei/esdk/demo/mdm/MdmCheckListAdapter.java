/**
 * 
 */
package com.huawei.esdk.demo.mdm;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.huawei.esdk.demo.R;
import com.huawei.esdk.demo.common.CommonItemHolder;

/**
 * @author cWX223941
 *
 */
public class MdmCheckListAdapter extends BaseAdapter
{
    private Context context;
    private List<MdmCheckResultEntity> showList;

    public MdmCheckListAdapter(Context context,
            List<MdmCheckResultEntity> showList)
    {
        this.context = context;
        this.showList = showList;
    }

    @Override
    public int getCount()
    {
        return showList.size();
    }

    @Override
    public Object getItem(int position)
    {
        return showList.get(position);
    }

    @Override
    public long getItemId(int position)
    {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent)
    {
        CommonItemHolder holder = null;
        if (convertView == null)
        {
            convertView = LayoutInflater.from(context).inflate(
                    R.layout.mdm_listview_item, null);
            holder = new CommonItemHolder();
            holder.setItemContent((TextView) convertView
                    .findViewById(R.id.tv_mdm_listview_con));
            holder.setItemIcon((ImageView) convertView
                    .findViewById(R.id.iv_mdm_listview_icon));
            convertView.setTag(holder);
        }
        else
        {
            holder = (CommonItemHolder) convertView.getTag();
        }
        MdmCheckResultEntity checkResult = showList.get(position);
        holder.getItemContent().setText(checkResult.getContent());
        if (checkResult.isCheckOk())
        {
            holder.getItemIcon().setImageResource(R.drawable.icon_check_ok);
        }
        else
        {
            holder.getItemIcon().setImageResource(R.drawable.icon_check_nook);
        }
        return convertView;
    }
}
