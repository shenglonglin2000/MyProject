#ifndef __CLASS_MYTHREAD_TEST__
#define __CLASS_MYTHREAD_TEST__

#include "CommonDef.h"

#include "Thread.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

class MyThreadTest : public Thread
{
private:
	enum EThreadJobState
	{
		Job_State_Begin,

		Job_State_InitEnv,

		Job_State_End,
	};
public:
	MyThreadTest(); 
	~MyThreadTest();
public:
	virtual void run() ;	
	virtual void terminate() ;

		
private:
	bool	m_toTerminate;
	EThreadJobState m_nThreadJobState;	
private:
	int    _ThreadJob();
	
//////////////////////////////////////////////////////////////////////////
public:
	bool  isFinishWork();

private:
	int	  _ProcessUserTerminate();  
//////////////////////////////////////////////////////////////////////////
};


NS_END(TA_TwoToOnePokerGameNet)

#endif  // __CLASS_MYTHREAD_TEST__
