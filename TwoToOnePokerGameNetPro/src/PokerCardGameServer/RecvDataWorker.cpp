#include "RecvDataWorker.h"

#include "CommonDef.h"
#include "CommonData.h"
#include "TcpSocket.h"
#include "Buffer.h"
#include "DebugUtil.h"
#include "ReEntrantThreadLockable.h"
#include "ThreadGuard.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)

   

CRecvDataWorker::CRecvDataWorker(TcpSocket* pSocketRef)
{
	m_pSocketRef = NULL;
	m_pSocketRef = pSocketRef;
	m_pRecvBufferOnce = new CBuffer(DEF_INT_BUFFER_LEN_READ);
	m_pRecvBufferTotal = new CBuffer(DEF_INT_BUFFER_LEN_READ);
	m_pBufferDataTmp = new CBuffer(DEF_INT_BUFFER_LEN_READ);
    m_pRecvBufferOnce->clear();
    m_pRecvBufferTotal->clear();
    m_nIndexForTest = 0;
}

CRecvDataWorker::~CRecvDataWorker()
{
	m_pSocketRef = NULL;
	DEF_DELETE(m_pRecvBufferOnce);
	DEF_DELETE(m_pRecvBufferTotal);
	DEF_DELETE(m_pBufferDataTmp);

}

int CRecvDataWorker::readData()
{
    int					nFunRes = 0;     
	unsigned int		nGetLength = 0;
	int					nRcvedRes = -1;		

	nGetLength = 0;
	nRcvedRes = -1;

	try
	{  
		m_pRecvBufferOnce->prepareSize(DEF_INT_BUFFER_LEN_READ);
		nRcvedRes = m_pSocketRef->read(m_pRecvBufferOnce->getWritePos(), &nGetLength, m_pRecvBufferOnce->getBlankSize()-1);
		m_pRecvBufferOnce->moveWritePos(nGetLength);
		nFunRes = 0;
	}
	catch (...)
	{
		m_pRecvBufferOnce->clear();
		nFunRes = -1;
		return nFunRes;	
	}

	if (nGetLength <= 0 || nRcvedRes != 0)
	{
		_LOG(SourceFLInfo, DebugError, "Read Data Error! GetDatlength = %d, RecvRes = %d", nGetLength, nRcvedRes);
		m_pRecvBufferOnce->clear();
		nFunRes = -1;
		return nFunRes;	
	}

	//copy data
	{
		ThreadGuard guid(m_lockBufferDataTmp);
		m_pBufferDataTmp->prepareSize(m_pRecvBufferOnce->getRemainDataLength());
		m_pBufferDataTmp->write(m_pRecvBufferOnce->getReadPos(), m_pRecvBufferOnce->getRemainDataLength());
		//m_pRecvBufferOnce->moveReadPos(m_pRecvBufferOnce->getRemainDataLength());
		m_pRecvBufferOnce->clear();		
	}
	nFunRes = 0;
		  
	return nFunRes;
}

bool CRecvDataWorker::_HaveFrameInBuffer(CBuffer* pRecvBufferTotal)
{
	bool bHaveFrame = false;

	char* pszFrameEnd = "\r\n";
	char* pPosFrameEnd = NULL;
	int nFrameSize = 0;

	pPosFrameEnd = NULL;
	nFrameSize = 0;
	pPosFrameEnd = strstr(pRecvBufferTotal->getReadPos(), pszFrameEnd);//find frame end pos
	if (NULL != pPosFrameEnd)
	{
		pPosFrameEnd += strlen(pszFrameEnd);
		nFrameSize = (pPosFrameEnd - pRecvBufferTotal->getReadPos());//frame end pos is in data area
	}

	bHaveFrame = false;
	if (NULL != pPosFrameEnd && nFrameSize > 0 && nFrameSize <= pRecvBufferTotal->getRemainDataLength())
	{
		bHaveFrame = true;
	}

	return bHaveFrame;
}

int CRecvDataWorker::_GetFrameFromBuffer(CBuffer* pRecvBufferTotal, lstFrameBufferConT& lstRecvFrame)
{
	int nFunRes = 0;

	char* pszFrameEnd = "\r\n";
	char* pPosFrameEnd = NULL;
	int nFrameSize = 0;
	CBuffer* pFrameBuff = NULL;

	pPosFrameEnd = NULL;
	nFrameSize = 0;
	pPosFrameEnd = strstr(pRecvBufferTotal->getReadPos(), pszFrameEnd);//find frame end pos
	if (NULL != pPosFrameEnd)
	{
		pPosFrameEnd += strlen(pszFrameEnd);
		nFrameSize = (pPosFrameEnd - pRecvBufferTotal->getReadPos());//frame end pos is in data area
	}
	else
	{
		nFunRes = -1;
		return nFunRes;
	}

	if (nFrameSize > 0 && nFrameSize <= pRecvBufferTotal->getRemainDataLength())
	{
		pFrameBuff = new CBuffer(nFrameSize + DEF_INT_BUFFER_LEN_READ);
		pRecvBufferTotal->read(pFrameBuff->getWritePos(), nFrameSize);
		pFrameBuff->moveWritePos(nFrameSize);
		pRecvBufferTotal->trim();
		lstRecvFrame.push_back(pFrameBuff);

		pFrameBuff = NULL;
		nFunRes = 0;
	}
	else
	{
		nFunRes = -1;
	}

	return nFunRes;
}


int CRecvDataWorker::analysisAndProcessData()
{
	int nFunRes = 0;
	bool bConninueProceeData = true;
	bool bHaveFrame = false;


	//copy data
	{
		ThreadGuard guid(m_lockBufferDataTmp);
		if (m_pBufferDataTmp->getRemainDataLength() > 0)
		{
			m_pRecvBufferTotal->prepareSize(m_pBufferDataTmp->getRemainDataLength());
			m_pRecvBufferTotal->write(m_pBufferDataTmp->getReadPos(), m_pBufferDataTmp->getRemainDataLength());
			//m_pBufferDataTmp->moveReadPos(m_pBufferDataTmp->getRemainDataLength());
			m_pBufferDataTmp->clear();
		}
	}



	if (m_pRecvBufferTotal->getRemainDataLength() <= 0)
	{
		nFunRes = 0;
		bConninueProceeData = false;
		return nFunRes;
	}

	while (true == bConninueProceeData)
	{
		bHaveFrame = false;
		bHaveFrame = _HaveFrameInBuffer(m_pRecvBufferTotal);
		if (true == bHaveFrame)
		{
			nFunRes = _GetFrameFromBuffer(m_pRecvBufferTotal, m_lstRecvFrame);
		}
		else
		{
			bConninueProceeData = false;
		}
	}//while (true == bConninueProceeData)
	
	_ProcessRecvFrame();//m_lstRecvFrame

	return nFunRes;
	
}



int CRecvDataWorker::_ProcessRecvFrame()
{
	int nFunRes = 0;

	if (m_lstRecvFrame.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}

	
	//process recv frame
	lstFrameBufferConIterT iterLst = m_lstRecvFrame.begin();
	while (iterLst != m_lstRecvFrame.end())
	{
		////TODO.
		//
		CBuffer* pRecvFrame = *iterLst;
		//print data
		pRecvFrame->print();
		DEF_DELETE(pRecvFrame);

		//
		iterLst++;
	}//while (iterLst != m_lstRecvFrame.end())

	m_lstRecvFrame.clear();

	return nFunRes;
}




int CRecvDataWorker::writeBuffer(CBuffer* pSendBuff)
{
	int		nSendRes = 0;
	char*	pSendPos = NULL;
	int     nSendLen = 0;


	pSendPos = pSendBuff->getReadPos();
	nSendLen = pSendBuff->getRemainDataLength();

	try
	{		
		nSendRes = m_pSocketRef->write(pSendPos, nSendLen);

		if (nSendRes != nSendLen)
		{
			_LOG(SourceFLInfo, DebugError, 
				"Write Data Error, DataLen = %d, SendDataLen = %d", nSendLen, nSendRes);
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
		nSendRes = -1;
	}

	return nSendRes;
}


NS_END(TA_TwoToOnePokerGameNet)





