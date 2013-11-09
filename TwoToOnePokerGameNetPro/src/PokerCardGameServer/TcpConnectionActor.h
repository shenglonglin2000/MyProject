#ifndef  __DEF_CLASS_TCP_CONNECTION_ACTOR__H__
#define  __DEF_CLASS_TCP_CONNECTION_ACTOR__H__


#include "CommonDef.h"
#include "CommonData.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

class TcpSocket;
class CServerComThread;

class CTCPConnectionActor 
{
public:
	CTCPConnectionActor(TcpSocket* pSocket);
	virtual ~CTCPConnectionActor();
public:
	TcpSocket*	getSocket();
	bool isStillConnected();
	int recvData(bool bCanRead);

private:
	int _GetSocketInfo();

private:
	TcpSocket*			m_pTcpSocket;
	int					m_nTcpSocketID;
	std::string			m_strClientName;
	CServerComThread*    m_pServerComThread;	//read

};


NS_END(TA_TwoToOnePokerGameNet)


#endif //__DEF_CLASS_TCP_CONNECTION_ACTOR__H__






