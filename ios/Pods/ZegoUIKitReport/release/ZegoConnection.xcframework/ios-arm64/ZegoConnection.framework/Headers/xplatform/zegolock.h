#if !defined(_ZEGOLOCK_INC_)
#define _ZEGOLOCK_INC_

#include "zegotypes.h"
#include "zegoexcept.h"
#include "zegomap.h"
#include "zegothread.h"

#ifdef _OS_WIN_
#include <windows.h>
#else
#include <pthread.h>
#endif	/* _OS_WIN_ */

#ifdef _OS_WIN_	 /* this is a definition for a mutex lock in different platform */
typedef CRITICAL_SECTION	zegolock_t;
#else 
typedef pthread_mutex_t		zegolock_t;
#endif	/* _OS_WIN_ */
/**
 System independent lock type. 
 It point to a CRITICAL_SECTION instance under windows, and to a pthread_mutex_t instance under posix.
 Please use reinterpret_cast<CRITICAL_SECTION*>(handle) or reinterpret_cast<pthread_mutex_t*>(handle) to convert it if you want it be used by native sys api.
 */
typedef	zegolock_t*			hzegolock;

/**
 System independent read+write-lock type. 
 */
typedef	void*				hzegolockrw;

#ifdef __cplusplus
extern "C" {
#endif
	
	/**
	 init a lock.
	 
	 @plock		- a memory space to hold data of mutex. This argument can't be NULL.

	 @return	- return a valid lock context if success, otherwise return NULL.
	 */
	_ZEGO_API hzegolock	zegolock_init(zegolock_t* plock);
	
	/**
	 destroy a lock context.
	 
	 @hlock		- lock context.
	 
	 @return	- return 0 if success, otherwise return error code.
	 */
	_ZEGO_API int32	zegolock_destroy(hzegolock hlock);
	
	/**
	 try to hold it on.
	 
	 @hlock		- lock context.
	 
	 @return	- return 0 if success, otherwise return error code.
	 */
	_ZEGO_API int32	zegolock_trylock(hzegolock hlock);
	
	/**
	 excute a lock operating(hold it on).
	 
	 @hlock		- lock context.
	 
	 @return	- return 0 if success, otherwise return error code.
	 */
	_ZEGO_API int32	zegolock_lock(hzegolock hlock);
	
	/**
	 unlock it.
	 
	 @hlock		- lock context.
	 
	 @return	- return 0 if success, otherwise return error code.
	 */
	_ZEGO_API int32	zegolock_unlock(hzegolock hlock);

	/**
	 alloc a rwlock context.
	 
	 @return	- return a valid lockrw context if success, otherwise return NULL.
	 */
	_ZEGO_API hzegolockrw zegorwlock_alloc(void);
	
	/**
	 destroy a rwlock.
	 
	 @hlock		- lock context.
	 
	 @return	- return 0 if success, otherwise return error code.
	 */
	_ZEGO_API int32	zegorwlock_destroy(hzegolockrw hlock);
	
	/**
	 try to hold it on in reading-lock mode.
	 
	 @hlock		- lock context.
	 
	 @return	- return 0 if success, otherwise return error code.
	 */
	_ZEGO_API int32	zegorwlock_tryrdlock(hzegolockrw hlock);
	
	/**
	 excute a reading-lock operating(hold it on).
	 in this case, excepted reading operations, all writing operations is not allowed.
	 
	 @hlock		- lock context.
	 
	 @return	- return 0 if success, otherwise return error code.
	 */
	_ZEGO_API int32	zegorwlock_rdlock(hzegolockrw hlock);
	
	/**
	 try to hold it on in writing+reading-lock mode.
	 
	 @hlock		- lock context.
	 
	 @return	- return 0 if success, otherwise return error code.
	 */
	_ZEGO_API int32	zegorwlock_tryrwlock(hzegolockrw hlock);
	
	/**
	 excute a reading+writing-lock operating(hold it on).
	 in this case, all reading and writing operations is not allowed.
	 
	 @hlock		- lock context.
	 
	 @return	- return 0 if success, otherwise return error code.
	 */
	_ZEGO_API int32	zegorwlock_wrlock(hzegolockrw hlock);
	
	/**
	 unlock it.
	 
	 @hlock		- lock context.
	 
	 @return	- return 0 if success, otherwise return error code.
	 */
	_ZEGO_API int32	zegorwlock_unlock(hzegolockrw hlock);
	
#ifdef __cplusplus
};
#endif

#ifdef __cplusplus

// A helper class to use zegolock_xxx
struct _ZEGO_CLS CZEGOLock {
	CZEGOLock(void) {
		zegolock_init(&m_lock);
	}
	CZEGOLock(const CZEGOLock & rhs) {}
	CZEGOLock & operator = (const CZEGOLock & rhs) { return *this; }
	~CZEGOLock(void) {
		zegolock_destroy(&m_lock);
	}
	boolean	Try(void) {
		return zegolock_trylock(&m_lock) == 0;
	}
	void	Lock(void) {
		zegolock_lock(&m_lock);
	}
	void	Unlock(void) {
		zegolock_unlock(&m_lock);
	}
private:
	zegolock_t	m_lock;
};

// A helper class that acquires the given Lock while the AutoLock is in scope.
struct _ZEGO_CLS CZEGOAutolock {
	explicit CZEGOAutolock(CZEGOLock& lock) : m_lock(lock){ m_lock.Lock(); }
	~ CZEGOAutolock(void) {m_lock.Unlock();}
private:
	CZEGOLock&	m_lock;
};

// A helper class that acquires the given Lock while the AutoLock is in scope.
struct _ZEGO_CLS CZEGOAutolockex {
	explicit CZEGOAutolockex(CZEGOLock *lock) : m_lock(lock){ if(m_lock) m_lock->Lock(); }
	~ CZEGOAutolockex(void) { if(m_lock) m_lock->Unlock();}
private:
	CZEGOLock*	m_lock;
};

// A helper class that acquires the given Lock while the AutoLock is in scope.
struct _ZEGO_CLS CZEGOAutoTrylock {
	explicit CZEGOAutoTrylock(CZEGOLock& lock) : m_lock(lock){ m_blocked = m_lock.Try(); }
	~ CZEGOAutoTrylock(void) { if (m_blocked) m_lock.Unlock();}
	boolean IsLocked() { return m_blocked; }
private:
	CZEGOLock&	m_lock;
	boolean m_blocked;
};

// A helper class to use zegorwlock_xxx
template class _ZEGO_API zegostl::map<zegothread_id, uint32>;	//fix warning C4251
struct _ZEGO_CLS CZEGOLockRW {
	CZEGOLockRW(void) {
		m_hlock = zegorwlock_alloc();
	}
	~CZEGOLockRW(void) {
		if (m_hlock) {
			zegorwlock_destroy(m_hlock);
			m_hlock = NULL;
		}
	}
	boolean	TryLockRD(void) {
        return zegorwlock_tryrdlock(m_hlock) == 0;
	}
	boolean	TryLockWR(void) {
		return zegorwlock_tryrwlock(m_hlock) == 0;
	}
	void	LockRD(void) {
        if( !IsLocked(true) ){
            //log_notice("rd lock....");
            zegorwlock_rdlock(m_hlock);
        }
        //else log_notice("r d lo ck....,but not lo ck");
	}
	void	LockRW(void) {
        if( !IsLocked(false) ){
            //log_notice("rw lock....");
            zegorwlock_wrlock(m_hlock);
        }
        else {
            //log_error("is already locked, now try to lockRW, why???????");
        }
	}
	void	Unlock(void) {
        if( IsNeedUnLock() ){
            //log_notice("unlock....");
            zegorwlock_unlock(m_hlock);
        }
	}
private:
	hzegolockrw           m_hlock;
  
    //down to keep one thread only one rdlock or one wrlock
private:
    typedef zegostl::map<zegothread_id,uint32>  CMapThreadId2Count;
    CZEGOLock             m_lockformap;
    CMapThreadId2Count  m_mapThreadId2Count;
    
    boolean  IsLocked(boolean bAddCountIfLocked){
        zegothread_id threadid = zegothread_selfid();
        boolean bIsLocked = false;
        
        m_lockformap.Lock();
        
        if( m_mapThreadId2Count.end() != m_mapThreadId2Count.find(threadid) ){
            if( bAddCountIfLocked )
                m_mapThreadId2Count[threadid]++;
            bIsLocked = true;
        }
        else {
            m_mapThreadId2Count[threadid] = 1;
        }
        
        m_lockformap.Unlock();
        
        return bIsLocked;
    }
    
    boolean  IsNeedUnLock(){
        zegothread_id threadid = zegothread_selfid();
        boolean bNeed = false;
        
        m_lockformap.Lock();
        
        if( m_mapThreadId2Count.end() != m_mapThreadId2Count.find(threadid) ){
            m_mapThreadId2Count[threadid]--;
            if( 0 == m_mapThreadId2Count[threadid] ){
                m_mapThreadId2Count.erase(threadid);
                bNeed = true;
            }
        }
        
        m_lockformap.Unlock();
        
        return bNeed;
    }
};

// A helper class that acquires the given Lock while the AutoLockReadonly is in scope.
struct _ZEGO_CLS CZEGOAutolockReadonly {
	explicit CZEGOAutolockReadonly(CZEGOLockRW& lock) : m_lock(lock){ m_lock.LockRD(); }
	~ CZEGOAutolockReadonly(void) {m_lock.Unlock();}
private:
	CZEGOLockRW&	m_lock;
};

// A helper class that acquires the given Lock while the AutolockReadWrite is in scope.
struct _ZEGO_CLS CZEGOAutolockReadWrite {
	explicit CZEGOAutolockReadWrite(CZEGOLockRW& lock) : m_lock(lock){ m_lock.LockRW(); }
	~ CZEGOAutolockReadWrite(void) { m_lock.Unlock(); }
private:
	CZEGOLockRW&	m_lock;
};

#endif

#endif /*_ZEGOLOCK_INC_*/
