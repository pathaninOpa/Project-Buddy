//
//  ZegoCallkitConverter.h
//  zego_callkit
//
//  Created by 武耀琳 on 2023/9/26.
//

#import <Foundation/Foundation.h>
#import <CallKit/Callkit.h>


NS_ASSUME_NONNULL_BEGIN

@interface ZegoCallkitConverter : NSObject

+(ZegoCallkitConverter *)sharedInstance;

-(CXCallUpdate *) oCXCallUpdate:(NSDictionary *)updateMap;

-(nullable NSDictionary *)mCXHandle:(CXHandle *)handle;

-(nullable NSDictionary *) mCXAction:(CXAction *)action;

-(nullable NSDictionary *) mCXCallAction:(CXCallAction *)callAction;

-(nullable NSDictionary *) mCXStartCallAction:(CXStartCallAction *)action;

-(nullable NSDictionary *)mCXAnswerCallAction:(CXCallAction *)action;

-(nullable NSDictionary *)mCXEndCallAction:(CXEndCallAction *)action;

-(nullable NSDictionary *)mCXSetHeldCallAction:(CXSetHeldCallAction *)action;

-(nullable NSDictionary *)mCXSetMutedCallAction:(CXSetMutedCallAction *)action;

-(nullable NSDictionary *)mCXSetGroupCallAction:(CXSetGroupCallAction *)action;

-(nullable NSDictionary *)mCXPlayDTMFCallAction:(CXPlayDTMFCallAction *)action;

@end

NS_ASSUME_NONNULL_END
