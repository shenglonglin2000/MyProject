#ifndef  __DEF_CLASS_GAME_MAINFORM__H__
#define  __DEF_CLASS_GAME_MAINFORM__H__

#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)

class CPokerCard;
class CPokerCardList;
class CPokerCardListList;
class CGamePlayer;
class CGamePlayerList;
class CCommonLogic;
class CCardsHelper;

class CGameMainForm
{
public:
	CGameMainForm(void);
	~CGameMainForm(void);

private:
	//ȫ�ֱ���
	//��ǰ����Ϸ״̬
	GameStatus CurrentGameStatus;

	//all CGamePlayer data managered by Players
	CGamePlayerList* m_pListThreePlayers;//data

	//��ǰ��һ����
	//all CPokerCard data managered by Deck
	CPokerCardList* m_pListDeck;//data
	
	//��ǰ������
	CPokerCardList* m_pListLordCards;//TMP

	//��ǰ�������
	CGamePlayer* m_pCurrentPlayer;//TMP
	//��һ�γ�����
	CPokerCardList* m_pListLastPlayedCards;//TMP
	//Pass�Ĵ���
	int PassTimes;//TMP

	//���ĸ��˿�ʼ���� West = 0,East = 2,South = 1,North = 3
	PlayerPosition m_nPositionGetFirstCard;//TMP

private:
	CCommonLogic* m_pCommonLogic;
	CCardsHelper* m_pCardsHelper;


public:
	void btn_PlayCards_Click();
	void startNewGame();

private:
	// #region GameManager
	/// <summary>
	/// ��Ϸ״̬����
	/// </summary>
	void _GameStatusProcess();

	/// <summary>
	/// ��������ҵ�������
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _SortCardsForAll(CGamePlayerList* pListPlayers);

	/// <summary>
	/// ��ʼѡ��һ������
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _ElectLord(CPokerCardList* pListDeck, CGamePlayerList* pListPlayers, PlayerPosition nPositionGetFirstCard);

	/// <summary>
	/// ���Ʒ�������
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _DealCardsToLord();

	/// <summary>
	/// �ҵ������ı��
	/// </summary>
	/// <returns></returns>
	int _FindLordIndex();

	/// <summary>
	/// �ҵ���ǰ�����Players�еı��0 1 2
	/// </summary>
	/// <returns></returns>
	int _FindCurrentPlayerIndex(CGamePlayer* pCurrentPlayer);

	/// <summary>
	/// ����ǰ��ҵ�������
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _SortCardsForCurrentPlayer();

	/// <summary>
	/// ��ϲӮ��
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _CongratulationWinner();


	/// <summary>
	/// ��Ϸ�����ķ���
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _GameOver();

private:
	//#region ��Ϸ�¼��������

	/// <summary>
	/// ����ҷ�������ָ��
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	CPokerCardList* _RequestCards(CGamePlayer* pCurrentPlayer);

	/// <summary>
	/// ��������ҿ��Գ���
	/// </summary>
	void _SetHumanPlayingCards(CGamePlayer* pCurrentPlayer);

	/// <summary>
	/// ���Ʒ���,ִ�и÷�������ɳ��ƣ����г��ƶ�����������һ״̬
	/// </summary>
	/// <param name="playingCards">Ҫ�����ƣ���Ҫ��֤��ȷ��</param>
	void _PlayCards(CPokerCardList* playingCards);

	/// <summary>
	/// һ�ֳ��ƽ���֮��ķ���
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _ClearOneRound();

	/// <summary>
	/// ����һ����ҳ���
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _NextPlayerPlayCards();

	/// <summary>
	/// �����һ�����Ƶ����
	/// </summary>
	/// <returns></returns>
	CGamePlayer* _GetNextPlayer();


private:
	//#region ��Ϸ�߼��������

	//������Ϸ���������������ϵ��ƣ�3������뿪��λ��
	void _CleanEnv();


	/// <summary>
	/// ϴ��
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _ShuffleCards(CPokerCardList** ppPokerCardList);

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _DealCards(PlayerPosition nFirstGet);

	/// <summary>
	/// ��ʼ��һ������ɫ��ֵ������е��Ƶķ���
	/// PokerSuit PokerValue
	/// </summary>
	/// <returns></returns>
	void _InitializePokerSuitValueDeck(CPokerCardList** ppPokerCardList);

	/// <summary>
	/// ��ʼ��һ���Ƶķ���
	/// </summary>
	/// <returns></returns>
	void _InitializeDeck(CPokerCardList** ppListDeck);

	void _UnInitializeDeck(CPokerCardList** ppListDeck);

	void _InitAllPlayers(CGamePlayerList** ppListThreePlayers, PlayerPosition nPositionGetFirstCard);

	void _UnInitAllPlayers(CGamePlayerList** ppListThreePlayers);
	
	void _InitializePokerCardList(CPokerCardList** ppPokerCardList, CPokerCardList* pPokerCardList);

	void _UnInitializePokerCardList(CPokerCardList** ppListLastPlayedCards);



};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_GAME_MAINFORM__H__
