#include "CardsHelper.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"
#include "PokerCard.h"
#include "PokerCardList.h"
#include "PokerCardListList.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

CCardsHelper::CCardsHelper(void)
{
}

CCardsHelper::~CCardsHelper(void)
{
}




/// <summary>
/// �Ƚ����ƴ�С
/// </summary>
/// <param name="card1"></param>
/// <param name="card2"></param>
/// <returns></returns>
int CCardsHelper::compareCardsByValue(CPokerCard* card1, CPokerCard* card2)
{
	int val1 = (int)card1->getRealValue();//
	int val2 = (int)card2->getRealValue();//

	return comparePokerValue(val1, val2);
}

/// <summary>
/// �Ƚ�����ֵ�ô�С
/// </summary>
/// <param name="val1"></param>
/// <param name="val2"></param>
/// <returns>Val1�󣬷���1,Val2�󣬷���-1��һ���󣬷���0</returns>
int CCardsHelper::comparePokerValue(int val1, int val2)
{
	if (val1 > val2)
	{
		return 1;
	}
	else if (val1 == val2)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


/// <summary>
/// ��ȡһϵ���������е�n����,����ָ��,��һ���б��� ������
/// </summary>
/// cards Ϊ�Ѿ�����ֵ����С�����ź��������
/// count=1, 2, 3, 4,
/// <param name="playingCards"></param>
/// <returns></returns>
CPokerCardList* CCardsHelper::getCardsOfSameKind(CPokerCardList* cards, int count)
{	
	CPokerCardList* pListReturn = NULL;	//new

	CPokerCard* pCurrPokerCard = NULL;
	CPokerCard* pPokerCardTmp = NULL;
	CPokerCard* pPokerCardTmpTwo = NULL;

	PokerValue currValue;
	int currCount = 0;
	int nIndex = 1;
	
	//�������һ���ƻ����������ֱ�ӷ���
	if (count < 1 || count > 4 || cards == NULL || cards->getCount() < 1)
	{
		return pListReturn;
	}
	pListReturn = new CPokerCardList();

	//frist node
	nIndex = 0;
	pCurrPokerCard = cards->getNodeByIndex(nIndex);

	//��ǰɨ�赽���Ƶ�ֵ
	currValue = pCurrPokerCard->getValue();
	//��ǰɨ����Ƶ�����
	currCount = 1;

	//�ӵ�һ�ſ�ʼɨ��
	//�ӵڶ�����㿪ʼ��
	for (nIndex = 1; nIndex < cards->getCount(); nIndex++)
	{
		
		pPokerCardTmp = cards->getNodeByIndex(nIndex);

		if (pPokerCardTmp->getValue() == currValue)
		{
			//ɨ�赽���ƺ��Ѿ�ɨ�����ֵ��ͬ������+1
			currCount++;

			//�������һ������
			if (currCount == count && nIndex == cards->getCount() - 1)
			{
				//��count���Ƽ��뷵���б�
				for (int nindexTwo = 0; nindexTwo < count; nindexTwo++)
				{
					pListReturn->addCard(pPokerCardTmp);
				}
			}
		}
		else
		{
			//ɨ�赽�µ�ֵ����¼������1
			if (currCount == count)
			{
				pPokerCardTmpTwo = cards->getNodeByIndex(nIndex - 1);
				//��count���Ƽ��뷵���б�
				for (int nindexTwo = 0; nindexTwo < count; nindexTwo++)
				{
					pListReturn->addCard(pPokerCardTmpTwo);
				}
			}
			currCount = 1;
			currValue = pPokerCardTmp->getValue();
		}

	}//for

	return pListReturn;
}


/// <summary>
/// ���һϵ������ĳ�����͵���
/// </summary>
/// <param name="cards"></param>
/// <param name="style"></param>
/// <returns></returns>

CPokerCardListList* CCardsHelper::getStyleCards(CPokerCardList* cards, CardsStyle cardsStyle)
{
	switch (cardsStyle)
	{
	case Single:
		return getSingleCards(cards);
	default: 
		return NULL;
	}
}

/// <summary>
/// ��ȡ���п��Գ��ĵ�����
/// </summary>
/// <param name="cards"></param>
/// <returns></returns>
CPokerCardListList*  CCardsHelper::getSingleCards(CPokerCardList* cards)
{
	CPokerCardListList* pPokerCardListList = NULL;
	CPokerCardList* pPokerCardList = NULL;
	CPokerCard* pPokerCard = NULL;
	int nIndex = 0;

	//�������һ���ƻ����������ֱ�ӷ���
	if (cards == NULL || cards->getCount() < 1)
	{
		return pPokerCardListList;
	}

	pPokerCardListList = new CPokerCardListList();

	for (nIndex = 0; nIndex < cards->getCount(); nIndex++)
	{
		pPokerCard = cards->getNodeByIndex(nIndex);

		pPokerCardList = new CPokerCardList();
		pPokerCardList->addCard(pPokerCard);
		pPokerCard = NULL;
		pPokerCardListList->addNode(pPokerCardList);
		pPokerCardList = NULL;		

	}
	return pPokerCardListList;
}

/// <summary>
/// ��ȡ���п��Գ���������
/// </summary>
/// <param name="cards"></param>
/// <returns></returns>
CPokerCardListList* CCardsHelper::getDoubleCards(CPokerCardList* cards)
{
	CPokerCardListList* pPokerCardListList = NULL;
	CPokerCardList* pPokerCardList = NULL;
	CPokerCard* pPokerCard = NULL;
	int nIndex = 0;

	//�������һ���ƻ����������ֱ�ӷ���
	if (cards == NULL || cards->getCount() < 1)
	{
		return pPokerCardListList;
	}

	pPokerCardListList = new CPokerCardListList();

	for (nIndex = 0; nIndex < cards->getCount(); nIndex++)
	{
		pPokerCard = cards->getNodeByIndex(nIndex);

		pPokerCardList = new CPokerCardList();
		pPokerCardList->addCard(pPokerCard);
		pPokerCard = NULL;
		pPokerCardListList->addNode(pPokerCardList);
		pPokerCardList = NULL;		

	}
	return pPokerCardListList;
}



NS_END(TA_TwoToOnePokerGameNet)





