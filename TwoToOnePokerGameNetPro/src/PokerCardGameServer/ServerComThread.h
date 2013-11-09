#ifndef __CLASS_TCP_SERVER_COM_THREAD__HH__
#define __CLASS_TCP_SERVER_COM_THREAD__HH__

#include "CommonDef.h"

#include "Thread.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)

class CRecvDataWorker;
class TcpSocket;


class CServerComThread : public Thread
{
private:
	enum EThreadJobState
	{
		Job_State_Begin,

		Job_State_DoingJob,

		Job_State_End,
	};
public:
	CServerComThread(TcpSocket* pSocketRef); 
	~CServerComThread();
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
//////////////////////////////////////////////////////////////////////////

public:
	int recvData(bool bCanRead);
	bool isStillConnected();

private:
	int _AnalysisAndProcessData();

private:
	CRecvDataWorker*    m_pRecvDataWorker;	//read
	bool                m_bIsStillConnected;


};


NS_END(TA_TwoToOnePokerGameNet)

#endif  // __CLASS_TCP_SERVER_COM_THREAD__HH__
