#ifndef CTCP_SERVER_CONNECTION_MONITOR_H_INCLUDED
#define CTCP_SERVER_CONNECTION_MONITOR_H_INCLUDED

#include "CommonDef.h"
#include "CommonData.h"
#include "Thread.h"
#include "TcpSocket.h"
#include "TcpServerSocket.h"
#include "SocketSet.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

class CTCPServerRequestMonitor;

class CTCPServerConnectionMonitor : public Thread
{
private:
	enum EThreadJobState
	{
		Job_State_Begin,

		Job_State_CheckParam,
		Job_State_CreateListenSocket,
		Job_State_WaitForNewClient,

		Job_State_End,
	};
public:
	CTCPServerConnectionMonitor(const std::string& strListenSocketPort); 
	~CTCPServerConnectionMonitor();
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


public:
	void  acceptNewClient();
	
private:	
	int _Process_CheckParam();
	int _Process_CreateListenSocket();
	int _Process_WaitForNewClient();

private:
	int _AcceptClient(TcpServerSocket<TcpSocket>* pTCPServerSocket);

private:
	std::string   m_strListenSocketPort;	
	int			  m_nListenSocketPort;	
	TcpServerSocket<TcpSocket>*					m_pTCPServerSocket;//server listen socket
	SocketSet< TcpServerSocket< TcpSocket > >	m_SetTCPServerSocket;

	bool 	      m_bTCPRead;
	bool		  m_bTCPWrite; 
	int			  m_nTCPSelectRes; 
	unsigned int  m_timeTCPWait;//MilliSeconds
private:
	CTCPServerRequestMonitor* m_pTCPServerRequestMonitor;

};


NS_END(TA_TwoToOnePokerGameNet)

#endif  // CTCP_SERVER_CONNECTION_MONITOR_H_INCLUDED
