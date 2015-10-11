package com.huawei.svn.sdk.ui;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.text.Editable;
import android.text.Selection;
import android.text.Spannable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.TextView;

import com.huawei.anyoffice.sdk.sandbox.SDKClipboard;
import com.huawei.shield.ProxyConstruct;
import com.huawei.shield.WedgeClass;

@WedgeClass(value="Landroid/widget/TextView;")
@ProxyConstruct(value="Landroid/widget/TextView;", isView=true)
public class SDKTextView extends TextView 
{
    private Context mContext;

    private static final String TAG = "SDKTextView";

    private SDKClipboard sdkClipboard;

    private ClipboardManager cm;

    public SDKTextView(Context context)
    {
        this(context, null);
    }

    public SDKTextView(Context context, AttributeSet attrs)
    {
        this(context, attrs, android.R.attr.textViewStyle);
    }

    public SDKTextView(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
        this.mContext = context;
        init();
    }

    private void init()
    {
        // 数据隔离字段
        // dataIsolationEn =
        // SettingsBSImpl.getInstance().getSettingsGatewayPolicyBD()
        // .getDataIsolationEn();

        // 获取自定义的剪切板
        sdkClipboard = SDKClipboard.getInstance();

        cm = (ClipboardManager) mContext
                .getSystemService(Context.CLIPBOARD_SERVICE);

        // 设置监听
        //setOnLongClickListener(this);

        //setOnTouchListener(this);
    }
    
    

    @Override
    public boolean onTextContextMenuItem(int id)
    {
        int min = 0;
        int max = getText().toString().length();

        if (isFocused())
        {
            final int selStart = getSelectionStart();
            final int selEnd = getSelectionEnd();

            min = Math.max(0, Math.min(selStart, selEnd));
            max = Math.max(0, Math.max(selStart, selEnd));
        }

        // 数据隔离开启的时候

        try
        {
            switch (id)
            {
            case android.R.id.paste:
                // SDK有数据的时候
                if (hasText())
                {
                    paste(min, max);
                    return true;
                }
                else
                {
                    // System.out.println("system paste");
                    return super.onTextContextMenuItem(id);
                }
            case android.R.id.cut:
                // System.out.println("sdk cut");
                sdkClipboard.setText(getTransformedText(min, max));
                deleteText_internal(min, max);
                // 新光标的位子
                int index = getSelectionEnd();
                setText(getmText().toString());
                // 设置光标的位子
                Selection.setSelection((Spannable) getText(), index);
                stopSelectionActionMode();
                return true;

            case android.R.id.copy:
                // System.out.println("sdk copy");
                sdkClipboard.setText(getTransformedText(min, max));
                stopSelectionActionMode();
                return true;

            default:
                // 额外按钮的交还给系统
                return super.onTextContextMenuItem(id);

            }
        }
        catch (NoSuchMethodException e)
        {
            // System.out.println("NoSuchMethodException");
        }
        catch (ClassNotFoundException e)
        {
            // System.out.println("ClassNotFoundException");
        }
        catch (IllegalAccessException e)
        {
            // System.out.println("IllegalAccessException");
        }
        catch (IllegalArgumentException e)
        {
            // System.out.println("IllegalArgumentException");
        }
        catch (InvocationTargetException e)
        {
            // System.out.println("InvocationTargetException");
        }
        catch (NoSuchFieldException e)
        {
            // System.out.println("NoSuchFieldException");
        }
        return false;

    }

    private void paste(int min, int max) throws NoSuchMethodException,
            ClassNotFoundException, IllegalAccessException,
            IllegalArgumentException, InvocationTargetException,
            NoSuchFieldException
    {
        boolean didFirst = false;
        CharSequence paste = sdkClipboard.getText();
        if (paste != null)
        {
            if (!didFirst)
            {
                long minMax = prepareSpacesAroundPaste(min, max, paste);
                Log.d("Clipboard", "minMax = " + minMax);
                min = (int) (minMax >>> 32);
                max = (int) (minMax & 0x00000000FFFFFFFFL);
                Selection.setSelection((Spannable) getmText(), max);
                ((Editable) getmText()).replace(min, max, paste);
                didFirst = true;
            }
            else
            {
                ((Editable) getmText()).insert(getSelectionEnd(), "\n");
                ((Editable) getmText()).insert(getSelectionEnd(), paste);
            }

            // 新光标的位子
            int index = getSelectionEnd();
            // System.out.println("getSelectionEnd:" + getSelectionEnd());
            setText(getmText().toString());
            // 设置光标的位子
            Selection.setSelection((Spannable) getText(), index);
            stopSelectionActionMode();
        }
    }

    private Object getmText() throws ClassNotFoundException,
            NoSuchFieldException, IllegalAccessException,
            IllegalArgumentException
    {
        // System.out.println("getmText");
        Class<?> textViewClass = Class.forName("android.widget.TextView");
        Field field = textViewClass.getDeclaredField("mText");
        field.setAccessible(true);
        return field.get(this);
    }

    private long prepareSpacesAroundPaste(int min, int max, CharSequence paste)
            throws NoSuchMethodException, ClassNotFoundException,
            IllegalAccessException, IllegalArgumentException,
            InvocationTargetException
    {
        // System.out.println("prepareSpacesAroundPaste");
        Class<?> textViewClass = Class.forName("android.widget.TextView");
        Method method = textViewClass.getDeclaredMethod(
                "prepareSpacesAroundPaste", int.class, int.class,
                CharSequence.class);
        method.setAccessible(true);
        return (Long) method.invoke(this, min, max, paste);
    }

    private void stopSelectionActionMode() throws ClassNotFoundException,
            NoSuchMethodException, IllegalAccessException,
            IllegalArgumentException, InvocationTargetException
    {
        // System.out.println("stopSelectionActionMode");
        Class<?> textViewClass = Class.forName("android.widget.TextView");
        Method method = textViewClass
                .getDeclaredMethod("stopSelectionActionMode");
        method.setAccessible(true);
        method.invoke(this);
    }

    private CharSequence getTransformedText(int start, int end)
            throws ClassNotFoundException, NoSuchMethodException,
            IllegalAccessException, IllegalArgumentException,
            InvocationTargetException
    {
        // System.out.println("getTransformedText");
        Class<?> textViewClass = Class.forName("android.widget.TextView");
        Method method = textViewClass.getDeclaredMethod("getTransformedText",
                int.class, int.class);
        method.setAccessible(true);
        return (CharSequence) method.invoke(this, start, end);
    }

    private void deleteText_internal(int start, int end)
            throws ClassNotFoundException, NoSuchFieldException,
            IllegalAccessException, IllegalArgumentException
    {
        // System.out.println("deleteText_internal");
        ((Editable) getmText()).delete(start, end);
    }



    private boolean canPaste() throws ClassNotFoundException,
            NoSuchMethodException, IllegalAccessException,
            IllegalArgumentException, InvocationTargetException
    {
        // System.out.println("canPaste");
        Class<?> textViewClass = Class.forName("android.widget.TextView");
        Method method = textViewClass.getDeclaredMethod("canPaste");
        method.setAccessible(true);
        return (Boolean) method.invoke(this);
    }

    public boolean hasText()
    {
        if (sdkClipboard.hasText())
        {
            System.out.println("hasText()" + true);
            return true;
        }
        else
        {
            System.out.println("hasText()" + false);
            return false;
        }
    }

    private long firTime, secTime;

    private int count = 0;
    
    
    

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        // TODO Auto-generated method stub
        // 防止粘贴键消失当自定义剪切板有内容的时候
        try
        {
            if (MotionEvent.ACTION_DOWN == event.getAction())
            {
                count++;
                if (count == 1)
                {
                    firTime = System.currentTimeMillis();
                }
                else
                {
                    secTime = System.currentTimeMillis();
                    if (secTime - firTime < 1000)
                    {
                        if (hasText() && !canPaste())
                        {
                            // System.out.println("onDoubleClick: dataIsolationEn");
                            cm.setPrimaryClip(ClipData.newPlainText(null, " "));
                        }
                        count = 0;
                    }
                    else
                    {
                        count = 1;
                        firTime = secTime;
                    }
                }

            }

        }
        catch (ClassNotFoundException e)
        {
            // System.out.println("ClassNotFoundException");
        }
        catch (NoSuchMethodException e)
        {
            // System.out.println("NoSuchMethodException");
        }
        catch (IllegalAccessException e)
        {
            // System.out.println("IllegalAccessException");
        }
        catch (IllegalArgumentException e)
        {
            // System.out.println("IllegalArgumentException");
        }
        catch (InvocationTargetException e)
        {
            // System.out.println("InvocationTargetException");
        }

        return super.onTouchEvent(event);
    }

    
    @Override
    public boolean performLongClick()
    {
        // TODO Auto-generated method stub
        try
        {
            if (hasText() && !canPaste())
            {
                // System.out.println("onLoginClick: dataIsolationEn");
                cm.setPrimaryClip(ClipData.newPlainText(null, " "));
            }
        }
        catch (ClassNotFoundException e)
        {
            // System.out.println("ClassNotFoundException");
        }
        catch (NoSuchMethodException e)
        {
            // System.out.println("NoSuchMethodException");
        }
        catch (IllegalAccessException e)
        {
            // System.out.println("IllegalAccessException");
        }
        catch (IllegalArgumentException e)
        {
            // System.out.println("IllegalArgumentException");
        }
        catch (InvocationTargetException e)
        {
            // System.out.println("InvocationTargetException");
        }

        
        
        return super.performLongClick();
    }
    
    
    
}
