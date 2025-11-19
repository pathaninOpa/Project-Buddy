//
//  ZPNsEventManager.h
//  zego_zpns
//
//  Created by 武耀琳 on 2023/3/6.
//

#import <Foundation/Foundation.h>
#import <Flutter/Flutter.h>
NS_ASSUME_NONNULL_BEGIN

@interface ZPNsEventManager : NSObject

+ (instancetype)sharedInstance;

- (void)ZPNsNotificationCenter:(UNUserNotificationCenter *)center
       willPresentNotification:(UNNotification *)notification
                      userInfo:(NSDictionary *)userInfo
         withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler;

- (void)ZPNsNotificationCenter:(UNUserNotificationCenter *)center
didReceiveNotificationResponse:(UNNotificationResponse *)response
                      userInfo:(NSDictionary *)userInfo
         withCompletionHandler:(void (^)(void))completionHandler;

- (void)ZPNsRegisterFailed:(NSError *)error;

- (void)ZPNsDidReceiveNotificationFromForeground:(NSDictionary *)userInfo;

@end

NS_ASSUME_NONNULL_END
