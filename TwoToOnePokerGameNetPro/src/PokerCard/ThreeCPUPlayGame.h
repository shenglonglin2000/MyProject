#ifndef __CLASS_THREE_CPU_PLAY_GAME__HH__
#define __CLASS_THREE_CPU_PLAY_GAME__HH__

#include "CommonDef.h"

#include "Thread.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

class CThreeCPUPlayGame : public Thread
{
private:
	enum EThreadJobState
	{
		Job_State_Begin,

		Job_State_InitEnv,

		Job_State_End,
	};
public:
	CThreeCPUPlayGame(); 
	~CThreeCPUPlayGame();
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

#endif  // __CLASS_THREE_CPU_PLAY_GAME__HH__
