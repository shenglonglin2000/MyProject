#ifndef CRECV_DATA_WOKER_H_INCLUDED
#define CRECV_DATA_WOKER_H_INCLUDED

#include "CommonDef.h"
#include "CommonData.h"

#include "ReEntrantThreadLockable.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

class TcpSocket;
class CBuffer;


class CRecvDataWorker 
{
private:
	typedef		std::list<CBuffer*>							lstFrameBufferConT;
	typedef		std::list<CBuffer*>::iterator				lstFrameBufferConIterT;
	typedef		std::list<CBuffer*>::value_type				lstFrameBufferValueTypeT;

public:
	CRecvDataWorker(TcpSocket* pSocketRef);
	virtual ~CRecvDataWorker();
public:
	int			readData();
	int         analysisAndProcessData();
	int			writeBuffer(CBuffer* pSendBuff);//TODO.

private:
	bool		_HaveFrameInBuffer(CBuffer* pRecvBufferTotal);	
	int			_GetFrameFromBuffer(CBuffer* pRecvBufferTotal, lstFrameBufferConT& lstRecvFrame);
	int			_ProcessRecvFrame();

private:
	TcpSocket*	m_pSocketRef;//m_pSocketRef

	CBuffer*	m_pRecvBufferOnce;
	CBuffer*	m_pRecvBufferTotal;

	lstFrameBufferConT   m_lstRecvFrame;


	ReEntrantThreadLockable m_lockBufferDataTmp;
	CBuffer*	m_pBufferDataTmp;

	
private:	
	int         m_nIndexForTest;

};


NS_END(TA_TwoToOnePokerGameNet)


#endif //CRECV_DATA_WOKER_H_INCLUDED






