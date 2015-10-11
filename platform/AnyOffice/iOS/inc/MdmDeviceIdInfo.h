//
//  DeviceIdInfo.h
//  SvnSdk
//
//  Created by wWX160618 on 14-1-6.
//
//

#import <Foundation/Foundation.h>

@interface MdmDeviceIdInfo : NSObject



/**
 *	获取deviceID
 *
 *	@return	deviceID
 */
+(NSString*) getDeviceID;

//+(bool) VOS_MDM_GetDeviceId:(char**) ppcValue;

@end
