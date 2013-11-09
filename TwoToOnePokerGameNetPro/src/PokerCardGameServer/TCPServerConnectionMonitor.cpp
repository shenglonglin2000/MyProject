#include "TCPServerConnectionMonitor.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "Thread.h"
#include "TcpSocket.h"
#include "TcpServerSocket.h"
#include "SocketSet.h"
#include "UtilityFun.h"
#include "TCPServerRequestMonitor.h"
#include "TcpConnectionActor.h"
#include "DebugUtil.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)


CTCPServerConnectionMonitor::CTCPServerConnectionMonitor(const std::string& strListenSocketPort)
{
	m_strListenSocketPort = strListenSocketPort;
	m_nThreadJobState = Job_State_Begin;
	m_toTerminate = false;
	m_pTCPServerSocket = NULL;//server listen socket
	m_bTCPRead = true;
	m_bTCPWrite = true; 
	m_nTCPSelectRes = 0; 
	m_timeTCPWait = DEF_INT_MonitorThreadSleep;
	m_pTCPServerRequestMonitor = NULL;

}

CTCPServerConnectionMonitor::~CTCPServerConnectionMonitor()
{		
	m_pTCPServerRequestMonitor = NULL;
}

void CTCPServerConnectionMonitor::run()
{
   int nFunRes = 0;

   _LOG(SourceFLInfo, DebugDebug,	"start CTCPServerConnectionMonitor::run()");	
   
   m_nThreadJobState = Job_State_Begin;

   while (false == m_toTerminate)
   {
	   _ThreadJob();
	   //Thread::sleep(DEF_INT_NormalThreadSleep);	//m_SetTCPServerSocket.waitForIO(1000)	
   }

   _ProcessUserTerminate();

   _LOG(SourceFLInfo, DebugDebug,	"end CTCPServerConnectionMonitor::run()");	
}	
void CTCPServerConnectionMonitor::terminate()
{
	m_toTerminate = true; 
	_LOG(SourceFLInfo, DebugDebug,	"end CTCPServer::terminate()");
}

int CTCPServerConnectionMonitor::_ThreadJob()
{
	int nFunRes = 0;
	
	switch (m_nThreadJobState)
	{
	case Job_State_Begin:
		if (NULL == m_pTCPServerRequestMonitor)
		{
			m_pTCPServerRequestMonitor = new CTCPServerRequestMonitor();	
			m_pTCPServerRequestMonitor->start();
		}
		m_nThreadJobState = Job_State_CheckParam;
		break;
	case Job_State_CheckParam:
		nFunRes = _Process_CheckParam();
		if (0 == nFunRes)
		{
		   m_nThreadJobState = Job_State_CreateListenSocket;
		}
		else
		{
			m_nThreadJobState = Job_State_End; 
		}
		break;
	case Job_State_CreateListenSocket:
		nFunRes = _Process_CreateListenSocket();
		if (0 == nFunRes)
		{
			m_nThreadJobState = Job_State_WaitForNewClient;
		}
		else
		{
			m_nThreadJobState = Job_State_End; 
		}
		break;
	case Job_State_WaitForNewClient:
		nFunRes = _Process_WaitForNewClient();
		if (0 == nFunRes)
		{
			m_nThreadJobState = Job_State_WaitForNewClient;
		}
		else
		{
			m_nThreadJobState = Job_State_End; 
		}
		break;
	case Job_State_End:
		Thread::sleep(DEF_INT_NormalThreadSleep);
		break;		
	default:
		Thread::sleep(DEF_INT_NormalThreadSleep);
		break;		
	}  //switch

	return nFunRes;
}

int CTCPServerConnectionMonitor::_ProcessUserTerminate()
{
	int nFunRes = 0;
	//user termin
	m_nThreadJobState = Job_State_End;
	
	if (NULL != m_pTCPServerRequestMonitor)
	{
		m_pTCPServerRequestMonitor->terminateAndWait();
		DEF_DELETE(m_pTCPServerRequestMonitor);
	}

	if (NULL != m_pTCPServerSocket)
	{
		m_SetTCPServerSocket.removeSocket(m_pTCPServerSocket);
		DEF_DELETE(m_pTCPServerSocket);		
	}
	   	
	return nFunRes;
}

bool CTCPServerConnectionMonitor::isFinishWork()
{
	bool bIsFinishWork = false;
	if (Job_State_End == m_nThreadJobState)
	{
		bIsFinishWork = true;
	}
	return bIsFinishWork;
}




int CTCPServerConnectionMonitor::_Process_CheckParam()
{
	int nFunRes = 0;

	if (m_strListenSocketPort.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}
	m_nListenSocketPort = CUtilityFun::getInstance().mysys_str2Int32(m_strListenSocketPort);

	if (0 == m_nListenSocketPort || m_nListenSocketPort > INT_MAX/2)
	{
		nFunRes = -1;
		return nFunRes;
	}

	return nFunRes;
}

int CTCPServerConnectionMonitor::_Process_CreateListenSocket()
{
	int nFunRes = 0;
	try
	{
		DEF_DELETE(m_pTCPServerSocket);
		m_pTCPServerSocket = new  TcpServerSocket<TcpSocket>(BIND_TO_ALL_INTERFACES, m_strListenSocketPort);	
		m_pTCPServerSocket->bind();
		m_pTCPServerSocket->listen(); 
		_LOG(SourceFLInfo, DebugDebug, "server port = %s listening", m_strListenSocketPort.c_str());
		
		m_SetTCPServerSocket.addSocket(m_pTCPServerSocket);
		nFunRes = 0;
	}
	catch (...)
	{
		_LOG(SourceFLInfo, DebugError, "bind server port = %s error!", m_strListenSocketPort.c_str());

		DEF_DELETE(m_pTCPServerSocket);
		nFunRes = -1;
		return nFunRes;
	}
	
	return nFunRes;
}


int CTCPServerConnectionMonitor::_Process_WaitForNewClient()
{
	int nFunRes = 0;
	TcpServerSocket<TcpSocket>* pTCPServerSocket = NULL;

	m_bTCPRead = true;
	m_bTCPWrite = true; 

	m_nTCPSelectRes = -1; 
	m_nTCPSelectRes = m_SetTCPServerSocket.waitForIO( m_bTCPRead, m_bTCPWrite, m_timeTCPWait );//put into io

	m_bTCPRead = false;
	m_bTCPWrite = false; 
	pTCPServerSocket = NULL;
	pTCPServerSocket = m_SetTCPServerSocket.getSocket(0, m_bTCPRead, m_bTCPWrite );

	if ((m_bTCPRead || m_bTCPWrite)&& NULL != pTCPServerSocket)
	{
		nFunRes = _AcceptClient(pTCPServerSocket);
	}

	if (m_SetTCPServerSocket.getSize() <= 0)
	{
		Thread::sleep(DEF_INT_NormalThreadSleep);
	}

	return nFunRes;
}



int CTCPServerConnectionMonitor::_AcceptClient(TcpServerSocket<TcpSocket>* pTCPServerSocket)
{
	int nFunRes = 0;
	TcpSocket*	pNewClientSocket = NULL;
	CTCPConnectionActor* pActor = NULL;

	try
	{
		pNewClientSocket = pTCPServerSocket->accept(SERVER_NON_BLOCKING_ACCEPT, false); //SERVER_BLOCKING_ACCEPT
		nFunRes = 0;		
	}
	catch(...)
	{
		_LOG(SourceFLInfo, DebugError, "ServerSocket accept new Tcp Client error");
		DEF_DELETE(pNewClientSocket);
		nFunRes = -1;
		return nFunRes;
	}


	try
	{
		pActor = new CTCPConnectionActor(pNewClientSocket);
		pNewClientSocket = NULL;

	}
	catch (...)
	{
		DEF_DELETE(pActor);
	}

	m_pTCPServerRequestMonitor->addSocket(pActor);
	

	return nFunRes;
}


NS_END(TA_TwoToOnePokerGameNet)




