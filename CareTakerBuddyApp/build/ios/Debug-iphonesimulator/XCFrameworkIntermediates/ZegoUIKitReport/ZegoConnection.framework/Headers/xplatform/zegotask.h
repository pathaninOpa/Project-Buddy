#if !defined(_ZEGOTASK_INC_)
#define _ZEGOTASK_INC_
#pragma once

#include "zegotypes.h"
#include "zegoexcept.h"
#include "zegothread.h"
#include "zegorefc.h"
#include "zegoepump.h"
#include "zegolock.h"
#include "zegoevent.h"

#ifdef __cplusplus

#define INVALID_TASKID		0

typedef uint32	task_id;

typedef enum etask_mode {
	etask_once		= 0,	/*excute once*/
	etask_repeat	= 1		/*excute until canceled*/
}etask_mode;

enum task_state_type
{
	task_state_dispatch = 1, //task dispatch start
	task_state_begin = 2, //task start begin
	task_state_end = 3, // task end
};

enum task_type
{
	task_type_normal = 1,
	task_type_delay = 2,
	task_type_timer = 3,
};

enum task_priority
{
	task_priority_low = 1,  //
	task_priority_normal = 2,
	task_priority_high = 3, //now can not used
};

struct task_observer_info
{
	uint32 taskid = 0;
	task_type type;
	task_state_type state;
	uint64 task_dispatch_time = 0; 
	uint64 task_begin_time = 0;
	uint64 task_end_time = 0;
};

//observe
class CZEGOTaskBase;
typedef void(*task_observer)(CZEGOTaskBase* pTaskBase, task_observer_info* observeInfo);

//thread idle
typedef void(*thread_idle_callback)(CZEGOTaskBase* pTaskBase);

class _ZEGO_CLS CZEGOTaskBase : public CRefCountSafe {
public:
	CZEGOTaskBase(const utf8* name, int sleepMS = 10, ethread_priority type = ethr_priority_normal);
	virtual ~ CZEGOTaskBase(void);
	
	/*push task to current thread*/
	static task_id	PushTask(const CScopeCall& task);
	static task_id	PushDelayTask(const CScopeCall& task, int64 period, etask_mode mode);
	
	/*push task to specified thread*/
	static task_id	PushTask(CZEGOTaskBase* target, const CScopeCall& task, task_priority priority = task_priority::task_priority_normal);
	static task_id	PushDelayTask(CZEGOTaskBase* target, const CScopeCall& task, int64 period /*in miliseconds*/, etask_mode mode);

	/*cancel task from current thread*/
	static boolean	CancelTask(task_id id);
	/*cancel task from specified thread*/
	static boolean	CancelTask(CZEGOTaskBase* target, task_id id);

	/*get the curretn run thread task */
	static CZEGOTaskBase* GetCurrentTask();

	/*add the observe to task */
	void SetTaskObserver(task_observer observer);
    
    const utf8*     GetName(void) const;
	
	boolean	Start(void);
	boolean	Stop(void);
	boolean IsStarted();

#if defined(_OS_IOS_)

#else
	void ActiveSignal();
#endif
	
	inline operator hzegothread (void) {
		return m_hThread;
	}
	
	zegothread_id getthreadid(){
		return m_threadid;
	}
	
protected:
#if defined(_OS_IOS_)
    virtual int64 Eachloop(void);
#else
	virtual boolean Eachloop(void);
#endif
    virtual void    OnStart(void);
    virtual void    OnStop(void);
    //when the thread no task
    virtual void    OnIdle(void);
    //thread is idle no stop and task is empty return true
    boolean IsIdle();

	
private:
	virtual void Runloop(void);
	static void* ThreadProc(void* param);

	static void Observer(CZEGOTaskBase* task, task_observer observer, task_id taskid, task_type type, task_state_type state, uint64 dispatch_time = 0, uint64 begin_time = 0, uint64 end_time = 0);

#if defined(_OS_IOS_)
    virtual void    Signal(void);
    virtual void    Waiting(int64 miliseconds);
#endif
protected:
	ethread_priority	m_eType;
	zegothread_id			m_threadid;
	hzegothread			m_hThread;
	CZEGOLock				m_lock;
	void*				m_internal;
    int                 m_sleepMS;
	boolean				m_bIsStop;

#if defined(_OS_IOS_)

#else
	CZEGOEvent* m_taskSignal; //use not ios and mac platform  active thread singnal when have task in thread
#endif
	
};

#if defined(_OS_WIN_)

class _ZEGO_CLS CZEGOTaskWin : public CZEGOTaskBase
{
public:
	CZEGOTaskWin(const utf8* name, ethread_priority type = ethr_priority_normal);
protected:
	void    OnStart(void);
	void    OnStop(void);
};

#endif

typedef CZEGOTaskBase	CZEGOTaskDefault;
_ZEGO_CLS CZEGOTaskBase * ZEGOTaskCreate(utf8 * name, ethread_priority type = ethr_priority_normal);

class _ZEGO_CLS CZEGOTaskIO : public CZEGOTaskBase {
public:
	CZEGOTaskIO(const utf8* name, int sleepMS = 10, ethread_priority type = ethr_priority_normal);
	virtual ~ CZEGOTaskIO(void);
	
	static hzegofevent GetFevent(void);
	
private:
#if defined(_OS_IOS_)
   
#else
	virtual boolean Eachloop(void);
#endif

#if defined(_OS_IOS_)
    virtual void    Signal(void);
    virtual void    Waiting(int64 miliseconds);
#endif
private:
	hzegofevent	m_hFevent;
};

#endif

#endif /* _ZEGOTASK_INC_ */
