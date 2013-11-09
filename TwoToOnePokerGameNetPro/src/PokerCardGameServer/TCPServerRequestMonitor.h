#ifndef CTCP_SERVER_REQUEST_MONITOR_H_INCLUDED
#define CTCP_SERVER_REQUEST_MONITOR_H_INCLUDED

#include "CommonDef.h"
#include "CommonData.h"
#include "Thread.h"
#include "ReEntrantThreadLockable.h"
#include "SocketSet.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

class CTCPConnectionActor;
class TcpSocket;
class CTCPServerRequestMonitor : public Thread
{
private:
	typedef std::map<TcpSocket*, CTCPConnectionActor*>					mapTCPSocketInfoConT;
	typedef std::map<TcpSocket*, CTCPConnectionActor*>::iterator		mapTCPSocketInfoConIterT;
	typedef std::map<TcpSocket*, CTCPConnectionActor*>::value_type		mapTCPSocketInfoValueTypeT;
private:
	enum EThreadJobState
	{
		Job_State_Begin,


		Job_State_End,
	};


public:
	CTCPServerRequestMonitor(); 
	~CTCPServerRequestMonitor();
	virtual void run() ;	
	virtual void terminate() ;

		
private:
	bool	m_toTerminate;
	EThreadJobState m_nThreadJobState;	
private:
	int    _ThreadJob();
public:
	bool  isFinishWork();

private:
	int	  _ProcessUserTerminate();

private:
	void  _AddNewMemberToMonitorQueue();
	int   _CheckMonitorQueue();
	int	  _ProcessMonitorQueue();
	int   _ProcessRemoveQueue();


	void  _ClearAllQueue();

public: 
	void addSocket(CTCPConnectionActor* pTCPSocketInfoRef );
	unsigned int getSize();
	

private:
	bool 						m_bTCPRead;
	bool						m_bTCPWrite;
	unsigned int				m_nWaitTimeMillis;

	/*
	* MonitorLock
	*/
	ReEntrantThreadLockable	    m_LockMonitorQueue;

	SocketSet<TcpSocket>		m_SetMonitorQueue;	//MonitorLock
	mapTCPSocketInfoConT		m_mapMonitorQueue;//MonitorLock

private:
	/*
	* WaitQueue MonitorLock
	*/
	ReEntrantThreadLockable	    m_LockWaitQueue;
	mapTCPSocketInfoConT		m_mapWaitQueue;//will be add to m_mapSocketMonitor

private:
	/*
	* RemoveQueue MonitorLock
	*/
	ReEntrantThreadLockable	    m_LockRemoveQueue;
	mapTCPSocketInfoConT		m_mapRemoveQueue;//remove from m_mapSocketMonitor

};


NS_END(TA_TwoToOnePokerGameNet)

#endif  // CTCP_SERVER_REQUEST_MONITOR_H_INCLUDED
