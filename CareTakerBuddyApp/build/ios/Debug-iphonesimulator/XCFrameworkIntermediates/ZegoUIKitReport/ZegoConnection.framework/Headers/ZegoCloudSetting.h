//
//  ZegoCloudSetting.h
//  ZegoCloudSetting
//
//
//  Note: this module depends on netAgent, so you must create connection module and initialize the netAgent module before using this module
//
//  Copyright © 2023 Zego. All rights reserved.
//
#import "ZegoConnectionDefines.h"

@interface ZegoCloudSetting : NSObject

+(ZegoCloudSetting*)createCloudSetting:(ZegoCloudSettingOptions*)options eventHandler:(id<ZegoCloudSettingEventHandler>)eventHandler;

+(void)destroyCloudSetting;

/*
*   设置用户ID
*/
-(void)setUserID:(NSString*)userID;

/*
*   使用token鉴权时需要传入
*/
-(void)setToken:(NSString*)token;

/*
*   请求云控时自定义的参数，为键值对key-value, 比如： setCustomParam("dev_info", "android")传入设备信息
*/
-(void)setCustomParam:(NSString*)key value:(NSString*)value;

/*
*   拉取云控配置，具体回包数据请关注 ZegoCloudSettingEventHandler
*/
-(void)fetchCloudSettingConfig;

/*
*   获取云控配置缓存信息
*/
-(NSDictionary<NSString*, NSString*>*)getCloudSettingCacheConfig;
@end