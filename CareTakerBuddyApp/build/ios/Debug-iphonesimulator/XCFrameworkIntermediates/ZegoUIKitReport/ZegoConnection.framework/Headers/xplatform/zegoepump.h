#if !defined(_ZEGOEPUMP_INC_)
#define _ZEGOEPUMP_INC_
#pragma once

#include "zegotypes.h"

/*file event type*/
#define FE_TIMEOUT	0x01	/*when a fd with a timeout parameter overide*/
#define FE_READ		0x02	/*has incoming data*/
#define FE_WRITE	0x04	/*has blank buffer to place more data*/
#define FE_EXCEPT	0x08	/*something wrong with fd, it may be closed or reset or ??*/
#define FE_COUNT	4

/*file event flags*/
#define FEF_READ	0x02	/*has incoming data*/
#define FEF_WRITE	0x04	/*has blank buffer to place more data*/
#define FEF_ALL		0xFF

typedef	void* hzegofevent;


/**
 Callback type.
 
 @hfe		- file event context
 @fd		- file description bound to hfe
 @events	- event bitmap, see FE_XXX for more detail
 @extra		- extra parameter passed by zegofe_add
 
 see zegofe_alloc(), zegofe_add()
 */
typedef void (*zegofe_callback)(hzegofevent hfe, FD fd, int32 event, void* extra);
typedef void (*zegofe_onrelease)(hzegofevent hfe, FD fd, void* extra);


#ifdef __cplusplus
extern "C" {
#endif
	
	/**
	 Alloc a file event context, use zegofe_free to free it if allthing done.
	 For threadsafe issue, please make sure all zegofe_xxx call to same hzegofevent in a same thread context.
	 
	 @return	- a file event handle if success, otherwise return NULL.
	 */
	_ZEGO_API hzegofevent	zegofe_alloc(void);
	
	/**
	 Free a file event context which alloced by zegofe_alloc.
	 
	 @hfe		- handle of file event to be freed.
	 */
	_ZEGO_API void	zegofe_free(hzegofevent hfe);
	
	/**
	 Clearup all fd in file event context specified.
	 
	 @hfe		- file event context.
	 */
	_ZEGO_API void	zegofe_clearup(hzegofevent hfe);
	
	/**
	 Return count of fd in file event context specified.
	 
	 @hfe		- count of fd.
	 */
	_ZEGO_API int32	zegofe_fdsize(hzegofevent hfe);

	/**
	 dispatch loop.
	 
	 @hfe		- file event context.
	 
	 @return	- counter of fd which something happen with.
	 */
	_ZEGO_API int32	zegofe_loop(hzegofevent hfe);

    
    
#if defined(_OS_IOS_)    
    /**
	 dispatch loop.
	 
	 @hfe		- file event context.
     @tov       - timeout value in miliseconds
	 
	 @return	- counter of fd which something happen with.
	 */
	_ZEGO_API int32	zegofe_loop2(hzegofevent hfe, uint32 tov /*timeout in miliseconds*/);
    
	/**
	 send a signal to break current loop.
	 
	 @hfe		- file event context.
	 
	 @return	- return 0 if success, otherwise return error code(also set errno).
	 */
	_ZEGO_API int32	zegofe_signal(hzegofevent hfe);
#endif
    
    
    
	/**
	 Add a file description to file event context.
	 
	 @hfe		- file event context.
	 @fd		- file description bound to hfe.
	 @flags		- initialized event flags [removed atfer used], see FEF_XXX for more detail. 
	 @persists	- persistent event flags [aways keep on], see FEF_XXX for more detail.
		flags[flag bits]	persists[keep bits]		means
		-----------------------------------------------
		FE_READ				FE_READ					aways keep FE_READ flags until zegofe_unset(e, FE_READ) called
		FE_WRITE			FE_WRITE				aways keep FE_WRITE flags until zegofe_unset(e, FE_WRITE) called
		FEF_ALL				FE_READ					unlike FE_READ, FE_WRITE will be removed atfer it happen
		FEF_ALL				FE_WRITE				unlike FE_WRITE, FE_READ will be removed atfer it happen
	 @cb		- callback function to handle events.
	 @onrl		- callback function to release fd.
	 @extra		- extra parameter pass to zegofe_callback when something happen.
	 
	 @return	- return 0 if success, otherwise return error code(also set errno).
	 */
	_ZEGO_API int32	zegofe_add(hzegofevent hfe, FD fd, int32 flags, int32 persists, zegofe_callback cb, void* extra, zegofe_onrelease onrl);
	
	/**
	 Add a file description to file event context.
	 
	 @hfe		- file event context.
	 @fd		- file description bound to hfe.
	 @flags		- event flags, see FEF_XXX for more detail.
	 @cb		- callback function to handle events.
	 @onrl		- callback function to release fd.
	 @extra		- extra parameter pass to zegofe_callback when something happen.
	 @to		- timeout period to hold in listening list, set 'to' to zero if want to disable timeout detecting.
	 
	 @return	- return 0 if success, otherwise return error code(also set errno).
	 */
	_ZEGO_API int32	zegofe_addonce(hzegofevent hfe, FD fd, int32 flags, zegofe_callback cb, void* extra, uint32 to /*miliseconds*/, zegofe_onrelease onrl);

	/**
	 query fd is add before
	 
	 @hfe		- file event context.
	 @fd		- file description bound to hfe.
	 
	 @return	- return 1 if exsited, return 0 not exsit
	 */
	_ZEGO_API int32	zegofe_is_added(hzegofevent hfe, FD fd);
	
	/**
	 add some event flags to fd.
	 
	 @hfe		- file event context.
	 @fd		- file description bound to hfe.
	 @flags		- event flags, see FEF_XXX for more detail.
	 
	 @return	- return 0 if success, otherwise return error code(also set errno).
	 */
	_ZEGO_API int32	zegofe_set(hzegofevent hfe, FD fd, int32 flags);
	
	/**
	 remove some event flags from fd. use FEF_ALL to remove fd from listening list if current event bitmap is unknown.
	 
	 @hfe		- file event context.
	 @fd		- file description bound to hfe.
	 @flags		- events flags to remove. If all events flags has been removed, fd will be remove from listening list.
	 
	 @return	- return 0 if success, otherwise return error code(also set errno).
	 */
	_ZEGO_API int32	zegofe_unset(hzegofevent hfe, FD fd, int32 flags);
	
#ifdef __cplusplus
};
#endif

#ifdef __cplusplus
#include "zegorefc.h"
 
class _ZEGO_CLS CFEventPump
{
public:
	struct CFECallbackWraper 
	{
		typedef void (CRefCount::*fnCallback)(hzegofevent hfe, FD fd, int32 event);
		template <class T>
		CFECallbackWraper(void (T::*cb)(hzegofevent hfe, int int64, int32 event)) {
			_cb = (fnCallback)cb;
			_owner = NULL;
		}
		~CFECallbackWraper(void) {
			if (_owner) {
				_owner->Release();
			}
		}
		
	private:
		friend class CFEventPump;
		static void FECallback(hzegofevent hfe, FD fd, int32 event, void* extra);
		static void FERelease(hzegofevent hfe, FD fd, void* extra);
		
		CFECallbackWraper(fnCallback cb, CRefCount* owner)
		{
			_cb = cb; 
			_owner = owner;
			
			if (_owner) 
			{
				_owner->AddRef();
			}
		}
		fnCallback	_cb;
		CRefCount*	_owner;
	};
public:
	CFEventPump(void);
	virtual ~ CFEventPump(void);
	
public:
	/*c++ version of zegofe_add*/
	boolean	Add(FD fd, int32 flags, int32 persists, CFECallbackWraper cb, CRefCount* owner);
	/*c++ version of zegofe_addonce*/
	boolean	AddOnce(FD fd, int32 flags, CFECallbackWraper cb, CRefCount* owner, uint32 to /*miliseconds*/);
	/*c++ version of zegofe_set*/
	boolean	Set(FD fd, int32 flags);
	/*c++ version of zegofe_unset*/
	boolean	Unset(FD fd, int32 flags);

protected:
	hzegofevent	m_hFEvent;
};

#endif

#endif /*_ZEGOEPUMP_INC_*/
