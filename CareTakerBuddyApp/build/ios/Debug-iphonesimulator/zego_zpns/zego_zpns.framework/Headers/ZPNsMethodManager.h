//
//  ZPNsMethodManager.h
//  zego_zpns
//
//  Created by 武耀琳 on 2023/3/6.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZPNsMethodManager : NSObject

+ (instancetype)sharedInstance;

@property bool debug;

@end

NS_ASSUME_NONNULL_END
