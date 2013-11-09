#include "ThreeCPUPlayGame.h"
#include "GameMainForm.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)


CThreeCPUPlayGame::CThreeCPUPlayGame()
{
	m_nThreadJobState = Job_State_Begin;
	m_toTerminate = false;
}

CThreeCPUPlayGame::~CThreeCPUPlayGame()
{

}

void CThreeCPUPlayGame::run()
{
   _LOG(SourceFLInfo, DebugDebug, "start CThreeCPUPlayGame::run()");	

   while (false == m_toTerminate)
   {
	   _ThreadJob();
	   Thread::sleep(DEF_INT_MonitorThreadSleep);		
   }

   _ProcessUserTerminate();

   _LOG(SourceFLInfo, DebugDebug, "end CThreeCPUPlayGame::run()");	
}	
void CThreeCPUPlayGame::terminate()
{
	m_toTerminate = true;

	_LOG(SourceFLInfo, DebugDebug, "CThreeCPUPlayGame::terminate()");

}

int CThreeCPUPlayGame::_ThreadJob()
{
	//_LOG(SourceFLInfo, DebugDebug, "begin CThreeCPUPlayGame::_ThreadJob()");

	int nFunRes = 0;
	CGameMainForm* pGameMainForm = NULL;
	int nIndex =0; 
	int nTestCount = 100;

	switch (m_nThreadJobState)
	{
	case Job_State_Begin:
	case Job_State_InitEnv:
		nTestCount = 10;
		for (nIndex = 0; nIndex < nTestCount; nIndex++)
		{
			pGameMainForm = new CGameMainForm();
			pGameMainForm->startNewGame();
			delete pGameMainForm;
			pGameMainForm = NULL;
		}


		m_nThreadJobState = Job_State_End;

		break;
	case Job_State_End:
		break;
	}	

	//_LOG(SourceFLInfo, DebugDebug, "end CThreeCPUPlayGame::_ThreadJob()");
	return nFunRes;
}

int CThreeCPUPlayGame::_ProcessUserTerminate()
{
	_LOG(SourceFLInfo, DebugDebug, "CThreeCPUPlayGame::_ProcessUserTerminate()");

	int nFunRes = 0;

	//user termin
	m_nThreadJobState = Job_State_End;

	return nFunRes;
}

bool CThreeCPUPlayGame::isFinishWork()
{
	bool bIsFinishWork = false;
	if (Job_State_End == m_nThreadJobState)
	{
		bIsFinishWork = true;
	}
	return bIsFinishWork;
}
NS_END(TA_TwoToOnePokerGameNet)




