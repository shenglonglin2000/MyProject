#ifndef  __DEF_CLASS_COMMON_LOGIC__H__
#define  __DEF_CLASS_COMMON_LOGIC__H__

#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)

class CPokerCardList;
class CCardsHelper;

class CCommonLogic
{
public:
	CCommonLogic(void);
	~CCommonLogic(void);

private:
	CCardsHelper* m_pCardsHelper;
public:

	/// <summary>
	/// ��ȡ����
	/// </summary>
	/// <param name="playingCards"></param>
	/// <returns></returns>
	CardsStyle getCardsStyle(CPokerCardList* cards);


	/// <summary>
	/// ����Ƶ�ֵ�����ԱȽϴ�С
	/// </summary>
	/// <param name="playingCards"></param>
	/// <returns></returns>
	int getCardsRealValue(CPokerCardList* playingCards);

	/// <summary>
	/// �жϳ����Ƿ���Ϲ���
	/// </summary>
	/// <param name="lastPlayedCards">�ϼҳ�����</param>
	/// <param name="playingCards">�Լ�������</param>
	/// <returns></returns>
	bool judgeCards(CPokerCardList* lastPlayedCards, CPokerCardList* playingCards);



};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_COMMON_LOGIC__H__
