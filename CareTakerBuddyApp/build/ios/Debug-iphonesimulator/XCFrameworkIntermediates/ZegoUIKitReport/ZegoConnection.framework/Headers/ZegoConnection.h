//
//  ZegoConnection.h
//  ZegoConnection
//
//  Copyright © 2023 Zego. All rights reserved.
//
#import "ZegoConnectionDefines.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoConnection : NSObject

+(ZegoConnection*)createConnection;

+(void)destroyConnection;

/*
 base module
*/
+(void)setLogFile:(NSString*)filePath;

+(BOOL)setCustomLogFile:(NSString*)filePath fileName:(NSString*)fileName headInfo:(NSString*)headInfo;

-(void)writeCustomLog:(NSString*)content level:(ZegoCustomLogLevel)level;

-(void)uploadCustomLog:(ZegoCustomLogUploadReq*)req;

-(BOOL)setEventHandler:(nullable id<ZegoConnectionEventHandler>)handler;

/*
 net http module
*/
-(void)netHttpInit;

-(void)netHttpUninit;

/*
 net agent module
*/

-(void)setZegoNsAddressInfo:(NSArray<ZegoNSAddressConfig*>*)config configType:(ZegoServerSourceType)configType;

-(void)setNetAgentAddressInfo:(ZegoNetAgentAddressConfig*)config;

-(void)setNetAgentBackupIps:(NSString*)ips;

-(void)setNetAgentDefaultDomain:(NSString*)domain;

-(void)setNetAgentUserID:(NSString*)userID;

-(void)netAgentInit:(ZegoNetAgentOptions*)option;

-(unsigned int)netAgentConnect:(NSString*)services timeOut:(unsigned int)timeOut;

-(void)netAgentStartHttpRequest:(ZegoNetAgentHttpRequest*)req rsp:(nullable ZegoNetAgentHttpResponseCallback)rsp;

-(void)netAgentClose:(unsigned int)conID;

-(void)netAgentUninit;

@end
NS_ASSUME_NONNULL_END