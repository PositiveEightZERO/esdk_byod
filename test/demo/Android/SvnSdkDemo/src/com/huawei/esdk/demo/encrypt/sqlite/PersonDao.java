package com.huawei.esdk.demo.encrypt.sqlite;

import java.util.ArrayList;
import java.util.List;

import android.annotation.SuppressLint;
import android.content.Context;
import android.database.Cursor;

import com.huawei.svn.sdk.sqlite.SQLiteDatabase;

//import android.database.sqlite.SQLiteDatabase;
/**
 * 实现类
 * @author miao
 *
 */
@SuppressLint("NewApi")
public class PersonDao
{
    // 辅助类属性
    private DBHelper helper;
    private SQLiteDatabase db;

    /**
     * 带参构造方法，传入context
     * 
     * @param context
     */
    public PersonDao(Context context)
    {
        helper = new DBHelper(context);
        db = helper.getWritableDatabase("sdk");
    }

    /**
     * 使用不同的方法删除记录1到多条记录
     * 
     * @param ids
     */
    public void delete(Integer... ids)
    {
        String[] c = new String[ids.length];
        StringBuffer sb = new StringBuffer();
        if (ids.length > 0)
        {
            for (int i = 0; i < ids.length; i++)
            {
                sb.append('?').append(',');
                // 把整数数组转换哼字符串数组
                c[i] = ids[i].toString();
            }
            // 删除最后一个元素
            sb.deleteCharAt(sb.length() - 1);
        }
        db.delete("person", "personid in (" + sb.toString() + ")", c);
        //		db.close();
    }

    /**
     * 添加纪录
     * 
     * @param person
     */
    public void save(Person person)
    {
        System.out.println("save db:" + db);
        db.execSQL("insert into person (name,age) values(?,?)", new Object[]
        { person.getName(), person.getAge() });
        //		db.close();
    }

    /**
    * 添加纪录
    * 
    * @param person
    */
    public void update(Person person)
    {
        System.out.println("save db:" + db);
        db.execSQL("update person set name = ?, age = ? where personid=?",
                new Object[]
                { person.getName(), person.getAge(), person.getId() });
        //        db.close();
    }

    /**
     * 根据id查找
     * 
     * @param id
     * @return
     */
    public Cursor find(String name, int age)
    {
        String querySQL = "select personid as _id, name,age from person ";
        if (name != null && name.length() > 0)
        {
            querySQL += "where name ='";
            querySQL += name;
            querySQL += "' ";
            if (age != 0)
            {
                querySQL += " and age=";
                querySQL += age;
            }
        }
        else
        {
            if (age != 0)
            {
                querySQL += " where age=";
                querySQL += age;
            }
        }
        Cursor cursor = db.rawQuery(querySQL, null);
        return cursor;
    }

    /**
     * 查找所有的记录
     * 
     * @return
     */
    public List<Person> getAll()
    {
        List<Person> persons = new ArrayList<Person>();
        Cursor cursor = db.rawQuery(
                "select personid as _id, name,age from person", null);
        while (cursor.moveToNext())
        {
            persons.add(new Person(cursor.getInt(0), cursor.getString(1),
                    cursor.getInt(2)));
        }
        return persons;
    }

    /**
     * 查询全部
     * 
     * @return 游标
     */
    public Cursor getAllPerson()
    {
        //System.out.println("getAllPerson db:" + db);
        // ListView 里的id是有个下划线的，所以这里要给个别名_id
        Cursor cursor = db.rawQuery(
                "select personid as _id, name,age from person", null);
        // 这里数据库不能关闭
        return cursor;
    }

    public void closeDB()
    {
        db.close();
    }
}
