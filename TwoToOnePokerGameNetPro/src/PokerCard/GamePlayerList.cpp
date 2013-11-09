#include "GamePlayerList.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"
#include "PokerCard.h"
#include "GamePlayer.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)



CGamePlayerList::CGamePlayerList(void)
{
	m_pGamePlayerList = NULL;
	m_pGamePlayerList = new std::list<CGamePlayer*>();

}


CGamePlayerList::~CGamePlayerList(void)
{
	m_pGamePlayerList->clear();
	DEF_DELETE(m_pGamePlayerList);
}



void CGamePlayerList::clearAllData()
{
	int nFunRes = 0;
	CGamePlayer* pGamePlayerGet = NULL;
	std::list<CGamePlayer*>::iterator  iterList;

	iterList = m_pGamePlayerList->begin();
	while (iterList != m_pGamePlayerList->end())
	{
		pGamePlayerGet = *iterList;
		DEF_DELETE(pGamePlayerGet);
		iterList++;
	}//while
	m_pGamePlayerList->clear();
}


void CGamePlayerList::clear()
{
	m_pGamePlayerList->clear();
}


void CGamePlayerList::addGamePlayer(CGamePlayer* pGamePlayer)
{
	m_pGamePlayerList->push_back(pGamePlayer);
}

int CGamePlayerList::removeGamePlayer(CGamePlayer* pGamePlayer)
{
	int nFunRes = 0;
	std::list<CGamePlayer*>::iterator iterList;

	iterList = m_pGamePlayerList->begin();

	iterList = std::find(m_pGamePlayerList->begin(), m_pGamePlayerList->end(), pGamePlayer);

	if (iterList != m_pGamePlayerList->end())
	{
		m_pGamePlayerList->erase(iterList);
	}
	else
	{
		nFunRes = -1;
	}
	return nFunRes;

}


int CGamePlayerList::getCount()
{
	int nCount = 0;
	nCount = (int)m_pGamePlayerList->size();
	return nCount;
}


//begin with 0
CGamePlayer* CGamePlayerList::getNodeByIndex(int nIndex)
{
	int nFunRes = 0;
	CGamePlayer* pGamePlayerGet = NULL;
	std::list<CGamePlayer*>::iterator  iterList;
	int nIndexTmp = 0;

	if ((int)m_pGamePlayerList->size() <= 0 || nIndex < 0 || nIndex >= (int)m_pGamePlayerList->size())
	{
		nFunRes = -1;
		return pGamePlayerGet;
	}
	
	iterList = m_pGamePlayerList->begin();
	nIndexTmp = 0;
	while (iterList != m_pGamePlayerList->end())
	{
		if (nIndexTmp == nIndex)
		{
			//find node
			nFunRes = 0;
			pGamePlayerGet = *iterList;
			break;
		}
		iterList++;
		nIndexTmp++;

	}//while


	return pGamePlayerGet;
}


int CGamePlayerList::getIndexByPlayerPosition(PlayerPosition nPlayerPosition)
{
	int nIndexPlayerPosition = 0;
	CGamePlayer* pGamePlayerGet = NULL;
	std::list<CGamePlayer*>::iterator  iterList;

	nIndexPlayerPosition = 0;
	iterList = m_pGamePlayerList->begin();
	while (iterList != m_pGamePlayerList->end())
	{
		pGamePlayerGet = *iterList;
		if (nPlayerPosition == pGamePlayerGet->getPosition())
		{
			//find node			
			break;
		}
		iterList++;
		nIndexPlayerPosition++;

	}//while

	return nIndexPlayerPosition;
}


NS_END(TA_TwoToOnePokerGameNet)





