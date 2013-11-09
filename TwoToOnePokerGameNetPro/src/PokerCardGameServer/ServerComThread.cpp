#include "ServerComThread.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "TcpSocket.h"
#include "TcpServerSocket.h"
#include "SocketSet.h"
#include "UtilityFun.h"
#include "DebugUtil.h"
#include "RecvDataWorker.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)


CServerComThread::CServerComThread(TcpSocket* pSocketRef)
{
	m_nThreadJobState = Job_State_Begin;
	m_toTerminate = false;
	m_pRecvDataWorker = new CRecvDataWorker(pSocketRef);
}

CServerComThread::~CServerComThread()
{
	DEF_DELETE(m_pRecvDataWorker);
}

void CServerComThread::run()
{
   _LOG(SourceFLInfo, DebugDebug, "start CTCPServerRecvThread::run()");	

   while (false == m_toTerminate)
   {
	   _ThreadJob();
	   Thread::sleep(DEF_INT_NormalThreadSleep);//TODO.  ... . ...		
   }

   _ProcessUserTerminate();

   _LOG(SourceFLInfo, DebugDebug, "end CTCPServerRecvThread::run()");	
}	
void CServerComThread::terminate()
{
	m_toTerminate = true;

	_LOG(SourceFLInfo, DebugDebug, "CTCPServerRecvThread::terminate()");

}

int CServerComThread::_ThreadJob()
{
	int nFunRes = 0;

	switch (m_nThreadJobState)
	{
	case Job_State_Begin:
		m_nThreadJobState = Job_State_DoingJob;
		break;
	case Job_State_DoingJob:
		_AnalysisAndProcessData();
		if (false == m_bIsStillConnected)
		{
			m_nThreadJobState = Job_State_End;
		}
		break;
	case Job_State_End:
		break;
	}	

	//_LOG(SourceFLInfo, DebugDebug, "end CThreeCPUPlayGame::_ThreadJob()");
	return nFunRes;
}

int CServerComThread::_ProcessUserTerminate()
{
	_LOG(SourceFLInfo, DebugDebug, "CTCPServerRecvThread::_ProcessUserTerminate()");

	int nFunRes = 0;

	//user termin
	m_nThreadJobState = Job_State_End;

	return nFunRes;
}

bool CServerComThread::isFinishWork()
{
	bool bIsFinishWork = false;
	if (Job_State_End == m_nThreadJobState)
	{
		bIsFinishWork = true;
	}
	return bIsFinishWork;
}


int CServerComThread::_AnalysisAndProcessData()
{
	int nFunRes = 0;

	nFunRes = m_pRecvDataWorker->analysisAndProcessData();


	return nFunRes;
}



int CServerComThread::recvData(bool bCanRead)
{
	int nFunRes = 0;

	nFunRes = m_pRecvDataWorker->readData();
	if (0 != nFunRes)
	{
		_LOG(SourceFLInfo, DebugDebug, "client disconnected to this server!");
		m_bIsStillConnected = false;
		nFunRes = -1;
	}

	return nFunRes;
}

bool CServerComThread::isStillConnected()
{
	return m_bIsStillConnected;
}

NS_END(TA_TwoToOnePokerGameNet)




