#ifndef __SVN_IOS_API_H
#define __SVN_IOS_API_H

/* BEGIN: Modified by zhaixianqi 90006553, 2013/12/18   问题单号:同步版本号与AnyOffice相同*/
/*****************************************************************************
 函 数 名  : sdkview_getversion
 功能描述  : 获取SDK中iOS的sdkview模块版本号
 输入参数  : 
 输出参数  : 无
 返 回 值  : const char*
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月10日
    作    者   : z90006553
    修改内容   : 新生成函数

*****************************************************************************/
const char* sdkview_getversion();
/* END:   Modified by zhaixianqi 90006553, 2013/12/18 */

/* BEGIN: Modified by zhaixianqi 90006553, 2014/02/28   问题单号:DTS2014022401305 */
unsigned long sdkview_cleanTempFile();
/* END:   Modified by zhaixianqi 90006553, 2014/02/28 */

#endif