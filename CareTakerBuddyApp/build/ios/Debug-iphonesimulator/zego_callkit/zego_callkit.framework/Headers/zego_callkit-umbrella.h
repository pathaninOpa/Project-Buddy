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

#import "NSDictionary+zegoCallkitSafeInvoke.h"
#import "NSMutableDictionary+zegoCallkitSafeInvoke.h"
#import "ZegoCallkitUtils.h"
#import "ZegoCallkitConverter.h"
#import "ZegoCallkitEventHandler.h"
#import "ZegoCallkitMethodHandler.h"
#import "ZegoCallkitSettings.h"
#import "ZegoCallkitPlugin.h"

FOUNDATION_EXPORT double zego_callkitVersionNumber;
FOUNDATION_EXPORT const unsigned char zego_callkitVersionString[];

