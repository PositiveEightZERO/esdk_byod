/**
 * 
 */
package com.huawei.esdk.demo.encrypt;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.huawei.esdk.demo.R;
import com.huawei.esdk.demo.encrypt.sqlite.Person;

/**
 * @author cWX223941
 *
 */
public class SqliteQueryDataAdapter extends BaseAdapter
{
    private Context context;
    private List<Person> showList;

    public SqliteQueryDataAdapter(Context context, List<Person> showList)
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
        SqliteDataItemHolder holder = null;
        if (convertView == null)
        {
            convertView = LayoutInflater.from(context).inflate(
                    R.layout.sqlite_listview_item, null);
            holder = new SqliteDataItemHolder();
            holder.setId((TextView) convertView
                    .findViewById(R.id.tv_sqlite_listview_item_id));
            holder.setName((TextView) convertView
                    .findViewById(R.id.tv_sqlite_listview_item_name));
            holder.setAge((TextView) convertView
                    .findViewById(R.id.tv_sqlite_listview_item_age));
            convertView.setTag(holder);
        }
        else
        {
            holder = (SqliteDataItemHolder) convertView.getTag();
        }
        Person checkResult = showList.get(position);
        holder.getId().setText(checkResult.getId() + "");
        holder.getName().setText(checkResult.getName());
        holder.getAge().setText(checkResult.getAge() + "");
        return convertView;
    }
    
    private class SqliteDataItemHolder
    {
        private TextView id;
        private TextView name;
        private TextView age;

        public TextView getId()
        {
            return id;
        }

        public void setId(TextView id)
        {
            this.id = id;
        }

        public TextView getName()
        {
            return name;
        }

        public void setName(TextView name)
        {
            this.name = name;
        }

        public TextView getAge()
        {
            return age;
        }

        public void setAge(TextView age)
        {
            this.age = age;
        }
    }
}
