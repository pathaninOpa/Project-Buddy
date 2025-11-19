//
//  http网络协议接口
//
//  zego_connection_http.h
//  
//
//  Created by arvin on 2019/07/18.
//

#ifndef zego_connection_http_hpp
#define zego_connection_http_hpp

#include <memory>
#include <vector>
#include "zego_connection_http_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{
	class HttpImpl;

    class ZEGOCONNECTION_API ZegoConnectionHttp
    {
    public:
        static std::shared_ptr<ZegoConnectionHttp> GetInstance();
        virtual ~ZegoConnectionHttp();
        
    private:
        ZegoConnectionHttp();
        
    public:
        /**
        初始化模块
        
        @param 
        */
		ZCError Init();

        /**
        反初始化模块
        
        @param
        */
        void UnInit();

        /**
        设置 http 请求限制频率
        
        @param freq 限制频率，如果不设置，则为 60/分钟
        @discussion
        */
        void SetRequestFreqLimit(uint32 freq);
        

        /**
        设置根证书

        @param cert_data 根证书内容
        @return 返回错误码
        @discussion 若cert_data被设置，则直接采用该证书内容，若未设置，则使用url自行获取证书内容
        */
		ZCError SetRootCertificate(std::shared_ptr<CZegoCACertData> cert_data);

        /**
        设置 证书时间
        
        @param time 证书时间，单位秒，若未设置，则默认为0，0表示不进行证书有效时间的校验
        @param fullTrust 时间可信赖，证书时间是否是可信赖的
        */
        void SetCertTime(uint64 time, bool fullTrust = false);
        
        /**
        设置IP协议栈模式

        @param mode IP协议栈模式
        @return 返回错误码
        @notce
        */
        void SetIPStackMode(IPStackMode mode);
        
        /**
        发起http请求
        
        @param req http请求函数对象
        @param rsp http返回函数对象
        @return 返回错误码
        @discussion rsp callback线程在内部线程，调用方注意是否需要切线程处理数据
        */
		ZCSeq StartRequest(const OnHttpRequestDelegate& req, const OnHttpResponseDelegate& rsp, HttpRequestConfig config = HttpRequestConfig());

        /**
        发起http请求

        @param seq 请求序列 调用文件中 ZegoMiscUtility 的ZegoGetNextSeq 生成
        @param req http请求函数对象
        @param rsp http返回函数对象
        @return 返回错误码
        @discussion rsp callback线程在内部线程，调用方注意是否需要切线程处理数据
        */
        bool StartRequest(ZCSeq seq, const OnHttpRequestDelegate& req, const OnHttpResponseDelegate& rsp, HttpRequestConfig config = HttpRequestConfig());


        //////////////////////////////////设置接口，在请求接口前调用////////////////////////////////////////
        /**
        添加请求头
        
        @param header 请求头
        @return 返回错误码
        @discussion 常用的字段“User-Agent”和“Content-Type”可以使用SetUserAgent和SetContentType进行设置
        */
		ZCError AddHeader(ConstHttpClientPtr client, const std::string& header);

        /**
        设置用户类型标识

        @param user_agent 用户类型标识
        @return 返回错误码
        @discussion 如果没有设置，默认没有该请求头
        */
		ZCError SetUserAgent(ConstHttpClientPtr client, const std::string& user_agent);

        /**
        设置请求主体类型
        
        @param content_type 主体类型，如果未设置，默认采用“application/json”
        @return 返回错误码
        @discussion 如果没有设置，默认为“Content-Type:application/json; charset=utf-8”
        */
		ZCError SetContentType(ConstHttpClientPtr client, const std::string& content_type);

        /**
        设置连接超时时间
        
        @param timeout 超时时间
        @return 返回错误码
        */
		ZCError SetConnectTimeout(ConstHttpClientPtr client, uint32 timeout);

		/**
		设置请求超时时间

		@param timeout 超时时间
		@return 返回错误码
		@notce
		*/
		ZCError SetRequestTimeout(ConstHttpClientPtr client, uint32 timeout);

        /**
        设置速度限制
        
        @param maxSpeed 限速, 单位 bytes/second, 0 表示不限制
        @return 返回错误码
        */
		ZCError SetMaxSendSpeedLarge(ConstHttpClientPtr client, uint32 maxSpeed);

		/**
		设置请求进度delegate

		@param delegate 域请求进度回调
		@return 返回错误码
		@notce
		*/
		ZCError SetProgressDelegate(ConstHttpClientPtr client, const OnProgressDelegate& delegate);

		/**
		设置此次请求是否做证书校验（默认是证书校验）

		@param bVerify 域请求进度回调
		@return 返回错误码
		@notce
		*/
		ZCError SetVerifyCertificate(ConstHttpClientPtr client, bool bVerify);

		/**
		添加multipart请求中的文件part

		@param client 请求client
		@param part_name part名称
		@param remote_file_name 远程文件名称
		@param file_path 本地文件路径
		@return 返回错误码
		@discussion 支持调用多次，添加多个part，要求在HttpClientMultiPartPost执行前调用
		*/
		ZCError AddMultiFilePart(ConstHttpClientPtr client, const std::string& part_name, const std::string& remote_file_name, const std::string& file_path);

		/**
		添加multipart请求中的数据part

		@param client 请求client
		@param part_name part名称
		@param data 请求数据
		@return 返回错误码
		@discussion 支持调用多次，添加多个part，要求在HttpClientMultiPartPost执行前调用
		*/
		ZCError AddMultiDataPart(ConstHttpClientPtr client, const std::string& part_name, const std::string& data);
        

		//////////////////////////////////请求接口////////////////////////////////////////

        /**
        post请求 域名通信（application/json）
        
        @param client 请求client
        @param url url地址		
        @param data 请求主体数据
        @return 返回错误码
        @discussion 调用本接口前，可调用参数设置接口进行本次请求的参数设置
        */
		ZCError HttpClientPost(ConstHttpClientPtr client, const std::string& url, const std::string& data);

        /**
        get请求 域名通信
        
        @param client 请求client
        @param url url地址
        @return 返回错误码
        @discussion 调用本接口前，可调用参数设置接口进行本次请求的参数设置
        */  
		ZCError HttpClientGet(ConstHttpClientPtr client, const std::string& url);

        /**
        multipart请求 域名通信（multipart/form-data）
        
        @param client 请求client
        @param url url地址
        @return 返回错误码
        @discussion 调用本接口前，可调用参数设置接口进行本次请求的参数设置
        */
		ZCError HttpClientMultiPartPost(ConstHttpClientPtr client, const std::string& url);

		/**
		终止multipart请求 域名通信（multipart/form-data）

		@param client 请求client
		*/
		ZCError HttpClientAbortMultiPartPost(ConstHttpClientPtr client);

		/**
		put请求 域名通信(用于上传本地文件)

		@param client 请求client
		@param url url地址
		@param file_path 待上传本地文件路径
		@return 返回错误码
		@notce 调用本接口前，可调用参数设置接口进行本次请求的参数设置
		*/
		ZCError HttpClientPut(ConstHttpClientPtr client, const std::string& url, const std::string& file_path);
        
        /**
		put请求 域名通信(用于上传本地文件读取出来的buffer)

		@param client 请求client
		@param url url地址
		@param content_buffer 待上传的文件buffer
		@return 返回错误码
		@notce 调用本接口前，可调用参数设置接口进行本次请求的参数设置
		*/
        
        ZCError HttpClientPutBuffer(ConstHttpClientPtr client, const std::string &url, const std::string &content_buffer);

		/**
		download请求 (用于断点下载文件)

		@param client 请求client
		@param url url地址
		@param file_path 将下载到本地的文件路径
		@return 返回错误码
		@notce 调用本接口前，可调用参数设置接口进行本次请求的参数设置
		*/
		ZCError HttpClientDownload(ConstHttpClientPtr client, const std::string& url, const std::string& file_path);
        
        /**
		download请求 不支持断点下载文件覆盖写 

		@param client 请求client
		@param url url地址
		@param file_path 将下载到本地的文件路径
		@return 返回错误码
		@notce 调用本接口前，可调用参数设置接口进行本次请求的参数设置
		*/
         ZCError HttpSimpleDownload(ConstHttpClientPtr client, const std::string &url, const std::string &file_path);

		/**
		停止download请求 

		@param seq StartRequest 请求下载的 seq
		@return 返回错误码
		*/
		ZCError HttpClientCancelDownload(ZCSeq seq);

		/**
		获取下载url的信息

		@param url url地址
		@param downloadSize 下载内容长度
		@param type 下载内容的类型
		*/
		ZCError HttpClientGetDownloadInfo(const std::string& url, double& downloadSize, std::string& type);

    private:
		std::shared_ptr<HttpImpl> http_impl_ = nullptr;
    };
    
}// CONNECTION
}// ZEGO


#endif /* zego_connection_http_hpp */
