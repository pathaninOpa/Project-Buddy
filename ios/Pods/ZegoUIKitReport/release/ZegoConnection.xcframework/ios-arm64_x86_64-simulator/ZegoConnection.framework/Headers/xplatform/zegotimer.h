#if !defined(_ZEGOTIMER_INC_)
#define _ZEGOTIMER_INC_
#pragma once

#include "zegotask.h"
#include "zegotask.h"


class CZEGORealTimer;

class _ZEGO_CLS CZEGOTimerBase
{
public:
	~CZEGOTimerBase(){};
	virtual void OnTimer(uint32 uId) = 0;
	
protected:
	CZEGORealTimer* m_pRealTimer;
};

class _ZEGO_CLS CZEGOTimer : public CZEGOTimerBase
{
public:
	
	CZEGOTimer(boolean bEnableMultiThread = false,CZEGOTaskDefault* pTask = NULL);
	virtual	 ~CZEGOTimer();
	
	void	SetTimerTask(CZEGOTaskBase* pTask);	
	void    SetTimer(uint32 nInterval_ms,uint32 uId = 0,boolean bOnce = false);
	void	SetTimerAtTask(CZEGOTaskDefault* pTask,uint32 nInterval_ms,uint32 uId = 0,boolean bOnce = false);
	
	void	KillTimer(uint32 uId = -1);//if uid == -1 ,will kill all timer in this CTimer
	
private:
	
	CScopePtr<CZEGOTaskDefault> m_pDefaultTask;
};


class _ZEGO_CLS CZEGOTimer_MultiThread: public CZEGOTimerBase
{
public:
	
	CZEGOTimer_MultiThread();
	virtual	 ~CZEGOTimer_MultiThread();	
	void	SetTimer(CZEGOTaskDefault* pTask,uint32 nInterval_ms,uint32 uId = 0,boolean bOnce = false);
	void	KillTimer(uint32 uId = -1);//if uid == -1 ,will kill all timer in this CTimer
};

#endif