#ifndef  __DEF_CLASS_AI_ELECT_LORD__H__
#define  __DEF_CLASS_AI_ELECT_LORD__H__

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

class CAIElectLord
{
public:
	//��ϴ�õ�һ���� 54  17 17 17 3
	//���ĸ��˿�ʼ���� West = 0,East = 2,South = 1,North = 3
	CAIElectLord(CPokerCardList* pTestCards, PlayerPosition nTestFirstGet, PlayerPosition nTestLord);
	~CAIElectLord(void);

private:
	//ȫ�ֱ���
	//��ǰ����Ϸ״̬
	GameStatus m_nCurrentAIElectLordStatus;

	//all CGamePlayer data managered by Players
	CGamePlayerList* m_pListThreePlayers;//data

	//��Ҫ������һ���� 54  17 17 17 3
	CPokerCardList* m_pTestCards;//Tmp

	//��ϴ�õ�һ���� 54  17 17 17 3
	//all CPokerCard data managered by Deck
	CPokerCardList* m_pListShuffledCards;//data
	

	//���ĸ��˿�ʼ���� West = 0,East = 2,South = 1,North = 3
	PlayerPosition m_nTestFirstGet;
	//���λ�õ����Ϊ���������ܲ���Ӯ  m_nWinnerOfTest == nTestLord
	PlayerPosition m_nTestLord;
	
	bool          m_bTestResult;

	//����˵�����Ӯ�ĸ������ West = 0,East = 2,South = 1,North = 3
	PlayerPosition m_nWinnerOfTest;

	
	
	//��ǰ������
	CPokerCardList* m_pListLordCards;//TMP

	//��ǰ�������
	CGamePlayer* m_pCurrentPlayer;//TMP
	//��һ�γ�����
	CPokerCardList* m_pListLastPlayedCards;//TMP
	//Pass�Ĵ���
	int PassTimes;//TMP

private:
	CCommonLogic* m_pCommonLogic;
	CCardsHelper* m_pCardsHelper;


public:
	void startTest();
	bool getTestResult();

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
	void _SortCardsForAll();

	/// <summary>
	/// ��ʼѡ��һ������
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _ElectLord(PlayerPosition nTestLord);

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
	void _RequestCards();


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
	/// ����
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void _DealCards(PlayerPosition nFirstGet);


	/// <summary>
	/// ��ʼ��һ���Ƶķ���
	/// </summary>
	/// <returns></returns>
	void _InitializeDeck(CPokerCardList** ppListDeck, CPokerCardList* pListDeckPokerCards);

	void _UnInitializeDeck(CPokerCardList** ppListDeck);

	//PlayerPosition nFirstPlayer ���Ƶ�ʱ����õ���һ����
	void _InitAllPlayers(CGamePlayerList** ppListThreePlayers, PlayerPosition nFirstPlayer);//south=0  East Weast

	void _UnInitAllPlayers(CGamePlayerList** ppListThreePlayers);
	
	void _InitializePokerCardList(CPokerCardList** ppPokerCardList, CPokerCardList* pPokerCardList);

	void _UnInitializePokerCardList(CPokerCardList** ppPokerCardList);



};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_AI_ELECT_LORD__H__
