#include "TCPServerRequestMonitor.h"
#include "TcpConnectionActor.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)



CTCPServerRequestMonitor::CTCPServerRequestMonitor()
{
	m_nThreadJobState = Job_State_Begin;
	m_toTerminate = false;
	
	m_bTCPRead = true;
	m_bTCPWrite = true;
	m_nWaitTimeMillis = DEF_INT_MonitorThreadSleep;

	m_mapWaitQueue.clear();
	m_mapMonitorQueue.clear();
	m_mapRemoveQueue.clear();

}

CTCPServerRequestMonitor::~CTCPServerRequestMonitor()
{
	_ClearAllQueue();
}

void CTCPServerRequestMonitor::_ClearAllQueue()
{
	{
		ThreadGuard guardWaitQueue(m_LockWaitQueue);

		//remove GateInTo to GateOutOf
		mapTCPSocketInfoConIterT iterMapWaitQueue;
		CTCPConnectionActor* pTCPSocketInfoGet;
		iterMapWaitQueue = m_mapWaitQueue.begin();
		while (iterMapWaitQueue != m_mapWaitQueue.end())
		{
			pTCPSocketInfoGet = iterMapWaitQueue->second;

			{
				ThreadGuard guardRemoveQueue(m_LockRemoveQueue);
				TcpSocket* pTcpSocketRef = pTCPSocketInfoGet->getSocket();
				m_mapRemoveQueue.insert(mapTCPSocketInfoValueTypeT(pTcpSocketRef, pTCPSocketInfoGet));
			}

			iterMapWaitQueue++;
		}
		m_mapWaitQueue.clear();
	}


	{
		//remove m_mapSocketMonitor to GateOutOf
		ThreadGuard guardMonitorQueue(m_LockMonitorQueue);
		mapTCPSocketInfoConIterT iterMapMonitorQueue;
		CTCPConnectionActor* pTCPSocketInfoGet;
		iterMapMonitorQueue = m_mapMonitorQueue.begin();
		while (iterMapMonitorQueue != m_mapMonitorQueue.end())
		{
			pTCPSocketInfoGet = iterMapMonitorQueue->second;

			{
				ThreadGuard guardRemoveQueue(m_LockRemoveQueue);
				TcpSocket* pTcpSocketRef = pTCPSocketInfoGet->getSocket();
				m_mapRemoveQueue.insert(mapTCPSocketInfoValueTypeT(pTcpSocketRef, pTCPSocketInfoGet));
			}

			iterMapMonitorQueue++;
		}
		m_mapMonitorQueue.clear();
	}
	

	{
		ThreadGuard guardRemoveQueue(m_LockRemoveQueue);
		mapTCPSocketInfoConIterT iterMapRemoveQueue;
		CTCPConnectionActor* pTCPSocketInfo;
		iterMapRemoveQueue = m_mapRemoveQueue.begin();
		while (iterMapRemoveQueue != m_mapRemoveQueue.end())
		{
			pTCPSocketInfo = iterMapRemoveQueue->second;
			DEF_DELETE(pTCPSocketInfo);
			iterMapRemoveQueue++;
		}
		m_mapRemoveQueue.clear();
	}


}


void CTCPServerRequestMonitor::run()
{
   _LOG(SourceFLInfo, DebugDebug, "start CTCPSocketInfoManager::run()");	

   while (false == m_toTerminate)
   {
	   _ThreadJob();
	   //Thread::sleep(DEF_INT_NormalThreadSleep);//m_SetMonitorQueue.waitForIO(1000)		
   }

   _ProcessUserTerminate();

   _LOG(SourceFLInfo, DebugDebug, "end CTCPSocketInfoManager::run()");	
}	
void CTCPServerRequestMonitor::terminate()
{
	m_toTerminate = true;
	_LOG(SourceFLInfo, DebugDebug,	"CTCPSocketInfoManager::terminate()");	
}

int CTCPServerRequestMonitor::_ThreadJob()
{
	int nFunRes = 0;
	
	_AddNewMemberToMonitorQueue();
	_CheckMonitorQueue();
	_ProcessMonitorQueue();
	return nFunRes;
}


int CTCPServerRequestMonitor::_ProcessMonitorQueue()
{
	ThreadGuard guard(m_LockMonitorQueue);
	//if m_SetMonitorQueue is empty, CPU useage will be high, 
	//so, sleep here
	if (m_SetMonitorQueue.getSize() <= 0)
	{
		Thread::sleep(DEF_INT_NormalThreadSleep);
	}

	int nFunRes = 0;
	TcpSocket* pSocketRef = NULL;
	CTCPConnectionActor* pTCPSocketInfoGet = NULL;
	unsigned int nCount = 0;

	m_bTCPRead = true;
	//m_bTCPWrite = true;//waitForIO will not sleep 
	m_bTCPWrite = false;
	m_SetMonitorQueue.waitForIO(m_bTCPRead, m_bTCPWrite, m_nWaitTimeMillis);

	nCount = m_SetMonitorQueue.getSize();
	for (unsigned int nIndex = 0; nIndex < nCount; nIndex++)
	{
		m_bTCPRead = false;
		m_bTCPWrite = false;
		pSocketRef = NULL;
		pSocketRef = m_SetMonitorQueue.getSocket(nIndex, m_bTCPRead, m_bTCPWrite );

		if (NULL != pSocketRef && m_bTCPRead)
		{
			//can read data from socket		 
			mapTCPSocketInfoConIterT iterFind;
			iterFind = m_mapMonitorQueue.find(pSocketRef);
			pTCPSocketInfoGet = iterFind->second;
			nFunRes = pTCPSocketInfoGet->recvData(m_bTCPRead);
		}

	}//for

   

	return nFunRes;
}

int CTCPServerRequestMonitor::_ProcessUserTerminate()
{
	int nFunRes = 0;

	//user termin
	m_nThreadJobState = Job_State_End;

	return nFunRes;
}

bool CTCPServerRequestMonitor::isFinishWork()
{
	bool bIsFinishWork = false;
	if (Job_State_End == m_nThreadJobState)
	{
		bIsFinishWork = true;
	}
	return bIsFinishWork;
}



void CTCPServerRequestMonitor::addSocket(CTCPConnectionActor* pTCPSocketInfoRef )
{
	ThreadGuard guard(m_LockWaitQueue);
	TcpSocket* pSocketRef = NULL;
	pSocketRef = pTCPSocketInfoRef->getSocket();
	m_mapWaitQueue.insert(mapTCPSocketInfoValueTypeT(pSocketRef, pTCPSocketInfoRef));
	pSocketRef = NULL;
}



unsigned int CTCPServerRequestMonitor::getSize()
{
	//TODO.
	unsigned int nClientCount = 0;
	ThreadGuard guard(m_LockMonitorQueue);
	nClientCount = (unsigned int)m_mapMonitorQueue.size();
	return nClientCount;
}


void CTCPServerRequestMonitor::_AddNewMemberToMonitorQueue()
{
	ThreadGuard guardWaitQueue(m_LockWaitQueue);
	ThreadGuard guardMonitorQueue(m_LockMonitorQueue);

	mapTCPSocketInfoConIterT iterMap;
	CTCPConnectionActor* pTCPSocketInfoGet = NULL;
	TcpSocket* pSocketRef = NULL;

	iterMap = m_mapWaitQueue.begin();
	while (iterMap != m_mapWaitQueue.end())
	{
		pTCPSocketInfoGet = iterMap->second;
		pSocketRef = iterMap->first;

		m_mapMonitorQueue.insert(mapTCPSocketInfoValueTypeT(pSocketRef, pTCPSocketInfoGet));
		m_SetMonitorQueue.addSocket(pSocketRef);

		iterMap++;
	}
	m_mapWaitQueue.clear();
}


int CTCPServerRequestMonitor::_CheckMonitorQueue()
{	
	int nFunRes = 0;
	ThreadGuard guardMonitorQueue(m_LockMonitorQueue);
	ThreadGuard guardRemoveQueue(m_LockRemoveQueue);	

	  
	mapTCPSocketInfoConIterT iterMap;
	CTCPConnectionActor* pTCPSocketInfoRef = NULL;
	TcpSocket* pSocketRef = NULL;
	
	iterMap = m_mapMonitorQueue.begin();
	while (iterMap != m_mapMonitorQueue.end())
	{
		pTCPSocketInfoRef = iterMap->second;
		
		if (false == pTCPSocketInfoRef->isStillConnected())
		{
			pSocketRef = iterMap->first;
			m_mapMonitorQueue.erase(iterMap);
			m_SetMonitorQueue.removeSocket(pSocketRef);

			m_mapRemoveQueue.insert(mapTCPSocketInfoValueTypeT(pSocketRef, pTCPSocketInfoRef));

			//reset iterMap Pos
			iterMap = m_mapMonitorQueue.begin();

			continue;
		}

		iterMap++;
	}//while
	
	return nFunRes;
}




int CTCPServerRequestMonitor::_ProcessRemoveQueue()
{	
	int nFunRes = 0;
	ThreadGuard guard(m_LockRemoveQueue);

	mapTCPSocketInfoConIterT iterMap;
	CTCPConnectionActor* pTCPSocketInfoRef = NULL;
	TcpSocket* pSocketRef = NULL;

	iterMap = m_mapRemoveQueue.begin();
	while (iterMap != m_mapRemoveQueue.end())
	{
		pTCPSocketInfoRef = iterMap->second;
		pSocketRef = iterMap->first;

		//TODO.
		DEF_DELETE(pTCPSocketInfoRef);
		pTCPSocketInfoRef = NULL;
		pSocketRef = NULL;

		iterMap++;
	}//while

	return nFunRes;
}

NS_END(TA_TwoToOnePokerGameNet)




