/************************************************************************/
/* AIʵ��                                                                     */
/************************************************************************/

#ifndef __DEF_CLASS_AI_COMPUTER_H__
#define __DEF_CLASS_AI_COMPUTER_H__


#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)

class CGamePlayer;
class CPokerCard;
class CPokerCardList;
class CPokerCardListList;
class CCommonLogic;
class CCardsHelper;

class CAIComputer
{
public:
	CAIComputer();
	~CAIComputer();
public:
	CGamePlayer* getPlayer() const; 
	void setPlayer(CGamePlayer* pGamePalyerRef);

	/// <summary>
	/// �����ϴγ����ƣ���ȡ���ֵĳ���
	/// </summary>
	/// <param name="handCards"></param>
	/// <param name="lastPlayedCards"></param>
	/// <returns></returns>
	CPokerCardList* chooseCardsToPlay(CPokerCardList* lastPlayedCards);


private:
	CPokerCardListList* _GetCardsForChoice(CPokerCardList* handCards, CPokerCardList* lastPlayedCards);



	//�ҳ�ָ�������ĵ�˳
	CPokerCardListList* _SearchSimpleConnectors(CPokerCardList*  handCards, int totalCount);

	//�ҳ�ָ��������˫˳
	CPokerCardListList* _SearchDoubleConnectors(CPokerCardList*  handCards, int totalCount);


	//�ҳ�ָ����������˳
	CPokerCardListList* _SearchTribleConnectors(CPokerCardList*  handCards, int totalCount);

	//������������
	CPokerCardListList* _SearchThreeAndTwo(CPokerCardList*  handCards);

	//������һ����
	CPokerCardListList* _SearchThreeAndOne(CPokerCardList*  handCards);

	/// <summary>
	/// �ҳ����еĻ��
	/// </summary>
	/// <param name="handCards"></param>
	/// <returns></returns>
	CPokerCardListList* _SearchRocket(CPokerCardList*  handCards);

	/// <summary>
	/// �ҳ����е�ը��
	/// </summary>
	/// <param name="handCards"></param>
	/// <returns></returns>
	CPokerCardListList* _SearchBomb(CPokerCardList*  handCards);

	/// <summary>
	/// �ҳ����е�����
	/// </summary>
	/// <param name="cards"></param>
	/// <returns></returns>
	CPokerCardListList* _SearchTrible(CPokerCardList*  handCards);

	/// <summary>
	/// �ҳ����е�����
	/// </summary>
	/// <param name="cards"></param>
	/// <returns></returns>
	CPokerCardListList* _SearchDouble(CPokerCardList*  handCards);

	/// <summary>
	/// �ҳ������ܳ��ĵ��ŵ���
	/// </summary>
	/// <param name="cards"></param>
	/// <returns></returns>
	CPokerCardListList* _SearchSingle(CPokerCardList*  handCards);

	/// <summary>
	/// �ҳ����е���ͬ��������,�����
	/// </summary>
	/// <returns></returns>
	CPokerCardListList* _SearchCardsOfSameKind(CPokerCardList*  cards, int count);

private:
	/// <summary>
	/// AI�����õ�Player
	/// </summary>
	CGamePlayer* _player;
	CCardsHelper* CardsHelper;
	CCommonLogic* CommonLogic;
};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_AI_COMPUTER_H__
