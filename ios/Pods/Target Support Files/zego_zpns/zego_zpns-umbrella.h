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

#import "NSDictionary+safeInvoke.h"
#import "NSMutableDictionary+safeInvoke.h"
#import "ZPNsAppEnvironmentUtil.h"
#import "ZPNsUtils.h"
#import "ZPNsConverter.h"
#import "ZPNsEventManager.h"
#import "ZPNsMethodManager.h"
#import "ZPNsSettings.h"
#import "ZegoZpnsPlugin.h"

FOUNDATION_EXPORT double zego_zpnsVersionNumber;
FOUNDATION_EXPORT const unsigned char zego_zpnsVersionString[];

