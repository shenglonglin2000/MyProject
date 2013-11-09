
#include "AIElectLord.h"

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
#include "AIComputer.h"
#include "DebugUtil.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

	//��ϴ�õ�һ���� 54  17 17 17 3
	//���ĸ��˿�ʼ���� West = 0,East = 2,South = 1,North = 3
CAIElectLord::CAIElectLord(CPokerCardList* pTestCards, PlayerPosition nTestFirstGet, PlayerPosition nTestLord)
{	
	m_pCommonLogic = NULL;
	m_pCommonLogic = new CCommonLogic();

	m_pCardsHelper = NULL;
	m_pCardsHelper = new CCardsHelper();
	
	m_nCurrentAIElectLordStatus = NewGame;
	m_pTestCards = pTestCards;
	m_nTestFirstGet = nTestFirstGet;//South
	m_nTestLord = nTestLord;//East
	m_bTestResult = false;
	m_nWinnerOfTest = nTestFirstGet;//check with nTestLord
	m_pListShuffledCards = NULL;

	//��ǰ����Ϸ���
	m_pListThreePlayers = NULL;
	m_pListLordCards = NULL;
	m_pCurrentPlayer = NULL;
	m_pListLastPlayedCards = NULL;
	PassTimes = 0;	
}


CAIElectLord::~CAIElectLord(void)
{
	_CleanEnv();
	DEF_DELETE(m_pCommonLogic);
	DEF_DELETE(m_pCardsHelper);
	m_pTestCards = NULL;
}

//////////////////////////////////////////////////////////////////////////

void CAIElectLord::startTest()
{
	m_nCurrentAIElectLordStatus = NewGame;
	while (m_nCurrentAIElectLordStatus != ExitGame)
	{
		_GameStatusProcess();
	}
	
}

bool CAIElectLord::getTestResult()
{
	return m_bTestResult;
}





//////////////////////////////////////////////////////////////////////////
//��ǰ����Ϸ��� ����λ��
void CAIElectLord::_InitAllPlayers(CGamePlayerList** ppListThreePlayers, PlayerPosition nFirstPlayer)
{
	_UnInitAllPlayers(ppListThreePlayers);

	CGamePlayerList* pListThreePlayers = NULL;

	//��ǰ����Ϸ���
	CGamePlayer* pNewGamePlayerSouth = NULL;
	CGamePlayer* pNewGamePlayerEast = NULL;
	CGamePlayer* pNewGamePlayerWest = NULL;

	pListThreePlayers = new CGamePlayerList();

	pNewGamePlayerSouth = new CGamePlayer();
	pNewGamePlayerSouth->setPosition(South);
	pNewGamePlayerSouth->setPlayerType(CPU);

	pNewGamePlayerEast = new CGamePlayer();
	pNewGamePlayerEast->setPosition(East);
	pNewGamePlayerEast->setPlayerType(CPU);
	
	pNewGamePlayerWest = new CGamePlayer();
	pNewGamePlayerWest->setPosition(West);
	pNewGamePlayerWest->setPlayerType(CPU);
	
	//����һ��
	//0
	pNewGamePlayerSouth->setNextPlayer(pNewGamePlayerEast);
	pNewGamePlayerSouth->setLastPlayer(pNewGamePlayerWest);
	//1
	pNewGamePlayerEast->setNextPlayer(pNewGamePlayerWest);
	pNewGamePlayerEast->setLastPlayer(pNewGamePlayerSouth);
	//2
	pNewGamePlayerWest->setNextPlayer(pNewGamePlayerSouth);
	pNewGamePlayerWest->setLastPlayer(pNewGamePlayerEast);


	//����λ��
	if (South == nFirstPlayer)
	{
		pListThreePlayers->addGamePlayer(pNewGamePlayerSouth);
		pListThreePlayers->addGamePlayer(pNewGamePlayerEast);
		pListThreePlayers->addGamePlayer(pNewGamePlayerWest);
	}
	else if (East == nFirstPlayer)
	{
		pListThreePlayers->addGamePlayer(pNewGamePlayerEast);
		pListThreePlayers->addGamePlayer(pNewGamePlayerWest);
		pListThreePlayers->addGamePlayer(pNewGamePlayerSouth);
	}
	else if  (West == nFirstPlayer)
	{
		pListThreePlayers->addGamePlayer(pNewGamePlayerWest);
		pListThreePlayers->addGamePlayer(pNewGamePlayerSouth);
		pListThreePlayers->addGamePlayer(pNewGamePlayerEast);
	}
	else
	{
		_LOG(SourceFLInfo, DebugError, "not support CAIElectLord InitAllPlayers [nFirstPlayer=%d]", nFirstPlayer);

		pListThreePlayers->addGamePlayer(pNewGamePlayerSouth);
		pListThreePlayers->addGamePlayer(pNewGamePlayerEast);
		pListThreePlayers->addGamePlayer(pNewGamePlayerWest);
	}


	*ppListThreePlayers = pListThreePlayers;


}



void CAIElectLord::_UnInitAllPlayers(CGamePlayerList** ppListThreePlayers)
{
	CGamePlayerList* pListThreePlayers = NULL;

	if (NULL != ppListThreePlayers)
	{
		pListThreePlayers = *ppListThreePlayers;
	}

	if (NULL != pListThreePlayers)
	{
		pListThreePlayers->clearAllData();
		delete pListThreePlayers;
		pListThreePlayers = NULL;

		*ppListThreePlayers = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

/// <summary>
/// ��Ϸ�����ķ���
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CAIElectLord::_GameOver()
{
	_CleanEnv();	
}

/// <summary>
/// ��ϲӮ��
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CAIElectLord::_CongratulationWinner()
{
	CGamePlayer* pGamePlayer = NULL;
	int winIndex = 0;
	//m_pCurrentPlayer is the winner
	winIndex = _FindCurrentPlayerIndex(m_pCurrentPlayer);
}


/// <summary>
/// ��Ϸ״̬����
/// </summary>
void CAIElectLord::_GameStatusProcess()
{
	switch (m_nCurrentAIElectLordStatus)
	{
	case NewGame:		
		m_nCurrentAIElectLordStatus = CleanEnv;
		break;
	case CleanEnv:
		_CleanEnv();
		m_nCurrentAIElectLordStatus = ThreePlayersSitDown;
		break;
	case ThreePlayersSitDown:
		//m_nFirstGet ��һ���õ���
		_InitAllPlayers(&m_pListThreePlayers, m_nTestFirstGet);
		m_nCurrentAIElectLordStatus = PreparePokerCards;
		break;
	case PreparePokerCards:
		//��ʼ��һ����
		//copy data from m_pListDeckPokerCards to m_pListShuffledCards
		_InitializeDeck(&m_pListShuffledCards, m_pTestCards);
		m_nCurrentAIElectLordStatus = DealPokerCards;//���Ѿ�ϴ�õģ�������ϴ�ƣ�ֱ�ӷ���
		break;
	case DealPokerCards:
		//����
		_DealCards(m_nTestFirstGet);	
		m_nCurrentAIElectLordStatus = DealedCardsWithNoSort;
		break;
	case DealedCardsWithNoSort:
		//״̬���ѷ��ƣ���δ����
		//���������˵���
		_SortCardsForAll();
		m_nCurrentAIElectLordStatus = DealedCardsWithSort;
		break;
	case DealedCardsWithSort:
		//״̬���ѷ�����������
		//��ʼѡ�ٵ���
		_ElectLord(m_nTestLord);
		m_nCurrentAIElectLordStatus = LordElected;
		break;
	case LordElected:
		//״̬�������Ѿ�ѡ�ٳ���
		//���õ�ǰ���Ϊ����
		m_pCurrentPlayer = m_pListThreePlayers->getNodeByIndex(_FindLordIndex());
		//���Ʒ�������
		_DealCardsToLord();
		m_nCurrentAIElectLordStatus = DealedCardsToLordWithNoSort;
		break;
	case DealedCardsToLordWithNoSort:
		//״̬�����ѷ�����������������δ����
		//�������������� ����Ϊ��ǰ���
		_SortCardsForCurrentPlayer();
		m_nCurrentAIElectLordStatus = WaitingForPlayerToPlayCards;
		break;
	case WaitingForPlayerToPlayCards:
		//״̬���ȴ���ҳ���
		//����ҷ�������ָ��
		_RequestCards();
		m_nCurrentAIElectLordStatus = CardsPlayedWithNoSort;
		break;
	case CardsPlayedWithNoSort:
		//״̬������Ѿ�����,����û������
		//����ǰ�������
		_SortCardsForCurrentPlayer();
		m_nCurrentAIElectLordStatus = CardsPlayedWithSort;
		break;
	case CardsPlayedWithSort:
		//������Ϸ����
		//�����ǰ�����ȫ�����꣬����Ϸ����
		if (m_pCurrentPlayer->getCards()->getCount() == 0)
		{			
			std::string strLogInfo = "is The  Winner Of Test!!";
			m_pCurrentPlayer->printInfoToLog(strLogInfo);
			m_nWinnerOfTest = m_pCurrentPlayer->getPosition();
			if (m_nTestLord == m_nWinnerOfTest)
			{
				m_bTestResult = true;
			}

			//��ǰ�����ȫ������,Ӯ��//��Ϸ����
			m_nCurrentAIElectLordStatus = CongratulationWinner;
		}
		else
		{
			//���Pass�����Σ���һ�ֽ��� 
			if (PassTimes == 2)
			{
				//һ�ֽ���
				_ClearOneRound();
			}

			//ע���������pass����һ����Ϊ�ϴγ���֮���
			//����һ����ҳ���
			_NextPlayerPlayCards();
			m_nCurrentAIElectLordStatus = WaitingForPlayerToPlayCards;
		}
		break;
	case CongratulationWinner:
		//_CongratulationWinner();
		m_nCurrentAIElectLordStatus = GamdOver;
		break;
	case GamdOver:
		//״̬����Ϸ����
		_GameOver();
		m_nCurrentAIElectLordStatus = ExitGame;
		break;
	case ExitGame:
		return;
	default:
		printf("�������ڵ��Խ׶Σ�����δ������쳣\n");
	}//switch

}//void GameStatusProcess()



//////////////////////////////////////////////////////////////////////////

//#region ��Ϸ�¼��������



//////////////////////////////////////////////////////////////////////////
//#region ��Ϸ�߼��������
//

/// <summary>
/// �������״̬,׼����ʼ����Ϸ
/// ������Ϸ���������������ϵ��ƣ�3������뿪��λ��
/// </summary>
void CAIElectLord::_CleanEnv()
{

	//��������ϵ�������
	//��յ�����
	_UnInitializePokerCardList(&m_pListLordCards);	

	//����ϴγ��Ƽ�¼
	_UnInitializePokerCardList(&m_pListLastPlayedCards);

	//����ƺ�
	_UnInitializeDeck(&m_pListShuffledCards);

	//���Pass����
	PassTimes = 0;

	//���ÿ����ҵĳ���״̬��ʾ

	//���ÿ��������е���
	//��յ�ǰ����Ϸ���
	_UnInitAllPlayers(&m_pListThreePlayers);

	//��ǰ�������
	m_pCurrentPlayer = NULL;//TMP
}


/// <summary>
/// ��ʼ��һ���Ƶķ���
/// </summary>
/// <returns></returns>
void CAIElectLord::_InitializeDeck(CPokerCardList** ppListDeck, CPokerCardList* pListDeckPokerCards)
{	
	_UnInitializeDeck(ppListDeck);
	//������Ҫ�������Ƶ���ǰ���ƺ���

	CPokerCardList* pListDeck = NULL;
	pListDeck = new CPokerCardList();
	
	//copy data
	*pListDeck = *pListDeckPokerCards;

	*ppListDeck = pListDeck;	
}//InitializeDeck


void CAIElectLord::_UnInitializeDeck(CPokerCardList** ppListDeck)
{
	CPokerCardList* pListDeck = NULL;

	if (NULL != ppListDeck)
	{
		pListDeck = *ppListDeck;
	}

	if (NULL != pListDeck)
	{
		pListDeck->clearAllData();
		delete pListDeck;
		pListDeck = NULL;

		*ppListDeck = NULL;
	}


}

void CAIElectLord::_InitializePokerCardList(CPokerCardList** ppPokerCardList, CPokerCardList* pPokerCardList)
{
	CPokerCardList* pListTemp = NULL;
	_UnInitializePokerCardList(ppPokerCardList);
	*ppPokerCardList = NULL;

	if (NULL == pPokerCardList)
	{
		pListTemp = new CPokerCardList();
		*ppPokerCardList = pListTemp;
	}
	else
	{
		pListTemp = pPokerCardList;
		*ppPokerCardList = pListTemp;
	}
	
}

void CAIElectLord::_UnInitializePokerCardList(CPokerCardList** ppPokerCardList)
{
	CPokerCardList* pListTmp = NULL;

	if (NULL != ppPokerCardList)
	{
		pListTmp = *ppPokerCardList;
	}

	if (NULL != pListTmp)
	{
		pListTmp->clear();
		delete pListTmp;
		pListTmp = NULL;

		*ppPokerCardList = NULL;
	}
}





/// <summary>
/// ����
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CAIElectLord::_DealCards(PlayerPosition nFirstGet)
{
	CPokerCard* pPokerCardRef = NULL;
	CGamePlayer* player = NULL;
	int nIndexPlayerGetCard = 0;

	//find the first one
	nIndexPlayerGetCard = m_pListThreePlayers->getIndexByPlayerPosition(nFirstGet);

	//��ʾ���ǣ����ڽ�����ע��Namescope

	//���Ƹ�����
	//ÿ��17�� һ��51��
	for (int nIndex = 0; nIndex < 51; nIndex++)
	{
		//ȡ��һ����
		pPokerCardRef = m_pListShuffledCards->getNodeByIndex(nIndex);
		//��������
		//��˳����
		player = m_pListThreePlayers->getNodeByIndex(nIndexPlayerGetCard % 3);//
		nIndexPlayerGetCard++;

		player->addSingleCard(pPokerCardRef);
		//��Y�����ƶ�
		//��X�����ƶ�
	}

	//���������Ϊ������
	//��յ�����
	_UnInitializePokerCardList(&m_pListLordCards);
	_InitializePokerCardList(&m_pListLordCards, NULL);

	for (int nIndex = 51; nIndex < 54; nIndex++)
	{
		pPokerCardRef = m_pListShuffledCards->getNodeByIndex(nIndex);	
		m_pListLordCards->addCard(pPokerCardRef);
	}
}


/// <summary>
/// ��������ҵ�������
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CAIElectLord::_SortCardsForAll()
{
	// MessageBox.Show("��ʼ����");
	//��ʼ����
	CGamePlayer* player = NULL;
	int nIndex = 0;
	for (nIndex = 0; nIndex < m_pListThreePlayers->getCount(); nIndex++)
	{
		player = m_pListThreePlayers->getNodeByIndex(nIndex);
		player->sortCards();
	}
}



/// <summary>
/// ��ʼѡ��һ������
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CAIElectLord::_ElectLord(PlayerPosition nTestLord)
{	
	int nLordIndex = 0;

	nLordIndex = m_pListThreePlayers->getIndexByPlayerPosition(nTestLord);

	m_pListThreePlayers->getNodeByIndex(0)->setIsLord(false);
	m_pListThreePlayers->getNodeByIndex(1)->setIsLord(false);
	m_pListThreePlayers->getNodeByIndex(2)->setIsLord(false);

	m_pListThreePlayers->getNodeByIndex(nLordIndex)->setIsLord(true);//nTestLord Player is Lord
}


/// <summary>
/// ���Ʒ�������
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CAIElectLord::_DealCardsToLord()
{
	//MessageBox.Show("��ʼ���Ʒ�������");
	//���õ�ǰ���Ϊ����
	//CurrentPlayer 
	//�ҵ�����
	int lordIndex = 0;	
	CGamePlayer* plordGamePlayer = NULL;

	lordIndex = _FindLordIndex();
	plordGamePlayer = m_pListThreePlayers->getNodeByIndex(lordIndex);

	//���Ƹ�����
	//Storyboard story = new Storyboard();
	for (int nIndex = 0; nIndex < m_pListLordCards->getCount(); nIndex++)
	{
		CPokerCard* lordCard = m_pListLordCards->getNodeByIndex(nIndex);

		//��ԭ��λ�������µ���
		lordCard->setIsFaceUp(true);
		lordCard->setCanSelect(false);		

		//�����ƶ�����������
		plordGamePlayer->addSingleCard(lordCard);	
	}
}


/// <summary>
/// �ҵ������ı��
/// </summary>
/// <returns></returns>
int CAIElectLord::_FindLordIndex()
{
	int lordIndex = 0;
	CGamePlayer* pPlayer = NULL;
	for (lordIndex = 0; lordIndex < m_pListThreePlayers->getCount(); lordIndex++)
	{
		pPlayer = m_pListThreePlayers->getNodeByIndex(lordIndex);

		if (pPlayer->getIsLord())
		{
			break;
		}
	}
	return lordIndex;
}

/// <summary>
/// �ҵ���ǰ�����Players�еı��0 1 2
/// </summary>
/// <returns></returns>
int CAIElectLord::_FindCurrentPlayerIndex(CGamePlayer* pCurrentPlayer)
{
	int nIndex = 0;
	CGamePlayer* pPlayer = NULL;
	for (nIndex = 0; nIndex < m_pListThreePlayers->getCount(); nIndex++)
	{
		pPlayer = m_pListThreePlayers->getNodeByIndex(nIndex);

		if (pPlayer == pCurrentPlayer)
		{
			break;
		}
	}
	return nIndex;
}


/// <summary>
/// ����ǰ��ҵ�������
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CAIElectLord::_SortCardsForCurrentPlayer()
{
	if (m_pCurrentPlayer != NULL)
	{
		m_pCurrentPlayer->sortCards();
		//��ʼ��ĳ��Player���ư��յ�ǰ״̬��������Ķ���
	}
	
}

/// <summary>
/// ����ҷ�������ָ��
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CAIElectLord::_RequestCards()
{
	//((DispatcherTimer)sender).Stop();
	//            MessageBox.Show("����ҷ�������ָ��");
	//��ǰ��Ҳ�Ϊ��
	if (m_pCurrentPlayer != NULL)
	{         
		//������ҽ���
		//SetCurrentPlayer(CurrentPlayer);

		if (m_pCurrentPlayer->getPlayerType() == Human)
		{
			//����������
			
		}
		else if (m_pCurrentPlayer->getPlayerType() == CPU)
		{
			//�������ǵ���
			//ѡ��Ҫ������
			CPokerCardList* playingCards = m_pCurrentPlayer->AILogic->chooseCardsToPlay(m_pListLastPlayedCards);

			//����
			_PlayCards(playingCards);
		}
	}
}



/// <summary>
/// ���Ʒ���,ִ�и÷�������ɳ��ƣ����г��ƶ�����������һ״̬
/// </summary>
/// <param name="playingCards">Ҫ�����ƣ���Ҫ��֤��ȷ��</param>
void CAIElectLord::_PlayCards(CPokerCardList* playingCards)
{
	//���Player��һ����������
	//����ʾ�Ѿ�������

	//���Player��һ�۵�Pass���

	//ɾ��Player���еĳ��ƣ������Ƶ�Player��LastPlayedCards��
	m_pCurrentPlayer->getLastPlayedCards()->clear();
	if (NULL != playingCards)
	{
		for (int nIndex = 0; nIndex < playingCards->getCount(); nIndex++)
		{
			CPokerCard* cardRef = NULL;
			cardRef = playingCards->getNodeByIndex(nIndex);
			//����
			m_pCurrentPlayer->getLastPlayedCards()->addCard(cardRef);
			m_pCurrentPlayer->removeSingleCard(cardRef);
		}
	}

	//����ϼҲ�ΪPass���ϼҳ��Ƹ���,�����Ѿ�Pass�Ĵ���
	if (playingCards == NULL || playingCards->getCount() == 0)
	{
		PassTimes++;
	}
	else
	{
		_UnInitializePokerCardList(&m_pListLastPlayedCards);
		//LastPlayedCards = playingCards;
		_InitializePokerCardList(&m_pListLastPlayedCards, playingCards);

		PassTimes = 0;
	}

}

/// <summary>
/// һ�ֳ��ƽ���֮��ķ���
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CAIElectLord::_ClearOneRound()
{
	//MessageBox.Show("һ�ֳ��ƽ���");

	//�����������Ѿ������Ƽ�¼
	//����ϴγ��Ƽ�¼
	_UnInitializePokerCardList(&m_pListLastPlayedCards);
	
	//����Pass����
	PassTimes = 0;
}


/// <summary>
/// ����һ����ҳ���
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CAIElectLord::_NextPlayerPlayCards()
{
	//MessageBox.Show("��һ����ҳ���");
	m_pCurrentPlayer = _GetNextPlayer();
	
}

/// <summary>
/// �����һ�����Ƶ����
/// </summary>
/// <returns></returns>
CGamePlayer* CAIElectLord::_GetNextPlayer()
{
	//return Players[(Players.IndexOf(CurrentPlayer) - 1 + 3) % 3];
	CGamePlayer* pNextGamePlayer = NULL;
	int nCurrentPlayerIndex = 0;
	int nNextPlayerIndex = 0;

	nCurrentPlayerIndex = _FindCurrentPlayerIndex(m_pCurrentPlayer);
	nNextPlayerIndex = (nCurrentPlayerIndex - 1 + 3) % 3;

	pNextGamePlayer = m_pListThreePlayers->getNodeByIndex(nNextPlayerIndex);

	return pNextGamePlayer;	
}


NS_END(TA_TwoToOnePokerGameNet)





