package com.huawei.esdk.demo.encrypt.sqlite;

import android.content.Context;
//import android.database.sqlite.SQLiteDatabase;
//import android.database.sqlite.SQLiteOpenHelper;
import com.huawei.svn.sdk.sqlite.SQLiteDatabase;
import com.huawei.svn.sdk.sqlite.SQLiteOpenHelper;

/**
 * 数据库辅助类
 * 
 * @author miao
 * 
 */
public class DBHelper extends SQLiteOpenHelper
{
    /*
     * @param context 上下文
     * 
     * @param name 数据库名字
     * 
     * @param factory 游标工厂对象，没指定就设置为null
     * 
     * @param version 版本号
     */
    // public DBHelper(Context context, String name, CursorFactory factory,
    // int version) {
    // super(context, name, factory, version);
    // }
    private static final String DB_NAME = "person.db";
    private static final int VERSION = 1;

    public DBHelper(Context context)
    {
        super(context, DB_NAME, null, VERSION);
    }

    /**
     * 第一次运行的时候创建
     */
    @Override
    public void onCreate(SQLiteDatabase db)
    {
        db.execSQL("CREATE TABLE IF NOT EXISTS person (personid integer primary key autoincrement, name text, age INTEGER)");
    }

    /**
     * 更新的时候
     */
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion)
    {
        db.execSQL("DROP TABLE IF EXISTS person");
        onCreate(db);
    }
}
