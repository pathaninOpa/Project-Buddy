//
//  ZPNsUtils.h
//  zego_zpns
//
//  Created by 武耀琳 on 2023/6/29.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZPNsUtils : NSObject

+(bool)wirteCustomLog:(NSString *)customLog;

+ (NSString *)getHexStringForData:(NSData *)data;


+ (BOOL)isNotNull:(NSObject *)object;

+ (BOOL)isSandboxApp;
@end

NS_ASSUME_NONNULL_END
