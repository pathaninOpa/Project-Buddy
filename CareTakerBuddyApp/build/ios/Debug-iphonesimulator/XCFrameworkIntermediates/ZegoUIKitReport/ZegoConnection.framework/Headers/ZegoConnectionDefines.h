#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>
#import <ReplayKit/ReplayKit.h>

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
typedef UIView ZGView;
typedef UIImage ZGImage;
#elif TARGET_OS_OSX
#import <AppKit/AppKit.h>
typedef NSView ZGView;
typedef NSImage ZGImage;
#endif

NS_ASSUME_NONNULL_BEGIN

@protocol ZegoConnectionEventHandler;
@protocol ZegoCloudSettingEventHandler;

/// ZegoServerProtocolType.
typedef NS_ENUM(NSUInteger, ZegoServerProtocolType) {
    /// TCP.
    ZegoServerProtocolTypeTcp = 0,
    /// UDP
    ZegoServerProtocolTypeUdp = 1
};

/// ZegoCustomLogLevel.
typedef NS_ENUM(NSUInteger, ZegoCustomLogLevel) {
    /// debug.
    ZegoCustomLogLevelDebug = 0,
    /// info
    ZegoCustomLogLevelInfo = 1,
    /// warn
    ZegoCustomLogLevelWarn = 2,
    /// err
    ZegoCustomLogLevelErr = 3
};

/// ZegoNetAgentHttpMethod.
typedef NS_ENUM(NSUInteger, ZegoNetAgentHttpMethod) {
    /// POST.
    ZegoNetAgentHttpMethodPost = 0,
    /// GET
    ZegoNetAgentHttpMethodGet = 1
};


/// ZegoServerSourceType.
typedef NS_ENUM(NSUInteger, ZegoServerSourceType) {
    /// HARDCODE_IP.
    ZegoServerSourceTypeHardcodeIP = 0,
    /// LOCAL_DNS_RESOLVE
    ZegoServerSourceTypeLocalDnsResolve = 1,
    /// SERVER_RESPOND
    ZegoServerSourceTypeServerResponse = 2
};

/// ZegoNetAgentContentType.
typedef NS_ENUM(NSUInteger, ZegoNetAgentContentType) {
    /// NONE.
    ZegoNetAgentContentTypeNone = 0,
    /// JSON
    ZegoNetAgentContentTypeJson = 1,
    /// OCTET_STREAM
    ZegoNetAgentContentTypeOctetStream = 2
};

/// ZegoDnsStrategy.
typedef NS_ENUM(NSUInteger, ZegoDnsStrategy) {
    /// LOCALDNS_FIRST. //!< 本地 DNS 优先. */
    ZegoDnsStrategyLocalDnsFirst = 0,
    /// ZEGONS_FIRST. //!< ZegoNS 优先. */
    ZegoDnsStrategyZegoNsFirst = 1
};

/// ZegoCustomLogUploadReq
@interface ZegoCustomLogUploadReq : NSObject

/// appID.
@property (nonatomic, assign) unsigned long long appID;

/// appSignature
@property (nonatomic, strong) NSString *appSignature;

/// token
@property (nonatomic, strong) NSString *token;

/// product
@property (nonatomic, strong) NSString *product;

/// idName
@property (nonatomic, strong) NSString *idName;

/// deviceID
@property (nonatomic, strong) NSString *deviceID;

/// url
@property (nonatomic, strong) NSString *url;

/// querykey
@property (nonatomic, strong) NSString *querykey;

/// fileFullPath
@property (nonatomic, strong) NSString *fileFullPath;

/// maxSpeed.
@property (nonatomic, assign) unsigned int maxSpeed;

@end

/// ZegoCustomLogUploadRsp
@interface ZegoCustomLogUploadRsp : NSObject

/// errorCode.
@property (nonatomic, assign) unsigned int errorCode;

/// detailErrorCode.
@property (nonatomic, assign) unsigned int detailErrorCode;

/// message
@property (nonatomic, strong) NSString *message;

@end

/// ZegoNetAgentOptions
@interface ZegoNetAgentOptions : NSObject

/// appID.
@property (nonatomic, assign) unsigned long long appID;

/// appSignature
@property (nonatomic, strong) NSString *appSignature;

/// userID
@property (nonatomic, strong) NSString *userID;

/// deviceID
@property (nonatomic, strong) NSString *deviceID;

@end


/// ZegoNSAddressConfig
@interface ZegoNSAddressConfig : NSObject

/// protocol type.
@property (nonatomic, assign) ZegoServerProtocolType protocolType;

/// server address
@property (nonatomic, strong) NSString *address;

/// address type, such as AF_INET(2)、AF_INET6（23) and so on.
@property (nonatomic, assign) int addressType;

/// server port.
@property (nonatomic, assign) unsigned int port;

@end


/// ZegoNetAgentAddressGroup
@interface ZegoNetAgentAddressGroup : NSObject

/// server groupName
@property (nonatomic, strong) NSString *groupName;

/// server quicNodes
@property (nonatomic, strong) NSString *quicNodes;

/// server mtcpNodes
@property (nonatomic, strong) NSString *mtcpNodes;

@end


/// ZegoNetAgentAddressConfig
@interface ZegoNetAgentAddressConfig : NSObject

/// txID.
@property (nonatomic, assign) unsigned long long txID;

/// rand
@property (nonatomic, strong) NSString *rand;

/// secret
@property (nonatomic, strong) NSString *secret;

/// httpStick
@property (nonatomic, strong) NSString *httpStick;

/// tcpStick
@property (nonatomic, strong) NSString *tcpStick;

/// locationID
@property (nonatomic, strong) NSString *locationID;

/// addressGroup
@property (nonatomic, strong) NSArray<ZegoNetAgentAddressGroup *> *addressGroup;

@end


/// ZegoNetAgentHttpRequest
@interface ZegoNetAgentHttpRequest : NSObject

/// service, such as "zpush"，"switch"，"metaworld" 和 "zegoland"
@property (nonatomic, strong) NSString *service;

/// api
@property (nonatomic, strong) NSString *api;

/// userID
@property (nonatomic, strong) NSString *userID;

/// method.
@property (nonatomic, assign) ZegoNetAgentHttpMethod method;

/// headers. !< 请求头信息，请求头使用等号分隔，多个请求头通过换行符分隔
@property (nonatomic, strong) NSString *headers;

/// contentType
@property (nonatomic, assign) ZegoNetAgentContentType contentType;

/// body. !< 请求包体内容
@property (nonatomic, strong) NSString *body;

/// isNeedHeaderInfo. !< 是否返回请求头信息
@property (nonatomic, assign) BOOL isNeedHeaderInfo;

/// retryCount. !< 重试次数，最大重试次数5
@property (nonatomic, assign) unsigned int retryCount;

@end


/// ZegoNetAgentHttpContext
@interface ZegoNetAgentHttpContext : NSObject

/// 请求序号seq
@property (nonatomic, assign) unsigned int seq;

/// http status code, http状态码
@property (nonatomic, assign) long long statusCode;

/// error code, 错误码
@property (nonatomic, assign) unsigned int errorCode;

/// errorMessage
@property (nonatomic, strong) NSString *errorMessage;

/// headers, 回包头信息
@property (nonatomic, strong) NSString *headers;

/// body, 回包头体信息
@property (nonatomic, strong) NSString *body;

@end

/// ZegoAppInfo
@interface ZegoAppInfo : NSObject

/// appID
@property (nonatomic, assign) unsigned long long appID;

/// businessType
@property (nonatomic, assign) int businessType;

/// mode
@property (nonatomic, assign) int mode;

/// userID
@property (nonatomic, strong) NSString *userID;

/// deviceID
@property (nonatomic, strong) NSString *deviceID;

@end

/// ZegoReporterOptions
@interface ZegoReporterOptions : NSObject

/// appID
@property (nonatomic, assign) unsigned long long appID;

/// appSign
@property (nonatomic, strong) NSString* appSign;

/// deviceID
@property (nonatomic, strong) NSString* deviceID;

/// dbPath
@property (nonatomic, strong) NSString *dbPath;

/// url
@property (nonatomic, strong) NSString *url;

@end

/*
    http reponse
*/
typedef void(^ZegoNetAgentHttpResponseCallback)(ZegoNetAgentHttpContext* rsp);

/// ZegoCloudSettingOptions
@interface ZegoCloudSettingOptions : NSObject

/// appID
@property (nonatomic, assign) unsigned long long appID;

/// appSign
@property (nonatomic, strong) NSString* appSign;

/// product
@property (nonatomic, strong) NSString* product;

/// deviceID
@property (nonatomic, strong) NSString* deviceID;

/// cachPath
@property (nonatomic, strong) NSString *cachPath;

@end

/// ZegoCloudSettingItem
@interface ZegoCloudSettingItem : NSObject

/// code
@property (nonatomic, assign) long code;

/// message
@property (nonatomic, strong) NSString* message;

/// cloudFileName
@property (nonatomic, strong) NSString* cloudFileName;

/// json
@property (nonatomic, strong) NSString* json;

/// version
@property (nonatomic, strong) NSString* version;

@end

/// ZegoCloudSettingInfo
@interface ZegoCloudSettingInfo : NSObject

/// code
@property (nonatomic, assign) long code;

/// message
@property (nonatomic, strong) NSString* message;

/// items
@property (nonatomic, strong) NSArray<ZegoCloudSettingItem *> *items;

@end

NS_ASSUME_NONNULL_END