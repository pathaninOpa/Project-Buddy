#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "NSArray+Log.h"
#import "NSDictionary+safeInvoke.h"
#import "NSMutableArray+safeInvoke.h"
#import "NSMutableDictionary+safeInvoke.h"
#import "NSObject+safeInvoke.h"
#import "ZIMEventHandler.h"
#import "ZIMMethodHandler.h"
#import "ZIMPluginConverter.h"
#import "ZegoZimPlugin.h"

FOUNDATION_EXPORT double zego_zimVersionNumber;
FOUNDATION_EXPORT const unsigned char zego_zimVersionString[];

