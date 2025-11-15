//
//  ZIMErrorCode.h
//  ZIM
//
//  Copyright © 2021 Zego. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// The define of error code.
///
/// Description: Developers can find the details of the error code in the developer documentation website according to the error code.
///
/// Use cases: It can be used to collect and record errors in the process of using the SDK.
///
typedef NS_ENUM(NSUInteger, ZIMErrorCode) {
    ZIMErrorCodeSuccess = 0,
    ZIMErrorCodeFailed = 1,

    ZIMErrorCodeCommonModuleParamsInvalid = 6000001,
    ZIMErrorCodeCommonModuleEngineNotInit = 6000002,
    ZIMErrorCodeCommonModuleInvalidAppID = 6000003,
    ZIMErrorCodeCommonModuleTriggerSDKFrequencyLimit = 6000004,
    ZIMErrorCodeCommonModuleTriggerServerFrequencyLimit = 6000005,
    ZIMErrorCodeCommonModuleSwitchServerError = 6000006,
    ZIMErrorCodeCommonModuleIMServerError = 6000007,
    ZIMErrorCodeCommonModuleIMDataBaseError = 6000008,
    ZIMErrorCodeCommonModuleImServerDisconnect = 6000009,
    ZIMErrorCodeCommonModuleUploadLogError = 6000010,
    ZIMErrorCodeCommonModuleUserIsNotExist = 6000011,
    ZIMErrorCodeCommonModuleUserInfoQueriedLimit = 6000012,
    ZIMErrorCodeCommonModuleUnsupportedRequest = 6000013,
    ZIMErrorCodeCommonModuleExceedDAULimit = 6000015,
    ZIMErrorCodeCommonModuleExceedMAULimit = 6000016,

    ZIMErrorCodeNetworkModuleCommonError = 6000101,
    ZIMErrorCodeNetworkModuleServerError = 6000102,
    ZIMErrorCodeNetworkModuleTokenInvalid = 6000103,
    ZIMErrorCodeNetworkModuleNetworkError = 6000104,
    ZIMErrorCodeNetworkModuleRequestTimeout = 6000105,
    ZIMErrorCodeNetworkModuleTokenExpired = 6000106,
    ZIMErrorCodeNetworkModuleTokenVersionError = 6000107,
    ZIMErrorCodeNetworkModuleTokenTimeIsTooShort = 6000108,
    ZIMErrorCodeNetworkModuleUserHasAlreadyLogged = 6000111,
    ZIMErrorCodeNetworkModuleUserIsNotLogged = 6000121,
    ZIMErrorCodeNetworkModuleUserHasAlreadyLoggedOut = 6000122,
    ZIMErrorCodeNetworkModuleUserOffline = 6000123,
    ZIMErrorCodeNetworkModuleUserIDError = 6000124,

    ZIMErrorCodeMessageModuleCommonError = 6000201,
    ZIMErrorCodeMessageModuleServerError = 6000202,
    ZIMErrorCodeMessageModuleSendMessageFailed = 6000203,
    ZIMErrorCodeMessageModuleTargetDoseNotExist = 6000204,
    ZIMErrorCodeMessageModuleEditMessageFailed = 6000205,
    ZIMErrorCodeMessageModuleSendMessageFailedForCancelled = 6000206,
    ZIMErrorCodeMessageModuleCancelSendingMessageFailed = 6000207,
    ZIMErrorCodeMessageModuleFileError = 6000210,
    ZIMErrorCodeMessageModuleFileNotExist = 6000211,
    ZIMErrorCodeMessageModuleFileServerError = 6000212,
    ZIMErrorCodeMessageModuleFileTypeUnsupported = 6000213,
    ZIMErrorCodeMessageModuleFileSizeInvalid = 6000214,
    ZIMErrorCodeMessageModuleFileDurationInvalid = 6000215,
    ZIMErrorCodeMessageModuleFilePermissionDenied = 6000216,
    ZIMErrorCodeMessageModuleFileDownloadFailed = 6000217,
    ZIMErrorCodeMessageModuleFileDownloadLimit = 6000218,
    ZIMErrorCodeMessageModuleFileDownloadUrlNotFound = 6000219,
    ZIMErrorCodeMessageModuleFileDownloadHttpRequestServerError = 6000220,
    ZIMErrorCodeMessageModuleAuditRejected = 6000221,
    ZIMErrorCodeMessageModuleAuditFailed = 6000222,
    ZIMErrorCodeMessageModuleAuditCustomSentRejected = 6000230,
    ZIMErrorCodeMessageModuleMuted = 6000231,
    ZIMErrorCodeMessageModuleCacheFileOpenFailed = 6000240,
    ZIMErrorCodeMessageModuleCacheFileExportOrImportIsInProgressCurrently = 6000241,
    ZIMErrorCodeMessageModuleCacheFileOnlyCanImportCacheOfOneself = 6000242,
    ZIMErrorCodeMessageModuleCacheFileExportFailed = 6000243,
    ZIMErrorCodeMessageModuleCacheFileImportFailed = 6000244,
    ZimErrorCodeMessageModuleCacheFileJsonParseFailed = 6000245,
    ZIMErrorCodeMessageModuleFileConnectError = 6000251,
    ZIMErrorCodeMessageModuleFileHttpError = 6000252,
    ZIMErrorCodeMessageModuleFileVerificationError = 6000253,
    ZIMErrorCodeMessageModuleFileClientError = 6000254,
    ZIMErrorCodeMessageModuleCallError = 6000270,
    ZIMErrorCodeMessageModuleCancelCallError = 6000271,
    ZIMErrorCodeMessageModuleCallServerError = 6000272,
    ZIMErrorCodeMessageModuleIsNotInvitor = 6000273,
    ZIMErrorCodeMessageModuleIsNotInvitee = 6000274,
    ZIMErrorCodeMessageModuleCallAlreadyExists = 6000275,
    ZIMErrorCodeMessageModuleCallDoseNotExist = 6000276,
    ZIMErrorCodeMessageModuleReceiptReadError = 6000277,
    ZIMErrorCodeMessageModuleMessageExceedsRevokeTime = 6000278,
    ZIMErrorCodeMessageModuleMessageHasBeenRevoked = 6000279,
    ZIMErrorCodeMessageModuleMessageReactionTypeExisted = 6000280,
    ZIMErrorCodeMessageModuleCallInviteUserDoesNotExist = 6000281,
    ZIMErrorCodeMessageModuleMessageReceiptLimit = 6000282,
    ZIMErrorCodeMessageModuleUserAlreadyInTheCall = 6000283,
    ZIMErrorCodeMessageModuleSenderInBlacklist = 6000284,
    ZIMErrorCodeMessageModuleNoCorrespondingOperationAuthority = 6000285,
    ZIMErrorCodeMessageModuleMessageExceedsEditTime = 6000286,

    ZIMErrorCodeRoomModuleCommonError = 6000301,
    ZIMErrorCodeRoomModuleServerError = 6000302,
    ZIMErrorCodeRoomModuleCreateRoomError = 6000303,
    ZIMErrorCodeRoomModuleJoinRoomError = 6000304,
    ZIMErrorCodeRoomModuleLeaveRoomError = 6000306,
    ZIMErrorCodeRoomModuleRoomMemberQueryFailed = 6000310,
    ZIMErrorCodeRoomModuleRoomMemberQueryFailedCompletely = 6000311,
    ZIMErrorCodeRoomModuleUserIsAlreadyInTheRoom = 6000320,
    ZIMErrorCodeRoomModuleUserIsNotInTheRoom = 6000321,
    ZIMErrorCodeRoomModuleTheRoomDoseNotExist = 6000322,
    ZIMErrorCodeRoomModuleTheRoomAlreadyExists = 6000323,
    ZIMErrorCodeRoomModuleTheNumberOfExistingRoomsHasReachedLimit = 6000324,
    ZIMErrorCodeRoomModuleTheNumberOfJoinedRoomsHasReachedLimit = 6000325,
    ZIMErrorCodeRoomModuleTheRoomIsConnecting = 6000326,
    ZIMErrorCodeRoomModuleRoomAttributesCommonError = 6000330,
    ZIMErrorCodeRoomModuleRoomAttributesOperationFailedCompletely = 6000331,
    ZIMErrorCodeRoomModuleRoomAttributesOperationFailedPartly = 6000332,
    ZIMErrorCodeRoomModuleRoomAttributesQueryFailed = 6000333,
    ZIMErrorCodeRoomModuleTheNumberOfRoomAttributesExceedsLimit = 6000334,
    ZIMErrorCodeRoomModuleTheLengthOfRoomAttributeKeyExceedsLimit = 6000335,
    ZIMErrorCodeRoomModuleTheLengthOfRoomAttributeValueExceedsLimit = 6000336,
    ZIMErrorCodeRoomModuleTheTotalLengthOfRoomAttributesValueExceedsLimit = 6000337,
    ZIMErrorCodeRoomModuleRoomMemberAttributesCommonError = 6000350,
    ZIMErrorCodeRoomModuleTheTotalLengthOfRoomMemberAttributesExceedsLimit = 6000351,
    ZIMErrorCodeRoomModuleTheLengthOfRoomMemberAttributesKeyExceedsLimit = 6000352,
    ZIMErrorCodeRoomModuleTheLengthOfRoomMemberAttributesValueExceedsLimit = 6000353,
    ZIMErrorCodeRoomModuleTheMemberNumberOfRoomMemberAttributesExceedsLimit = 6000357,
    ZIMErrorCodeZPNSModulePushIDInvalid = 6000401,

    ZIMErrorCodeGroupModuleCommonError = 6000501,
    ZIMErrorCodeGroupModuleServerError = 6000502,
    ZIMErrorCodeGroupModuleCreateGroupError = 6000503,
    ZIMErrorCodeGroupModuleDismissGroupError = 6000504,
    ZIMErrorCodeGroupModuleJoinGroupError = 6000505,
    ZIMErrorCodeGroupModuleLeaveGroupError = 6000506,
    ZIMErrorCodeGroupModuleKickoutGroupMemberError = 6000507,
    ZIMErrorCodeGroupModuleInviteUserIntoGroupError = 6000508,
    ZIMErrorCodeGroupModuleTransferOwnerError = 6000509,
    ZIMErrorCodeGroupModuleUpdateGroupInfoError = 6000510,
    ZIMErrorCodeGroupModuleQueryGroupInfoError = 6000511,
    ZIMErrorCodeGroupModuleGroupAttributesOperationFailed = 6000512,
    ZIMErrorCodeGroupModuleGroupAttributesQueryFailed = 6000513,
    ZIMErrorCodeGroupModuleUpdateGroupMemberInfoError = 6000514,
    ZIMErrorCodeGroupModuleQueryGroupMemberInfoError = 6000515,
    ZIMErrorCodeGroupModuleQueryGroupListError = 6000516,
    ZIMErrorCodeGroupModuleQueryGroupMemberListError = 6000517,
    ZIMErrorCodeGroupModuleUserIsNotInTheGroup = 6000521,
    ZIMErrorCodeGroupModuleMemberIsAlreadyInTheGroup = 6000522,
    ZIMErrorCodeGroupModuleGroupDoseNotExist = 6000523,
    ZIMErrorCodeGroupModuleGroupAlreadyExists = 6000524,
    ZIMErrorCodeGroupModuleGroupMemberHasReachedLimit = 6000525,
    ZIMErrorCodeGroupModuleGroupAttributeDoseNotExist = 6000526,
    ZIMErrorCodeGroupModuleGroupWithDismissed = 6000527,
    ZIMErrorCodeGroupModuleTheNumberOfGroupAttributesExceedsLimit = 6000531,
    ZIMErrorCodeGroupModuleTheLengthOfGroupAttributeKeyExceedsLimit = 6000532,
    ZIMErrorCodeGroupModuleTheLengthOfGroupAttributeValueExceedsLimit = 6000533,
    ZIMErrorCodeGroupModuleTheTotalLengthOfGroupAttributeValueExceedsLimit = 6000534,
    ZIMErrorCodeGroupModuleNoCorrespondingOperationAuthority = 6000541,
    ZIMErrorCodeGroupModuleForbidJoinGroupError = 6000542,

    ZIMErrorCodeConversationModuleCommonError = 6000601,
    ZIMErrorCodeConversationModuleServerError = 6000602,
    ZIMErrorCodeConversationModuleConversationDoseNotExist = 6000603,
    ZIMErrorCodeConversationModuleConversationPinnedListReachedLimit = 6000604,

    ZIMErrorCodeDataBaseModuleOpenDataBaseError = 6000701,
    ZIMErrorCodeDataBaseModuleModifyDataBaseError = 6000702,
    ZIMErrorCodeDataBaseModuleDeleteDataBaseError = 6000703,
    ZIMErrorCodeDataBaseModuleSeleteDataBaseError = 6000704,

    // 好友关系链模块错误码
    ZIMErrorCodeFriendModuleFriendsNumLimit = 6000801,
    // 同意或者拒绝好友申请状态错误
    ZIMErrorCodeFriendModuleFriend_application_status_error = 6000802,
    // 对方已经是好友
    ZIMErrorCodeFriendModuleIsAlreadyYourFriend = 6000803,
    // 已经在黑名单内
    ZIMErrorCodeFriendModuleAlreadyAddToBlacklist = 6000804,
    // 不能添加自己到黑名单
    ZIMErrorCodeFriendModuleCannotAddSelfToBlacklist = 6000805,
    // 已经从黑名单中删除
    ZIMErrorCodeFriendModuleAlreadyDeleteFromBlacklist = 6000806,
    // 该用户不在黑名单里
    ZIMErrorCodeFriendModuleUserNotInBlacklist = 6000807,
    // 黑名单限制
    ZIMErrorCodeFriendModuleBlacklistListQuantityLimit = 6000808,
    // 好友列表超出限制
    ZIMErrorCodeFriendModuleFriendOperationLimitExceeded = 6000809,
    // 不允许添加自己
    ZIMErrorCodeFriendModuleCannotAddSelfToFriendList = 6000810,
    // 该好友没注册
    ZIMErrorCodeFriendModuleFriendAreUnregistered = 6000811,
    // 不是你的好友
    ZIMErrorCodeFriendModuleNotYourFriend = 6000812,
    // 不允许删除自己
    ZIMErrorCodeFriendModuleCannotDeleteSelf = 6000813,
    // 申请已经过期
    ZIMErrorCodeFriendModuleFriendApplicationExpired = 6000814,
    ZIMErrorCodeFriendModuleAddBlacklistFail = 6000815,
    ZIMErrorCodeFriendModuleDelBlacklistFail = 6000816,

};

NS_ASSUME_NONNULL_END
