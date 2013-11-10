

#include "AIComputer.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"

#include "PokerCard.h"
#include "PokerCardList.h"
#include "PokerCardListList.h"
#include "GamePlayer.h"
#include "GamePlayerList.h"
#include "CommonLogic.h"
#include "CardsHelper.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)

CAIComputer::CAIComputer()
{
	_player = NULL;
	CardsHelper = NULL;
	CardsHelper = new CCardsHelper();
	CommonLogic = NULL;
	CommonLogic = new CCommonLogic();
}
CAIComputer::~CAIComputer()
{
	_player = NULL;

	delete CardsHelper;
	CardsHelper = NULL;

	delete CommonLogic;
	CommonLogic = NULL;
}

CGamePlayer* CAIComputer::getPlayer() const
{
	return _player;
}
void CAIComputer::setPlayer(CGamePlayer* pGamePalyerRef)
{
	_player = pGamePalyerRef;
}

/// <summary>
/// �����ϴγ����ƣ���ȡ���ֵĳ���
/// </summary>
/// <param name="handCards"></param>
/// <param name="lastPlayedCards"></param>
/// <returns></returns>
CPokerCardList* CAIComputer::chooseCardsToPlay(CPokerCardList* lastPlayedCards)
{
	CPokerCardList* pPokerCardListPlay = NULL;

	//���ϵ���
	CPokerCardList* handCards = _player->getCards();

	//���ҵ��ɹ�ѡ�����������
	CPokerCardListList* pListListChoices = _GetCardsForChoice(handCards, lastPlayedCards);

	if (pListListChoices == NULL || pListListChoices->getCount() == 0)
	{
		if (NULL !=  pListListChoices)
		{
			pListListChoices->clearAllData();
			delete pListListChoices;
			pListListChoices = NULL;

		}
		return pPokerCardListPlay;
	}
	else
	{
		//����ϼҲ��ǵ��������ҳ����ƴ���10���򲻳�
		if (!_player->getLastPlayer()->getIsLord() && NULL != lastPlayedCards && lastPlayedCards->getCount() > 0 
			&& 1 == CardsHelper->comparePokerValue(CommonLogic->getCardsRealValue(lastPlayedCards), 8))//Poker_Ten_IntValue=8
		{

			pListListChoices->clearAllData();
			delete pListListChoices;
			pListListChoices = NULL;

			return pPokerCardListPlay;
		}

		//���ص�һ�����Գ����Ƶ�
		pPokerCardListPlay = pListListChoices->getNodeByIndex(0);
		pListListChoices->removeNode(pPokerCardListPlay);

		pListListChoices->clearAllData();
		delete pListListChoices;
		pListListChoices = NULL;

		return pPokerCardListPlay;
	}	
}

CPokerCardListList* CAIComputer::_GetCardsForChoice(CPokerCardList* handCards, CPokerCardList* lastPlayedCards)
{
	CPokerCardListList* pListListChoice=new CPokerCardListList();
	CPokerCardList* pListForJudge = NULL;

	CardsStyle cardStyle = CommonLogic->getCardsStyle(lastPlayedCards);
	//�ҳ�������ȷ����
	CPokerCardListList* pListListSearchRes = new CPokerCardListList();
	CPokerCardListList* pListListSearchTmp = NULL;

	switch (cardStyle)
	{
		//���ţ��ҳ����еĿɳ��ĵ�����
	case Single:
		pListListSearchTmp = _SearchSingle(handCards);
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;
		break;

		//���ţ��ҳ����пɳ���������
	case Double:
		pListListSearchTmp = _SearchDouble(handCards);
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;

		break;

		//���ţ��ҳ����пɳ���������
	case Trible:
		pListListSearchTmp = _SearchTrible(handCards);
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;
		break;

		//�ҳ����п��Գ�������һ
	case ThreeAndOne:
		pListListSearchTmp = _SearchThreeAndOne(handCards);
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;
		break;

		//�ҳ����п��Գ���������
	case ThreeAndTwo:
		pListListSearchTmp = _SearchThreeAndTwo(handCards);
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;
		break;

		//�ҳ����п��Գ��ĵ�˳
	case SimpleConnectors:
		pListListSearchTmp = _SearchSimpleConnectors(handCards, lastPlayedCards->getCount());
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;
		break;

		//�ҳ����п��Գ���˫˳
	case DoubleConnectors:
		pListListSearchTmp = _SearchDoubleConnectors(handCards, lastPlayedCards->getCount());
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;
		break;


		//�ҳ����п��Գ�����˳
	case TribleConnectors:
		pListListSearchTmp = _SearchTribleConnectors(handCards, lastPlayedCards->getCount());
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;
		break;

		//Pass����������������ȳ�
	case Pass:
		pListListSearchTmp = _SearchThreeAndTwo(handCards);		
		//pListListSearchTmp->printInfoToLog();//log
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;

		//log
		//pListListSearchRes->printInfoToLog();


		pListListSearchTmp = _SearchThreeAndOne(handCards);
		//pListListSearchTmp->printInfoToLog();//log
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;

		//log
		//pListListSearchRes->printInfoToLog();


		pListListSearchTmp = _SearchTrible(handCards);
		//pListListSearchTmp->printInfoToLog();//log
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;

		//log
		//pListListSearchRes->printInfoToLog();


		pListListSearchTmp = _SearchDouble(handCards);
		//pListListSearchTmp->printInfoToLog();//log
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;

		//log
		//pListListSearchRes->printInfoToLog();


		pListListSearchTmp = _SearchSingle(handCards);
		//pListListSearchTmp->printInfoToLog();//log
		pListListSearchRes->addRange(pListListSearchTmp);
		pListListSearchTmp->clear();
		delete pListListSearchTmp;
		pListListSearchTmp = NULL;

		//log
		//pListListSearchRes->printInfoToLog();

		break;
	}//switch


	//�ҳ�ը���ͻ��������еĻ�
	pListListSearchTmp = _SearchBomb(handCards);
	//pListListSearchTmp->printInfoToLog();//log
	pListListSearchRes->addRange(pListListSearchTmp);
	pListListSearchTmp->clear();
	delete pListListSearchTmp;
	pListListSearchTmp = NULL;
	
	//log
	//pListListSearchRes->printInfoToLog();

	pListListSearchTmp = _SearchRocket(handCards);
	//pListListSearchTmp->printInfoToLog();//log
	pListListSearchRes->addRange(pListListSearchTmp);
	pListListSearchTmp->clear();
	delete pListListSearchTmp;
	pListListSearchTmp = NULL;

	//log
	//pListListSearchRes->printInfoToLog();

	//ȡ����ֵ��С��ȷ����
	int nIndex = 0;
	for (nIndex = 0; nIndex < pListListSearchRes->getCount(); nIndex++)
	{
		pListForJudge = pListListSearchRes->getNodeByIndex(nIndex);
		if (true == CommonLogic->judgeCards(lastPlayedCards, pListForJudge))
		{
			pListListChoice->addNode(pListForJudge);
			pListListSearchRes->removeNode(pListForJudge);
			pListForJudge = NULL;
			nIndex = 0;//begin form 0
		}
	}
	pListListSearchRes->clearAllData();
	delete pListListSearchRes;
	pListListSearchRes = NULL;

	return pListListChoice;

}



//�ҳ�ָ�������ĵ�˳
CPokerCardListList* CAIComputer::_SearchSimpleConnectors(CPokerCardList*  handCards, int totalCount)
{
	//���صı���
	CPokerCardListList* pListListSearchRes = new CPokerCardListList();

	CPokerCardListList* pListSearchResSingle = _SearchSingle(handCards);

	if (pListSearchResSingle->getCount() * 1 < totalCount) 
	{
		return pListListSearchRes;
	}

	//log
	//pListSearchResSingle->printInfoToLog();

	//������˳��������������
	for (int index = 0; index < pListSearchResSingle->getCount(); index++)
	{
		//�ҳ������ƺ���������
		int index1 = index;

		while (index1 < index + totalCount && index1 < pListSearchResSingle->getCount() - 1)
		{
			CPokerCardList* pListSingleFirst = NULL;
			CPokerCardList* pListSingleSecond = NULL;

			pListSingleFirst = pListSearchResSingle->getNodeByIndex(index1);
			pListSingleSecond = pListSearchResSingle->getNodeByIndex(index1 + 1);

			if (pListSingleFirst->getCount() > 0 && pListSingleSecond->getCount() > 0)
			{
				if (pListSingleFirst->getNodeByIndex(0)->getRealValue() + 1 != pListSingleSecond->getNodeByIndex(0)->getRealValue())
				{
					//������
					break;
				}
			}

			index1++;

		}//while

		//�ҵ�һ����˳
		if (index1 - index + 1 == totalCount / 1)
		{
			CPokerCardList* pListSingleFind = new CPokerCardList();
			for (int index2 = index1 - totalCount/2 + 1; index2 <= index1; index2++)
			{
				CPokerCardList* pListTmp = NULL;
				pListTmp = pListSearchResSingle->getNodeByIndex(index2);
				pListSingleFind->addRange(pListTmp);
			}
			pListListSearchRes->addNode(pListSingleFind);
			pListSingleFind = NULL;
		}

	}//for (int index = 0;

	pListSearchResSingle->clearAllData();
	delete pListSearchResSingle;
	pListSearchResSingle = NULL;


	return pListListSearchRes;
}


//�ҳ�ָ��������˫˳
CPokerCardListList* CAIComputer::_SearchDoubleConnectors(CPokerCardList*  handCards, int totalCount)
{
	//���صı���
	CPokerCardListList* pListListSearchRes = new CPokerCardListList();

	CPokerCardListList* pListListDouble = _SearchDouble(handCards);

	if (pListListDouble->getCount() * 2 < totalCount) 
	{
		return pListListSearchRes;
	}

	//
	//pListListDouble->printInfoToLog();

	//������˳��������������
	for (int index = 0; index < pListListDouble->getCount(); index++)
	{
		//�ҳ������ƺ���������
		int index1 = index;

		while (index1 < index + totalCount && index1 < pListListDouble->getCount() - 1)
		{
			CPokerCardList* doubleCardsFirst = NULL;
			CPokerCardList* doubleCardsSecond = NULL;

			doubleCardsFirst = pListListDouble->getNodeByIndex(index1);
			doubleCardsSecond = pListListDouble->getNodeByIndex(index1 + 1);

			if (doubleCardsFirst->getCount() > 0 && doubleCardsSecond->getCount() > 0)
			{
				if (doubleCardsFirst->getNodeByIndex(0)->getRealValue() + 1 != doubleCardsSecond->getNodeByIndex(0)->getRealValue())
				{
					//������
					break;
				}
			}
			index1++;
		}//while

		//�ҵ�һ��˫˳
		if (index1 - index + 1 == totalCount / 2)
		{
			CPokerCardList* pListDoubleFind = new CPokerCardList();
			for (int index2 = index1 - totalCount/2 + 1; index2 <= index1; index2++)
			{
				CPokerCardList* pListTmp = NULL;
				pListTmp = pListListDouble->getNodeByIndex(index2);
				pListDoubleFind->addRange(pListTmp);
				pListTmp = NULL;
			}
			pListListSearchRes->addNode(pListDoubleFind);	
			pListDoubleFind = NULL;
		}

	}//for (int index = 0;

	pListListDouble->clearAllData();
	delete pListListDouble;
	pListListDouble = NULL;

	return pListListSearchRes;
}


//�ҳ�ָ����������˳
CPokerCardListList* CAIComputer::_SearchTribleConnectors(CPokerCardList*  handCards, int totalCount)
{
	//���صı���
	CPokerCardListList* pListListSearchRes = new CPokerCardListList();

	CPokerCardListList* pListListTrible = _SearchTrible(handCards);

	if (pListListTrible->getCount() * 3 < totalCount) 
	{
		return pListListSearchRes;
	}

	//������˳��������������
	for (int index = 0; index < pListListTrible->getCount(); index++)
	{
		//�ҳ������ƺ���������
		int index1 = index;

		while (index1 < index + totalCount && index1 < pListListTrible->getCount() - 1)
		{
			CPokerCardList* pListTribleFirst = NULL;
			CPokerCardList* pListTribleSecond = NULL;

			pListTribleFirst = pListListTrible->getNodeByIndex(index1);
			pListTribleSecond = pListListTrible->getNodeByIndex(index1 + 1);

			if (pListTribleFirst->getCount() > 0 && pListTribleSecond->getCount() > 0)
			{
				if (pListTribleFirst->getNodeByIndex(0)->getRealValue() + 1 != pListTribleSecond->getNodeByIndex(0)->getRealValue())
				{
					//������
					break;
				}
			}
						
			index1++;

		}//while

		//�ҵ�һ����˳
		if (index1 - index + 1 == totalCount / 3)
		{
			CPokerCardList* pListFind = new CPokerCardList();
			for (int index2 = index1 - totalCount/3 + 1; index2 <= index1; index2++)
			{
				CPokerCardList* pListTmp = NULL;
				pListTmp = pListListTrible->getNodeByIndex(index2);
				pListFind->addRange(pListTmp);
				pListTmp = NULL;
			}
			pListListSearchRes->addNode(pListFind);
			pListFind = NULL;
		}

	}//for (int index = 0;

	pListListTrible->clearAllData();
	delete pListListTrible;
	pListListTrible = NULL;

	return pListListSearchRes;
}

//������������
CPokerCardListList* CAIComputer::_SearchThreeAndTwo(CPokerCardList*  handCards)
{
	//���صı���
	CPokerCardListList* pListListSearchRes = new CPokerCardListList();
	//������������
	CPokerCardListList* pListListSearchResTrible = _SearchCardsOfSameKind(handCards, 3);

	//���ҳ���������
	CPokerCardListList* pListListSearchResDouble = _SearchCardsOfSameKind(handCards, 2);

	//log
	//pListListSearchResTrible->printInfoToLog();
	//pListListSearchResDouble->printInfoToLog();

	//unknow error here
	int nIndexTrible = 0;
	int nIndexDouble = 0;
	for (nIndexTrible = 0; nIndexTrible < pListListSearchResTrible->getCount(); nIndexTrible++)
	{
		for (nIndexDouble = 0; nIndexDouble < pListListSearchResDouble->getCount(); nIndexDouble++)
		{
			//û���ظ� ����Ե���
			CPokerCardList* pListTrible = pListListSearchResTrible->getNodeByIndex(nIndexTrible);
			CPokerCardList* pListDouble = pListListSearchResDouble->getNodeByIndex(nIndexDouble);

			if (0 < pListTrible->getCount() && 0 < pListDouble->getCount())
			{
				if (pListTrible->getNodeByIndex(0)->getRealValue() != pListDouble->getNodeByIndex(0)->getRealValue())
				{
					CPokerCardList* newCardsList = new CPokerCardList();
					newCardsList->addRange(pListTrible);//3
					newCardsList->addRange(pListDouble);//2
					pListListSearchRes->addNode(newCardsList);
					newCardsList = NULL;
				}  
			}
			 
		}//for (nIndexDouble = 0;

	}//for

	//log
	//pListListSearchRes->printInfoToLog();
	
	//free list but not free data
	pListListSearchResTrible->clearAllData();
	pListListSearchResDouble->clearAllData();

	delete pListListSearchResTrible;
	pListListSearchResTrible = NULL;

	delete pListListSearchResDouble;
	pListListSearchResDouble = NULL;

	return pListListSearchRes;
}

//������һ����
CPokerCardListList* CAIComputer::_SearchThreeAndOne(CPokerCardList*  handCards)
{
	//���صı���
	CPokerCardListList* pListListSearchRes = new CPokerCardListList();

	//������������
	CPokerCardListList* pListListSearchResTrible = _SearchCardsOfSameKind(handCards, 3);

	//���ҳ�����һ��
	CPokerCardListList* pListListSearchResSingle = _SearchCardsOfSameKind(handCards, 1);
	int nIndexTrible = 0;
	int nIndexSingle = 0;
	for (nIndexTrible = 0; nIndexTrible < pListListSearchResTrible->getCount(); nIndexTrible++)
	{
		for (nIndexSingle = 0; nIndexSingle < pListListSearchResSingle->getCount(); nIndexSingle++)
		{
			//û���ظ� ����Ե���
			CPokerCardList* pListTrible = pListListSearchResTrible->getNodeByIndex(nIndexTrible);
			CPokerCardList* pListSingle = pListListSearchResSingle->getNodeByIndex(nIndexSingle);

			if (0 < pListListSearchResTrible->getCount() && 0 < pListListSearchResSingle->getCount())
			{
				if (pListTrible->getNodeByIndex(0)->getRealValue() != pListSingle->getNodeByIndex(0)->getRealValue())
				{
					CPokerCardList* newCardsList = new CPokerCardList();
					newCardsList->addRange(pListTrible);//3
					newCardsList->addRange(pListSingle);//2
					pListListSearchRes->addNode(newCardsList);
					newCardsList = NULL;
				}  
			}
				
		}//for

	}//for

	pListListSearchResTrible->clearAllData();
	pListListSearchResSingle->clearAllData();

	delete pListListSearchResTrible;
	pListListSearchResTrible = NULL;

	delete pListListSearchResSingle;
	pListListSearchResSingle = NULL;

	return pListListSearchRes;
}

/// <summary>
/// �ҳ����еĻ��
/// </summary>
/// <param name="handCards"></param>
/// <returns></returns>
CPokerCardListList* CAIComputer::_SearchRocket(CPokerCardList*  handCards)
{
	int nIndexBigJoker = 0;
	int nIndexSmallJoker = 0;
	CPokerCard* pPokerCardBigJoker = NULL;
	CPokerCard* pPokerCardSmallJoker = NULL;

	//���صı���
	CPokerCardListList* pListListSearchRes = new CPokerCardListList();
	//�һ��
	//����������л��������ض����������
	handCards->sortCards();
	
	if (handCards->getCount() >= 2)
	{
		nIndexSmallJoker = handCards->getCount() - 2;
		nIndexBigJoker = handCards->getCount() - 1;

		pPokerCardSmallJoker = handCards->getNodeByIndex(nIndexSmallJoker);
		pPokerCardBigJoker = handCards->getNodeByIndex(nIndexBigJoker);

		//�����������Ǵ�С�����򷵻�
		if (pPokerCardBigJoker->getValue() == BigJoker 
			&& pPokerCardSmallJoker->getValue() == SmallJoker)
		{
			CPokerCardList* newCards = new CPokerCardList();
			newCards->addCard(pPokerCardSmallJoker);
			newCards->addCard(pPokerCardBigJoker);

			pListListSearchRes->addNode(newCards);
			newCards = NULL;
		}
	}
	return pListListSearchRes;
	
}


/// <summary>
/// �ҳ����е�ը��
/// </summary>
/// <param name="handCards"></param>
/// <returns></returns>
CPokerCardListList* CAIComputer::_SearchBomb(CPokerCardList*  handCards)
{
	return _SearchCardsOfSameKind(handCards, 4);
}


/// <summary>
/// �ҳ����е�����
/// </summary>
/// <param name="cards"></param>
/// <returns></returns>
CPokerCardListList* CAIComputer::_SearchTrible(CPokerCardList*  handCards)
{
	return _SearchCardsOfSameKind(handCards, 3);
}

/// <summary>
/// �ҳ����е�����
/// </summary>
/// <param name="cards"></param>
/// <returns></returns>
CPokerCardListList* CAIComputer::_SearchDouble(CPokerCardList*  handCards)
{
	return _SearchCardsOfSameKind(handCards, 2);
}

/// <summary>
/// �ҳ������ܳ��ĵ��ŵ���
/// </summary>
/// <param name="cards"></param>
/// <returns></returns>
CPokerCardListList* CAIComputer::_SearchSingle(CPokerCardList*  handCards)
{
	return _SearchCardsOfSameKind(handCards, 1);
}

/// <summary>
/// �ҳ����е���ͬ��������,�����
/// </summary>
/// <returns></returns>
CPokerCardListList* CAIComputer::_SearchCardsOfSameKind(CPokerCardList*  cards, int count)
{
	CPokerCardListList* pListListSearchRes = new CPokerCardListList();
	CPokerCard* pPokerCardGet = NULL;
	PokerValue currValue = BigJoker;
	int currCount = 0;

	//�Ƚ�������
	cards->sortCards();

	 for (int index = 0; index < cards->getCount(); index++)
	 {
		 pPokerCardGet = cards->getNodeByIndex(index);

		 //�����ƺ���һ������ͬ
		 if (pPokerCardGet->getValue() == currValue)
		 {
			 //�ۼ���ͬ������
			 currCount++;
			 pPokerCardGet = NULL;
		 }
		 else
		 {
			 //���¿�ʼ����
			 currCount = 1;
			 currValue = pPokerCardGet->getValue();
			 pPokerCardGet = NULL;
		 }


		 //����ҵ���һ�ֿ��Գ����ƣ�������������ͬ����
		 if (currCount == count)
		 {
			 //�ڽ�������n��Ҫ������,��ǰ����ǰn��
			 CPokerCardList* newCards = new CPokerCardList();
			 for (int index1 = index - count + 1; index1 <= index; index1++)
			 {
				 pPokerCardGet = cards->getNodeByIndex(index1);
				 newCards->addCard(pPokerCardGet);
				 pPokerCardGet = NULL;
			 }
			 pListListSearchRes->addNode(newCards);
			 newCards = NULL;

			 //����������ͬ����
			 pPokerCardGet = cards->getNodeByIndex(index + 1);
			 while (index != cards->getCount() - 1 && pPokerCardGet->getValue() == currValue)
			 {
				 index++;

				 if (index != cards->getCount() - 1)
				 {
					  pPokerCardGet = cards->getNodeByIndex(index + 1);
				 }
				
			 }//while (index
		 }// if (currCount == count)
		 
	 }//for (int index = 0

	return pListListSearchRes;
}




NS_END(TA_TwoToOnePokerGameNet)





