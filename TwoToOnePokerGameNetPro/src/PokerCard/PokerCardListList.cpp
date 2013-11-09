#include "PokerCardListList.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"
#include "PokerCard.h"
#include "PokerCardList.h"
#include "DebugUtil.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)



CPokerCardListList::CPokerCardListList(void)
{
	m_pPokerCardListList = NULL;
	m_pPokerCardListList = new std::list<CPokerCardList*>();
}


CPokerCardListList::~CPokerCardListList(void)
{
	m_pPokerCardListList->clear();
	DEF_DELETE(m_pPokerCardListList);
}


void CPokerCardListList::printInfoToLog()
{
	CPokerCardList* pPokderCardList = NULL;
	std::list<CPokerCardList*>::iterator  iterListList;

	_LOG(SourceFLInfo, DebugDebug, "CPokerCardListList [size=%d]", m_pPokerCardListList->size());

	iterListList = m_pPokerCardListList->begin();
	while (iterListList != m_pPokerCardListList->end())
	{
		pPokderCardList = *iterListList;
		pPokderCardList->printInfoToLog();
		iterListList++;
	}//while
}

void CPokerCardListList::clear()
{
	m_pPokerCardListList->clear();
}


void CPokerCardListList::clearAllData()
{
	CPokerCardList* pPokderCardListGet = NULL;
	std::list<CPokerCardList*>::iterator  iterListList;

	iterListList = m_pPokerCardListList->begin();
	while (iterListList != m_pPokerCardListList->end())
	{
		pPokderCardListGet = *iterListList;
		pPokderCardListGet->clear();//not use pPokderCardListGet->clearAllData()		

		delete pPokderCardListGet;
		pPokderCardListGet = NULL;

		iterListList++;
	}//while



}



void CPokerCardListList::addNode(CPokerCardList* pPokerCardList)
{
	m_pPokerCardListList->push_back(pPokerCardList);
}

void CPokerCardListList::addRange(CPokerCardListList* pPokerCardListList)
{
	CPokerCardList* pPokerCardListGet = NULL;
	
	for (int nIndex = 0; nIndex < (int)pPokerCardListList->getCount(); nIndex++)
	{
		pPokerCardListGet = pPokerCardListList->getNodeByIndex(nIndex);
		this->addNode(pPokerCardListGet);
	}

}


int  CPokerCardListList::getCount()
{
	return m_pPokerCardListList->size();
}


//begin with 0
 CPokerCardList* CPokerCardListList::getNodeByIndex(int nIndex)
{
	int nFunRes = 0;
	CPokerCardList* pPokderCardListGet = NULL;
	std::list<CPokerCardList*>::iterator  iterListList;
	int nIndexTmp = 0;

	if (m_pPokerCardListList->size() <= 0)
	{
		nFunRes = -1;
		return pPokderCardListGet;
	}

	if (nIndex < 0 || nIndex >= (int)m_pPokerCardListList->size())
	{
		nFunRes = -1;
		return pPokderCardListGet;
	}
	
	iterListList = m_pPokerCardListList->begin();
	nIndexTmp = 0;
	while (iterListList != m_pPokerCardListList->end())
	{
		if (nIndexTmp == nIndex)
		{
			//find node
			nFunRes = 0;
			pPokderCardListGet = *iterListList;
			break;
		}
		iterListList++;
		nIndexTmp++;

	}//while

	return pPokderCardListGet;
}


 int  CPokerCardListList::removeNode(CPokerCardList* pPokerCardList)
 {
	 int nFunRes = 0;
	 CPokerCardList* pPokderCardListGet = NULL;
	 std::list<CPokerCardList*>::iterator  iterListList;

	 if (m_pPokerCardListList->size() <= 0)
	 {
		 nFunRes = -1;
		 return nFunRes;
	 }
	 
	 nFunRes = -1;
	 iterListList = m_pPokerCardListList->begin();
	 while (iterListList != m_pPokerCardListList->end())
	 {
		 pPokderCardListGet = *iterListList;

		 if (pPokerCardList == pPokderCardListGet)
		 {
			 //find node
			 nFunRes = 0;
			 m_pPokerCardListList->erase(iterListList);
			 iterListList = m_pPokerCardListList->begin();
			 break;
		 }
		 iterListList++;
	 }//while

	 return nFunRes;
 }


NS_END(TA_TwoToOnePokerGameNet)





