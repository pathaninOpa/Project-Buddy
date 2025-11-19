//
//  ZegoCallkitMethodHandler.h
//  zego_callkit
//
//  Created by 武耀琳 on 2023/9/27.
//

#import <Foundation/Foundation.h>
#import "CallKit/CallKit.h"
#import "PushKit/PushKit.h"
#import <Flutter/Flutter.h>
NS_ASSUME_NONNULL_BEGIN

@interface ZegoCallkitMethodHandler: NSObject

+(ZegoCallkitMethodHandler *)sharedInstance;

-(void)setInitConfig:(NSDictionary *)configMap;

-(void)enableVoIP;

-(void)isSetEvent:(BOOL)isSetEvent;

@end

NS_ASSUME_NONNULL_END
