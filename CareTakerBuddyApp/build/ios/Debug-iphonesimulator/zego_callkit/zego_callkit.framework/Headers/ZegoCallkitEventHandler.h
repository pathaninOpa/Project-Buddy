//
//  ZegoCallkitEventHandler.h
//  zego_callkit
//
//  Created by 武耀琳 on 2023/9/28.
//

#import <Foundation/Foundation.h>
#import "CallKit/CallKit.h"
#import "PushKit/PushKit.h"
NS_ASSUME_NONNULL_BEGIN

@interface ZegoCallkitEventHandler : NSObject<CXProviderDelegate,PKPushRegistryDelegate>

+(ZegoCallkitEventHandler *)sharedInstance;

@end

NS_ASSUME_NONNULL_END
