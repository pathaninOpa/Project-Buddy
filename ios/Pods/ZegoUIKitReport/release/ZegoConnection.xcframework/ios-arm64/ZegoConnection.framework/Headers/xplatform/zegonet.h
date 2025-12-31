#if !defined(_ZEGONET_INC_)
#define _ZEGONET_INC_

#include "zegotypes.h"
#include "zegoexcept.h"
#include "zegovector.h"
#include "zegostring.h"
#include "zegomap.h"


#if defined(ARCH_CPU_32_BITS)
	typedef int32 zegosocket;
#elif defined(ARCH_CPU_64_BITS)
	typedef int64 zegosocket;
#else
#	error Please add support for your architecture in zegotypes.h
#endif

#ifdef _OS_WIN_
#include <WinSock2.h>
#include <WS2tcpip.h>
#	ifndef SO_REUSEPORT
#		define SO_REUSEPORT SO_REUSEADDR
#	endif
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#define ZEGOINVALID_SOCKET -1

#define ZEGONET_IPARRAYSIZE 10

typedef struct zegonet_iparray
{
	uint32 count;
	uint32 ip_array[ZEGONET_IPARRAYSIZE];
} zegonet_iparray;

typedef struct zegonet_ipv6array
{
    uint32 count;
    zegostl::pair<zego::strutf8, int> ip_array[ZEGONET_IPARRAYSIZE];
} zegonet_ipv6array;

typedef struct zegonet_endpoint {
    uint32  ipv4;
    uint16  port;
} zegonet_endpoint;

zego::strutf8  zegonet_iptostr_v4(struct in_addr* ip);
zego::strutf8  zegonet_iptostr_v6(struct in6_addr* ip);

//resolve domain
boolean zegonet_resolve_domain(const utf8 *domain, zegonet_ipv6array &iparray,
                               zego::strutf8 &errormsg);

#ifdef __cplusplus
extern "C" {
#endif
	
	//net common
	_ZEGO_API void	zegonet_init();
	_ZEGO_API uint32  zegonet_getwanip();
    _ZEGO_API void    zegonet_setwanip(uint32 dwWanIP);
	_ZEGO_API uint32	zegonet_getlocalip(boolean bSelectLanIPFirst=false);
	_ZEGO_API boolean zegonet_getlocaliplist(zegostl::vector<zegostl::string>* vecIpList);
	_ZEGO_API uint32	zegonet_gethostbyname(const utf8* domain,boolean &bsupport);
	_ZEGO_API boolean	zegonet_gethostbyname_ex(const utf8* domain,zegonet_iparray &iparray,boolean &bsupport);
    
    _ZEGO_API struct in_addr*  zegonet_strtoip_v4(const utf8*  ip);
    _ZEGO_API struct in6_addr*  zegonet_strtoip_v6(const utf8*  ip);
    _ZEGO_API boolean zegonet_getaddrinfo(const utf8* domain, zegonet_ipv6array& iparray, boolean &bsupport, zego::strutf8& errormsg);
    _ZEGO_API boolean zegonet_synthesize_ipv4(const zego::strutf8 &ip4, zego::strutf8 &ip6);
    _ZEGO_API boolean zegonet_is_v4(const utf8* ip);
	
    _ZEGO_API uint32  zegonet_getbroadcast(uint32 &uLocalIp, uint32 &uNetmask);
    _ZEGO_API boolean zegonet_ipinnet(uint32 ip, uint32 uNetMask, uint32 broadcast);
    
	_ZEGO_API uint16	zegonet_hton16(uint16 v);
	_ZEGO_API uint16	zegonet_ntoh16(uint16 v);
	_ZEGO_API uint32	zegonet_hton32(uint32 v);
	_ZEGO_API uint32	zegonet_ntoh32(uint32 v);
	_ZEGO_API uint64	zegonet_hton64(uint64 v);
	_ZEGO_API uint64	zegonet_ntoh64(uint64 v);
	
	_ZEGO_API utf8*	zegonet_iptostr(uint32 ip);
	_ZEGO_API uint32  zegonet_strtoip(const utf8* ip);
	
	//socket common
	_ZEGO_API zegosocket zegosocket_create(boolean istcp, boolean reuse = false, int type = PF_INET);

	_ZEGO_API zegosocket zegosocket_create_icmp(int type = PF_INET);
	_ZEGO_API int32 zegosocket_sendto_icmp(zegosocket s, int type,const void* data, uint32 len, const utf8* ip, uint16 port);
	_ZEGO_API boolean zegosocket_setttl_icmp(zegosocket s, int ttl,int type = PF_INET);

	_ZEGO_API zegosocket zegosocket_create_block(boolean istcp,int32 rtm,int32 ttm);
	_ZEGO_API boolean	zegosocket_close(zegosocket s);
	_ZEGO_API boolean	zegosocket_isvalid(zegosocket s);
	_ZEGO_API boolean	zegosocket_setsendbufsize(zegosocket s,int32 size);
	_ZEGO_API boolean	zegosocket_setrecvbufsize(zegosocket s,int32 size);
	_ZEGO_API boolean zegosocket_getsendbufsize(zegosocket s,int32 &size);
	_ZEGO_API boolean zegosocket_getrecvbufsize(zegosocket s,int32 &size);
	
	_ZEGO_API boolean zegosocket_setnodelay(zegosocket s,boolean bistoset);
    _ZEGO_API boolean zegosocket_setmulticastttl(zegosocket s, int ttl);
    _ZEGO_API boolean zegosocket_joingroup(zegosocket s, const char *mcastAddr, const char *ifAddr);
	//socket tcp
#ifdef _OS_IOS_
    _ZEGO_API boolean zegosocket_connect_ios(zegosocket s,const utf8* ip,uint16 port, boolean& ip_is_v6);
    _ZEGO_API boolean zegosocket_connect_ios_nat64(zegosocket s,const utf8* ip,uint16 port);
#endif
	_ZEGO_API boolean	zegosocket_connect(zegosocket s, const utf8* addr, uint16 port, bool nat64);
	_ZEGO_API boolean	zegosocket_connectV4(zegosocket s,const utf8* addr,uint16 port);
	//_ZEGO_API boolean	zegosocket_bind(zegosocket s,int32 ip,uint16 port);
    _ZEGO_API boolean	zegosocket_bind(zegosocket s, const utf8* ip,uint16 port);
	_ZEGO_API boolean	zegosocket_listen(zegosocket s,int32 backlog);
	_ZEGO_API zegosocket zegosocket_accept(zegosocket s);
	_ZEGO_API int32	zegosocket_send(zegosocket s,const void* data,uint32 len);
	_ZEGO_API int32	zegosocket_recv(zegosocket s,void* data,uint32 len);
	_ZEGO_API uint32	zegosocket_getunreaddatalen(zegosocket s);
	//_ZEGO_API boolean	zegosocket_getpeername(zegosocket s,uint32 &ip,uint16 &port);
	//_ZEGO_API boolean	zegosocket_getsockname(zegosocket s,uint32 &ip,uint16 &port);
    boolean	zegosocket_getpeername(zegosocket s,uint16 &port);
    boolean	zegosocket_getsockname(zegosocket s,uint16 &port);
	//socket udp
	_ZEGO_API int32	zegosocket_recvfrom(zegosocket s,void* data,uint32 len,const utf8* fromip,uint16 &fromport);

	//socket udp for icmp
	_ZEGO_API int32	zegosocket_recvfrom_icmp(zegosocket s, void* data, uint32 len, const utf8* fromip, uint16 &fromport,int& ttl, int type = PF_INET);
	
	_ZEGO_API int32   zegonet_getlasterror();
	
#ifdef _OS_WIN8_	
	_ZEGO_API boolean	zegosocket_set_iptable(uint32* iptable, uint32 count);
#endif

    //return ip stack, 1st: ipv4, 2st: ipv6, 3st: nat64
    _ZEGO_API int32 zegonet_detect_ip_stack();

    //true: support ipv4, false: not support ipv4
    _ZEGO_API boolean zegonet_test_connect_v4();
    //true: support ipv6, false: not support ipv6
    _ZEGO_API boolean zegonet_test_connect_v6();

    //check current net is nat64
    //ipv6_state: 0-unknown, 1-support ipv6, 2-not support ipv6
	//force: true-will detect api to support ipv6, false- if have detect cache, will return cache . return 
	//only_check: true- will only check need  to  detect support ipv6
    _ZEGO_API boolean zegonet_is_nat64(bool force, bool only_check = false);

#ifdef __cplusplus
};
#endif

_ZEGO_API int32	zegosocket_sendto(zegosocket s, int type,const void* data,uint32 len,const utf8* ip,uint16 port);
_ZEGO_API int32	zegosocket_v6_sendto(zegosocket s, const void* data, uint32 len, const utf8* ip, int ip_type, uint16 port, bool is_nat64);

//_ZEGO_API int32	zegosocket_sendto(zegosocket s,const void* data,uint32 len,uint32 uAddr,uint16 port);


#endif /*_FILE*NAME_INC_*/
