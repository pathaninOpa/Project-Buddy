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

#import "KitRemoteView.h"
#import "PipManager.h"
#import "ZegoUikitPlugin.h"

FOUNDATION_EXPORT double zego_uikitVersionNumber;
FOUNDATION_EXPORT const unsigned char zego_uikitVersionString[];

