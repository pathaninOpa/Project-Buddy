//
//  ZIMDefines.h
//  ZIM
//
//  Copyright © 2021 Zego. All rights reserved.
//

#import "ZIMErrorCode.h"
#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#elif TARGET_OS_OSX
#import <AppKit/AppKit.h>
#endif

NS_ASSUME_NONNULL_BEGIN

// MARK: - Enum

/// Connection state.
///
/// Description: The state machine that identifies the current connection state.
///
/// Use cases: It can be used to determine whether the login/logout is successful, and to handle abnormal situations such as network disconnection.
///
/// Caution: Please use it with the connection event parameter.
///
typedef NS_ENUM(NSUInteger, ZIMConnectionState) {

    /// Description: Unconnected state, enter this state before logging in and after logging out.
    ///
    /// Use cases: If there is a steady state abnormality in the process of logging in, such as AppID or Token are incorrect, or if the same user name is logged in elsewhere and the local end is kicked out, it will enter this state.
    ZIMConnectionStateDisconnected = 0,

    /// Description: The state that the connection is being requested. It will enter this state after successful execution login function.
    ///
    /// Use cases: The display of the UI is usually performed using this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to this state.
    ZIMConnectionStateConnecting = 1,

    /// Description: The state that is successfully connected.
    ///
    /// Use cases: Entering this state indicates that login successfully and the user can use the SDK functions normally.
    ZIMConnectionStateConnected = 2,

    /**
     * Description: The state that the reconnection is being requested. It will enter this state after successful execution login function. 
     *
     * Use cases: The display of the UI is usually performed using this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to this state.
     */
    ZIMConnectionStateReconnecting = 3
};

// MARK: - Enum

/// Connection state.
///
/// Description: The state machine that identifies the current connection state.
///
/// Use cases: It can be used to judge whether the user enters/exit the room successfully, and handles abnormal situations such as network disconnection.
///
/// Caution: Please use it with the connection event parameter.
///
typedef NS_ENUM(NSUInteger, ZIMRoomState) {

    /// Description: Disconnected state.
    ///
    /// Use cases: enter this state before entering the room and after exiting the room.
    ZIMRoomStateDisconnected = 0,

    /// Description: The connection state is being requested.
    ///
    /// Use cases: and it will enter this state after the action of entering the room is executed successfully. The application interface is usually displayed through this state.
    ZIMRoomStateConnecting = 1,

    /// Description: The connection is successful.
    ///
    /// Use cases: Entering this state means that the room has been successfully entered, and the user can use the room's functions normally.
    ZIMRoomStateConnected = 2

};

/// The event that caused the connection status to change.
///
/// Description: The reason for the change of the connection state.
///
/// Use cases: It can be used to determine whether the login/logout is successful, and to handle abnormal situations such as network disconnection.
///
/// Caution: Please use it with the connection state parameter.
///
typedef NS_ENUM(NSUInteger, ZIMConnectionEvent) {

    /// Description: Success.
    ZIMConnectionEventSuccess = 0,

    /// Description: The user actively logs in.
    ZIMConnectionEventActiveLogin = 1,

    /// Description: Connection timed out.
    ZIMConnectionEventLoginTimeout = 2,

    /// Description: The network connection is temporarily interrupted.
    ZIMConnectionEventLoginInterrupted = 3,

    /// Description: Being kicked out.
    ZIMConnectionEventKickedOut = 4,

    /// Being token expierd.
    ZIMConnectionEventTokenExpired = 5,

    /// Being token expierd.
    ZIMConnectionEventUnregistered = 6

};

/// The event that caused the room connection status to change.
///
/// Description: The reason for the change of the connection state.
///
/// Use cases: It can be used to determine whether the login/logout is successful, and to handle abnormal situations such as network disconnection.
///
/// Caution: Please use it with the connection state parameter.
///
typedef NS_ENUM(NSUInteger, ZIMRoomEvent) {

    /// Description: Success.
    ZIMRoomEventSuccess = 0,

    /// Description: The network in the room is temporarily interrupted.
    ZIMRoomEventNetworkInterrupted = 1,

    /// Description: The network in the room is disconnected.
    ZIMRoomEventNetworkDisconnected = 2,

    /// Description: The room not exist.
    ZIMRoomEventRoomNotExist = 3,

    /// Description: The user actively creates a room.
    ZIMRoomEventActiveCreate = 4,

    /// Description: Failed to create room.
    ZIMRoomEventCreateFailed = 5,

    /// Description: The user starts to enter the room.
    ZIMRoomEventActiveEnter = 6,

    /// Description: user failed to enter the room.
    ZIMRoomEventEnterFailed = 7,

    /// Description: user was kicked out of the room.
    ZIMRoomEventKickedOut = 8,

    /// Description: user was out of the room because of connect timeout
    ZIMRoomEventConnectTimeout = 9,

    ZIMRoomEventKickedOutByOtherDevice = 10,

    ZIMRoomEventActiveSwitch = 11,

    ZIMRoomEventSwitchFailed = 12
};

/// The priority of the message.
///
/// Description: Identifies the priority of a message.
///
/// Use cases: It can be used to set the priority when a message is sent. The higher the priority, the higher the reliability. Low priority may be discarded due to weak network.
///
/// Caution: The higher the priority, the higher the cost.
///
typedef NS_ENUM(NSUInteger, ZIMMessagePriority) {

    /// Description: Low priority.
    ///
    /// Use cases: Generally used to send unimportant messages such as barrage message in a room.
    ZIMMessagePriorityLow = 1,

    /// Description: Medium priority.
    ///
    /// Use cases: Generally used to send regular chat messages.
    ZIMMessagePriorityMedium = 2,

    /// Description: High priority.
    ///
    /// Use cases: Generally used to send important information such as gifts and rewards in a room.
    ZIMMessagePriorityHigh = 3

};

/// The type of the message.
///
/// Description: Identifies the type of current message.
///
/// Use cases: It can be used to determine what type of message this message is.
///
typedef NS_ENUM(NSUInteger, ZIMMessageType) {
    /// Description: Unknown message.
    ///
    /// Use cases: A message of an unknown type is received, indicating that the sender may have sent a message type that the user does not support, and the user needs to be advised to update the version.
    ZIMMessageTypeUnknown = 0,
    /// Description: Normal text message.
    ///
    /// Use cases: Can be used to deliver ordinary text messages.
    ZIMMessageTypeText = 1,
    /// Description: Custom binary message.
    ///
    /// Use cases: Can be used to transfer custom binary messages. This message type does not support offline messages and local storage.
    ZIMMessageTypeCommand = 2,

    ZIMMessageTypeMultiple = 10,
    /// Description: Image message.
    ///
    /// Use cases: Can be used to send image messages, only ".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tiff" image types are supported. After sending the image, the server will generate a large image and a thumbnail of the original image.
    ZIMMessageTypeImage = 11,
    /// Description: File message.
    ///
    /// Use cases: For sending file messages, no file type restrictions.
    ZIMMessageTypeFile = 12,
    /// Description: Audio message.
    ///
    /// Use cases: For sending audio messages, only ".mp3" audio type is supported.
    ZIMMessageTypeAudio = 13,
    /// Description: Video message.
    ///
    /// Use cases: For sending video messages, only ".mp4", ".mov" video types are supported. After sending the video message, the server will generate the first frame of the video file.
    ZIMMessageTypeVideo = 14,
    /// Description: Barrage message.
    ///
    /// Use cases: Can be used for the barrage sent by the live room. This message type does not support offline messages and local storage.
    ZIMMessageTypeBarrage = 20,

    ZIMMessageTypeSystem = 30,

    ZIMMessageTypeRevoke = 31,

    ZIMMessageTypeTips = 32,

    ZIMMessageTypeCombine = 100,

    ZIMMessageTypeCustom = 200,
};

typedef NS_ENUM(NSUInteger, ZIMTipsMessageEvent) {
    ZIMTipsMessageEventGroupCreated = 1,

    ZIMTipsMessageEventGroupDismissed = 2,

    ZIMTipsMessageEventGroupJoined = 3,

    ZIMTipsMessageEventGroupInvited = 4,

    ZIMTipsMessageEventGroupLeft = 5,

    ZIMTipsMessageEventGroupKickedOut = 6,

    ZIMTipsMessageEventGroupInfoChanged = 7,

    ZIMTipsMessageEventGroupMemberInfoChanged = 8,
};

typedef NS_ENUM(NSUInteger, ZIMTipsMessageChangeInfoType) {
    ZIMTipsMessageChangeInfoTypeGroupDataChanged = 1,
    ZIMTipsMessageChangeInfoTypeGroupNoticeChanged = 2,
    ZIMTipsMessageChangeInfoTypeGroupNameChanged = 3,
    ZIMTipsMessageChangeInfoTypeGroupAvatarUrlChanged = 4,
    ZIMTipsMessageChangeInfoTypeGroupMuteChanged = 5,

    ZIMTipsMessageChangeInfoTypeGroupOwnerTransferred = 10,
    ZIMTipsMessageChangeInfoTypeGroupMemberRoleChanged = 11,
    ZIMTipsMessageChangeInfoTypeGroupMemberMuteChanged = 12,
};

typedef NS_ENUM(NSInteger, ZIMRevokeType) {

    ZIMRevokeTypeUnknown = -1,

    ZIMRevokeTypeTwoWay = 0,

    ZIMRevokeTypeOneWay = 1,

};

typedef NS_ENUM(NSInteger, ZIMBlacklistChangeAction) {

    ZIMBlacklistChangeActionAdded = 0,

    ZIMBlacklistChangeActionRemoved = 1,

};

typedef NS_ENUM(NSInteger, ZIMMessageRevokeStatus) {

    ZIMMessageRevokeStatusUnknown = -1,

    ZIMMessageRevokeStatusSelfRevoke = 0,

    ZIMMessageRevokeStatusSystemRevoke = 1,

    ZIMMessageRevokeStatusServerApiRevoke = 2,

    ZIMMessageRevokeStatusGroupAdminRevoke = 3,

    ZIMMessageRevokeStatusGroupOwnerRevoke = 4,

    ZIMMessageRevokeStatusAuditRejectRevoke = 5,

};

/// he types of media files that are allowed to be downloaded include original images, large images, thumbnails, general files, audio files, video files and their first frame images.
typedef NS_ENUM(NSUInteger, ZIMMediaFileType) {
    /// Original file type, suitable for original images, audio files, and video files. After calling [downloadMediaFile], the SDK will update the fileLocalPath property in [ZIMMediaMessage].
    ZIMMediaFileTypeOriginalFile = 1,
    /// Large image type. After calling [downloadMediaFile], the SDK will update the largeImageLocalPath property in [ZIMImageMessage].
    ZIMMediaFileTypeLargeImage = 2,
    /// Image thumbnail type. After calling [downloadMediaFile], the SDK will update the thumbnailLocalPath property in [ZIMImageMessage].
    ZIMMediaFileTypeThumbnail = 3,
    /// The type of the first frame of the video. After calling [downloadMediaFile], the SDK will update the videoFirstFrameLocalPath property in [ZIMVideoMessage].
    ZIMMediaFileTypeVideoFirstFrame = 4
};

typedef NS_ENUM(NSUInteger, ZIMFriendListChangeAction) {
    ZIMFriendListChangeActionAdded = 0,
    ZIMFriendListChangeActionDeleted = 1,
};

typedef NS_ENUM(NSUInteger, ZIMFriendApplicationListChangeAction) {
    ZIMFriendApplicationListChangeActionAdded = 0,
    ZIMFriendApplicationListChangeActionDeleted = 1,
};

typedef NS_ENUM(NSUInteger, ZIMMessageReceiptStatus) {
    ZIMMessageReceiptStatusNone = 0,
    ZIMMessageReceiptStatusProcessing = 1,
    ZIMMessageReceiptStatusDone = 2,
    ZIMMessageReceiptStatusExpired = 3,
    ZIMMessageReceiptStatusFailed = 4,
};

/// Room attributes update action.
///
/// Description: Room attributes update action.
typedef NS_ENUM(NSUInteger, ZIMRoomAttributesUpdateAction) {
    /// Set action.
    ZIMRoomAttributesUpdateActionSet = 0,
    /// Delete action.
    ZIMRoomAttributesUpdateActionDelete = 1

};

/// Description: Room attributes update action.
typedef NS_ENUM(NSUInteger, ZIMRoomMemberAttributesUpdateAction) {
    /// Set action.
    ZIMRoomMemberAttributesUpdateActionSet = 0

};

/// the direction of the message.
///
/// Description: Describes whether the current message was sent or received.
typedef NS_ENUM(NSUInteger, ZIMMessageDirection) {
    /// Message has been sent.
    ZIMMessageDirectionSend = 0,
    /// Message accepted.
    ZIMMessageDirectionReceive = 1
};

/// The status of the message being sent.
///
/// Description: Describes the condition of the currently sent message.
typedef NS_ENUM(NSUInteger, ZIMMessageSentStatus) {
    /// The message is being sent.
    ZIMMessageSentStatusSending = 0,
    /// Message sent successfully.
    ZIMMessageSentStatusSendSuccess = 1,
    /// Message sending failed.
    ZIMMessageSentStatusSendFailed = 2

};

typedef NS_ENUM(NSUInteger, ZIMMessageOrder) {
    // DESC
    ZIMMessageOrderDescending = 0,
    // ASC
    ZIMMessageOrderAscending = 1
};

typedef NS_ENUM(NSUInteger, ZIMMessageMentionedType) {
    ZIMMessageMentionedTypeMentionMe = 1,
    ZIMMessageMentionedTypeMentionAll = 2,
    ZIMMessageMentionedTypeMentionAllAndMe = 3
};

typedef NS_ENUM(NSUInteger, ZIMMessageDeleteType) {

    ZIMMessageDeleteTypeMessageListDeleted = 0,

    ZIMMessageDeleteTypeConversationAllMessagesDeleted = 1,

    ZIMMessageDeleteTypeAllConversationMessagesDeleted = 2
};

typedef NS_ENUM(NSUInteger, ZIMMessageRepliedInfoState) {
    ZIMMessageRepliedInfoStateNormal = 0,
    ZIMMessageRepliedInfoStateDeleted = 1,
    ZIMMessageRepliedInfoStateNotFound = 2
};

/// conversation type.
typedef NS_ENUM(NSInteger, ZIMConversationType) {

    ZIMConversationTypeUnknown = -1,

    ZIMConversationTypePeer = 0,

    ZIMConversationTypeRoom = 1,

    ZIMConversationTypeGroup = 2

};

typedef NS_ENUM(NSUInteger, ZIMConversationEvent) {

    ZIMConversationEventAdded = 0,

    ZIMConversationEventUpdated = 1,

    ZIMConversationEventDisabled = 2,

    ZIMConversationEventDeleted = 3,

};

typedef NS_ENUM(NSUInteger, ZIMConversationSyncState) {

    ZIMConversationSyncStateStarted = 0,

    ZIMConversationSyncStateFinished = 1,

    ZIMConversationSyncStateFailed = 2,

};

typedef NS_ENUM(NSUInteger, ZIMConversationNotificationStatus) {

    ZIMConversationNotificationStatusNotify = 1,

    ZIMConversationNotificationStatusDoNotDisturb = 2

};

/// Description: Group events.
typedef NS_ENUM(NSUInteger, ZIMGroupEvent) {
    /// Create groups.
    ZIMGroupEventCreated = 1,
    /// Disband the group.
    ZIMGroupEventDismissed = 2,
    /// Join the group.
    ZIMGroupEventJoined = 3,
    /// Invite to the group.
    ZIMGroupEventInvited = 4,
    /// Leave the group.
    ZIMGroupEventLeft = 5,
    /// Kick out of the group.
    ZIMGroupEventKickedout = 6

};

typedef NS_ENUM(NSUInteger, ZIMGroupState) {
    /// Quit
    ZIMGroupStateQuit = 0,
    /// Enter
    ZIMGroupStateEnter = 1

};

typedef NS_ENUM(NSUInteger, ZIMGroupMuteMode) {

    ZIMGroupMuteModeNone = 0,

    ZIMGroupMuteModeNormal = 1,

    ZIMGroupMuteModeAll = 2,

    ZIMGroupMuteModeCustom = 3

};

typedef NS_ENUM(NSUInteger, ZIMGroupMemberEvent) {

    ZIMGroupMemberEventJoined = 1,

    ZIMGroupMemberEventLeft = 2,

    ZIMGroupMemberEventKickedout = 4,

    ZIMGroupMemberEventInvited = 5

};

typedef NS_ENUM(NSUInteger, ZIMGroupMemberState) {

    ZIMGroupMemberStateQuit = 0,

    ZIMGroupMemberStateEnter = 1

};

typedef NS_ENUM(NSUInteger, ZIMGroupAttributesUpdateAction) {

    ZIMGroupAttributesUpdateActionSet = 0,

    ZIMGroupAttributesUpdateActionDelete = 1

};

typedef NS_ENUM(NSUInteger, ZIMGroupMessageNotificationStatus) {

    ZIMGroupMessageNotificationStatusNotify = 1,

    ZIMGroupMessageNotificationStatusDoNotDisturb = 2

};

typedef NS_ENUM(NSUInteger, ZIMGroupJoinMode) {
    ZIMGroupJoinModeAny = 0,
    ZIMGroupJoinModeAuth = 1,
    ZIMGroupJoinModeForbid = 2
};

typedef NS_ENUM(NSUInteger, ZIMGroupInviteMode) {
    ZIMGroupInviteModeAny = 0,
    ZIMGroupInviteModeAdmin = 1,
};

typedef NS_ENUM(NSUInteger, ZIMGroupBeInviteMode) {
    ZIMGroupBeInviteModeNone = 0,
    ZIMGroupBeInviteModeAuth = 1,
};

typedef NS_ENUM(NSUInteger, ZIMGroupEnterType) {
    ZIMGroupEnterTypeUnknown = 0,
    ZIMGroupEnterTypeCreated = 1,
    ZIMGroupEnterTypeJoinApply = 2,
    ZIMGroupEnterTypeJoined = 3,
    ZIMGroupEnterTypeInvited = 4,
    ZIMGroupEnterTypeInviteApply = 5
};

typedef NS_ENUM(NSUInteger, ZIMGroupApplicationType) {
    ZIMGroupApplicationTypeNone = 0,
    ZIMGroupApplicationTypeJoin = 1,
    ZIMGroupApplicationTypeInvite = 2,
    ZIMGroupApplicationTypeBeInvite = 3
};

typedef NS_ENUM(NSUInteger, ZIMGroupApplicationState) {
    ZIMGroupApplicationStateWaiting = 1,
    ZIMGroupApplicationStateAccepted = 2,
    ZIMGroupApplicationStateRejected = 3,
    ZIMGroupApplicationStateExpired = 4,
    ZIMGroupApplicationStateDisabled = 5
};

typedef NS_ENUM(NSUInteger, ZIMGroupApplicationListChangeAction) {
    ZIMGroupApplicationListChangeActionAdded = 0
};

typedef int ZIMGroupMemberRole;
#define ZIMGroupMemberRoleOwner 1
#define ZIMGroupMemberRoleAdmin 2
#define ZIMGroupMemberRoleMember 3

typedef unsigned int ZIMGroupDataFlag;
#define ZIMGroupDataFlagName 0b0001
#define ZIMGroupDataFlagNotice 0b0010
#define ZIMGroupDataFlagAvatarUrl 0b0100

typedef NS_ENUM(NSInteger, ZIMGeofencingType) {
    ZIMGeofencingTypeNone = 0,
    ZIMGeofencingTypeInclude = 1,
    ZIMGeofencingTypeExclude = 2
};

typedef int ZIMGeofencingArea;
#define ZIMZIMGeofencingAreaCN 2
#define ZIMZIMGeofencingAreaNA 3
#define ZIMZIMGeofencingAreaEU 4
#define ZIMZIMGeofencingAreaAS 5
#define ZIMZIMGeofencingAreaIN 6

typedef NS_ENUM(NSInteger, ZIMCallState) {
    ZIMCallStateUnknown = -1,
    ZIMCallStateStarted = 1,
    ZIMCallStateEnded = 2
};

typedef NS_ENUM(NSInteger, ZIMCallInvitationMode) {
    ZIMCallInvitationModeUnknown = -1,
    ZIMCallInvitationModeGeneral = 0,
    ZIMCallInvitationModeAdvanced = 1
};

typedef NS_ENUM(NSInteger, ZIMCXHandleType) {
    ZIMCXHandleTypeGeneric = 1,
    ZIMCXHandleTypePhoneNumber = 2,
    ZIMCXHandleTypeEmailAddress = 3,
};

typedef NS_ENUM(NSInteger, ZIMPlatformType) {
    ZIMPlatformTypeWin = 1,
    ZIMPlatformTypeIPhoneOS = 2,
    ZIMPlatformTypeAndroid = 3,
    ZIMPlatformTypeMacOS = 4,
    ZIMPlatformTypeLinux = 5,
    ZIMPlatformTypeWeb = 6,
    ZIMPlatformTypeMiniProgram = 7,
    ZIMPlatformTypeIPadOS = 9,
    ZIMPlatformTypeOHOS = 10,
    ZIMPlatformTypeUnknown = 32
};

typedef NS_ENUM(NSUInteger, ZIMUserOnlineStatus) {

    ZIMUserOnlineStatusOnline = 0,

    ZIMUserOnlineStatusOffline = 1,

    ZIMUserOnlineStatusLogout = 2,

    ZIMUserOnlineStatusUnknown = 99

};
// MARK: - Model

/// Error infomation
///
/// Description: Error infomation.
///
@interface ZIMError : NSObject

/// The storage path of the log files. Refer to the official website document for the default path.
@property (nonatomic, assign) ZIMErrorCode code;

/// Error infomation description.
@property (nonatomic, copy) NSString *message;

@end

/// User information object.
///
/// Description: Identifies a unique user.
///
/// Caution: Note that the userID must be unique under the same appID, otherwise mutual kicks out will occur.
/// It is strongly recommended that userID corresponds to the user ID of the business APP,
/// that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID.
/// Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
///
@interface ZIMUserInfo : NSObject

/// User ID, a string with a maximum length of 32 bytes or less. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
@property (nonatomic, copy) NSString *userID;

/// User name, a string with a maximum length of 64 bytes or less
@property (nonatomic, copy) NSString *userName;

@property (nonatomic, copy) NSString *userAvatarUrl;

@property (nonatomic, copy) NSString *userExtendedData;

@end

@interface ZIMUserFullInfo : NSObject

/// User base info. including userID and userName.
@property (nonatomic, strong) ZIMUserInfo *baseInfo;

//userAvatarUrl is deprecated since ZIM 2.13.0
@property (nonatomic, copy) NSString *userAvatarUrl;
/// User extended data.
@property (nonatomic, copy) NSString *extendedData;

@end

@interface ZIMUsersInfoQueryConfig : NSObject

/// User base info. including userID and userName.
@property (nonatomic, assign) BOOL isQueryFromServer;

@end

//
@interface ZIMUserOfflinePushRule : NSObject
//这些指定的平台在线时
@property (nonatomic, strong) NSArray<NSNumber *> *onlinePlatforms;
//这些指定的平台不收到离线推送
@property (nonatomic, strong) NSArray<NSNumber *> *notToReceiveOfflinePushPlatforms;

@end

@interface ZIMUserRule : NSObject

@property (nonatomic, strong) ZIMUserOfflinePushRule *offlinePushRule;

@end

@interface ZIMUserStatus : NSObject

@property (nonatomic, copy) NSString *userID;

@property (nonatomic, assign) ZIMUserOnlineStatus onlineStatus;

@property (nonatomic, strong) NSArray<NSNumber *> *onlinePlatforms;

@property (nonatomic, strong) NSString *customStatus;

@property (nonatomic, assign) long long lastUpdateTime;

@property (nonatomic, assign) long long lastOnlineStatusUpdateTime;

@property (nonatomic, assign) long long lastCustomStatusUpdateTime;

@end

@interface ZIMUserStatusSubscription : NSObject

@property (nonatomic, strong) ZIMUserStatus *userStatus;

@property (nonatomic, assign) long long subscribeExpiredTime;

@end

@interface ZIMSelfUserInfo : NSObject

@property (nonatomic, strong) ZIMUserRule *userRule;

@property (nonatomic, strong) ZIMUserFullInfo *userFullInfo;

@property (nonatomic, strong) ZIMUserStatus *userStatus;

@end
// MARK: - Model

@interface ZIMAppConfig : NSObject

@property (nonatomic, assign) unsigned int appID;

@property (nonatomic, copy) NSString *appSign;

@end

/// Log configuration
///
/// Description: Configure the storage path of log files and the maximum log file size.
///
@interface ZIMLogConfig : NSObject

/// The storage path of the log files. Refer to the official website document for the default path.
@property (nonatomic, copy) NSString *logPath;

/// The maximum log file size (Bytes). The default maximum size is 5MB (5 * 1024 * 1024 Bytes)
@property (nonatomic, assign) unsigned long long logSize;

@end

@interface ZIMCacheConfig : NSObject

@property (nonatomic, copy) NSString *cachePath;

@end

@interface ZIMVoIPConfig : NSObject

@property (nonatomic, assign) ZIMCXHandleType iOSVoIPHandleType;

@property (nonatomic, copy) NSString *iOSVoIPHandleValue;

@property (nonatomic, assign) BOOL iOSVoIPHasVideo;

@end

@interface ZIMLoginConfig : NSObject

@property (nonatomic, copy) NSString *userName;

@property (nonatomic, copy) NSString *token;

@property (nonatomic, assign) BOOL isOfflineLogin;

@property (nonatomic, copy) NSString *customStatus;

@end

@interface ZIMOfflinePushPrivateMessageTemplate : NSObject

@property (nonatomic, strong) NSString *templateID;

@property (nonatomic, strong) NSString *titleParameters;

@property (nonatomic, strong) NSString *contentParameters;

@end

/// Description:Offline push configuration.
@interface ZIMPushConfig : NSObject

/// Description: Used to set the push title.
@property (nonatomic, copy) NSString *title;

/// Description: Used to set offline push content.
@property (nonatomic, copy) NSString *content;

/// Description: This parameter is used to set the pass-through field of offline push.
@property (nonatomic, copy) NSString *payload;

@property (nonatomic, copy) NSString *resourcesID;

@property (nonatomic, assign) BOOL enableBadge;

@property (nonatomic, assign) int badgeIncrement;

@property (nonatomic, strong) ZIMVoIPConfig *voIPConfig;

@property (nonatomic, strong) ZIMOfflinePushPrivateMessageTemplate *privateMessageTemplate;

@end

@interface ZIMMessageReactionUserInfo : NSObject
@property (nonatomic, copy) NSString *userID;
@end

@interface ZIMMessageReaction : NSObject
@property (nonatomic, copy) NSString *reactionType;
@property (nonatomic, copy) NSString *conversationID;
@property (nonatomic, assign) ZIMConversationType conversationType;
@property (nonatomic, assign) long long messageID;
@property (nonatomic, assign) unsigned int totalCount;
@property (nonatomic, assign) BOOL isSelfIncluded;
@property (nonatomic, strong) NSArray<ZIMMessageReactionUserInfo *> *userList;

@end

@interface ZIMMessageReactionUserQueryConfig : NSObject
@property (nonatomic, assign) unsigned long long nextFlag;
@property (nonatomic, assign) unsigned int count;
@property (nonatomic, copy) NSString *reactionType;

@end

@interface ZIMTipsMessageChangeInfo : NSObject

@property (nonatomic, assign) ZIMTipsMessageChangeInfoType type;

@end

@class ZIMGroupMuteInfo;

@interface ZIMTipsMessageGroupChangeInfo : ZIMTipsMessageChangeInfo

@property (nonatomic, assign) ZIMGroupDataFlag groupDataFlag;

@property (nonatomic, copy) NSString *groupName;

@property (nonatomic, copy) NSString *groupNotice;

@property (nonatomic, copy) NSString *groupAvatarUrl;

@property (nonatomic, strong) ZIMGroupMuteInfo *groupMutedInfo;

@end

@class ZIMGroupMemberSimpleInfo;

@interface ZIMTipsMessageGroupMemberChangeInfo : ZIMTipsMessageChangeInfo

@property (nonatomic, assign) int memberRole;

@property (nonatomic, assign) long long muteExpiredTime;

@property (nonatomic, strong, nullable) ZIMGroupMemberSimpleInfo *groupNewOwner;

@end

@interface ZIMMessageLiteInfo : NSObject

@property (nonatomic, assign) ZIMMessageType type;

@end

@interface ZIMTextMessageLiteInfo : ZIMMessageLiteInfo

@property (nonatomic, copy) NSString *message;

@end

@interface ZIMCustomMessageLiteInfo : ZIMMessageLiteInfo

@property (nonatomic, copy) NSString *message;

@property (nonatomic, copy) NSString *searchedContent;

@property (nonatomic, assign) unsigned int subType;

@end

@interface ZIMCombineMessageLiteInfo : ZIMMessageLiteInfo

@property (nonatomic, copy) NSString *title;

@property (nonatomic, copy) NSString *summary;

@end

@interface ZIMRevokeMessageLiteInfo : ZIMMessageLiteInfo

@end

@interface ZIMMediaMessageLiteInfo : ZIMMessageLiteInfo

@property (nonatomic, assign) long long fileSize;

@property (nonatomic, copy) NSString *fileName;

@property (nonatomic, copy) NSString *fileLocalPath;

@property (nonatomic, copy) NSString *fileDownloadUrl;

@end

@interface ZIMImageMessageLiteInfo : ZIMMediaMessageLiteInfo

@property (nonatomic, assign) CGSize originalImageSize;

@property (nonatomic, assign) CGSize largeImageSize;

@property (nonatomic, copy) NSString *largeImageLocalPath;

@property (nonatomic, copy) NSString *largeImageDownloadUrl;

@property (nonatomic, assign) CGSize thumbnailSize;

@property (nonatomic, copy) NSString *thumbnailLocalPath;

@property (nonatomic, copy) NSString *thumbnailDownloadUrl;

@end

@interface ZIMFileMessageLiteInfo : ZIMMediaMessageLiteInfo

@end

@interface ZIMAudioMessageLiteInfo : ZIMMediaMessageLiteInfo

@property (nonatomic, assign) unsigned int audioDuration;

@end

@interface ZIMVideoMessageLiteInfo : ZIMMediaMessageLiteInfo

@property (nonatomic, assign) unsigned int videoDuration;

@property (nonatomic, copy) NSString *videoFirstFrameDownloadUrl;

@property (nonatomic, copy) NSString *videoFirstFrameLocalPath;

@property (nonatomic, assign) CGSize videoFirstFrameSize;

@end

@interface ZIMMultipleMessageLiteInfo : ZIMMessageLiteInfo

@property (nonatomic, strong) NSArray<ZIMMessageLiteInfo *> *messageInfoList;

@end

@interface ZIMMessageRepliedInfo : NSObject

@property (nonatomic, assign) ZIMMessageRepliedInfoState state;

@property (nonatomic, strong) ZIMMessageLiteInfo *messageInfo;

@property (nonatomic, copy) NSString *senderUserID;

@property (nonatomic, assign) unsigned long long sentTime;

@property (nonatomic, assign) long long messageID;

@property (nonatomic, assign) long long messageSeq;

@end

@interface ZIMMessageRootRepliedCountInfo : NSObject

@property (nonatomic, assign) long long messageID;

@property (nonatomic, copy) NSString *conversationID;

@property (nonatomic, assign) ZIMConversationType conversationType;

@property (nonatomic, assign) unsigned int count;

@end

/// Base class of message object
///
/// Description: Identifies the basic parameters of a message.
///
/// Caution: Some of the parameters, such as Message ID, only have values ​​during the callback.
/// Developers do not need to assign values ​​to these parameters when they actively create this object for sending messages.
///
@interface ZIMMessage : NSObject

/// Identifies the type of this message.
@property (nonatomic, assign, readonly) ZIMMessageType type;

/// Description: The unique ID that identifies this message.
///
/// Use cases: Can be used to index other messages.
///
/// Caution: When the developer actively creates a message, there is no need to modify this parameter.
/// This parameter only has a value during callback.
@property (nonatomic, assign, readonly) long long messageID;

@property (nonatomic, assign, readonly) long long localMessageID;

@property (nonatomic, assign, readonly) long long messageSeq;

/// Description: Identifies which User sent this message.
///
/// Use cases: Used to show who sent a message.
///
/// Caution: When the developer actively creates a message, there is no need to modify this parameter.
/// This parameter only has a value during callback.
@property (nonatomic, copy, readonly) NSString *senderUserID;

/// Description: Session ID. Ids of the same session type are unique.
@property (nonatomic, copy, readonly) NSString *conversationID;

/// Description: Used to describe whether a message is sent or received.
@property (nonatomic, assign, readonly) ZIMMessageDirection direction;

/// Description: Describes the sending status of a message.
@property (nonatomic, assign, readonly) ZIMMessageSentStatus sentStatus;

/// Description: The type of session to which the message belongs.
@property (nonatomic, assign, readonly) ZIMConversationType conversationType;

/// Description: Identifies the sending time of a message
///
/// Use cases: Used to present the sending time of a message, and can be used for message sorting.
///
/// Caution: This is a standard UNIX timestamp, in milliseconds.
@property (nonatomic, assign, readonly) unsigned long long timestamp;

/// Description: Indicates the sequence number of the message in the session.
@property (nonatomic, assign, readonly) long long conversationSeq;

/// Description:The larger the orderKey, the newer the message, and can be used for ordering messages.
@property (nonatomic, assign, readonly) long long orderKey;

@property (nonatomic, assign, readonly) BOOL isUserInserted;

@property (nonatomic, assign, readonly) ZIMMessageReceiptStatus receiptStatus;

@property (nonatomic, copy) NSString *extendedData;

@property (nonatomic, copy) NSString *localExtendedData;

@property (nonatomic, strong) NSArray<ZIMMessageReaction *> *reactions;

@property (nonatomic, assign, readonly) BOOL isBroadcastMessage;

@property (nonatomic, strong) NSArray<NSString *> *mentionedUserIDs;

@property (nonatomic, assign) BOOL isMentionAll;

@property (nonatomic, assign, readonly) unsigned int rootRepliedCount;

@property (nonatomic, strong, nullable, readonly) ZIMMessageRepliedInfo *repliedInfo;

@property (nonatomic, assign, readonly) BOOL isServerMessage;

@property (nonatomic, copy, readonly) NSString *cbInnerID;

@property (nonatomic, copy, readonly) NSString *editorUserID;

@property (nonatomic, assign, readonly) long long editedTime;

@property (nonatomic, assign, readonly) BOOL isGroupTargetedMessage;

@end

/// Base class for media message objects.
///
/// Detail description: Identifies a media message.
///
/// Note: This base class is the basis of all media messages and contains the properties required by media messages.
@interface ZIMMediaMessage : ZIMMessage

/// Detail description: The local absolute path of the media message.
///
/// Required: Required by the sender, otherwise the message will fail to be sent.
@property (nonatomic, copy) NSString *fileLocalPath;

/// Detail description: The external download url of the media message is used for the developer to transparently transmit the media file to other users by filling in this URL when the developer uploads the media file to his own server.
///
/// Required or not: optional at the sender.
@property (nonatomic, copy) NSString *fileDownloadUrl;

/// Detail description: The unique ID of the media file.
///
/// Required or not: The sender does not need to fill in, this value is generated by the SDK.
@property (nonatomic, copy, readonly) NSString *fileUID;

/// Detail description: The filename of the media file.
///
/// Required or not: The sender does not need to fill in, this value is generated by the SDK.
@property (nonatomic, copy) NSString *fileName;

/// Detail description: The size of the media file.
///
/// Required or not: The sender does not need to fill in, this value is generated by the SDK.
@property (nonatomic, assign, readonly) long long fileSize;

@end

/// Normal text message object.
///
/// Description: Identifies the basic parameters of a message.
///
/// Caution: If the Type parameter of the base class is Text during callback,
/// you can force the base class message object to be of this type.
///
@interface ZIMTextMessage : ZIMMessage

/// The content of the text message.
@property (nonatomic, copy) NSString *message;

- (instancetype)initWithMessage:(NSString *)message;

@end

__attribute__((deprecated("Deprecated since ZIM 2.8.0, please use [ZIMCustomMessage] instead.")))
@interface ZIMSystemMessage : ZIMMessage

/// The content of the system message.
@property (nonatomic, copy) NSString *message;

- (instancetype)initWithMessage:(NSString *)message;

@end

/// Available since: 2.8.0 and above.
///
/// Description: Identifies a custom message.
///
/// Caution: When calling back, if the Type parameter of the base class is Custom, the base class message object can be forcibly converted to this class.
@interface ZIMCustomMessage : ZIMMessage

/// The content of the system message.
@property (nonatomic, copy) NSString *message;

/// Description: Search fields for custom messages. Since you cannot search for a custom message by directly searching the message field, you can concatenate the content you want to be searched in the custom message (such as the title of the poll, etc.) and put it in this parameter (the length is 64 bytes by default), so that follow-up search.
@property (nonatomic, copy) NSString *searchedContent;

/// Description: The subtype of the message, which is used by customers to customize the usage of different custom messages.
/// Required: The sender is required, otherwise the message will fail to be sent.
@property (nonatomic, assign) unsigned int subType;

- (instancetype)initWithMessage:(NSString *)message subType:(unsigned int)subType;

@end

/// Custom binary message object.
///
/// Description: Identifies a binary message.
///
/// Caution: If the Type parameter of the base class is Custom during callback,
/// you can force the base class message object to be of this type.
///
@interface ZIMCommandMessage : ZIMMessage

/// The content of the command message.
@property (nonatomic, copy) NSData *message;

- (instancetype)initWithMessage:(NSData *)message;

@end

/// Description: The barrage message class does not appear in the session and does not store historical messages.
@interface ZIMBarrageMessage : ZIMMessage

/// The content of the barrage message.
@property (nonatomic, copy) NSString *message;

- (instancetype)initWithMessage:(NSString *)message;

@end

/// Description: Identifies the basic parameters of a message.
///
/// Caution: If the Type parameter of the base class is Image during callback, you can force the base class message object to be of this type.
@interface ZIMImageMessage : ZIMMediaMessage

/// Detail description: Thumbnail external download URL of the image file. When developers upload thumbnails to their own servers, the SDK can pass through this field to other users.
///
/// Required or not: optional on the sender side, this field will only take effect when fileDownloadUrl is filled in.
@property (nonatomic, copy) NSString *thumbnailDownloadUrl;

/// Detailed description: The thumbnail local path of the image file.
///
/// Required or not: The sender does not need to fill in it. After calling [downloadMediaFile] to download, the SDK will generate this value.
@property (nonatomic, copy, readonly) NSString *thumbnailLocalPath;

/// Detail description: Large image external download URL of the image file. When developers upload large image to their own servers, the SDK can pass through this field to other users.
///
/// Required or not: optional on the sender side, this field will only take effect when fileDownloadUrl is filled in.
@property (nonatomic, copy) NSString *largeImageDownloadUrl;

/// Detailed description: The large image local path of the image file.
///
/// Required or not: The sender does not need to fill in it. After calling [downloadMediaFile] to download, the SDK will generate this value.
@property (nonatomic, copy, readonly) NSString *largeImageLocalPath;

@property (nonatomic, assign) CGSize originalImageSize;

@property (nonatomic, assign) CGSize largeImageSize;

@property (nonatomic, assign) CGSize thumbnailSize;

- (instancetype)initWithFileLocalPath:(NSString *)fileLocalPath;

@end

/// Description: Identifies the basic parameters of a message.
///
/// Caution: If the Type parameter of the base class is File during callback,
/// you can force the base class message object to be of this type.
@interface ZIMFileMessage : ZIMMediaMessage

- (instancetype)initWithFileLocalPath:(NSString *)fileLocalPath;

@end

/// Description: Identifies the basic parameters of a message.
///
/// Caution: If the Type parameter of the base class is Audio during callback,
/// you can force the base class message object to be of this type.
@interface ZIMAudioMessage : ZIMMediaMessage

/// Detail description: The duration of the audio file.
///
/// Required: Required by the sender, if not filled, the audio message will fail to be sent.
@property (nonatomic, assign) unsigned int audioDuration;

- (instancetype)initWithFileLocalPath:(NSString *)fileLocalPath
                        audioDuration:(unsigned int)audioDuration;

@end

@interface ZIMRevokeMessage : ZIMMessage

@property (nonatomic, assign, readonly) ZIMRevokeType revokeType;

@property (nonatomic, assign, readonly) unsigned long long revokeTimestamp;

@property (nonatomic, copy, readonly) NSString *operatedUserID;

@property (nonatomic, assign, readonly) ZIMMessageType originalMessageType;

@property (nonatomic, copy, readonly) NSString *originalTextMessageContent;

@property (nonatomic, copy, readonly) NSString *revokeExtendedData;

@property (nonatomic, assign, readonly) ZIMMessageRevokeStatus revokeStatus;

@end

/// Description: Identifies the basic parameters of a message.
///
/// Caution: If the Type parameter of the base class is Video during callback,
/// you can force the base class message object to be of this type.
@interface ZIMVideoMessage : ZIMMediaMessage

/// Detail description: The duration of the video file.
///
/// Required: Required by the sender, if not filled, the video message will fail to be sent.
@property (nonatomic, assign) unsigned int videoDuration;

/// Detail description: Video first frame external download URL of the video file. When developers upload thumbnails to their own servers, the SDK can pass through this field to other users.
///
/// Required or not: optional on the sender side, this field will only take effect when fileDownloadUrl is filled in.
@property (nonatomic, copy) NSString *videoFirstFrameDownloadUrl;

/// Detailed description: The video first frame local path of the video file.
///
/// Required or not: The sender does not need to fill in it. After calling [downloadMediaFile] to download, the SDK will generate this value.
@property (nonatomic, copy, readonly) NSString *videoFirstFrameLocalPath;

@property (nonatomic, assign) CGSize videoFirstFrameSize;

- (instancetype)initWithFileLocalPath:(NSString *)fileLocalPath
                        videoDuration:(unsigned int)videoDuration;

@end

@interface ZIMCombineMessage : ZIMMessage

@property (nonatomic, copy) NSString *title;

@property (nonatomic, copy) NSString *summary;

@property (nonatomic, readonly) NSString *combineID;

@property (nonatomic, strong) NSArray<ZIMMessage *> *messageList;

- (instancetype)initWithTitle:(NSString *)title
                      summary:(NSString *)summary
                  messageList:(NSArray<ZIMMessage *> *)messageList;

@end

@interface ZIMTipsMessage : ZIMMessage

@property (nonatomic, assign) ZIMTipsMessageEvent event;

@property (nonatomic, strong) ZIMUserInfo *operatedUser;

@property (nonatomic, strong) NSArray<ZIMUserInfo *> *targetUserList;

@property (nonatomic, strong, nullable) ZIMTipsMessageChangeInfo *changeInfo;

@end

@interface ZIMMultipleMessage : ZIMMessage

@property (nonatomic, strong) NSArray<ZIMMessageLiteInfo *> *messageInfoList;

- (instancetype)initWithMessageInfoList:(NSArray<ZIMMessageLiteInfo *> *)messageInfoList;

@end

@interface ZIMMessageMentionedInfo : NSObject

@property (nonatomic, assign) long long messageID;

@property (nonatomic, assign) long long messageSeq;

@property (nonatomic, copy) NSString *fromUserID;

@property (nonatomic, assign) ZIMMessageMentionedType type;

@end

@interface ZIMMessageRootRepliedInfo : NSObject

@property (nonatomic, assign) ZIMMessageRepliedInfoState state;

@property (nonatomic, strong, nullable) ZIMMessage *message;

@property (nonatomic, copy) NSString *senderUserID;

@property (nonatomic, assign) unsigned long long sentTime;

@property (nonatomic, assign) unsigned int repliedCount;

@end

@interface ZIMMessageReceivedInfo : NSObject

@property (nonatomic, assign) BOOL isOfflineMessage;

@end

/// Details: Configure message sending.
@interface ZIMMessageSendConfig : NSObject

/// Description: Configures the offline push function.
@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

/// Enumeration value used to set message priority. The default value is Low.
@property (nonatomic, assign) ZIMMessagePriority priority;

@property (nonatomic, assign) BOOL hasReceipt;

@property (nonatomic, assign) BOOL isNotifyMentionedUsers;

@property (nonatomic, assign) BOOL isRetrySend;

@property (nonatomic, assign) BOOL disableUnreadMessageCount;

@end

@interface ZIMMessageEditConfig : NSObject

@end

@interface ZIMSendingMessageCancelConfig : NSObject

@end

/// Example Query message configuration.
@interface ZIMMessageQueryConfig : NSObject

/// Description: Query the anchor point of the message.
/// Required: This parameter is not required for the first query but is required for subsequent paging queries.
/// Default value: The default value is nil.
@property (nonatomic, strong, nullable) ZIMMessage *nextMessage;

/// Description: Number of query messages. The default value is 0.
@property (nonatomic, assign) unsigned int count;

/// Description: Indicates whether the query is in reverse order. The default value is NO.
@property (nonatomic, assign) BOOL reverse;

@end

/// Description: Revoke configurations related to messages.
@interface ZIMMessageRevokeConfig : NSObject

/// Description: Configures the offline push function, If Android or iOS platform is integrated, it is strongly recommended to configure this.
/// Required: Not mandatory.
@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

/// Description: revoking additional messages.
@property (nonatomic, copy) NSString *revokeExtendedData;

@end

/// Delete message configuration.
@interface ZIMMessageDeleteConfig : NSObject

/// Description: Whether to remove flags for server messages. The default value is YES.
@property (nonatomic, assign) BOOL isAlsoDeleteServerMessage;

@end

@interface ZIMMessageDeletedInfo : NSObject

@property (nonatomic, copy) NSString *conversationID;

@property (nonatomic, assign) ZIMConversationType conversationType;

@property (nonatomic, assign) ZIMMessageDeleteType messageDeleteType;

@property (nonatomic, assign) BOOL isDeleteConversationAllMessage;

@property (nonatomic, strong) NSArray<ZIMMessage *> *messageList;

@end

typedef void (^ZIMMessageAttachedCallback)(ZIMMessage *message);

/// Supported versions: 2.1.0 and above
///
/// Detail description: The progress callback for sending media messages.
///
/// Business scenario: The developer can obtain the sending progress of the media message through this callback.
///
/// Notification timing: When the developer calls the [sendMediaMessage] interface, the callback will be triggered, and will be triggered multiple times during the sending process.
///
/// Related interface: Through [sendMediaMessage], the sending progress will be notified through this callback.
typedef void (^ZIMMediaUploadingProgress)(ZIMMediaMessage *message,
                                          unsigned long long currentFileSize,
                                          unsigned long long totalFileSize);

typedef void (^ZIMMultipleMediaUploadingProgress)(ZIMMultipleMessage *message,
                                                  unsigned long long currentFileSize,
                                                  unsigned long long totalFileSize,
                                                  unsigned int messageInfoIndex,
                                                  unsigned long long currentIndexFileSize,
                                                  unsigned long long totalIndexFileSize);

@interface ZIMMessageSendNotification : NSObject

@property (nonatomic, copy, nullable) ZIMMessageAttachedCallback onMessageAttached;
@property (nonatomic, copy, nullable) ZIMMediaUploadingProgress onMediaUploadingProgress;
@property (nonatomic, copy, nullable)
    ZIMMultipleMediaUploadingProgress onMultipleMediaUploadingProgress;

@end

@interface ZIMMediaMessageSendNotification : NSObject

@property (nonatomic, copy, nullable) ZIMMessageAttachedCallback onMessageAttached;
@property (nonatomic, copy, nullable) ZIMMediaUploadingProgress onMediaUploadingProgress;

@end

/// Detailed description: receipt information.
@interface ZIMMessageReceiptInfo : NSObject

/// Detail description: receipt status.
@property (nonatomic, assign) ZIMMessageReceiptStatus status;

/// Detailed Description: Message ID.
///
/// Business scenario: Developers can match the loaded message list according to this ID.
///
/// Is it required: No, SDK fills in.
@property (nonatomic, assign) long long messageID;

/// Detailed Description: Session ID.
///
/// Business scenario: Receipt information used to indicate which session this belongs to.
///
/// Is it required: No, SDK fills in.
@property (nonatomic, copy) NSString *conversationID;

/// Detailed Description: Session type.
///
/// Business scenario: Indicates the type of the session.
///
/// Is it required: If no, the SDK is filled.
@property (nonatomic, assign) ZIMConversationType conversationType;

/// Detailed description: Indicates the number of people who have read the receipt.
///
/// Business scenario: When used to query receipt information, it can display how many people have read the message.
///
/// Is it required: no.
///
/// Default: 0.
///
/// Note: This value indicates how many people have read the message, it is only applicable to the message has been read; if the message is not sent by yourself, the value is 0.
@property (nonatomic, assign) unsigned int readMemberCount;

/// Detailed description: Indicates the number of unread people of the receipt.
///
/// Business scenario: When used to query receipt information, it can display how many people have unread the message.
///
/// Is it required: no.
///
/// Default: 0.
///
/// Note: This value indicates how many people have not read the message, and it is only applicable to the message that has been read; if the message is not sent by yourself, the value is 0.
@property (nonatomic, assign) unsigned int unreadMemberCount;

@property (nonatomic, assign) BOOL isSelfOperated;

@property (nonatomic, assign) unsigned long long readTime;

@end

@interface ZIMConversation : NSObject

/// Description: conversationID.
@property (nonatomic, copy) NSString *conversationID;

/// Description: conversationName is the same as the groupName/userName value corresponding to the session.
@property (nonatomic, copy) NSString *conversationName;

@property (nonatomic, copy) NSString *conversationAlias;

@property (nonatomic, copy) NSString *conversationAvatarUrl;

/// Description: conversation type.
@property (nonatomic, assign) ZIMConversationType type;

/// Description: session notification status.
@property (nonatomic, assign) ZIMConversationNotificationStatus notificationStatus;

/// Description: Session unread.
@property (nonatomic, assign) unsigned int unreadMessageCount;

/// Description: last message.
@property (nonatomic, strong) ZIMMessage *lastMessage;

/// Description: OrderKey is used to describe the order of messages in the session. The larger orderKey is, the newer it is.
@property (nonatomic, assign) long long orderKey;

/// Description: conversation pinned state
@property (nonatomic, assign) BOOL isPinned;

@property (nonatomic, strong) NSArray<ZIMMessageMentionedInfo *> *mentionedInfoList;
/// Description: conversation draft.
@property (nonatomic, copy) NSString *draft;

@property (nonatomic, strong) NSArray<NSNumber *> *marks;

@end

@interface ZIMConversationBaseInfo : NSObject

@property (nonatomic, copy) NSString *conversationID;

@property (nonatomic, assign) ZIMConversationType conversationType;

@end

@interface ZIMGroupConversation : ZIMConversation

@property (nonatomic, assign) long long mutedExpiredTime;

@property (nonatomic, assign) BOOL isDisabled;

@end

/// Description: Query the relevant configuration of the session.
@interface ZIMConversationQueryConfig : NSObject

/// Description: Session that needs to be queried.
@property (nonatomic, strong, nullable) ZIMConversation *nextConversation;

/// Description: The number of sessions to query.
@property (nonatomic, assign) unsigned int count;

@end

@interface ZIMConversationFilterOption : NSObject

@property (nonatomic, strong) NSArray<NSNumber *> *marks;

@property (nonatomic, strong) NSArray<NSNumber *> *conversationTypes;

@property (nonatomic, assign) BOOL isOnlyUnreadConversation;

@end

@interface ZIMConversationTotalUnreadMessageCountQueryConfig : NSObject

@property (nonatomic, strong) NSArray<NSNumber *> *marks;

@property (nonatomic, strong) NSArray<NSNumber *> *conversationTypes;

@end

/// Description: Delete session configuration.
@interface ZIMConversationDeleteConfig : NSObject

/// Description: just remove the session flag.
@property (nonatomic, assign) BOOL isAlsoDeleteServerConversation;

@end

@interface ZIMConversationsAllDeletedInfo : NSObject

@property (nonatomic, assign) unsigned int count;

@end

/// Description: Session change information.
@interface ZIMConversationChangeInfo : NSObject

/// Description: session events.
@property (nonatomic, assign) ZIMConversationEvent event;

/// Description: Session specific information.
@property (nonatomic, strong) ZIMConversation *conversation;

@end

/// Description: Session sent status change information.
@interface ZIMMessageSentStatusChangeInfo : NSObject

/// Description: sentStatus
@property (nonatomic, assign) ZIMMessageSentStatus status;

/// Description: Session specific information.
@property (nonatomic, strong) ZIMMessage *message;

/// Descruotion: Custom reason
@property (nonatomic, copy) NSString *reason;

@end

@interface ZIMMessageImportConfig : NSObject

@end

@interface ZIMMessageExportConfig : NSObject

@end

@interface ZIMFileCacheClearConfig : NSObject

@property (nonatomic, assign) long long endTime;

@end

@interface ZIMFileCacheQueryConfig : NSObject

@property (nonatomic, assign) long long endTime;

@end

@interface ZIMFileCacheInfo : NSObject

@property (nonatomic, assign) long long totalFileSize;

@end

@interface ZIMMessageSearchConfig : NSObject

@property (nonatomic, strong, nullable) ZIMMessage *nextMessage;

@property (nonatomic, assign) unsigned int count;

@property (nonatomic, assign) ZIMMessageOrder order;

@property (nonatomic, strong) NSArray<NSString *> *keywords;

@property (nonatomic, strong) NSArray<NSNumber *> *messageTypes;

@property (nonatomic, strong) NSArray<NSNumber *> *subMessageTypes;

@property (nonatomic, strong) NSArray<NSString *> *senderUserIDs;

@property (nonatomic, assign) long long startTime;

@property (nonatomic, assign) long long endTime;

@end

@interface ZIMMessageRepliedListQueryConfig : NSObject

@property (nonatomic, assign) long long nextFlag;

@property (nonatomic, assign) unsigned int count;

@end

@interface ZIMMediaDownloadConfig : NSObject

@property (nonatomic, assign) unsigned int messageInfoIndex;

@end

@interface ZIMConversationSearchConfig : NSObject

@property (nonatomic, assign) unsigned int nextFlag;

@property (nonatomic, assign) unsigned int totalConversationCount;

@property (nonatomic, assign) unsigned int conversationMessageCount;

@property (nonatomic, strong) NSArray<NSString *> *keywords;

@property (nonatomic, strong) NSArray<NSNumber *> *messageTypes;

@property (nonatomic, strong) NSArray<NSNumber *> *subMessageTypes;

@property (nonatomic, strong) NSArray<NSString *> *senderUserIDs;

@property (nonatomic, assign) long long startTime;

@property (nonatomic, assign) long long endTime;

@end

@interface ZIMConversationSearchInfo : NSObject

@property (nonatomic, copy) NSString *conversationID;

@property (nonatomic, assign) ZIMConversationType conversationType;

@property (nonatomic, assign) unsigned int totalMessageCount;

@property (nonatomic, strong) NSArray<ZIMMessage *> *messageList;

@end

/// Room information object.
///
/// Description: Identifies a unique room.
///
@interface ZIMRoomInfo : NSObject

/// Room ID, a string with a maximum length of 32 bytes or less. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
@property (nonatomic, copy) NSString *roomID;

/// Room name, a string with a maximum length of 64 bytes or less
@property (nonatomic, copy) NSString *roomName;

@end

/// The room details object.
///
/// Description: Identifies the basic parameters of a message.
///
/// Caution: Identifies the detailed information of a room.
///
@interface ZIMRoomFullInfo : NSObject

/// The base object of the room.
@property (nonatomic, strong) ZIMRoomInfo *baseInfo;

@end

@interface ZIMRoomMemberInfo : ZIMUserInfo

@end

/// Configuration for querying member.
///
/// Description: When querying member, you need to configure this object.
///
@interface ZIMRoomMemberQueryConfig : NSObject

/// Description: The flag of the paging query. For the first query, set this field to an empty string. If the "nextFlag" field of the callback is not an empty string, it needs to be set here to continue the query on the next page.
@property (nonatomic, copy) NSString *nextFlag;

/// Description: How many messages are retrieved in one query.
///
/// Caution: To obtain messages in pages to reduce overhead, it is recommended to obtain within 100 messages at a time.
@property (nonatomic, assign) unsigned int count;

@end

///
///
///
///
/// Room advanced config.
@interface ZIMRoomAdvancedConfig : NSObject

/// Description: Room attributes of a room.
@property (nonatomic, strong, nullable) NSDictionary<NSString *, NSString *> *roomAttributes;

@property (nonatomic, assign) unsigned int roomDestroyDelayTime;

@end

/// The behavior attribute set by the room attribute.
@interface ZIMRoomAttributesSetConfig : NSObject

/// Description: Whether the operation is mandatory, that is, the property of the room whose owner is another user can be modified.
@property (nonatomic, assign) BOOL isForce;

/// Description: Room attributes are automatically deleted after the owner leaves the room.
@property (nonatomic, assign) BOOL isDeleteAfterOwnerLeft;

/// Description: Whether to update the owner of the room attribute involved.
@property (nonatomic, assign) BOOL isUpdateOwner;

@end

/// The behavior attribute set by the room attribute.
@interface ZIMRoomAttributesBatchOperationConfig : NSObject

/// Description: Whether the operation is mandatory, that is, the property of the room whose owner is another user can be modified.
@property (nonatomic, assign) BOOL isForce;

/// Description: Room attributes are automatically deleted after the owner leaves the room.
@property (nonatomic, assign) BOOL isDeleteAfterOwnerLeft;

/// Description: Whether to update the owner of the room attribute involved.
@property (nonatomic, assign) BOOL isUpdateOwner;

@end
///
///
///
///
@interface ZIMRoomAttributesDeleteConfig : NSObject

/// Description: Whether the operation is mandatory, that is, the property of the room whose owner is another user can be deleted.
@property (nonatomic, assign) BOOL isForce;

@end

///
///
///
///Notice of Room Attribute Change.
@interface ZIMRoomAttributesUpdateInfo : NSObject

/// Description: Behavioral information of room attribute change notification.
@property (nonatomic, assign) ZIMRoomAttributesUpdateAction action;

/// Description:  Room attributes.
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *roomAttributes;

@end

///Room user attribute information.
@interface ZIMRoomMemberAttributesInfo : NSObject

/// Description: Detail description: User ID.
@property (nonatomic, copy) NSString *userID;
///
/// Description: room user attributes.
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *attributes;

@end

///Room user attribute operation information.
@interface ZIMRoomMemberAttributesOperatedInfo : NSObject

/// Description: room user attribute information.
@property (nonatomic, strong) ZIMRoomMemberAttributesInfo *attributesInfo;

/// Description: The key of the room user attribute operation failure.
@property (nonatomic, strong) NSArray<NSString *> *errorKeys;

@end

///Room user attribute update information.
@interface ZIMRoomMemberAttributesUpdateInfo : NSObject

/// Description: room user attributes.
@property (nonatomic, strong) ZIMRoomMemberAttributesInfo *attributesInfo;

@end

///Room user property settings configuration.
@interface ZIMRoomMemberAttributesSetConfig : NSObject

/// Description: Configure whether the room user attributes are saved after the user leaves the room.
@property (nonatomic, assign) BOOL isDeleteAfterOwnerLeft;

@end

///Room user attribute query configuration.
@interface ZIMRoomMemberAttributesQueryConfig : NSObject

/// Description: Query anchor for room user properties.
@property (nonatomic, copy) NSString *nextFlag;

/// Description: The number of paginated queries.
@property (nonatomic, assign) unsigned int count;

@end

///Room operation information.
@interface ZIMRoomOperatedInfo : NSObject

/// Description: UserID of the operator.
@property (nonatomic, copy) NSString *userID;

@end
///
///
///
///
/// When userInfo is queried, the failed userInfo is displayed through this data class.
@interface ZIMErrorUserInfo : NSObject

/// Description:userID.
@property (nonatomic, copy) NSString *userID;

/// Description: Description Reason for the query failure.
@property (nonatomic, assign) unsigned int reason;

@end

@interface ZIMGroupMemberSimpleInfo : ZIMUserInfo

/// Description: Group nickname.
@property (nonatomic, copy) NSString *memberNickname;

/// Description: group role.
@property (nonatomic, assign) ZIMGroupMemberRole memberRole;

@end

///
///
///
/// group information.
@interface ZIMGroupInfo : NSObject

/// Description: groupID.
@property (nonatomic, copy) NSString *groupID;

/// Description: Group name.
@property (nonatomic, copy) NSString *groupName;

@property (nonatomic, copy) NSString *groupAvatarUrl;

@end

@interface ZIMGroupMuteInfo : NSObject

@property (nonatomic, assign) ZIMGroupMuteMode mode;

@property (nonatomic, assign) long long expiredTime;

@property (nonatomic, strong) NSArray<NSNumber *> *roles;

@end

@interface ZIMGroupVerifyInfo : NSObject

@property (nonatomic, assign) ZIMGroupJoinMode joinMode;

@property (nonatomic, assign) ZIMGroupInviteMode inviteMode;

@property (nonatomic, assign) ZIMGroupBeInviteMode beInviteMode;

@end

@interface ZIMGroupEnterInfo : NSObject

@property (nonatomic, assign) long long enterTime;

@property (nonatomic, assign) ZIMGroupEnterType enterType;

@property (nonatomic, strong, nullable) ZIMGroupMemberSimpleInfo *operatedUser;

@end

///
///
///
/// Description: complete group information.
@interface ZIMGroupFullInfo : NSObject

/// Description: basic group information.
@property (nonatomic, strong) ZIMGroupInfo *baseInfo;

@property (nonatomic, copy) NSString *groupAlias;

/// Description: basic group notice.
@property (nonatomic, copy) NSString *groupNotice;

/// Description: where developers can customize key-value
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *groupAttributes;

/// Description: group DND status.
@property (nonatomic, assign) ZIMGroupMessageNotificationStatus notificationStatus;

@property (nonatomic, strong) ZIMGroupMuteInfo *mutedInfo;

@property (nonatomic, assign) long long createTime;

@property (nonatomic, assign) unsigned int maxMemberCount;

@property (nonatomic, strong) ZIMGroupVerifyInfo *verifyInfo;

@end

/// Description:  group class.
@interface ZIMGroup : NSObject

/// Description: basic group information.
@property (nonatomic, strong) ZIMGroupInfo *baseInfo;

@property (nonatomic, copy) NSString *groupAlias;

/// Description: group DND status.
@property (nonatomic, assign) ZIMGroupMessageNotificationStatus notificationStatus;

@end

///
///
///
/// Group member information.
@interface ZIMGroupMemberInfo : ZIMUserInfo

/// Description: Group nickname.
@property (nonatomic, copy) NSString *memberNickname;

/// Description: group role.
@property (nonatomic, assign) ZIMGroupMemberRole memberRole;

//memberAvatarUrl is deprecated since ZIM 2.13.0
@property (nonatomic, copy) NSString *memberAvatarUrl;

@property (nonatomic, assign) long long muteExpiredTime;

@property (nonatomic, strong) ZIMGroupEnterInfo *groupEnterInfo;

@end

@interface ZIMGroupApplicationInfo : NSObject

@property (nonatomic, assign) ZIMGroupApplicationType type;

@property (nonatomic, assign) ZIMGroupApplicationState state;

@property (nonatomic, strong) ZIMGroupInfo *groupInfo;

@property (nonatomic, strong) ZIMUserInfo *applyUser;

@property (nonatomic, assign) long long createTime;

@property (nonatomic, assign) long long updateTime;

@property (nonatomic, copy) NSString *wording;

@property (nonatomic, strong, nullable) ZIMGroupMemberSimpleInfo *operatedUser;

@end

///
///
///
/// Information that the group has operated on.
@interface ZIMGroupOperatedInfo : NSObject
/// Description: Group member information.
//ZIMGroupMemberInfo is deprecated.
@property (nonatomic, strong) ZIMGroupMemberInfo *operatedUserInfo;

@property (nonatomic, copy) NSString *userID;

@property (nonatomic, copy) NSString *userName;

@property (nonatomic, copy) NSString *memberNickname;

@property (nonatomic, assign) ZIMGroupMemberRole memberRole;

@end

///
///
///
/// group member query configuration.
@interface ZIMGroupMemberQueryConfig : NSObject

/// Description: nextFlag.
@property (nonatomic, assign) unsigned int nextFlag;

/// Description: count.
@property (nonatomic, assign) unsigned int count;

@end

///
///
///
/// Group advanced configuration.
@interface ZIMGroupAdvancedConfig : NSObject

@property (nonatomic, copy) NSString *groupNotice;

@property (nonatomic, strong, nullable) NSDictionary<NSString *, NSString *> *groupAttributes;

@property (nonatomic, assign) unsigned int maxMemberCount;

@property (nonatomic, assign) ZIMGroupJoinMode joinMode;

@property (nonatomic, assign) ZIMGroupInviteMode inviteMode;

@property (nonatomic, assign) ZIMGroupBeInviteMode beInviteMode;

@end

@interface ZIMGroupMuteConfig : NSObject

@property (nonatomic, assign) ZIMGroupMuteMode mode;

@property (nonatomic, assign) int duration;

@property (nonatomic, strong) NSArray<NSNumber *> *roles;

@end

@interface ZIMGroupMemberMuteConfig : NSObject

@property (nonatomic, assign) int duration;

@end

@interface ZIMGroupMemberMutedListQueryConfig : NSObject

/// Description: nextFlag.
@property (nonatomic, assign) unsigned long long nextFlag;

/// Description: count.
@property (nonatomic, assign) unsigned int count;

@end

///
///
///
/// Group attribute update information.
@interface ZIMGroupAttributesUpdateInfo : NSObject

/// Description: Group attribute update action.
@property (nonatomic, assign) ZIMGroupAttributesUpdateAction action;

/// Description: group properties.
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *groupAttributes;

@end

@interface ZIMGroupMessageReceiptMemberQueryConfig : NSObject

@property (nonatomic, assign) unsigned int nextFlag;

@property (nonatomic, assign) unsigned int count;

@end

@interface ZIMGroupSearchConfig : NSObject

@property (nonatomic, assign) unsigned int nextFlag;

@property (nonatomic, assign) unsigned int count;

@property (nonatomic, strong) NSArray<NSString *> *keywords;

@property (nonatomic, assign) BOOL isAlsoMatchGroupMemberUserName;

@property (nonatomic, assign) BOOL isAlsoMatchGroupMemberNickname;

@end

@interface ZIMGroupSearchInfo : NSObject

@property (nonatomic, strong) ZIMGroupInfo *groupInfo;

@property (nonatomic, strong) NSArray<ZIMGroupMemberInfo *> *userList;

@end

@interface ZIMGroupMemberSearchConfig : NSObject

@property (nonatomic, assign) unsigned int nextFlag;

@property (nonatomic, assign) unsigned int count;

@property (nonatomic, strong) NSArray<NSString *> *keywords;

@property (nonatomic, assign) BOOL isAlsoMatchGroupMemberNickname;

@end

@interface ZIMGroupJoinApplicationSendConfig : NSObject

@property (nonatomic, copy) NSString *wording;

@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

@end

@interface ZIMGroupJoinApplicationAcceptConfig : NSObject

@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

@end

@interface ZIMGroupJoinApplicationRejectConfig : NSObject

@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

@end

@interface ZIMGroupInviteApplicationSendConfig : NSObject

@property (nonatomic, copy) NSString *wording;

@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

@end

@interface ZIMGroupInviteApplicationAcceptConfig : NSObject

@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

@end

@interface ZIMGroupInviteApplicationRejectConfig : NSObject

@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

@end

@interface ZIMGroupApplicationListQueryConfig : NSObject

@property (nonatomic, assign) unsigned int nextFlag;

@property (nonatomic, assign) unsigned int count;

@end

typedef NS_ENUM(NSInteger, ZIMCallUserState) {
    ZIMCallUserStateUnknown = -1,
    ZIMCallUserStateInviting = 0,
    ZIMCallUserStateAccepted = 1,
    ZIMCallUserStateRejected = 2,
    ZIMCallUserStateCancelled = 3,
    //ZIMCallUserStateOffline is deprecated since ZIM 2.9.0
    ZIMCallUserStateOffline = 4,
    ZIMCallUserStateReceived = 5,
    ZIMCallUserStateTimeout = 6,
    ZIMCallUserStateQuit = 7,
    ZIMCallUserStateEnd = 8,
    ZIMCallUserStateNotYetReceived = 9,
    ZIMCallUserStateBeCancelled = 10,
};

typedef NS_ENUM(NSInteger, ZIMFriendApplicationType) {
    ZIMFriendApplicationTypeUnknown = -1,
    ZIMFriendApplicationTypeNone = 0,
    ZIMFriendApplicationTypeReceived = 1,
    ZIMFriendApplicationTypeSent = 2,
    ZIMFriendApplicationTypeBoth = 3,
};

typedef NS_ENUM(NSInteger, ZIMFriendRelationCheckType) {
    ZIMFriendRelationCheckTypeUnknown = -1,
    ZIMFriendRelationCheckTypeBoth = 0,
    ZIMFriendRelationCheckTypeSingle = 1,
};

typedef NS_ENUM(NSInteger, ZIMFriendDeleteType) {
    ZIMFriendDeleteTypeBoth = 0,
    ZIMFriendDeleteTypeSingle = 1,
};

typedef NS_ENUM(NSInteger, ZIMFriendApplicationState) {
    ZIMFriendApplicationStateUnknown = -1,
    ZIMFriendApplicationStateWaiting = 1,
    ZIMFriendApplicationStateAccepted = 2,
    ZIMFriendApplicationStateRejected = 3,
    ZIMFriendApplicationStateExpired = 4,
    ZIMFriendApplicationStateDisabled = 5,
};

typedef NS_ENUM(NSUInteger, ZIMUserRelationType) {
    ZIMUserRelationTypeUnknown = 0,
    ZIMUserRelationTypeSingleNo = 1,
    ZIMUserRelationTypeSingleHave = 2,
    ZIMUserRelationTypeBothAllNo = 3,
    ZIMUserRelationTypeBothSelfHave = 4,
    ZIMUserRelationTypeBothOtherHave = 5,
    ZIMUserRelationTypeBothAllHave = 6,
};

/// Call invitation user information.
@interface ZIMCallUserInfo : NSObject

/// Description:  userID.
@property (nonatomic, copy) NSString *userID;

/// Description:  user status.
@property (nonatomic, assign) ZIMCallUserState state;

@property (nonatomic, copy) NSString *extendedData;

@end

/// The behavior property of the Send Call Invitation setting.
@interface ZIMCallInviteConfig : NSObject
/// Description: The timeout setting of the call invitation, the unit is seconds. The default value is 90s.
@property (nonatomic, assign) unsigned int timeout;

@property (nonatomic, assign) ZIMCallInvitationMode mode;

/// Description: Extended field, through which the inviter can carry information to the invitee.
@property (nonatomic, copy) NSString *extendedData;

@property (nonatomic, assign) BOOL enableNotReceivedCheck;

/// Description: Configures the offline push function.
@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

@end

@interface ZIMCallingInviteConfig : NSObject

@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

@end

@interface ZIMCallJoinConfig : NSObject

@property (nonatomic, copy) NSString *extendedData;

@end

@interface ZIMCallQuitConfig : NSObject

@property (nonatomic, copy) NSString *extendedData;

@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

@end

@interface ZIMCallEndConfig : NSObject

/// Description: Extended field, through which the inviter can carry information to the invitee.
///
/// Required: not required.
@property (nonatomic, copy) NSString *extendedData;

/// Description: Offline push configuration.
/// Required: not required.
@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

@end

/// Behavior property that cancels the call invitation setting.
@interface ZIMCallCancelConfig : NSObject

/// Description: Configures the offline push function.
@property (nonatomic, strong, nullable) ZIMPushConfig *pushConfig;

/// Description: Extended field.
@property (nonatomic, copy) NSString *extendedData;

@end

/// Behavior property that accept the call invitation setting.
@interface ZIMCallAcceptConfig : NSObject

/// Description: Extended field.
@property (nonatomic, copy) NSString *extendedData;

@end

/// The behavior property of the reject call invitation setting.
@interface ZIMCallRejectConfig : NSObject

/// Description: Extended field, through which the inviter can carry information to the invitee.
@property (nonatomic, copy) NSString *extendedData;

@end

/// Call invitation sent message.
@interface ZIMCallInvitationSentInfo : NSObject

/// Description: The timeout setting of the call invitation, the unit is seconds.
@property (nonatomic, assign) unsigned int timeout;

/// Description: User id that has not received a call invitation.
@property (nonatomic, strong) NSArray<ZIMErrorUserInfo *> *errorUserList;

@property (nonatomic, strong) NSArray<ZIMCallUserInfo *> *errorInvitees DEPRECATED_MSG_ATTRIBUTE(
    "Deprecated since ZIM 2.9.0, please use errorList instead.");

@end

@interface ZIMCallingInvitationSentInfo : NSObject

/// Description: User id that has not received a call invitation.
@property (nonatomic, strong) NSArray<ZIMErrorUserInfo *> *errorUserList;

@end

@interface ZIMCallJoinSentInfo : NSObject

@property (nonatomic, copy) NSString *extendedData;

/// Description: Timestamp of the time when the call invitation was created.
@property (nonatomic, assign) long long createTime;

/// Description: Time stamp of the person joining the call invitation.
@property (nonatomic, assign) long long joinTime;

@property (nonatomic, strong) NSArray<ZIMCallUserInfo *> *callUserList;

@end

@interface ZIMCallQuitSentInfo : NSObject

/// Description: Timestamp of the time when the call invitation was created.
@property (nonatomic, assign) long long createTime;

/// Description: Time stamp of the person accepting the call invitation.
@property (nonatomic, assign) long long acceptTime;

/// Description: The time stamp for me to exit the call.
@property (nonatomic, assign) long long quitTime;

@end

@interface ZIMCallEndedSentInfo : NSObject

/// Description: Timestamp of the time when the call invitation was created.
@property (nonatomic, assign) long long createTime;

/// Description: Time stamp of the person accepting the call invitation.
@property (nonatomic, assign) long long acceptTime;

/// Description: The time stamp for me to exit the call.
@property (nonatomic, assign) long long endTime;

@end

/// Information to accept the call invitation.
@interface ZIMCallInvitationReceivedInfo : NSObject

@property (nonatomic, assign) ZIMCallInvitationMode mode;

/// Description: The timeout setting of the call invitation, the unit is seconds.
@property (nonatomic, assign) unsigned int timeout;

@property (nonatomic, copy) NSString *caller;

/// Description: Inviter ID.
@property (nonatomic, copy) NSString *inviter;

@property (nonatomic, assign) long long createTime;

@property (nonatomic, strong) NSArray<ZIMCallUserInfo *> *callUserList;

/// Description: Extended field, through which the inviter can carry information to the invitee.
@property (nonatomic, copy) NSString *extendedData;

@end

@interface ZIMCallInvitationCreatedInfo : NSObject

@property (nonatomic, assign) ZIMCallInvitationMode mode;

@property (nonatomic, copy) NSString *caller;

@property (nonatomic, copy) NSString *extendedData;

@property (nonatomic, assign) unsigned int timeout;

@property (nonatomic, assign) long long createTime;

@property (nonatomic, strong) NSArray<ZIMCallUserInfo *> *callUserList;

@end

/// Cancel the call invitation message.
@interface ZIMCallInvitationCancelledInfo : NSObject

/// Description:  The inviter ID of the call invitation.
@property (nonatomic, copy) NSString *inviter;

@property (nonatomic, assign) ZIMCallInvitationMode mode;

/// Description: Extended field, through which the inviter can carry information to the invitee.
@property (nonatomic, copy) NSString *extendedData;

@end

/// Accept the call invitation message.
@interface ZIMCallInvitationAcceptedInfo : NSObject

/// Description: Invitee ID.
@property (nonatomic, copy) NSString *invitee;

/// Description: Extended field, through which the inviter can carry information to the invitee.
@property (nonatomic, copy) NSString *extendedData;

@end

/// Reject the call invitation message.
@interface ZIMCallInvitationRejectedInfo : NSObject

/// Description: Invitee ID.
@property (nonatomic, copy) NSString *invitee;

/// Description: Extended field, through which the inviter can carry information to the invitee.
@property (nonatomic, copy) NSString *extendedData;

@end

@interface ZIMCallInvitationEndedInfo : NSObject

@property (nonatomic, copy) NSString *caller;

@property (nonatomic, copy) NSString *operatedUserID;

@property (nonatomic, copy) NSString *extendedData;

@property (nonatomic, assign) ZIMCallInvitationMode mode;

@property (nonatomic, assign) long long endTime;

@end

@interface ZIMCallInvitationTimeoutInfo : NSObject

@property (nonatomic, assign) ZIMCallInvitationMode mode;

@end

@interface ZIMCallInvitationQueryConfig : NSObject

/// Description: The query number is limited to up to 100.
///
/// Required: required.
@property (nonatomic, assign) unsigned int count;

/// Description: The flag of the query list.No padding is required for the first query. After the query, callback will return a nextFlag, which can be filled in the next query to continue the query based on the last query.
/// Required: not required.
@property (nonatomic, assign) long long nextFlag;

@end

@interface ZIMCallInfo : NSObject

/// Call invite ID.
@property (nonatomic, copy) NSString *callID;

/// Call invitation initiator ID.
@property (nonatomic, copy) NSString *caller;

@property (nonatomic, copy) NSString *inviter;

/// Call invites to create a timestamp.
@property (nonatomic, assign) long long createTime;

/// Call invitation end time.
@property (nonatomic, assign) long long endTime;

/// Call invitation status.
@property (nonatomic, assign) ZIMCallState state;

/// Call invitation mode.
@property (nonatomic, assign) ZIMCallInvitationMode mode;

/// Create a call invitation additional information.
@property (nonatomic, copy) NSString *extendedData;

/// The list of call member.
@property (nonatomic, strong) NSArray<ZIMCallUserInfo *> *callUserList;

@end

@interface ZIMBlacklistQueryConfig : NSObject

/// Description: The query number is limited to up to 100.
///
/// Required: required.
@property (nonatomic, assign) unsigned int count;

/// Description: The flag of the query list.No padding is required for the first query. After the query, callback will return a nextFlag, which can be filled in the next query to continue the query based on the last query.
/// Required: not required.
@property (nonatomic, assign) unsigned int nextFlag;

@end

@interface ZIMCallUserStateChangeInfo : NSObject

/// Description: List of call members whose status changes.
@property (nonatomic, strong) NSArray<ZIMCallUserInfo *> *callUserList;

@end

@interface ZIMFriendAddConfig : NSObject

@property (nonatomic, copy) NSString *friendAlias;
@property (nonatomic, copy) NSString *wording;
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *friendAttributes;

@end

@interface ZIMFriendApplicationListQueryConfig : NSObject

@property (nonatomic, assign) unsigned int count;
@property (nonatomic, assign) unsigned int nextFlag;

@end

@interface ZIMFriendSearchConfig : NSObject

@property (nonatomic, assign) unsigned int count;
@property (nonatomic, assign) unsigned int nextFlag;
@property (nonatomic, strong) NSArray<NSString *> *keywords;
@property (nonatomic, assign) BOOL isAlsoMatchFriendAlias;

@end

@interface ZIMFriendRelationCheckConfig : NSObject

@property (nonatomic, assign) ZIMFriendRelationCheckType type;

@end

@interface ZIMFriendDeleteConfig : NSObject

@property (nonatomic, assign) ZIMFriendDeleteType type;

@end

@interface ZIMFriendListQueryConfig : NSObject

@property (nonatomic, assign) unsigned int count;
@property (nonatomic, assign) unsigned int nextFlag;

@end

@interface ZIMFriendApplicationInfo : NSObject

@property (nonatomic, strong) ZIMUserInfo *applyUser;
@property (nonatomic, copy) NSString *wording;
@property (nonatomic, assign) long long createTime;
@property (nonatomic, assign) long long updateTime;
@property (nonatomic, assign) ZIMFriendApplicationType type;
@property (nonatomic, assign) ZIMFriendApplicationState state;

@end

@interface ZIMFriendApplicationRejectConfig : NSObject

@property (nonatomic, strong) ZIMPushConfig *pushConfig;

@end

@interface ZIMFriendRelationInfo : NSObject

@property (nonatomic, assign) ZIMUserRelationType type;
@property (nonatomic, copy) NSString *userID;

@end

@interface ZIMFriendApplicationSendConfig : NSObject

@property (nonatomic, copy) NSString *wording;
@property (nonatomic, copy) NSString *friendAlias;
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *friendAttributes;
@property (nonatomic, strong) ZIMPushConfig *pushConfig;

@end

@interface ZIMFriendApplicationAcceptConfig : NSObject
@property (nonatomic, copy) NSString *friendAlias;
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *friendAttributes;
@property (nonatomic, strong) ZIMPushConfig *pushConfig;
@end

@interface ZIMFriendInfo : ZIMUserInfo

@property (nonatomic, copy) NSString *friendAlias;
@property (nonatomic, assign) long long createTime; // 添加时间
@property (nonatomic, copy) NSString *wording;
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *friendAttributes;

@end

@interface ZIMUserStatusSubscribeConfig : NSObject

@property (nonatomic, assign) unsigned int subscriptionDuration;

@end

@interface ZIMSubscribedUserStatusQueryConfig : NSObject

@property (nonatomic, strong) NSArray<NSString *> *userIDs;

@end

/// Supported version: 1.2.0 and above.
///
/// Detailed description: After the developer uploads the log, execute the callback of the result.
///
/// Use cases: Developers can use the [errorCode] in the callback to determine whether the upload is successful.
///
/// Notification timing: When the developer calls the [uploadLog] interface, the callback will be triggered.
///
/// Related interface: call log upload via [uploadLog], success or failure will be notified through this callback.
typedef void (^ZIMLogUploadedCallback)(ZIMError *errorInfo);

/// @param errorInfo Error code.
typedef void (^ZIMLoggedInCallback)(ZIMError *errorInfo);

// MARK: Main

/// Callback of the result of renewing the token.
///
/// @param token The renewed token.
/// @param errorInfo Error code.
typedef void (^ZIMTokenRenewedCallback)(NSString *token, ZIMError *errorInfo);

typedef void (^ZIMUserNameUpdatedCallback)(NSString *userName, ZIMError *errorInfo);

typedef void (^ZIMUserAvatarUrlUpdatedCallback)(NSString *userAvatarUrl, ZIMError *errorInfo);

typedef void (^ZIMUserExtendedDataUpdatedCallback)(NSString *extendedData, ZIMError *errorInfo);

typedef void (^ZIMUserCustomStatusUpdatedCallback)(NSString *customStatus, ZIMError *errorInfo);

/// Supported version: 2.0.0 and above.
///
/// Detailed description: Callback after developer queries user information.
///
/// Use cases: The developer can check whether the login succeeded by using [errorCode] in this callback.
///
/// Notification timing: This callback is triggered when a developer invokes the [queryUserInfo] interface.
///
/// Related interface: Run the queryUserInfo command to query information.
///
/// @param errorInfo  Error code.
/// @param userList  List of the userInfo queried.
/// @param errorUserList Failed to query the userInfo list.
typedef void (^ZIMUsersInfoQueriedCallback)(NSArray<ZIMUserFullInfo *> *userList,
                                            NSArray<ZIMErrorUserInfo *> *errorUserList,
                                            ZIMError *errorInfo);

//修改推送行为回调
typedef void (^ZIMUserOfflinePushRuleUpdatedCallback)(ZIMUserOfflinePushRule *offlinePushRule,
                                                      ZIMError *errorInfo);

//查询本人用户信息回调
typedef void (^ZIMSelfUserInfoQueriedCallback)(ZIMSelfUserInfo *selfUserInfo, ZIMError *errorInfo);

// MARK: Conversation

typedef void (^ZIMConversationQueriedCallback)(ZIMConversation *conversation, ZIMError *errorInfo);

/// Available since: 2.0.0 and above.
///
/// Description: After the session list is queried, the callback is used to return the query result.
///
/// Use cases: The logic after the session list query can be done in this callback.
///
/// When to call /Trigger: Description Triggered when the session list is queried.
///
/// Related APIs: [queryConversationList].
///
/// @param conversationList Session list.
/// @param errorInfo Error code for querying the conversation list. 0 means querying the session list succeeds, non-0 means querying the session list fails. If there is a failure to query the call list, the developer should check the official website error code document to query the solution.
typedef void (^ZIMConversationListQueriedCallback)(NSArray<ZIMConversation *> *conversationList,
                                                   ZIMError *errorInfo);

typedef void (^ZIMConversationTotalUnreadMessageCountQueriedCallback)(
    unsigned int unreadMessageCount, ZIMError *errorInfo);
/// Available since: 2.0.0 and above.
///
/// Description: After a session is deleted, the deletion result is returned using this callback.
///
/// Use cases: You can do the deleted session logic in this callback.
///
/// When to call /Trigger: Description Triggered after the session was deleted.
///
/// Related APIs: [[deleteConversation].
///
/// @param errorInfo Delete the error code of the call. 0 means that the session was deleted successfully, non-0 means that the session was deleted failed. If the deletion of the call fails, the developer should check the official website error code document to query the solution.
/// @param conversationID Conversation ID.
/// @param conversationType Conversation type.
typedef void (^ZIMConversationDeletedCallback)(NSString *conversationID,
                                               ZIMConversationType conversationType,
                                               ZIMError *errorInfo);

typedef void (^ZIMConversationsAllDeletedCallback)(ZIMError *errorInfo);

/// Available since: 2.0.0 and above.
///
/// Description: This callback returns the result of clearing a session if the session is not read.
///
/// Use cases: You can do clear unread logic in this callback.
///
/// When to call /Trigger: Triggered after clearing session unread.
///
/// Related APIs: [clearConversationUnreadMessageCount].
///
/// @param errorInfo Clears the error code for the number of unread calls. 0 means clearing the unread number of the session successfully, non-0 means the clearing the unread number of the session failed. If there is a failure to clear the number of unread calls, the developer should check the official website error code document to query the solution.
/// @param conversationID  Conversation ID.
/// @param conversationType Conversation type.
typedef void (^ZIMConversationUnreadMessageCountClearedCallback)(
    NSString *conversationID, ZIMConversationType conversationType, ZIMError *errorInfo);

typedef void (^ZIMConversationTotalUnreadMessageCountClearedCallback)(ZIMError *errorInfo);

/// Available since: 2.0.0 and above.
///
/// Description: This callback returns the result of group notification after the group notification status is set.
///
/// Use cases: You can do the logic after setting the group notification status in this callback.
///
/// When to call /Trigger: Triggered when the group notification status is set.
///
/// Related APIs: [setConversationNotificationStatus].
///
/// @param conversationID conversationID.
/// @param errorInfo Set the error code for the callback notification state. 0 means setting the call back notification state successfully, non-0 means setting the call back notification state fails. If there is a failure to set the callback notification status, the developer should check the official website error code document to query the solution.
/// @param conversationType Conversation type.
typedef void (^ZIMConversationNotificationStatusSetCallback)(NSString *conversationID,
                                                             ZIMConversationType conversationType,
                                                             ZIMError *errorInfo);

/// Available since: 2.5.0 and above.
///
/// Description: Set the callback interface for the read receipt conversation.
///
/// Use cases: Developers can judge whether the sending is successful through [errorCode] in the callback.
///
/// Trigger: When the developer calls the [sendConversationMessageReceiptRead] interface, this callback will be triggered.
///
/// Related APIs: The success or failure of the conversation read result set by [sendConversationMessageReceiptRead] will be notified through this callback.
///
/// @param conversationID ConversationID.
/// @param conversationType Conversation Type.
/// @param errorInfo  Set the error code of the receipt conversation read. 0 indicates that the setting is successful, and non-zero indicates that the setting fails. If the setting fails, the developer should check the [official website error code document](https://doc-zh.zego.im/article/11605) for solutions.
typedef void (^ZIMConversationMessageReceiptReadSentCallback)(NSString *conversationID,
                                                              ZIMConversationType conversationType,
                                                              ZIMError *errorInfo);

typedef void (^ZIMConversationPinnedStateUpdatedCallback)(NSString *conversationID,
                                                          ZIMConversationType conversationType,
                                                          ZIMError *errorInfo);

typedef void (^ZIMConversationPinnedListQueriedCallback)(
    NSArray<ZIMConversation *> *conversationList, ZIMError *errorInfo);

typedef void (^ZIMConversationDraftSetCallback)(NSString *conversationID,
                                                ZIMConversationType conversationType,
                                                ZIMError *errorInfo);

typedef void (^ZIMConversationMarkSetCallback)(
    NSArray<ZIMConversationBaseInfo *> *failedConversationInfos, ZIMError *errorInfo);
/// Supported Versions: 2.4.0 and above.

/// Detail description: A callback for the result of inserting a local message.

/// Business scenario: The developer can use the [errorCode] in the callback to determine whether the insertion is successful.

/// Notification timing: This callback is triggered when the developer calls the [insertMessageToLocalDB] interface.

/// Related interfaces: Insert local messages through [insertMessageToLocalDB], and the success or failure will be notified through this callback.
/// @param Message object.
/// @param errorInfo Insert the error code for the local message. 0 means inserting the message successfully, non-0 means inserting the message failed. If there is a failure to insert the message, the developer should check the official website error code document to query the solution.
typedef void (^ZIMMessageInsertedCallback)(ZIMMessage *message, ZIMError *errorInfo);

/// Callback of the result of sending the message.
///
/// Available since: 1.1.0 or above.
///
/// Description: This callback is triggered when the developer calls the [sendPeerMessage] and [sendRoomMessage] interfaces. The developer can check whether the callback is sent successfully by [errorCode] in the callback.
///
/// @param message The sent message object, from which parameters such as messageID can be obtained. If the sending fails, the messageID parameter in the message will be an empty string.
/// @param errorInfo Error information.
typedef void (^ZIMMessageSentCallback)(ZIMMessage *message, ZIMError *errorInfo);

typedef void (^ZIMMessageEditedCallback)(ZIMMessage *message, ZIMError *errorInfo);

typedef void (^ZIMSendingMessageCancelledCallback)(ZIMError *errorInfo);

/// The developer uses this callback to get a list of queried messages, which can be used to display historical messages.
///
/// @param messageList The message list of the query result.
/// @param errorInfo Error information.
typedef void (^ZIMMessageQueriedCallback)(NSString *conversationID,
                                          ZIMConversationType conversationType,
                                          NSArray<ZIMMessage *> *messageList, ZIMError *errorInfo);

/// Supported versions: 2.0.0 and above.
///
/// Detail description: After the message is deleted, the result of message deletion is returned through this callback.
///
/// Business scenario: The developer can judge whether the deletion is successful through the [errorCode] in the callback.
///
/// Notification timing: Triggered after calling the delete message interface [deleteMessage].
///
/// Related interface: [deleteMessage].
///
/// @param conversationID Conversation ID.
/// @param conversationType Conversation Type.
/// @param errorInfo Delete the error code of the message. 0 means the deletion of the message succeeded, non-0 means the deletion of the message failed. If the deletion message fails, the developer should check the official website error code document to query the solution.
typedef void (^ZIMMessageDeletedCallback)(NSString *conversationID,
                                          ZIMConversationType conversationType,
                                          ZIMError *errorInfo);

typedef void (^ZIMConversationMessagesAllDeletedCallback)(ZIMError *errorInfo);

/// Available sinces: 2.5.0 and above.
///
/// Detail description: Consequential recap of withdrawal news.
///
/// Use cases: Developer allowed through this process during response [errorCode] Successful delivery.
///
/// Trigger: For our developers [revokeMessage] When contacting, this revoke will be sent.
///
/// Related APIs: If the revoked message is the latest message of the session, the [conversationChanged] callback will be triggered, and if the message is unread, the [conversationTotalUnreadMessageCountUpdated] callback will be triggered.
/// @param message Message object.
/// @param errorInfo Retraction notice. 0 representative withdraw successfully, non 0 withdraw unsuccessful. When the withdrawal fails, the developer responds to the [official website](https://doc-zh.zego.im/article/11605) Review the solution.
typedef void (^ZIMMessageRevokedCallback)(ZIMMessage *message, ZIMError *errorInfo);

typedef void (^ZIMMessageLocalExtendedDataUpdatedCallback)(ZIMMessage *message,
                                                           ZIMError *errorInfo);

/// Available since: 2.5.0 and above.
///
/// Description: Set the callback interface for the read receipt message.
///
/// Use cases: Developers can judge whether the setting is successful through [errorCode] in the callback.
///
/// Trigger: When the developer calls the [sendMessageReceiptsRead] interface, this callback will be triggered.
///
/// Related APIs: The success or failure of the message read result set by [sendMessageReceiptsRead] will be notified through this callback.
/// @param conversationID  ConversationID.
/// @param conversationType Conversation Type.
/// @param errorMessageIDs Set the message ID corresponding to the message receipt failure.
/// @param errorInfo Set the error code of the receipt message read. 0 indicates that the setting is successful, and non-zero indicates that the setting fails. If the setting fails, the developer should check the [official website error code document](https://doc-zh.zego.im/article/11605) for solutions.
typedef void (^ZIMMessageReceiptsReadSentCallback)(NSString *conversationID,
                                                   ZIMConversationType conversationType,
                                                   NSArray<NSNumber *> *errorMessageIDs,
                                                   ZIMError *errorInfo);

/// Available since: 2.5.0 and above.
///
/// Description: Callback interface for querying receipt message information.
///
/// Use cases: Developers can judge whether the sending is successful through [errorCode] in the callback.
///
/// Trigger: When the developer calls the [queryMessageReceiptsInfo] interface, this callback will be triggered.
///
/// Related APIs: through [queryMessageReceiptsInfo], whether the result of querying the receipt information is successful or not will be notified through this callback.
/// @param infos Query receipt information.
/// @param errorMessageIDs Query the wrong message ID of the message receipt information.
/// @param errorInfo Query the error code of the message receipt. 0 means the query is successful, and non-zero means the query failed. If the query fails, the developer should check the [official website error code document](https://doc-zh.zego.im/article/11605) for solutions.
typedef void (^ZIMMessageReceiptsInfoQueriedCallback)(NSArray<ZIMMessageReceiptInfo *> *infos,
                                                      NSArray<NSString *> *errorMessageIDs,
                                                      ZIMError *errorInfo);

/// Supported versions: 2.1.0 and above.
///
/// Detail description: The progress callback for downloading media messages.
///
/// Business scenario: The developer can obtain the download progress of the media message through this callback.
///
/// Notification timing: When the developer calls the [downloadMediaFile] interface, this callback will be triggered, and will be triggered multiple times during the download process.
///
/// Related APIs: Through [downloadMediaFile], the download progress will be notified through this callback.
typedef void (^ZIMMediaDownloadedCallback)(ZIMMessage *message, ZIMError *errorInfo);

/// Supported versions: 2.1.0 and above.
///
/// Detail description: The result callback of media message download.
///
/// Business scenario: The developer can use the [errorCode] in the callback to determine whether the download is successful.
///
/// Notification timing: This callback is triggered when the developer calls the [downloadMediaFile] interface.
///
/// Related APIs: Download media messages through [downloadMediaFile], and it will be notified through this callback whether it is successful or not.
typedef void (^ZIMMediaDownloadingProgress)(ZIMMessage *message, unsigned long long currentFileSize,
                                            unsigned long long totalFileSize);

typedef void (^ZIMMessagesSearchedCallback)(NSString *conversationID,
                                            ZIMConversationType conversationType,
                                            NSArray<ZIMMessage *> *messageList,
                                            ZIMMessage *_Nullable nextMessage, ZIMError *errorInfo);

typedef void (^ZIMMessagesGlobalSearchedCallback)(NSArray<ZIMMessage *> *messageList,
                                                  ZIMMessage *_Nullable nextMessage,
                                                  ZIMError *errorInfo);

typedef void (^ZIMConversationsSearchedCallback)(
    NSArray<ZIMConversationSearchInfo *> *conversationSearchInfoList, unsigned int nextFlag,
    ZIMError *errorInfo);

typedef void (^ZIMMessageReactionDeletedCallback)(ZIMMessageReaction *reaction,
                                                  ZIMError *errorInfo);
typedef void (^ZIMMessageReactionAddedCallback)(ZIMMessageReaction *reaction, ZIMError *errorInfo);
typedef void (^ZIMMessageReactionUserListQueriedCallback)(
    ZIMMessage *message, NSArray<ZIMMessageReactionUserInfo *> *userList, NSString *reactionType,
    long long nextFlag, int totalCount, ZIMError *errorInfo);

typedef void (^ZIMCombineMessageDetailQueriedCallback)(ZIMCombineMessage *message,
                                                       ZIMError *errorInfo);

typedef void (^ZIMMessageRepliedListQueriedCallback)(NSArray<ZIMMessage *> *messageList,
                                                     long long nextFlag,
                                                     ZIMMessageRootRepliedInfo *rootRepliedInfo,
                                                     ZIMError *errorInfo);

// MARK: Room

/// Callback of the result of creating the room.
///
/// Available since: 1.1.0 or above.
///
/// Description: The callback of the result of creating the room.
///
/// Related APIs: Create a room through [createRoom], and the result of the creation will be notified through this callback.
///
/// @param roomInfo Details of the room created. If the creation fails, the roomID parameter in roomInfo will be an empty string.
/// @param errorInfo Error information.
typedef void (^ZIMRoomCreatedCallback)(ZIMRoomFullInfo *roomInfo, ZIMError *errorInfo);

/// Callback of the result of joining the room.
///
/// Available since: 1.1.0 or above.
///
/// Description: The callback of the result of joining the room.
///
/// Related APIs: Join the room through [joinRoom], and the result of joining will be notified through this callback.
///
/// @param roomInfo Details of the room joined. If the join fails, the roomID parameter in roomInfo will be an empty string.
/// @param errorInfo Error information.
typedef void (^ZIMRoomJoinedCallback)(ZIMRoomFullInfo *roomInfo, ZIMError *errorInfo);

/// Callback of the result of entering the room.
///
/// Available since: 2.1.0 or above.
///
/// Description: The callback of the result of entering the room.
///
/// Related APIs: Join the room through [enterRoom], and the result of joining will be notified through this callback.
///
/// @param roomInfo Details of the room joined. If the join fails, the roomID parameter in roomInfo will be an empty string.
/// @param errorInfo Error information.
typedef void (^ZIMRoomEnteredCallback)(ZIMRoomFullInfo *roomInfo, ZIMError *errorInfo);

typedef void (^ZIMRoomSwitchedCallback)(ZIMRoomFullInfo *roomInfo, ZIMError *errorInfo);

/// Callback of the result of leaving the room.
///
/// Available since: 1.1.0 or above.
///
/// Description: The callback of the result of leaving the room.
///
/// Related APIs: Leave the room through [leaveRoom], and the result of leaving will be notified through this callback.
///
/// @param errorInfo Error information.
typedef void (^ZIMRoomLeftCallback)(NSString *roomID, ZIMError *errorInfo);

typedef void (^ZIMRoomAllLeftCallback)(NSArray<NSString *> *roomIDs, ZIMError *errorInfo);

/// Callback of the result of querying the room members list.
///
/// Available since: 1.1.0 or above.
///
/// Description: Callback for the result of querying the room member list.
///
/// Related APIs: Query the list of room members through [queryRoomMember], and the query result will be notified through this callback.
///
/// @param memberList List of members in the room.
/// @param nextFlag The flag of the paging query. If this field is an empty string, the query has been completed. Otherwise, you need to set this value to the "nextFlag" field of ZIMRoomMemberQueryConfig for the next page query.
/// @param errorInfo Error information.
typedef void (^ZIMRoomMemberQueriedCallback)(NSString *roomID, NSArray<ZIMUserInfo *> *memberList,
                                             NSString *nextFlag, ZIMError *errorInfo);

typedef void (^ZIMRoomMembersQueriedCallback)(NSString *roomID,
                                              NSArray<ZIMRoomMemberInfo *> *memberList,
                                              NSArray<ZIMErrorUserInfo *> *errorUserList,
                                              ZIMError *errorInfo);

/// Callback of the result of querying the online members count in the room.
///
/// Available since:  1.1.0 or above.
///
/// Description: Callback of the result of querying the online members count in the room.
///
/// Related APIs: You can check the online number of people in the room through [queryRoomOnlineMemberCount].
///
/// @param count The number of online members of the room.
/// @param errorInfo Error information.
typedef void (^ZIMRoomOnlineMemberCountQueriedCallback)(NSString *roomID, unsigned int count,
                                                        ZIMError *errorInfo);

/// Callback of the result of the room attributes operation.
///
/// Available since: 1.3.0.
///
/// Description: The callback of the result of room attributes operation.
///
/// @param errorInfo Error information.
typedef void (^ZIMRoomAttributesOperatedCallback)(NSString *roomID, NSArray<NSString *> *errorKeys,
                                                  ZIMError *errorInfo);

/// Callback of the result of the room attributes operation.
///
/// Available since: 1.3.0.
///
/// Description: The callback of the result of room attributes operation.
///
/// @param errorInfo Error information.
typedef void (^ZIMRoomAttributesBatchOperatedCallback)(NSString *roomID, ZIMError *errorInfo);

/// Callback of the result of the room attributes quering.
///
/// Available since: 1.3.0.
///
/// Description: The callback of the result of room attributes operation.
///
/// @param roomAttributes Room attributes.
/// @param errorInfo Error information.
typedef void (^ZIMRoomAttributesQueriedCallback)(
    NSString *roomID, NSDictionary<NSString *, NSString *> *roomAttributes, ZIMError *errorInfo);

/// Supported version: 2.4.0.
///
/// Detail description: Returns the result of the room user attribute operation.
///
/// Business scenario: After the custom attribute operation is performed, the success or failure can be known through this callback.
///
/// Notification timing: The result is returned after the room user attribute operation is completed.
///
/// Related interface: [setRoomMembersAttributes], add or modify room user attributes.
/// @param roomID Room ID.
/// @param infos The attributes information of the room member after the operation.
/// @param errorUserList List of UserIDs with errors.
/// @param errorInfo Error code for room user attribute operation. 0 means the room user attribute operation is successful, non-0 means the room user attribute operation fails. If the room user attribute operation fails, the developer should check the official website error code document to query the solution.
typedef void (^ZIMRoomMembersAttributesOperatedCallback)(
    NSString *roomID, NSArray<ZIMRoomMemberAttributesOperatedInfo *> *infos,
    NSArray<NSString *> *errorUserList, ZIMError *errorInfo);

/// Supported version: 2.4.0.
///
/// Detailed description: According to the UserID list, batch query results of room user attributes are returned.
///
/// Business scenario: After querying room user attributes, the success or failure and query results can be known through this callback.
///
/// Notification timing: The result will be returned after the room user attribute query is completed.
///
/// Related interface: [queryRoomMembersAttributes], query room user attributes.
/// @param roomID Room ID.
/// @param infos List of room user attributes.
/// @param errorInfo Error code for room user attribute operation. 0 means the room user attribute operation is successful, non-0 means the room user attribute operation fails. If the room user attribute operation fails, the developer should check the official website error code document to query the solution.
typedef void (^ZIMRoomMembersAttributesQueriedCallback)(
    NSString *roomID, NSArray<ZIMRoomMemberAttributesInfo *> *infos, ZIMError *errorInfo);

/// Supported version: 2.4.0.
///
/// Detail description: Returns the result of paging query of all user attribute lists in the room.
///
/// Business scenario: After querying room user attributes, the success or failure and query results can be known through this callback.
///
/// Notification timing: The result will be returned after the room user attribute query is completed.
///
/// Related interface: [queryRoomMemberAttributesList], query room user attributes.
/// @param roomID Room ID.
/// @param infos List of room user attributes.
/// @param The anchor of the next paging query. If it is empty, it means that the query has been completed.
/// @param errorInfo Error code for room user attribute operation. 0 means the room user attribute operation is successful, non-0 means the room user attribute operation fails. If the room user attribute operation fails, the developer should check the official website error code document to query the solution.
typedef void (^ZIMRoomMemberAttributesListQueriedCallback)(
    NSString *roomID, NSArray<ZIMRoomMemberAttributesInfo *> *infos, NSString *nextFlag,
    ZIMError *errorInfo);
/// Description: Returns the result of the group creation operation.
///
/// Use cases: After a group creation operation is performed, the success or failure can be determined by this callback.
///
/// When to call /Trigger: The result is returned after the group creation operation is complete.
///
/// Related API: [createGroup] : creates a group.
///
/// @param groupInfo groupInfo.
/// @param userList user list.
/// @param errorUserList errorUserList.
/// @param errorInfo Error code for group creation. 0 means the group creation is successful, non-0 means the group creation fails. If there is a failure to create a group, the developer should check the official website error code document to query the solution.
typedef void (^ZIMGroupCreatedCallback)(ZIMGroupFullInfo *groupInfo,
                                        NSArray<ZIMGroupMemberInfo *> *userList,
                                        NSArray<ZIMErrorUserInfo *> *errorUserList,
                                        ZIMError *errorInfo);

/// Description: Returns the result of the group dismiss operation.
///
/// Use cases: After a group disband operation is performed, the success of the operation can be determined by the callback.
///
/// When to call /Trigger: The result of the group disband operation is returned.
///
/// Related API: [createGroup],creates a group. [dismissGroup],dismissGroup.
///
/// @param errorInfo The error code for disbanding the group. 0 means that the group is successfully disbanded, and non-0 means that the group failed to be disbanded. If there is a failure to dissolve the group, the developer should check the official website error code document to find the solution.
/// @param groupID  Group ID.
typedef void (^ZIMGroupDismissedCallback)(NSString *groupID, ZIMError *errorInfo);

/// Description: Returns the result of the group join operation.
///
/// Use cases: After a group join operation is performed, the success or failure can be determined by this callback.
///
/// When to call /Trigger: The result of the group join operation is returned.
///
/// Related API:[joinGroup] : joins a group. [leaveGroup], leave the group.
typedef void (^ZIMGroupJoinedCallback)(ZIMGroupFullInfo *groupInfo, ZIMError *errorInfo);

/// Description: Returns the result of the group departure operation.
///
/// Use cases: After a group exit operation is performed, the success or failure can be determined by the callback.
///
/// When to call /Trigger: The result of the group departure operation is returned.
///
/// Related API:[leaveGroup], leave the group. [joinGroup], enter the group.
typedef void (^ZIMGroupLeftCallback)(NSString *groupID, ZIMError *errorInfo);

/// Description: Returns the result of inviting the user to join the group.
///
/// Use cases: After a user is invited to a group, the success or failure can be determined by the callback.
///
/// When to call /Trigger: Results are returned after the user is invited to the group.
///
/// Related API:[inviteUsersIntoGroup] invites users to join the group.
typedef void (^ZIMGroupUsersInvitedCallback)(NSString *groupID,
                                             NSArray<ZIMGroupMemberInfo *> *userList,
                                             NSArray<ZIMErrorUserInfo *> *errorUserList,
                                             ZIMError *errorInfo);

/// Description: Returns the result of the kick out group member operation.
///
/// Use cases: After a group member is kicked out, the success or failure can be determined by the callback.
///
/// When to call /Trigger: The result is returned after the group member is kicked out.
///
/// Related API:[kickGroupMembers] Kick out group members.
typedef void (^ZIMGroupMemberKickedCallback)(NSString *groupID,
                                             NSArray<NSString *> *kickedUserIDList,
                                             NSArray<ZIMErrorUserInfo *> *errorUserList,
                                             ZIMError *errorInfo);

/// Description: Returns the result of the group master transfer operation.
///
/// Use cases: After a group master transfer operation is performed, the success of the operation can be determined by this callback.
///
/// When to call /Trigger: The result of the group master transfer operation is returned.
///
/// Related API:[transferGroupOwner], group master transfer.
typedef void (^ZIMGroupOwnerTransferredCallback)(NSString *groupID, NSString *toUserID,
                                                 ZIMError *errorInfo);

/// Description: Return result of group name update operation.
///
/// Use cases: After a group name update operation is performed, the success or failure can be determined by this callback.
///
/// When to call /Trigger: The result of the group name update operation is returned.
///
/// Related API:[updateGroupName], the group name is updated.
typedef void (^ZIMGroupNameUpdatedCallback)(NSString *groupID, NSString *groupName,
                                            ZIMError *errorInfo);

typedef void (^ZIMGroupAvatarUrlUpdatedCallback)(NSString *groupID, NSString *groupAvatarUrl,
                                                 ZIMError *errorInfo);

typedef void (^ZIMGroupAliasUpdatedCallback)(NSString *groupID, NSString *groupAlias,
                                             ZIMError *errorInfo);

typedef void (^ZIMGroupMutedCallback)(NSString *groupID, BOOL isMute, ZIMGroupMuteInfo *info,
                                      ZIMError *errorInfo);

typedef void (^ZIMGroupMembersMutedCallback)(NSString *groupID, BOOL isMute, int duration,
                                             NSArray<NSString *> *mutedMemberIDs,
                                             NSArray<ZIMErrorUserInfo *> *errorUserList,
                                             ZIMError *errorInfo);

typedef void (^ZIMGroupMemberMutedListQueriedCallback)(NSString *groupID,
                                                       unsigned long long nextFlag,
                                                       NSArray<ZIMGroupMemberInfo *> *userList,
                                                       ZIMError *errorInfo);

/// Description: Return result of group name update operation.
///
/// Use cases: After a group name update operation is performed, the success or failure can be determined by this callback.
///
/// When to call /Trigger: The result of the group name update operation is returned.
///
/// Related API:[updateGroupName], the group name is updated.
typedef void (^ZIMGroupNoticeUpdatedCallback)(NSString *groupID, NSString *groupNotice,
                                              ZIMError *errorInfo);

/// Description: Returns the result of the group dismiss operation.
///
/// Use cases: After a group disband operation is performed, the success of the operation can be determined by the callback.
///
/// When to call /Trigger: The result of the group disband operation is returned.
///
/// Related API: [createGroup],creates a group. [dismissGroup],dismissGroup.
typedef void (^ZIMGroupInfoQueriedCallback)(ZIMGroupFullInfo *groupInfo, ZIMError *errorInfo);

/// Description: Returns the result of the group property operation.
///
/// Use cases: This callback tells you whether a custom property operation is successful.
///
/// When to call /Trigger: The result of the group property operation is returned.
///
/// Related API: [setGroupAttributes], set the room properties. [deleteGroupAttributes], delete the room attribute.
typedef void (^ZIMGroupAttributesOperatedCallback)(NSString *groupID,
                                                   NSArray<NSString *> *errorKeys,
                                                   ZIMError *errorInfo);

/// Description: Returns the result of group attribute query.
///
/// Use cases: This callback is used to determine the success of a custom property query.
///
/// When to call /Trigger: Group attribute query results are returned.
///
/// Related API: [queryGroupAttributes], query room attributes.
typedef void (^ZIMGroupAttributesQueriedCallback)(
    NSString *groupID, NSDictionary<NSString *, NSString *> *groupAttributes, ZIMError *errorInfo);

/// Description: Return of the result of the member role update operation.
///
/// Use cases: After a member role update operation is performed, the success or failure can be determined by this callback.
///
/// When to call /Trigger: The result of the member role update operation is returned.
///
/// Related API:[setGroupMemberRole], the member role is updated.
typedef void (^ZIMGroupMemberRoleUpdatedCallback)(NSString *groupID, NSString *forUserID,
                                                  ZIMGroupMemberRole role, ZIMError *errorInfo);

/// Description: Return result of group member nickname update operation.
///
/// Use cases: After a group member nickname update operation is performed, the success or failure can be determined by this callback.
///
/// When to call /Trigger: The result of the group member nickname update operation is returned.
///
/// Related API:[setGroupMemberNickname], the nickname of the group member is updated.
typedef void (^ZIMGroupMemberNicknameUpdatedCallback)(NSString *groupID, NSString *forUserID,
                                                      NSString *nickname, ZIMError *errorInfo);

/// Description: Return of group member query results.
///
/// Use cases: After a group member query is performed, the success or failure can be determined by this callback.
///
/// When to call /Trigger: Group member query results are returned.
///
/// Related API:[queryGroupMemberInfo], queryGroupMemberInfo.
typedef void (^ZIMGroupMemberInfoQueriedCallback)(NSString *groupID, ZIMGroupMemberInfo *userInfo,
                                                  ZIMError *errorInfo);

/// Description: Returns the group list query result.
///
/// Use cases: The success of a group list query can be determined by this callback.
///
/// When to call /Trigger: The result of the group list query is returned.
///
/// Related API:[queryGroupList] to query the group list.
typedef void (^ZIMGroupListQueriedCallback)(NSArray<ZIMGroup *> *groupList, ZIMError *errorInfo);

/// Description: Returns the result of querying the group member list.
///
/// Use cases: After querying the group member list, you can use the callback to determine whether the query is successful.
///
/// When to call /Trigger: The result is displayed after the group member list is queried.
///
/// Related API:[queryGroupMemberList], query the group member list.
typedef void (^ZIMGroupMemberListQueriedCallback)(NSString *groupID,
                                                  NSArray<ZIMGroupMemberInfo *> *userList,
                                                  unsigned int nextFlag, ZIMError *errorInfo);

typedef void (^ZIMGroupMemberCountQueriedCallback)(NSString *groupID, unsigned int count,
                                                   ZIMError *errorInfo);

typedef void (^ZIMGroupMessageReceiptMemberListQueriedCallback)(
    NSString *groupID, NSArray<ZIMGroupMemberInfo *> *userList, unsigned int nextFlag,
    ZIMError *errorInfo);

typedef void (^ZIMGroupsSearchedCallback)(NSArray<ZIMGroupSearchInfo *> *groupSearchInfoList,
                                          unsigned int nextFlag, ZIMError *errorInfo);

typedef void (^ZIMGroupMembersSearchedCallback)(NSString *groupID,
                                                NSArray<ZIMGroupMemberInfo *> *userList,
                                                unsigned int nextFlag, ZIMError *errorInfo);

typedef void (^ZIMGroupJoinModeUpdatedCallback)(NSString *groupID, ZIMGroupJoinMode mode,
                                                ZIMError *errorInfo);

typedef void (^ZIMGroupInviteModeUpdatedCallback)(NSString *groupID, ZIMGroupInviteMode mode,
                                                  ZIMError *errorInfo);

typedef void (^ZIMGroupBeInviteModeUpdatedCallback)(NSString *groupID, ZIMGroupBeInviteMode mode,
                                                    ZIMError *errorInfo);

typedef void (^ZIMGroupJoinApplicationSentCallback)(NSString *groupID, ZIMError *errorInfo);

typedef void (^ZIMGroupJoinApplicationAcceptedCallback)(NSString *groupID, NSString *userID,
                                                        ZIMError *errorInfo);

typedef void (^ZIMGroupJoinApplicationRejectedCallback)(NSString *groupID, NSString *userID,
                                                        ZIMError *errorInfo);

typedef void (^ZIMGroupInviteApplicationsSentCallback)(NSString *groupID,
                                                       NSArray<ZIMErrorUserInfo *> *errorUserList,
                                                       ZIMError *errorInfo);

typedef void (^ZIMGroupInviteApplicationAcceptedCallback)(ZIMGroupFullInfo *groupInfo,
                                                          NSString *inviterUserID,
                                                          ZIMError *errorInfo);

typedef void (^ZIMGroupInviteApplicationRejectedCallback)(NSString *groupID,
                                                          NSString *inviterUserID,
                                                          ZIMError *errorInfo);

typedef void (^ZIMGroupApplicationListQueriedCallback)(
    NSArray<ZIMGroupApplicationInfo *> *applicationList, unsigned int nextFlag,
    ZIMError *errorInfo);

/// Supported version: 2.0.0.
///
/// Detail description: Operation callback for sending a call invitation.
///
/// Business scenario: After the operation of sending a call invitation is performed, the success or failure can be known through this callback.
///
/// Notification timing: The result is returned after the operation of sending the call invitation is completed.
///
/// Related interface: [callInvite], send a call invitation.
typedef void (^ZIMCallInvitationSentCallback)(NSString *callID, ZIMCallInvitationSentInfo *info,
                                              ZIMError *errorInfo);

/// Callback result of inviting the current call.
typedef void (^ZIMCallingInvitationSentCallback)(NSString *callID,
                                                 ZIMCallingInvitationSentInfo *info,
                                                 ZIMError *errorInfo);

/// Supported version: 2.0.0.
///
/// Detail description: The operation callback for canceling the call invitation.
///
/// Business scenario: After canceling the call invitation operation, the success or failure can be known through this callback.
///
/// Notification timing: The result is returned after the cancel call invitation operation is completed.
///
/// Related interface: [callCancel], cancel the call invitation.
typedef void (^ZIMCallCancelSentCallback)(NSString *callID, NSArray<NSString *> *errorInvitees,
                                          ZIMError *errorInfo);

/// Supported version: 2.0.0.
///
/// Detail description: The operation callback for accepting the call invitation.
///
/// Business scenario: After accepting the call invitation operation, the success or failure can be known through this callback.
///
/// Notification timing: The result will be returned after accepting the call invitation operation.
///
/// Related interface: [callAccept], accept the call invitation.
typedef void (^ZIMCallAcceptanceSentCallback)(NSString *callID, ZIMError *errorInfo);

/// Supported version: 2.0.0.
///
/// Detail description: Operation callback for rejecting the call invitation.
///
/// Business scenario: After the operation of rejecting the call invitation is performed, the success or failure can be known through this callback.
///
/// Notification timing: The result is returned after the operation of rejecting the call invitation is completed.
///
/// Related interface: [callReject], rejects the call invitation.
typedef void (^ZIMCallRejectionSentCallback)(NSString *callID, ZIMError *errorInfo);

typedef void (^ZIMCallJoinSentCallback)(NSString *callID, ZIMCallJoinSentInfo *info,
                                        ZIMError *errorInfo);

/// Quit the callback closure of the call invitation operation.
typedef void (^ZIMCallQuitSentCallback)(NSString *callID, ZIMCallQuitSentInfo *info,
                                        ZIMError *errorInfo);

/// Result callback of ending the call invitation.
typedef void (^ZIMCallEndSentCallback)(NSString *callID, ZIMCallEndedSentInfo *info,
                                       ZIMError *errorInfo);

typedef void (^ZIMCallInvitationListQueriedCallback)(NSArray<ZIMCallInfo *> *callList,
                                                     long long nextFlag, ZIMError *errorInfo);

/// Result Callback of querying the call list.
typedef void (^ZIMBlacklistUsersAddedCallback)(NSArray<ZIMErrorUserInfo *> *errorUserList,
                                               ZIMError *errorInfo);

typedef void (^ZIMBlacklistUsersRemovedCallback)(NSArray<ZIMErrorUserInfo *> *errorUserList,
                                                 ZIMError *errorInfo);

typedef void (^ZIMBlacklistQueriedCallback)(NSArray<ZIMUserInfo *> *blacklist,
                                            unsigned int nextFlag, ZIMError *errorInfo);

typedef void (^ZIMBlacklistCheckedCallback)(BOOL isUserInBlacklist, ZIMError *errorInfo);

typedef void (^ZIMMessageImportedCallback)(ZIMError *errorInfo);

typedef void (^ZIMMessageImportingProgress)(unsigned long long importedMessageCount,
                                            unsigned long long totalMessageCount);

typedef void (^ZIMMessageExportedCallback)(ZIMError *errorInfo);

typedef void (^ZIMMessageExportingProgress)(unsigned long long exportedMessageCount,
                                            unsigned long long totalMessageCount);

typedef void (^ZIMFileCacheClearedCallback)(ZIMError *errorInfo);

typedef void (^ZIMFileCacheQueriedCallback)(ZIMFileCacheInfo *fileCacheInfo, ZIMError *errorInfo);
typedef void (^ZIMFriendApplicationListQueriedCallback)(
    NSArray<ZIMFriendApplicationInfo *> *applicationList, unsigned int nextFlag,
    ZIMError *errorInfo);

typedef void (^ZIMFriendListQueriedCallback)(NSArray<ZIMFriendInfo *> *friendList,
                                             unsigned int nextFlag, ZIMError *errorInfo);

typedef void (^ZIMFriendApplicationRejectedCallback)(ZIMUserInfo *userInfo, ZIMError *errorInfo);

typedef void (^ZIMFriendApplicationAcceptedCallback)(ZIMFriendInfo *friendInfo,
                                                     ZIMError *errorInfo);

typedef void (^ZIMFriendsInfoQueriedCallback)(NSArray<ZIMFriendInfo *> *friendInfos,
                                              NSArray<ZIMErrorUserInfo *> *errorUserList,
                                              ZIMError *errorInfo);

typedef void (^ZIMFriendAttributesUpdatedCallback)(ZIMFriendInfo *friendInfo, ZIMError *errorInfo);

typedef void (^ZIMFriendAliasUpdatedCallback)(ZIMFriendInfo *friendInfo, ZIMError *errorInfo);

typedef void (^ZIMFriendsRelationCheckedCallback)(NSArray<ZIMFriendRelationInfo *> *relationInfos,
                                                  NSArray<ZIMErrorUserInfo *> *errorUserList,
                                                  ZIMError *errorInfo);

typedef void (^ZIMFriendsDeletedCallback)(NSArray<ZIMErrorUserInfo *> *errorUserList,
                                          ZIMError *errorInfo);

typedef void (^ZIMFriendApplicationSentCallback)(ZIMFriendApplicationInfo *applicationInfo,
                                                 ZIMError *errorInfo);

typedef void (^ZIMFriendAddedCallback)(ZIMFriendInfo *friendInfo, ZIMError *errorInfo);

typedef void (^ZIMFriendsSearchedCallback)(NSArray<ZIMFriendInfo *> *friendInfos,
                                           unsigned int nextFlag, ZIMError *errorInfo);

typedef void (^ZIMUsersStatusQueriedCallback)(NSArray<ZIMUserStatus *> *userStatusList,
                                              NSArray<ZIMErrorUserInfo *> *errorUserList,
                                              ZIMError *errorInfo);

typedef void (^ZIMUsersStatusSubscribedCallback)(NSArray<ZIMErrorUserInfo *> *errorUserList,
                                                 ZIMError *errorInfo);

typedef void (^ZIMUsersStatusUnsubscribedCallback)(NSArray<ZIMErrorUserInfo *> *errorUserList,
                                                   ZIMError *errorInfo);

typedef void (^ZIMSubscribedUserStatusListQueriedCallback)(
    NSArray<ZIMUserStatusSubscription *> *userStatusSubscriptionList, ZIMError *errorInfo);

NS_ASSUME_NONNULL_END
