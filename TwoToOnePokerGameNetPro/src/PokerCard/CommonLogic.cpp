#include "CommonLogic.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"
#include "PokerCard.h"
#include "PokerCardList.h"
#include "PokerCardListList.h"
#include "CardsHelper.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)



CCommonLogic::CCommonLogic(void)
{
	m_pCardsHelper = NULL;
	m_pCardsHelper = new CCardsHelper();

}


CCommonLogic::~CCommonLogic(void)
{
	DEF_DELETE(m_pCardsHelper);
}

/// <summary>
/// ��ȡ����
/// </summary>
/// <param name="playingCards"></param>
/// <returns></returns>
CardsStyle CCommonLogic::getCardsStyle(CPokerCardList* cards)
{
	PokerValue currValue;
	int nIndex = 1;
	int currCount = 1;
	CPokerCard* pPokerCard = NULL;
	CPokerCard* pPokerCardTmp2 = NULL;

	int nIntCardsNumCountOne = 0;
	int nIntCardsNumCountTwo = 0;
	int nIntCardsNumCountThree = 0;
	int nIntCardsNumCountFour = 0;
	int nIntCardsNumCountFive = 0;


	//δ���� ������ΪPASS
	if (cards == NULL || cards->getCount() == 0)
	{
		return Pass;
	}

	//count[i]��ʾi���Ƶĸ���
	int* nIntArrayCardsNumCount = new int[5];
	memset(nIntArrayCardsNumCount, 0, 5 * sizeof(int));


	//�Ƚ�������
	//TODO.

	//ɨ�������ƣ��ҳ����ţ����ţ����ţ����ŵĸ���
	nIndex = 0;
	currValue = cards->getNodeByIndex(nIndex)->getValue(); //��ǰɨ�赽���Ƶ�ֵ
	currCount = 1; //��ǰɨ����Ƶ�����
	//begin form 1
	for (nIndex = 1; nIndex < cards->getCount(); nIndex++)
	{
		pPokerCard = cards->getNodeByIndex(nIndex);
		if (pPokerCard->getValue() == currValue)
		{
			//ɨ�赽���ƺ��Ѿ�ɨ�����ֵ��ͬ������+1
			currCount++;
		}
		else
		{
			//ɨ�赽�µ�ֵ����¼������1
			nIntArrayCardsNumCount[currCount]++;
			currCount = 1;
			currValue = pPokerCard->getValue();
		}
	}
	nIntArrayCardsNumCount[currCount]++;


	//copy value
	nIntCardsNumCountOne = nIntArrayCardsNumCount[1];
	nIntCardsNumCountTwo = nIntArrayCardsNumCount[2];
	nIntCardsNumCountThree = nIntArrayCardsNumCount[3];
	nIntCardsNumCountFour = nIntArrayCardsNumCount[4];
	nIntCardsNumCountFive = nIntArrayCardsNumCount[5];

	delete[] nIntArrayCardsNumCount;
	nIntArrayCardsNumCount = NULL;


	if (nIntCardsNumCountOne == 1 && nIntCardsNumCountTwo == 0 && nIntCardsNumCountThree == 0 && nIntCardsNumCountFour == 0)
	{
		//ֻ�е��ŵ���1����������Ϊ����
		return Single;
	}

	if (nIntCardsNumCountOne == 0 && nIntCardsNumCountTwo == 1 && nIntCardsNumCountThree == 0 && nIntCardsNumCountFour == 0)
	{
		//ֻ�����ŵ���һ��,����ΪDouble
		return  Double;
	}


	if (nIntCardsNumCountOne == 0 && nIntCardsNumCountTwo == 0 && nIntCardsNumCountThree == 1 && nIntCardsNumCountFour == 0)
	{
		//ֻ��������һ��������ΪTrible
		return Trible;
	}

	if (nIntCardsNumCountOne == 0 && nIntCardsNumCountTwo == 0 && nIntCardsNumCountThree == 0 && nIntCardsNumCountFour == 1)
	{
		//ֻ��������һ��������ΪBomb
		return Bomb;
	}

	if (nIntCardsNumCountOne == 2 && nIntCardsNumCountTwo == 0 && nIntCardsNumCountThree == 0 && nIntCardsNumCountFour == 0)
	{
		//���Ų�ͬ ��Ϊ��С��������Ϊ���
		nIndex = 0;
		pPokerCard = cards->getNodeByIndex(nIndex);
		nIndex = 1;
		pPokerCardTmp2 = cards->getNodeByIndex(nIndex);
		if (pPokerCard->getSuit() == Other && pPokerCardTmp2->getSuit() == Other) //Ϊ��������
		{
			return Rocket;
		}
	}

	if (nIntCardsNumCountOne == 1 && nIntCardsNumCountTwo == 0 && nIntCardsNumCountThree == 1  && nIntCardsNumCountFour == 0)
	{
		//����Ϊ����һ
		return ThreeAndOne;
	}

	if (nIntCardsNumCountOne == 0 && nIntCardsNumCountTwo == 1 && nIntCardsNumCountThree == 1 && nIntCardsNumCountFour == 0)
	{
		//����Ϊ������
		return ThreeAndTwo;
	}


	if (nIntCardsNumCountOne >= 5 && nIntCardsNumCountTwo == 0 && nIntCardsNumCountThree == 0 && nIntCardsNumCountFour == 0)
	{
		bool flag = true;
		//����Ϊ��˳ 5��������������
		for (nIndex = 0; nIndex < cards->getCount() - 1; nIndex++)
		{
			pPokerCard = cards->getNodeByIndex(nIndex);
			pPokerCardTmp2 = cards->getNodeByIndex(nIndex + 1);

			if (pPokerCard->getValue() == Two || pPokerCard->getValue() >= King || pPokerCard->getValue() + 1 != pPokerCardTmp2->getValue())
			{
				flag = false;
				break;
			}
		}

		if (flag) 
		{
			return SimpleConnectors;
		}
	}



	if (nIntCardsNumCountOne == 0 && nIntCardsNumCountTwo >= 3 && nIntCardsNumCountThree == 0 && nIntCardsNumCountFour == 0)
	{
		bool flag = true;
		//����Ϊ˫˳ 3��������������
		for (nIndex = 1; nIndex < cards->getCount() - 1; nIndex=nIndex + 2)
		{
			pPokerCard = cards->getNodeByIndex(nIndex);
			pPokerCardTmp2 = cards->getNodeByIndex(nIndex + 1);

			if (pPokerCard->getValue() == Two || pPokerCard->getValue() >= King || pPokerCard->getValue() + 1 != pPokerCardTmp2->getValue())
			{
				flag = false;
				break;
			}
		}

		if (flag) 
		{
			return DoubleConnectors;
		}

	}


	if (nIntCardsNumCountOne == 0 && nIntCardsNumCountTwo == 0 && nIntCardsNumCountThree >= 2 && nIntCardsNumCountFour == 0)
	{
		bool flag = true;
		//����Ϊ��˳ 2��������������
		for (nIndex = 2; nIndex <= cards->getCount() - 1; nIndex=nIndex + 3)
		{
			pPokerCard = cards->getNodeByIndex(nIndex);
			pPokerCardTmp2 = cards->getNodeByIndex(nIndex + 1);

			if (pPokerCard->getValue() == Two || pPokerCard->getValue() >= King || pPokerCard->getValue() + 1 != pPokerCardTmp2->getValue())
			{
				flag = false;
				break;
			}
		}

		if (flag) 
		{
			return TribleConnectors;
		}
	}

	if ((nIntCardsNumCountOne == 0 && nIntCardsNumCountTwo == 2 && nIntCardsNumCountThree == 0 && nIntCardsNumCountFour == 1)
		|| (nIntCardsNumCountOne == 2 && nIntCardsNumCountTwo == 0 && nIntArrayCardsNumCount[3] == 0 && nIntCardsNumCountFour == 1))
	{
		//����Ϊ�Ĵ���
		return FourAndTwo;
	}



	//����Ϊ�ɻ������(3+3+1+1 3+3+2+2 3+3+3+1+1+1 3+3+3+2+2+2 )
	if ((nIntCardsNumCountThree == nIntCardsNumCountOne && nIntCardsNumCountTwo == 0 && nIntCardsNumCountFour == 0)
		|| (nIntCardsNumCountThree == nIntCardsNumCountTwo && nIntCardsNumCountOne == 0 && nIntCardsNumCountFour == 0))
	{
		//ȡ�������������ŵ��Ʒŵ��µ��б���
		CPokerCardList* pListTmp = m_pCardsHelper->getCardsOfSameKind(cards, 3);
		
		//������ŵ�����ɵ�����������˳�����Ƿɻ������
		if (getCardsStyle(pListTmp) == TribleConnectors)
		{
			pListTmp->clear();
			delete pListTmp;
			pListTmp = NULL;

			return Plane;
		}
		pListTmp->clear();
		delete pListTmp;
		pListTmp = NULL;
	}

	return Error;
}


/// <summary>
/// ����Ƶ�ֵ�����ԱȽϴ�С
/// </summary>
/// <param name="playingCards"></param>
/// <returns></returns>
int CCommonLogic::getCardsRealValue(CPokerCardList* playingCards)
{
	//�Ƚ�������
	//TODO.
	//playingCards.Sort(CardsHelper.CompareCardsByValue);

	//�Ȼ�ȡ����
	CardsStyle cardStyle = getCardsStyle(playingCards);

	//��þ����ƴ�С�ı�־��
	CPokerCard* signCard = NULL;
	CPokerCardList* pListTmp = NULL;

	switch (cardStyle)
	{
	case Single:
		//����ȡ��һ����Ϊ��־
		signCard = playingCards->getNodeByIndex(0);
		break;

	case Double:
		//����ȡ��һ����Ϊ��־
		signCard = playingCards->getNodeByIndex(0);
		break;

	case Trible:
		//����ȡ��һ����Ϊ��־
		signCard = playingCards->getNodeByIndex(0);
		break;

	case Bomb:
		//ը��ȡ��һ����Ϊ��־
		signCard = playingCards->getNodeByIndex(0);
		break;

	case Rocket:
		//����õڶ�����Ϊ��־��������
		signCard = playingCards->getNodeByIndex(1);
		break;

	case ThreeAndOne:
		//����һ�������Ƶĵ�һ�ŵ���Ϊ��־
		pListTmp = m_pCardsHelper->getCardsOfSameKind(playingCards, 3);
		signCard = pListTmp->getNodeByIndex(0);
		pListTmp->clear();
		delete pListTmp;
		pListTmp = NULL;
		break;
		
	case ThreeAndTwo:
		//�������������Ƶĵ�һ�ŵ���Ϊ��־
		pListTmp = m_pCardsHelper->getCardsOfSameKind(playingCards, 3);		
		signCard = pListTmp->getNodeByIndex(0);
		pListTmp->clear();
		delete pListTmp;
		pListTmp = NULL;
		break;

	case SimpleConnectors:
		//��˳��������һ������Ϊ��־
		signCard = playingCards->getNodeByIndex(playingCards->getCount() - 1);
		break;

	case DoubleConnectors:
		//˫˳��������һ������Ϊ��־
		signCard = playingCards->getNodeByIndex(playingCards->getCount() - 1);
		break;

	case TribleConnectors:
		//��˳��������һ������Ϊ��־
		signCard = playingCards->getNodeByIndex(playingCards->getCount() - 1);
		break;

	case FourAndTwo:
		//�Ĵ����������ŵĵ�һ������Ϊ��־
		pListTmp = m_pCardsHelper->getCardsOfSameKind(playingCards, 4);
		signCard = pListTmp->getNodeByIndex(0);
		pListTmp->clear();
		delete pListTmp;
		pListTmp = NULL;
		break;

	case Plane:
		//�ɻ������ȡ���ŵ�����һ��
		pListTmp = NULL;
		pListTmp = m_pCardsHelper->getCardsOfSameKind(playingCards, 3);
		signCard = pListTmp->getNodeByIndex(pListTmp->getCount() - 1);
		pListTmp->clear();
		pListTmp = NULL;
		break;
	default:
		signCard = NULL;
		break;
	}


	//���ر�־�Ƶ�Ȩֵ
	if (signCard == NULL)
	{
		return 0;
	}
	else
	{
		return (int)signCard->getRealValue();
	}	

	return 0;
}

/// <summary>
/// �жϳ����Ƿ���Ϲ���
/// </summary>
/// <param name="lastPlayedCards">�ϼҳ�����</param>
/// <param name="playingCards">�Լ�������</param>
/// <returns></returns>
bool CCommonLogic::judgeCards(CPokerCardList* lastPlayedCards, CPokerCardList* playingCards)
{
	//�ֱ��õ�ǰ������ƺ����ִ�����Ƶ�����
	CardsStyle lastPlayedCardsStyle = getCardsStyle(lastPlayedCards);
	CardsStyle playingCardsStyle = getCardsStyle(playingCards);


	//����ϴ����ͳ���,��ֱ����֤�ɹ� �ų�AI���ͳ�������
	if (lastPlayedCardsStyle == Error)
	{
		return true;
	}

	//�������ͳ������ֱ����֤ʧ��
	if (playingCardsStyle == Error)
	{
		return false;
	}

	//�ϴγ���Ϊ�� ���һ������
	if (lastPlayedCards == NULL || lastPlayedCards->getCount() == 0)
	{
		//��һ�β�����PASS,ֻҪ���Ͳ�ΪPASS,�����κ����Ͷ�ͨ��
		return playingCardsStyle != Pass;
	}
	else  //���ǵ�һ������
	{
		//���ΪPASS����ͨ��
		if (playingCardsStyle == Pass)
		{
			return true;
		}


		//�����������ǰ�治ͬ�ҳ��Ĳ��ǻ������ը����ֱ��ʧ�� ����ʧ��
		if (!(playingCardsStyle == lastPlayedCardsStyle
			|| playingCardsStyle == Bomb
			|| playingCardsStyle == Rocket))
		{
			return false;
		}


		//������Ͳ�ͬ����������ը�����߻��
		if (playingCardsStyle != lastPlayedCardsStyle)
		{
			//ը������ѹ���
			if (playingCardsStyle == Bomb && lastPlayedCardsStyle == Rocket)
			{
				return false;
			}
			//�����ը��
			return true;
		}
		else
		{
			//������ͬ���ȱȽ�����,������ͬ��ͨ��
			if (playingCards->getCount() != lastPlayedCards->getCount())
			{
				return false;
			}
			//����Ҳ��ͬ����Ƚ��Ƶ�ֵ
			//�ȵõ�һ�ֳ��ƵĴ�С
			int playingCardsValue = getCardsRealValue(playingCards);
			int lastPlayedCardsValue = getCardsRealValue(lastPlayedCards);
			//�����Ʊ�ԭ���ƴ�,��ͨ��
			return (playingCardsValue > lastPlayedCardsValue);

		}//if (playingCardsStyle != lastPlayedCardsStyle)


	}//if (lastPlayedCards == NULL || lastPlayedCards->getCount() == 0)


}







NS_END(TA_TwoToOnePokerGameNet)





