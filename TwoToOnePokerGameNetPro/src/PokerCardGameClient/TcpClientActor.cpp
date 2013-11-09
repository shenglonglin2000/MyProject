#include "TcpClientActor.h"

#include "CommonDef.h"
#include "CommonData.h"
#include "TcpSocket.h"
#include "DebugUtil.h"
#include "ClientComThread.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

   

CTCPClientActor::CTCPClientActor(const std::string& strServerIP, const std::string& strServerPort)
{
	m_strServerIP = strServerIP;
	m_strServerPort = strServerPort;
	m_pClientComThread = NULL;
	m_bIsStillConnected = false;
	m_pClientComThread = new CClientComThread(strServerIP, strServerPort );
	m_pClientComThread->start();
	
}

CTCPClientActor::~CTCPClientActor()
{
	_LOG(SourceFLInfo, DebugDebug, "delete one client : m_strServerIP=%s, m_strServerPort=%s", m_strServerIP.c_str(), m_strServerPort.c_str());
	
	if (NULL != m_pClientComThread)
	{
		m_pClientComThread->terminateAndWait();
		DEF_DELETE(m_pClientComThread);
	}
	


}







NS_END(TA_TwoToOnePokerGameNet)





