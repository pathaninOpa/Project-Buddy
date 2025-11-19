//
//  ZegoCallkitUtils.h
//  zego_callkit
//
//  Created by 武耀琳 on 2023/9/26.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZegoCallkitUtils : NSObject

+(ZegoCallkitUtils *)sharedInstance;

-(bool)wirteCustomLog:(NSString *)customLog;

-(BOOL)isNotNull:(NSObject *)object;

-(NSString *)getHexStringForData:(NSData *)data;

-(NSString *)generateSeqStr;

@end

NS_ASSUME_NONNULL_END
