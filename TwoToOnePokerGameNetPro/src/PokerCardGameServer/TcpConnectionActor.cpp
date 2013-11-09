#include "TcpConnectionActor.h"

#include <string>
#include "CommonDef.h"
#include "CommonData.h"
#include "TcpSocket.h"
#include "DebugUtil.h"
#include "ServerComThread.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)

   

CTCPConnectionActor::CTCPConnectionActor(TcpSocket* pSocket)
{
	m_pTcpSocket = pSocket;	
	m_nTcpSocketID = 0;
	m_strClientName.clear();
	_GetSocketInfo();

	m_pServerComThread = new CServerComThread(m_pTcpSocket);//ref
	m_pServerComThread->start();
}

CTCPConnectionActor::~CTCPConnectionActor()
{
	_LOG(SourceFLInfo, DebugDebug, "delete one client : strClientName=%s, nSocketID=%d", m_strClientName.c_str(), m_nTcpSocketID);
	
    DEF_DELETE(m_pTcpSocket);
	if (NULL != m_pServerComThread)
	{
		m_pServerComThread->terminateAndWait();
		DEF_DELETE(m_pServerComThread);
	}
	
	m_nTcpSocketID = 0;
	m_strClientName.clear();

}

int CTCPConnectionActor::_GetSocketInfo()
{
   int nFunRes = 0;
   
   try
   {
		m_strClientName = m_pTcpSocket->getClientName();
		m_nTcpSocketID = m_pTcpSocket->getSocketId();	
		
		_LOG(SourceFLInfo, DebugDebug, "server recv new client: strClientName=%s, nSocketID=%d", m_strClientName.c_str(), m_nTcpSocketID);
		
   }
   catch (...)
   {
	   _LOG(SourceFLInfo, DebugError, "get Socket info error:  strClientName=%s, nSocketID=%d", m_strClientName.c_str(), m_nTcpSocketID);

		nFunRes = -1;
   }
   
   
   return nFunRes;
}


TcpSocket* CTCPConnectionActor::getSocket()
{
	return m_pTcpSocket;
}

bool CTCPConnectionActor::isStillConnected()
{
	bool bIsStillConnected = false;
	if (NULL != m_pServerComThread)
	{
		bIsStillConnected = m_pServerComThread->isStillConnected();
	}

	if (false == bIsStillConnected)
	{
		_LOG(SourceFLInfo, DebugDebug, "client disconnected to server: strClientName=%s, nSocketID=%d", m_strClientName.c_str(), m_nTcpSocketID);
	}

	return bIsStillConnected;
}

int CTCPConnectionActor::recvData(bool bCanRead)
{
	int nFunRes = 0;

	//TODO..
	if ( true == bCanRead)
	{
		m_pServerComThread->recvData(bCanRead);
	}

	return nFunRes;
}



NS_END(TA_TwoToOnePokerGameNet)





