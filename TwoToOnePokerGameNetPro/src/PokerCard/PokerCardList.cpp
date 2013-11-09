#include "PokerCardList.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"
#include "PokerCard.h"
#include "DebugUtil.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)



CPokerCardList::CPokerCardList(void)
{
	m_pPokerCardList = NULL;
	m_pPokerCardList = new std::list<CPokerCard*>();
}


CPokerCardList::~CPokerCardList(void)
{
	m_pPokerCardList->clear();
	DEF_DELETE(m_pPokerCardList);
}


//=
CPokerCardList& CPokerCardList::operator = (const CPokerCardList& rhs)
{
	int nIndex = 0;
	int nCount = rhs.getCount();
	CPokerCard* pPokerCardGet = NULL;
	CPokerCard* pPokerCardNew = NULL;

	for (nIndex = 0; nIndex < nCount; nIndex++)
	{

		pPokerCardGet = rhs.getNodeByIndex(nIndex);
		pPokerCardNew = new CPokerCard();
		//copy node
		*pPokerCardNew = *pPokerCardGet;

		m_pPokerCardList->push_back(pPokerCardNew);
		pPokerCardNew = NULL;
		pPokerCardGet = NULL;
	}

	return *this;
}


void CPokerCardList::clearAllData()
{
	int nFunRes = 0;
	CPokerCard* pPokderCardGet = NULL;
	std::list<CPokerCard*>::iterator  iterList;

	iterList = m_pPokerCardList->begin();
	while (iterList != m_pPokerCardList->end())
	{
		pPokderCardGet = *iterList;
		DEF_DELETE(pPokderCardGet);
		iterList++;
	}//while
	m_pPokerCardList->clear();
}

void CPokerCardList::clear()
{
	m_pPokerCardList->clear();
}


void CPokerCardList::addCard(CPokerCard* card)
{
	m_pPokerCardList->push_back(card);
}
void CPokerCardList::addRange(CPokerCardList* pPokerCardList)
{
	CPokerCard* pPokerCard = NULL;

	for (int nIndex = 0; nIndex < pPokerCardList->getCount(); nIndex++)
	{
		pPokerCard = pPokerCardList->getNodeByIndex(nIndex);
		this->addCard(pPokerCard);
		pPokerCard = NULL;
	}

}


int CPokerCardList::removeCard(CPokerCard* card)
{
	int nFunRes = 0;
	std::list<CPokerCard*>::iterator iterList;

	iterList = m_pPokerCardList->begin();

	iterList = std::find(m_pPokerCardList->begin(), m_pPokerCardList->end(), card);

	if (iterList != m_pPokerCardList->end())
	{
		m_pPokerCardList->erase(iterList);
	}
	else
	{
		nFunRes = -1;
	}
	return nFunRes;

}


int CPokerCardList::getCount() const
{
	int nCount = 0;
	nCount = (int)m_pPokerCardList->size();
	return nCount;
}


//begin with 0
CPokerCard* CPokerCardList::getNodeByIndex(int nIndex) const
{
	int nFunRes = 0;
	CPokerCard* pPokderCardGet = NULL;
	std::list<CPokerCard*>::iterator  iterList;
	int nIndexTmp = 0;

	if ((int)m_pPokerCardList->size() <= 0 || nIndex < 0 || nIndex >= (int)m_pPokerCardList->size())
	{
		nFunRes = -1;
		return pPokderCardGet;
	}
	
	iterList = m_pPokerCardList->begin();
	nIndexTmp = 0;
	while (iterList != m_pPokerCardList->end())
	{
		if (nIndexTmp == nIndex)
		{
			//find node
			nFunRes = 0;
			pPokderCardGet = *iterList;
			break;
		}
		iterList++;
		nIndexTmp++;

	}//while


	return pPokderCardGet;
}

void CPokerCardList::printInfoToLog()
{
	CPokerCard* pPokderCardGet = NULL;
	std::list<CPokerCard*>::iterator  iterList;

	_LOG(SourceFLInfo, DebugDebug, "CPokerCardList [size=%d]", m_pPokerCardList->size());

	iterList = m_pPokerCardList->begin();
	while (iterList != m_pPokerCardList->end())
	{
		pPokderCardGet = *iterList;
		pPokderCardGet->printInfoToLog();
		iterList++;
	}//while
}





//���������㷨 quitsort
void CPokerCardList::sortCards()
{
	std::list<CPokerCard*>::iterator  iterList;
	CPokerCard* pPokderCard = NULL;
	CPokerCard* pPokderCardAterSort = NULL;

	std::list<CPokerCard> lstForSort;
	std::list<CPokerCard>::iterator iterForSort;
		
	if (m_pPokerCardList->size() < 2)
	{
		return;
	}
	//copy m_pPokerCardList  to std::list<CPokerCard>
	iterList = m_pPokerCardList->begin();
	while (iterList != m_pPokerCardList->end())
	{
		CPokerCard PokerCarForSort;
		//
		pPokderCard = *iterList;
		//copy data
		PokerCarForSort = *pPokderCard;
		//save the old data's point
		PokerCarForSort.m_pThis = pPokderCard;
		lstForSort.push_back(PokerCarForSort);
		iterList++;
	}
	//m_pPokerCardList
	m_pPokerCardList->clear();//Deck Manager all CPokerCard

	//sort
	std::stable_sort(lstForSort.begin(), lstForSort.end(), std::less<CPokerCard>());

	//copy   std::list<CPokerCard> to m_pPokerCardList
	iterForSort = lstForSort.begin();
	while (iterForSort != lstForSort.end())
	{
		//get the old data's point
		pPokderCardAterSort	= (*iterForSort).m_pThis;
		//
		m_pPokerCardList->push_back(pPokderCardAterSort);
		iterForSort++;
	}
	//free all copy data
	lstForSort.clear();//free temp CPokerCard

}

NS_END(TA_TwoToOnePokerGameNet)





