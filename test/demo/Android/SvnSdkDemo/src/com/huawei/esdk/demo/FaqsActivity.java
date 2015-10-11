/**
 * 
 */
package com.huawei.esdk.demo;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

import com.huawei.esdk.demo.common.BaseActivity;
import com.huawei.esdk.demo.common.Constants;
import com.huawei.esdk.demo.utils.StringUtil;

/**
 * @author cWX223941
 *
 */
public class FaqsActivity extends BaseActivity
{
    private static final String TAG = "FaqsActivity";
    private TextView btnBack;
    private ListView faqListView;
    
    private String faqType = "faqs_tunnel";
    private List<FaqsItem> faqsItems = new ArrayList<FaqsItem>();
   
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_faq_detail);
        
        String type =  (String) this.getIntent().getStringExtra(
                Constants.ACTIVITY_SEND_FAQ);
        if(type != null)
        {
            faqType = type;
        }
        
        init();
    }

    private void init()
    {
        initView();
        initData();
    }

    private void initView()
    {
        btnBack = (TextView) findViewById(R.id.btn_faq_detail_back);
        faqListView = (ListView) findViewById(R.id.lv_faqs);
        btnBack.setOnClickListener(onClickListener);
        
        faqListView.setAdapter(new FaqListAdapter());
    }

    private void initData()
    {
        String filePath = getFilePathByType(faqType);
        
        
        StringBuilder sb = new StringBuilder();
        InputStream inputStream = null;
        InputStreamReader inputStreamReader = null;
        try
        {
            inputStream = getAssets().open(filePath);
            inputStreamReader = new InputStreamReader(inputStream);
     
            BufferedReader reader = new BufferedReader(inputStreamReader);
            String line;
        
            while ((line = reader.readLine()) != null)
            {
                if(line.length()> 0)
                {
                    sb.append(line).append("\r\n");
                }
               
                //Log.d(TAG, "add content:*" + line + "*");
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

        
        Pattern pattern = Pattern.compile("\\[===[Q|A]===\\]"); 
        String[] items = pattern.split(sb.toString().trim(), -1); 
        
        for (int i=0; i<items.length -1;)
        {
            if(items[i].length() == 0)
            {
                i+=1;
                continue;
            }
            String question = items[i];
            String answer = items[i+1];
            if(question != null && answer != null)
            {
                faqsItems.add(new FaqsItem(question.trim(), answer.trim()));
            }
            
            i+=2;
        }
      
    }
    
    
    
    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            if (v.getId() == btnBack.getId())
            {
                FaqsActivity.this.finish();
            }
        }
    };

    private String getFilePathByType(String faqType)
    {
        String filePath = "";
        if (!StringUtil.isEmpty(faqType))
        {
            filePath = "faq/" + faqType + ".txt";
        }
        return filePath;
    }
    
    
    private class FaqsItem
    {
        private String question;
        private String answer;

        public FaqsItem(String question, String answer)
        {
            super();
            this.question = question;
            this.answer = answer;
        }

        public String getQuestion()
        {
            return question;
        }

        public void setQuestion(String question)
        {
            this.question = question;
        }

        public String getAnswer()
        {
            return answer;
        }

        public void setAnswer(String answer)
        {
            this.answer = answer;
        }
    }
    private class FaqListItemHolder
    {
        private TextView questionView;
        private TextView answerView;

        public TextView getQuestionView()
        {
            return questionView;
        }

        public void setQuestionView(TextView questionView)
        {
            this.questionView = questionView;
        }

        public TextView getAnswerView()
        {
            return answerView;
        }

        public void setAnswerView(TextView answerView)
        {
            this.answerView = answerView;
        }
    }
    private class FaqListAdapter extends BaseAdapter
    {
        @Override
        public int getCount()
        {
            // TODO Auto-generated method stub
            return faqsItems.size();
        }

        @Override
        public Object getItem(int pos)
        {
            // TODO Auto-generated method stub
            return faqsItems.get(pos);
        }

        @Override
        public long getItemId(int pos)
        {
            // TODO Auto-generated method stub
            return pos;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent)
        {
            FaqListItemHolder holder = null;
            if (convertView == null)
            {
                convertView = LayoutInflater.from(FaqsActivity.this)
                        .inflate(R.layout.faqs_list_item, null);
                holder = new FaqListItemHolder();
                holder.setQuestionView((TextView) convertView
                        .findViewById(R.id.tv_faq_question));
                holder.setAnswerView((TextView) convertView
                        .findViewById(R.id.tv_faq_answer));
                convertView.setTag(holder);
            }
            else
            {
                holder = (FaqListItemHolder) convertView.getTag();
            }
            FaqsItem item = faqsItems.get(position);
            holder.getQuestionView().setText(item.getQuestion());
            holder.getAnswerView().setText(item.getAnswer());
            return convertView;
        }
    }

}
