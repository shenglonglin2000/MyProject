#include "MyThreadTest.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)


MyThreadTest::MyThreadTest()
{
	m_nThreadJobState = Job_State_Begin;
	m_toTerminate = false;
}

MyThreadTest::~MyThreadTest()
{

}

void MyThreadTest::run()
{
   _LOG(SourceFLInfo, DebugDebug, "start MyThreadTest::run()");	

   while (false == m_toTerminate)
   {
	   _ThreadJob();
	   Thread::sleep(DEF_INT_MonitorThreadSleep);		
   }

   _ProcessUserTerminate();

   _LOG(SourceFLInfo, DebugDebug, "end MyThreadTest::run()");	
}	
void MyThreadTest::terminate()
{
	m_toTerminate = true;

	_LOG(SourceFLInfo, DebugDebug, "MyThreadTest::terminate()");

}

int MyThreadTest::_ThreadJob()
{
	int nFunRes = 0;
	m_nThreadJobState = Job_State_InitEnv;

	for (int nIndex = 0; nIndex < 10; nIndex++)
	{
		_LOG(SourceFLInfo, DebugDebug, "_ThreadJob() nIndex=%d", nIndex);
		Thread::sleep(200);
	}
	m_nThreadJobState = Job_State_End;

	return nFunRes;
}

int MyThreadTest::_ProcessUserTerminate()
{
	int nFunRes = 0;

	//user termin
	m_nThreadJobState = Job_State_End;

	return nFunRes;
}

bool MyThreadTest::isFinishWork()
{
	bool bIsFinishWork = false;
	if (Job_State_End == m_nThreadJobState)
	{
		bIsFinishWork = true;
	}
	return bIsFinishWork;
}
NS_END(TA_TwoToOnePokerGameNet)




