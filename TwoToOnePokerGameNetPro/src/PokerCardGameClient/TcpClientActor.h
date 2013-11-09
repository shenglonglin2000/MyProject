#ifndef  __DEF_CLASS_TCP_CLIENT_ACTOR__H__
#define  __DEF_CLASS_TCP_CLIENT_ACTOR__H__


#include "CommonDef.h"
#include "CommonData.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

class TcpSocket;
class CClientComThread;


class CTCPClientActor 
{
public:
	CTCPClientActor(const std::string& strServerIP, const std::string& strServerPort);
	virtual ~CTCPClientActor();

private:
	CClientComThread*      m_pClientComThread;	//read
	bool                   m_bIsStillConnected;


private:
	std::string			m_strServerIP;
	std::string			m_strServerPort;

};


NS_END(TA_TwoToOnePokerGameNet)


#endif //__DEF_CLASS_TCP_CLIENT_ACTOR__H__






