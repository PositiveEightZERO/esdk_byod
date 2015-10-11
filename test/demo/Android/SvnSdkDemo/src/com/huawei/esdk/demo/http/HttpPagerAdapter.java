/**
 * 
 */
package com.huawei.esdk.demo.http;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

/**
 * @author cWX223941
 *
 */
public class HttpPagerAdapter extends FragmentPagerAdapter
{
    //private ArrayList<HttpInfoEntity> showList;

    //    private ArrayList<BaseFragment> fragmentCache;
    public HttpPagerAdapter(FragmentManager fm)
    {
        super(fm);
        //this.showList = showList;
        //        fragmentCache = new ArrayList<BaseFragment>();
    }

    @Override
    public Fragment getItem(int position)
    {
        //        Fragment ft = new MainPageChildFragment();
        //        Bundle args = new Bundle();
        //        args.putSerializable(Constants.FRAGMENT_BUNDLE_MAINPAGE, firstCategory.get(position));
        //        ft.setArguments(args);
        Fragment ft = null;
        switch (position)
        {
            case 0:
                ft = new HttpLoginFragment();
                break;
            case 1:
                ft = new UserInfoFragment();
                break;
            case 2:
                ft = new DownloadFragment();
                break;
            case 3:
                ft = new UploadFragment();
                break;
            default:
                break;
        }
        //        fragmentCache.add(ft);
        return ft;
    }

    @Override
    public int getCount()
    {
        return 4;
    }

}
