#import "ZegoConnectionDefines.h"

NS_ASSUME_NONNULL_BEGIN
#pragma mark - Zego Connection Event Handler

@protocol ZegoConnectionEventHandler <NSObject>

@optional

/// The callback for net type change.
/// @param type net type
- (void)onNetTypeChanged:(unsigned int)type;

/// The callback for upload log.
/// @param type net type
- (void)onCustomLogUploadResult:(ZegoCustomLogUploadRsp*)rsp;

/// The callback for init netAgent.
/// @param errorCode
- (void)onNetAgentInited:(unsigned int)errorCode;

/// The callback for Ns Address Info.
/// @param appInfo
/// @param dnsStrategy
/// @param addressInfo
- (void)onSaveNsAddressInfo:(ZegoAppInfo*)appInfo dnsStrategy:(ZegoDnsStrategy)dnsStrategy addressInfo:(NSArray<ZegoNSAddressConfig *> *)addressInfo;

/// The callback for NetAgent Address Info.
/// @param appInfo
/// @param addressInfo
- (void)onSaveNetAgentAddressInfo:(ZegoAppInfo*)appInfo addressInfo:(ZegoNetAgentAddressConfig*)addressInfo;

/// The callback NetAgent connected.
/// @param conID
/// @param code
- (void)onNetAgentConnected:(unsigned int)conID code:(unsigned int)code;

/// The callback for NetAgent receive.
/// @param conID
/// @param data
- (void)onNetAgentReceived:(unsigned int)conID data:(NSData* )data;

/// The callback for NetAgent close.
/// @param conID
/// @param code
- (void)onNetAgentClosed:(unsigned int)conID code:(unsigned int)code;

@end

@protocol ZegoCloudSettingEventHandler <NSObject>

@optional

/// The callback for Cloud Setting Result.
/// @param changedInfo
/// @param allInfo
- (void)onCloudSettingResult:(ZegoCloudSettingInfo*)changedInfo allInfo:(ZegoCloudSettingInfo*)allInfo;

@end

NS_ASSUME_NONNULL_END