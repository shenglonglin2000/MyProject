#ifndef  __DEF_CLASS_CARDS_HELPER__H__
#define  __DEF_CLASS_CARDS_HELPER__H__

#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)

class CPokerCard;
class CPokerCardList;
class CPokerCardListList;


class CCardsHelper
{
public:
	CCardsHelper(void);
	~CCardsHelper(void);
public:

	/// <summary>
	/// �Ƚ����ƴ�С
	/// </summary>
	/// <param name="card1"></param>
	/// <param name="card2"></param>
	/// <returns></returns>
	int compareCardsByValue(CPokerCard* card1, CPokerCard* card2);

	/// <summary>
	/// �Ƚ�����ֵ�ô�С
	/// </summary>
	/// <param name="val1"></param>
	/// <param name="val2"></param>
	/// <returns>Val1�󣬷���1,Val2�󣬷���-1��һ���󣬷���0</returns>
	int comparePokerValue(int val1, int val2);


	/// <summary>
	/// ��ȡһϵ���������е�n����,����ָ��,��һ���б��� ������
	/// </summary>
	/// <param name="playingCards"></param>
	/// <returns></returns>
	CPokerCardList* getCardsOfSameKind(CPokerCardList* cards, int count);


	/// <summary>
	/// ���һϵ������ĳ�����͵���
	/// </summary>
	/// <param name="cards"></param>
	/// <param name="style"></param>
	/// <returns></returns>

	CPokerCardListList* getStyleCards(CPokerCardList* cards, CardsStyle cardsStyle);

	/// <summary>
	/// ��ȡ���п��Գ��ĵ�����
	/// </summary>
	/// <param name="cards"></param>
	/// <returns></returns>
	CPokerCardListList*  getSingleCards(CPokerCardList* cards);

	/// <summary>
	/// ��ȡ���п��Գ���������
	/// </summary>
	/// <param name="cards"></param>
	/// <returns></returns>
	CPokerCardListList* getDoubleCards(CPokerCardList* cards);



};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_CARDS_HELPER__H__
