#ifndef zego_connection_http_define_hpp
#define zego_connection_http_define_hpp

#include "zego_connection_define.hpp"
#include "zego_connection_dns_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{
	class HttpClient;
    using ConstHttpClientPtr = std::shared_ptr<HttpClient>;
    
	//http通信过程中数据，用于上报
    struct HttpDetailData
    {
        std::string url = "";
        
        uint64 startTime = 0;
        uint64 finishTime = 0;
        
        std::string originIP = "";     //设置的connect ip address
        std::string primaryIP = "";    //curl返回的get primary ip
        
        double namelookupTime = 0.0;      //从起始至域名解析完成时间，秒
        double connectTime = 0.0;         //从起始至TCP连接建立完成时间，秒
        double appConnectTime = 0.0;      //从起始至SSL连接/握手完成时间，秒
        double preTransferTime = 0.0;     //从起始至准备开始传输数据时间，秒
        double startTransferTime = 0.0;   //从起始至开始传输数据（收到第一个字节）时间，秒
        double totalTime = 0.0;           //总时间，秒
        
        double uploadSpeed = 0.0;         //平均上传速度，bytes/second
        double downloadSpeed = 0.0;       //平均下载速度，bytes/second
        double uploadBytes = 0.0;         //上传数据量
        double downloadBytes = 0.0;       //下载数据量
        double uploadContentLenth = 0.0;  //上传主体内容长度
        double downloadContentLenth = 0.0;//下载主体内容长度
        
        std::string localIP = "";           //本地IP
        uint16 localPort = 0;               //本地端口
        
        uint32 curlError = 0;               //libcurl库的错误码，可能没有
        uint32 statusCode = 0;              //本次执行http返回错误码
        bool isForceNewConnection = false; //是否强制创建新连接，libcurl库使用，可能没有
    };

	struct HttpCollectedData
	{
        std::string requestUrl = "";    //原始请求的url
        
        std::vector<HttpDetailData> detailList;
        std::vector<DNSData> dnsResult;   //请求的dns结果
	};

	//请求进度信息
	struct ProgressInfo
	{
		long totalDownload = 0;    //期望下载总量（字节）
		long alreadyDownload = 0;  //已下载量（字节）
		long currentDownload = 0;  //本次下载量（字节）
		long totalUpload = 0;      //期望上传总量（字节）
		long alreadyUpload = 0;    //已上传总量（字节）
		long currentUpload = 0;    //本次上传总量（字节）
	};

    //代理信息
    struct HttpProxyConfig
    {
        struct IPInfo {
            std::string address;    //代理服务地址，形如：socks5://192.168.100.49:1080
            std::string userName;   //如果开启了用户帐号验证，则需要填入用户名，如admin，否则填空
            std::string password;   //如果开启了用户帐号验证，则需要填入用户密码，如bigsecret，否则填空
        };
        std::vector<IPInfo> ipList;
        bool enable = false;    //是否启用代理，默认关闭
        bool isCloudProxy = false;
    };
      
    /**
    http请求
    
    @param seq 本次请求序列号
    @param client 请求client，透传出去，用于后续调用
    @return 返回请求执行结果 
    @notce 
    */
    using OnHttpRequestDelegate = std::function<ZCError(ZCSeq seq, ConstHttpClientPtr client)>;

    /**
    http响应
    
    @param seq 响应序列号
    @param context http请求执行结果
    @return  
    @notce 
    */
    using OnHttpResponseDelegate = std::function<void(std::shared_ptr<HttpContext> context)>;

	/**
	请求（上传或下载）进度delegate

	@return 序号
	@notce
	*/
	using OnProgressDelegate = std::function<void(const ProgressInfo& progress)>;
   
}// CONNECTION
}// ZEGO


#endif /* zego_connection_http_define_hpp */
