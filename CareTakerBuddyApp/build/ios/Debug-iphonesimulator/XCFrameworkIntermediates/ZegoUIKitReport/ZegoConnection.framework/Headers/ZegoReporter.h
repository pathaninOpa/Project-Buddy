//
//  ZegoReporter.h
//  ZegoReporter
//
//
//  Note: this module depends on http, so you must create connection module and initialize the http module before using this module
//
//  Copyright © 2023 Zego. All rights reserved.
//
#import "ZegoConnectionDefines.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoReporter : NSObject

+(ZegoReporter*)createReporter:(NSString*)productName options:(ZegoReporterOptions*)options;

+(void)destroyReporter;

-(void)setToken:(NSString*)token;

-(void)setUrl:(NSString*)url;

-(void)setPackageSizeByte:(unsigned int)size;

-(void)setPackageItemCount:(unsigned int)count;

-(void)setReportInterval:(unsigned int)interval;

-(void)setRetryInterval:(unsigned int)interval;

-(void)setWaitNtpTimeout:(unsigned int)timeout;

-(void)setNtpOffset:(int)offset;

-(void)startReport:(NSString*)userID process:(NSString*)process;

-(void)report:(NSString*)eventName content:(NSString*)content level:(int)level instant:(BOOL)instant;

-(void)instantReport;

@end
NS_ASSUME_NONNULL_END