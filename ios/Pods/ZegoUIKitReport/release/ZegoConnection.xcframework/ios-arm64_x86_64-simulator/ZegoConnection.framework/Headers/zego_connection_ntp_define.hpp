#ifndef zego_connection_ntp_define_hpp
#define zego_connection_ntp_define_hpp

#include "zego_connection_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{
    struct NTPSyncData
	{
		uint32 uCode = 0;
		double offset = 0.0;
        double maxDeviation = 0.0;
        bool bFinished = true;
	};

    struct StartNTPSyncConfig
    {
        uint32 maxSendNum = 8;
        uint32 minSendNum = 8;
        uint32 minRecvNum = 5;
        uint32 interval = 1000;
        uint32 packageTimeOut = 2000;
        uint32 syncTimeOut = 30000;
        uint32 maxSuccessedNum = 10;
        uint32 referenceAccuracy = 50;
    };

	using OnStartNTPSyncDelegate = std::function<void(const NTPSyncData& data)>;

}// CONNECTION
}// ZEGO


#endif /* zego_connection_ntp_define_hpp */
