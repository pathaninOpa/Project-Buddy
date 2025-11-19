//
//  ZegoCallkitSettings.h
//  zego_callkit
//
//  Created by 武耀琳 on 2023/9/26.
//

#import <Foundation/Foundation.h>
#import <Flutter/Flutter.h>
#import <PushKit/PushKit.h>
#import <Callkit/Callkit.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZegoCallkitSettings : NSObject

+(ZegoCallkitSettings *)sharedInstance;

@property (nonatomic,strong,nullable)FlutterEventSink event;

@property (nonatomic,strong)PKPushRegistry *pkPushRegistry;

@property (nonatomic,strong)CXProviderConfiguration *cxProviderConfiguration;

@property (nonatomic,assign)BOOL enableVoIP;

@property (nonatomic,strong)NSData *voIPToken;

@property (nonatomic,assign)BOOL isProduct;

@property (nonatomic,strong)CXProvider *cxProvider;

@property (nonatomic,strong)CXCallController *cxCallController;

@property (nonatomic,strong)NSMutableDictionary<NSString *,CXAction *> *actionMap;

-(void)addResultDictionaryToHoldNomalResultArray:(NSDictionary *)result;

-(void)clearHoldNomalResultArray;

@end

NS_ASSUME_NONNULL_END
