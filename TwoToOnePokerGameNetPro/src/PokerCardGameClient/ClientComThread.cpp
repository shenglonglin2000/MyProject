#include "ClientComThread.h"
	
#include "TcpSocket.h"
#include "DebugUtil.h"
#include "RecvDataWorker.h"
#include "Buffer.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

CClientComThread::CClientComThread(
const std::string& strServerIP, 
const std::string& strServerPort
)
{	
	m_strServerIP = strServerIP;
	m_strServerPort = strServerPort;
	m_strServerID = m_strServerIP + "__" + m_strServerPort;

	m_socketTimeout = DEF_INT_LIMIT_TCP_TIMEOUT;
	m_pSocket = NULL;
	m_isConnected = false;
	m_toTerminate = false;
}

CClientComThread::~CClientComThread()
{
	DEF_DELETE(m_pSocket);
}


void CClientComThread::run()
{
	while( false == m_toTerminate ) 
	{
		_ThreadJob();
		//TA_Base_Core::Thread::sleep(SERVER_LIMIT_THREAD_SLEEP);
	}//while
	

}



void CClientComThread::terminate()
{		
	m_toTerminate = true;		
}



void CClientComThread::_ThreadJob()
{
	int nFunRes = 0;
	bool bRecvRes = true;
	bool bDataReadly = true;
		
	nFunRes = _SocketRead();
	if (-1 == nFunRes)
	{
		//LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "err:read data from socket error!");
		Thread::sleep(DEF_INT_NormalThreadSleep);
		nFunRes = -1;
		return;
	}
	else if (TCP_NODATAFORREAD == nFunRes)
	{
		Thread::sleep(DEF_INT_NormalThreadSleep);
	}


	_ProcessRecvBuffer();
	//_ProcessRcvMsgPackages();	
		
	return;
	
}

int CClientComThread::_ConnectToServer(int nTimeOut)
{
	int nFunRes = 0;
	
	ThreadGuard guardRead(m_SocketReadLock);
	ThreadGuard guardWrite(m_SocketWriteLock);
	ThreadGuard guardInfo(m_SocketInfoLock);


	//init data	 
	DEF_DELETE(m_pRecvDataWorker);
	DEF_DELETE(m_pSocket);
	m_isConnected = false;

	//connect to server
	try
	{
		//create new Socket
		m_pSocket = new TcpSocket(m_strServerIP, m_strServerPort);
		_LOG(SourceFLInfo, DebugDebug, "begin connect to [ServerID=%s]", m_strServerID.c_str());

		m_pSocket->connect();
		m_isConnected = m_pSocket->stillConnected();
		_LOG(SourceFLInfo, DebugDebug, "connect to [ServerID=%s][isConnected=%d] ok!", m_strServerID.c_str(), m_isConnected);
				
		m_pRecvDataWorker = new CRecvDataWorker(m_pSocket);
		nFunRes = 0;									
	}
	catch (...)
	{
		m_isConnected = false;

		_LOG(SourceFLInfo, DebugError, "Failed to connect to %s:%s", 
			m_strServerIP.c_str(), m_strServerPort.c_str());
		//int WError = WSAGetLastError();
		nFunRes = -1;
	}
	
	return nFunRes;
}	



int	CClientComThread::connectToServer(int nTimeOut)
{
	int nFunRes = 0;
	nFunRes = _ConnectToServer(nTimeOut);
	return nFunRes;
}

 


int CClientComThread::_SocketRead()
{ 
	int					nFunRes = 0;
	unsigned int		nGetLength = 0;
	int					nRcvedRes = -1;		
	bool				bCanRead = false;
	 	
	ThreadGuard guard(m_SocketReadLock);

	{
		ThreadGuard guard(m_SocketInfoLock);
		if (!m_isConnected)
		{
			//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "client disconnect to server");
			nFunRes = -1;
			return nFunRes;
		}
		else
		{
			//check if TCP is still connected
			//when the server side disconnected and the client has no data to server, 
			//then the client side does not know the disconnection except calling this interface
			m_isConnected = m_pSocket->stillConnected();
		}		

		if (!m_isConnected)
		{
			_LOG(SourceFLInfo, DebugError, "lost connect with server,[ServerID=%s]", m_strServerID.c_str());
			m_isConnected = false;

			nFunRes = -1;
			return nFunRes;
		}

	}



	//check if have data to read
	bCanRead = m_pSocket->isReadRequired(m_socketTimeout);		
	if (!bCanRead)
	{
		//if bCanRead = false, no data in socket can be readed
		nFunRes = TCP_NODATAFORREAD;
		return nFunRes;
	} 

	_LOG(SourceFLInfo, DebugDebug, "[ServerID=%s][isReadRequired() == %d]", 
		m_strServerID.c_str(), bCanRead );

	try
	{  
		m_pRecvDataWorker->readData();
	}
	catch (...)
	{
		ThreadGuard guard(m_SocketInfoLock);
		_LOG(SourceFLInfo, DebugError, "err: read data from socket error! [ServerID=%s]", m_strServerID.c_str());
		m_isConnected = false;
		nFunRes = -1;	
	}

	return nFunRes;
}


int CClientComThread::_ProcessRecvBuffer()
{
	ThreadGuard guard(m_SocketReadLock);
	int			nFunRes = 0;
	bool		bHaveOneFrame = false;

	m_pRecvDataWorker->analysisAndProcessData();

	return nFunRes;
}



int CClientComThread::clientSendData(const std::string& strServerID, CBuffer* pBuffer)
{
	int		nSendRes = 0;
	char*	pSendPos = NULL;
	int     nSendLen = 0;

	ThreadGuard guard(m_SocketWriteLock);

	{
		ThreadGuard guard(m_SocketInfoLock);
		if (!m_isConnected)
		{
			nSendRes = -1;
			return nSendRes;
		}
	}

	pSendPos = pBuffer->getReadPos();
	nSendLen = pBuffer->getRemainDataLength();

	try
	{		
		nSendRes = m_pSocket->write(pSendPos, nSendLen);
		
		_LOG(SourceFLInfo, DebugDebug, "<--client send data to server [nSendRes=%d][nBufLen=%d][ServerID=%s]",
			nSendRes, nSendLen, m_strServerID.c_str());

		if (nSendRes != nSendLen)
		{
			ThreadGuard guard(m_SocketInfoLock);
			m_isConnected = false;

			_LOG(SourceFLInfo, DebugError, "error! client send data to server error![nSendRes=%d][nBufLen=%d][ServerID=%s]",
				nSendRes, nSendLen, m_strServerID.c_str()); 
			nSendRes = -1;
		}
		else
		{
			//send data ok, return value is 0
			nSendRes = 0;
		}
	}
	catch (...)
	{
		ThreadGuard guard(m_SocketInfoLock);
		m_isConnected = false;

		_LOG(SourceFLInfo, DebugError,  "error! client send data to server error! [ServerID=%s]", m_strServerID.c_str());
		nSendRes = -1;
	}

	return nSendRes;
}




 
NS_END(TA_TwoToOnePokerGameNet)





