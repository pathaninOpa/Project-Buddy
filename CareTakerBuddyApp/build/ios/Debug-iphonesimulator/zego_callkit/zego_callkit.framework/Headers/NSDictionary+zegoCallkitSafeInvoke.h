//
//  NSDictionary+safeInvoke.h
//  zego_callkit
//
//  Created by 武耀琳 on 2023/9/26.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSDictionary (callkitSafeInvoke)

-(nullable id)safeObjectForKey:(nonnull NSString *)key;

@end

NS_ASSUME_NONNULL_END
