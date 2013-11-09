// Buffer.h: interface for the CBuffer class.

#if !defined(__BUFFER_H_INCLUDED__)
#define __BUFFER_H_INCLUDED__

#include "CommonDef.h"
#include "CommonData.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)




class CBuffer  
{
private:
	int		m_nBufferSize;			
	int		m_nBufferSizeLimit;
	int		m_nRemainDataLen;			
	char*	m_pBuffer;			
	char*	m_pWritePos;		
	char*	m_pReadPos;		
public:
	CBuffer(int nBufferSize = DEF_INT_BUFFER_LEN_READ);
	~CBuffer();
public:
	char* getWritePos(); 
	char* getReadPos();
	int getRemainDataLength();
	int getBlankSize();
	int getBufferSize();
	int getLimit();
	void print();

public:
	bool write(const void* src, int nLen);
	bool moveWritePos(int nLen);

	bool read(void* dest, int nLen);
	bool moveReadPos(int nLen);

	void clear();		
	void trim();		

	bool resize(int nSize);
	bool resizeFitData();
	void prepareSize(int nSize);	

};


NS_END(TA_TwoToOnePokerGameNet)


#endif //__BUFFER_H_INCLUDED__






