//
//  ReportUtil.h
//  UIKitReport
//
//  Created by tripli on 2024/12/6.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSString * const OS;
FOUNDATION_EXPORT NSString * const OS_VERSION;
FOUNDATION_EXPORT NSString * const PLATFORM;
FOUNDATION_EXPORT NSString * const PLATFORM_VERSION;
FOUNDATION_EXPORT NSString * const UIKIT_VERSION;
FOUNDATION_EXPORT NSString * const KIT_VERSION;
FOUNDATION_EXPORT NSString * const USER_ID;
FOUNDATION_EXPORT NSString * const START_TIME;
FOUNDATION_EXPORT NSString * const ERROR;
FOUNDATION_EXPORT NSString * const MSG;

@interface ReportUtil : NSObject

+ (NSString *)getVersion;

+ (instancetype)sharedInstance;

- (BOOL)createWithAppID:(unsigned int)appID signOrToken:(NSString *)signOrToken commonParams:(NSDictionary *)commonParams;

- (void)updateToken:(NSString *)token;

- (void)updateCommonParams:(NSDictionary *)commonParams;

- (void)reportEvent:(NSString *)event paramsDict:(NSDictionary *)paramsDict;

- (void)destroy;

@end

NS_ASSUME_NONNULL_END
