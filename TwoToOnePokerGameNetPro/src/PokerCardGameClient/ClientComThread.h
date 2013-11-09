#if !defined(___CLASS_CLIENT_COM_THREAD_HH____)
#define ___CLASS_CLIENT_COM_THREAD_HH____


#include "CommonDef.h"
#include "CommonData.h"

#include "Thread.h"
#include "ReEntrantThreadLockable.h"

#include "TcpSocket.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)

class CBuffer;
class CRecvDataWorker;

class CClientComThread : public Thread
{

public:
	CClientComThread(const std::string& strServerIP, const std::string& strServerPort);
	virtual ~CClientComThread();

public:		
	virtual void	run();	
	virtual void	terminate();
	
public:
	int		clientSendData(const std::string& strServerID, CBuffer* pBuffer);
	int		connectToServer(int nTimeOut);
	
private:	
	void		_ThreadJob();

private: 
	int			_ConnectToServer(int nTimeOut);
	int			_SocketRead();
	int			_ProcessRecvBuffer();	
	//int			_ProcessRcvMsgPackages();
	
	/*
	*  _SocketWriteBuffer return value 0 or -1
	* return value is 0  : send data ok
	* return value is -1  : send data error
	*/
	int			_SocketWriteBuffer(CBuffer* pBuffer);	

private:
	/*
	* socket Read lock
	*/
	ReEntrantThreadLockable	    m_SocketReadLock;
	CRecvDataWorker*    m_pRecvDataWorker;	//use socket Read lock

	
	/*
	* socket info lock
	*/
	ReEntrantThreadLockable		m_SocketInfoLock;
	bool                        m_isConnected;//use socket info lock

	/*
	* socket write lock
	*/
	ReEntrantThreadLockable	    m_SocketWriteLock;
	
	TcpSocket*					m_pSocket;

private:
	int							m_socketTimeout; 
	std::string					m_strServerIP;
	std::string					m_strServerPort;
	std::string					m_strServerID;

private:
	bool						m_toTerminate;

};


NS_END(TA_TwoToOnePokerGameNet)






#endif//___CLASS_CLIENT_COM_THREAD_HH____














