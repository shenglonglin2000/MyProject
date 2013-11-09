
#include <string>

#include "CommonDef.h"
#include "Buffer.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)



CBuffer::CBuffer(int nBufferSize)
{
	//init value
	m_pBuffer = NULL;			
	m_pWritePos = NULL;		
	m_pReadPos = NULL;	
	m_nBufferSize = 0;			
	m_nBufferSizeLimit = DEF_INT_BUFFER_LEN_MAX;//268M
	m_nRemainDataLen = 0;
	
	//set value
	if (nBufferSize > m_nBufferSizeLimit) 
	{
		nBufferSize = DEF_INT_BUFFER_LEN_MAX;
	}
	m_pBuffer = new char[nBufferSize];
	memset(m_pBuffer, 0, nBufferSize);

	m_pWritePos = m_pBuffer;
	m_pReadPos = m_pBuffer;
	m_nBufferSize = nBufferSize;
	m_nRemainDataLen = m_pWritePos - m_pReadPos;

}

CBuffer::~CBuffer()
{
	if (m_pBuffer) 
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
		
	m_pBuffer = NULL;			
	m_pWritePos = NULL;		
	m_pReadPos = NULL;	
	m_nBufferSize = 0;			
	m_nBufferSizeLimit = 0;
	m_nRemainDataLen = 0;
}


char* CBuffer::getWritePos()  
{
	return m_pWritePos; 
}

char* CBuffer::getReadPos()  
{
	return m_pReadPos; 
}

int CBuffer::getRemainDataLength()  
{
	m_nRemainDataLen = (int)(m_pWritePos - m_pReadPos);

	return m_nRemainDataLen; 
}
int CBuffer::getBlankSize()  
{
	return m_pBuffer + m_nBufferSize - m_pWritePos; 
}
int CBuffer::getLimit()  
{
	return m_nBufferSizeLimit; 
}
int CBuffer::getBufferSize()  
{ 
	return m_nBufferSize; 
}
//////////////////////////////////////////////////////////////////////////

bool CBuffer::write(const void* src, int nLen)
{
	char* pPosTmp = NULL;
	const char* byteaddr = (const char*)src;
	if (nLen <= 0)	
	{
		return false;
	}
	
	pPosTmp = m_pWritePos + nLen;
	if (pPosTmp > (m_pBuffer + m_nBufferSize))
	{
		return false;
	}

	memcpy(m_pWritePos, byteaddr, nLen);
	m_pWritePos = pPosTmp;
	
	getRemainDataLength();

	pPosTmp = NULL;
	return true;
}

bool CBuffer::read(void* dest, int nLen)
{
	char* pPosTmp = NULL;

	if (nLen <= 0)
	{
		return false;
	}
	
	pPosTmp = m_pReadPos + nLen;
	if (pPosTmp > m_pWritePos)
	{
		return false;
	}

	memcpy(dest, m_pReadPos, nLen);
	m_pReadPos = pPosTmp;
	
	getRemainDataLength();
	
	pPosTmp = NULL;
	return true;
}

bool CBuffer::moveWritePos(int nLen)
{
	char* pPosTmp = NULL;
	
	pPosTmp = m_pWritePos + nLen;
	
	if (pPosTmp < m_pReadPos)
	{
		return false;
	}
	
	if (pPosTmp > (m_pBuffer + m_nBufferSize))	
	{
		return false;
	}
	m_pWritePos = pPosTmp;
	getRemainDataLength();

	pPosTmp = NULL;
	return true;
}

bool CBuffer::moveReadPos(int nLen)
{
	char* pPosTmp = NULL;

	pPosTmp = m_pReadPos + nLen;
	if (pPosTmp < m_pBuffer)
	{
		return false;
	}
	
	if (pPosTmp > m_pWritePos)	
	{
		return false;
	}
	m_pReadPos = pPosTmp;
	
	getRemainDataLength();

	pPosTmp = NULL;
	return true;
}

void CBuffer::clear()		
{
	m_pWritePos = m_pBuffer;
	m_pReadPos = m_pBuffer;
	getRemainDataLength();

}

void CBuffer::trim()		
{
	int nDataLen = 0;
	
	nDataLen = m_pWritePos - m_pReadPos;
	if (nDataLen > 0)
	{
		memcpy(m_pBuffer, m_pReadPos, nDataLen);
	}
	m_pReadPos = m_pBuffer;
	m_pWritePos = m_pBuffer + nDataLen;
	memset(m_pWritePos, 0, m_nBufferSize - nDataLen);
	
	getRemainDataLength();

}


//////////////////////////////////////////////////////////////////////////

bool CBuffer::resize(int nsize)
{
	int nDataLen = 0;
	char* pszTmp = NULL;

	if (nsize > m_nBufferSizeLimit)
	{
		nsize = m_nBufferSizeLimit;
	}
	
	nDataLen = m_pWritePos - m_pReadPos;
	if (nDataLen >= nsize)
	{
		return false;
	}
	try
	{
		pszTmp = new char[nsize];
		memset(pszTmp, 0, nsize);
		memcpy(pszTmp, m_pReadPos, nDataLen);
		delete[] m_pBuffer;
		m_pBuffer = NULL;

		m_pBuffer = pszTmp;
		pszTmp = NULL;
		m_pReadPos = m_pBuffer;
		m_pWritePos = m_pBuffer + nDataLen;
		m_nBufferSize = nsize;
	}
	catch (...)
	{
		return false;
	}
	
	getRemainDataLength();

	return true;
}

bool CBuffer::resizeFitData()
{
	char* pszTmp = NULL;

	int nDataLen = m_pWritePos - m_pReadPos;
	int nBufferSize = nDataLen + DEF_INT_BUFFER_LEN_ADDEX;
	try
	{
		pszTmp = new char[nBufferSize];
		memset(pszTmp, 0, nBufferSize);
		if (nDataLen < nBufferSize)
		{
			memcpy(pszTmp, m_pReadPos, nDataLen);
		}
		delete[] m_pBuffer;
		m_pBuffer = NULL;

		m_pBuffer = pszTmp;
		pszTmp = NULL;
		m_pReadPos = m_pBuffer;
		m_pWritePos = m_pBuffer + nDataLen;
		m_nBufferSize = nBufferSize;
	}
	catch (...)
	{
		return false;
	}
	
	getRemainDataLength();

	return true;
}

void CBuffer::prepareSize( int nSize )
{
	if (this->getBlankSize() < nSize)
	{
		this->resize(this->getRemainDataLength() + nSize + DEF_INT_BUFFER_LEN_ADDEX);
	}

}
void CBuffer::print()
{
	char* pPrintPos = m_pReadPos;
	if (getRemainDataLength() <= 0)
	{
		printf("[BufferData=%s]\n","");
		return;
	}
	
	printf("[BufferData=");
	for (pPrintPos = m_pReadPos; pPrintPos < m_pWritePos; pPrintPos++)
	{
		char chPos = *pPrintPos;
		if (0x20 <= chPos && chPos <= 0x7E)	  //32 <=chpso <= 126
		{
			printf("%c", chPos);
		}
		else
		{
			printf("%02X", chPos);
		}
		
	}
	printf("]\n");
}


NS_END(TA_TwoToOnePokerGameNet)





