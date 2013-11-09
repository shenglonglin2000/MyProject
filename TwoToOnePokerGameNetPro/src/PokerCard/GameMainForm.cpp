
#include "GameMainForm.h"
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
#include "AIElectLord.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

CGameMainForm::CGameMainForm(void)
{	
	m_pCommonLogic = NULL;
	m_pCommonLogic = new CCommonLogic();

	m_pCardsHelper = NULL;
	m_pCardsHelper = new CCardsHelper();
	
	CurrentGameStatus = NewGame;
	//��ǰ����Ϸ���
	m_nPositionGetFirstCard = South;
	m_pListThreePlayers = NULL;
	m_pListDeck = NULL;
	m_pListLordCards = NULL;
	m_pCurrentPlayer = NULL;
	m_pListLastPlayedCards = NULL;
	PassTimes = 0;	
}


CGameMainForm::~CGameMainForm(void)
{
	_CleanEnv();
	DEF_DELETE(m_pCommonLogic);
	DEF_DELETE(m_pCardsHelper);
	m_nPositionGetFirstCard = South;

}

//////////////////////////////////////////////////////////////////////////

void CGameMainForm::startNewGame()
{
	CurrentGameStatus = NewGame;
	while (CurrentGameStatus != ExitGame)
	{
		_GameStatusProcess();
	}
	
}



void CGameMainForm::btn_PlayCards_Click()
{
	CPokerCardList* playingCards = new CPokerCardList();
	CPokerCard* pPokerCardRef = NULL;
	//Ѱ������ѡ�е���
	int nIndex = 0;
	for (nIndex = 0; nIndex < m_pCurrentPlayer->getCards()->getCount(); nIndex++)
	{
		pPokerCardRef = m_pCurrentPlayer->getCards()->getNodeByIndex(nIndex);
		if (pPokerCardRef->getIsSelected())
		{
			playingCards->addCard(pPokerCardRef);
		}

	}

	//MessageBox.Show(CommonLogic.GetCardsStyle(playingCards).ToString());
	//MessageBox.Show(CommonLogic.GetCardsValue(playingCards).ToString());
	//TODO:���ƺϷ����ж�
	if (!m_pCommonLogic->judgeCards(m_pListLastPlayedCards, playingCards))
	{
		//���Ʋ���ȷ
		//MessageBox.Show("���Ʋ���ȷ");
		printf("���Ʋ���ȷ\n");
		return;
	}
	//���Ʋ�����ѡ��,���ó��ư�ť
	//SetPlayCardsButton(false);


	nIndex = 0;
	for (nIndex = 0; nIndex < m_pCurrentPlayer->getCards()->getCount(); nIndex++)
	{
		pPokerCardRef = m_pCurrentPlayer->getCards()->getNodeByIndex(nIndex);
		pPokerCardRef->setCanSelect(false);
		pPokerCardRef->setHasResponse(false);
	}

	//PlayCards(playingCards);
}









//////////////////////////////////////////////////////////////////////////
//��ǰ����Ϸ��� ����λ��
void CGameMainForm::_InitAllPlayers(CGamePlayerList** ppListThreePlayers, PlayerPosition nPositionGetFirstCard)
{
	_UnInitAllPlayers(ppListThreePlayers);

	CGamePlayerList* pListThreePlayers = NULL;

	//��ǰ����Ϸ���
	CGamePlayer* pNewGamePlayerSouth = NULL;
	CGamePlayer* pNewGamePlayerEast = NULL;
	CGamePlayer* pNewGamePlayerWest = NULL;

	pListThreePlayers = new CGamePlayerList();
	*ppListThreePlayers = pListThreePlayers;

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
	if (South == nPositionGetFirstCard)
	{
		pListThreePlayers->addGamePlayer(pNewGamePlayerSouth);
		pListThreePlayers->addGamePlayer(pNewGamePlayerEast);
		pListThreePlayers->addGamePlayer(pNewGamePlayerWest);
	}
	else if (East == nPositionGetFirstCard)
	{
		pListThreePlayers->addGamePlayer(pNewGamePlayerEast);
		pListThreePlayers->addGamePlayer(pNewGamePlayerWest);
		pListThreePlayers->addGamePlayer(pNewGamePlayerSouth);
	}
	else if  (West == nPositionGetFirstCard)
	{
		pListThreePlayers->addGamePlayer(pNewGamePlayerWest);
		pListThreePlayers->addGamePlayer(pNewGamePlayerSouth);
		pListThreePlayers->addGamePlayer(pNewGamePlayerEast);
	}
	else
	{
		_LOG(SourceFLInfo, DebugError, "not support CGameMainForm InitAllPlayers [nFirstPlayer=%d]", nPositionGetFirstCard);

		pListThreePlayers->addGamePlayer(pNewGamePlayerSouth);
		pListThreePlayers->addGamePlayer(pNewGamePlayerEast);
		pListThreePlayers->addGamePlayer(pNewGamePlayerWest);
	}
		

}
void CGameMainForm::_UnInitAllPlayers(CGamePlayerList** ppListThreePlayers)
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
void CGameMainForm::_GameOver()
{
	_CleanEnv();	
}

/// <summary>
/// ��ϲӮ��
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CGameMainForm::_CongratulationWinner()
{
	CGamePlayer* pGamePlayer = NULL;
	int winIndex = 0;

	winIndex = _FindCurrentPlayerIndex(m_pCurrentPlayer);
}


/// <summary>
/// ��Ϸ״̬����
/// </summary>
void CGameMainForm::_GameStatusProcess()
{
	CPokerCardList* pPlayingCardsTmp = NULL;


	switch (CurrentGameStatus)
	{
	case NewGame:		
		CurrentGameStatus = CleanEnv;
		break;
	case CleanEnv:
		_CleanEnv();
		CurrentGameStatus = ThreePlayersSitDown;
		break;
	case ThreePlayersSitDown:
		_InitAllPlayers(&m_pListThreePlayers, m_nPositionGetFirstCard);
		CurrentGameStatus = PreparePokerCards;
		break;
	case PreparePokerCards:
		//��ʼ��һ����
		_InitializeDeck(&m_pListDeck);
		CurrentGameStatus = ShufflePokerCards;
		break;
	case ShufflePokerCards:
		//ϴ��
		_ShuffleCards(&m_pListDeck);
		CurrentGameStatus = DealPokerCards;
		break;
	case DealPokerCards:
		//����
		_DealCards(m_nPositionGetFirstCard);	
		CurrentGameStatus = DealedCardsWithNoSort;
		break;
	case DealedCardsWithNoSort:
		//״̬���ѷ��ƣ���δ����
		//���������˵���
		_SortCardsForAll(m_pListThreePlayers);
		CurrentGameStatus = DealedCardsWithSort;
		break;
	case DealedCardsWithSort:
		//״̬���ѷ�����������
		//��ʼѡ�ٵ���
		_ElectLord(m_pListDeck, m_pListThreePlayers, m_nPositionGetFirstCard);
		CurrentGameStatus = LordElected;
		break;
	case LordElected:
		//״̬�������Ѿ�ѡ�ٳ���
		//SetElectLordButton(false);
		//���õ�ǰ���Ϊ����
		m_pCurrentPlayer = NULL;
		m_pCurrentPlayer = m_pListThreePlayers->getNodeByIndex(_FindLordIndex());
		if (NULL == m_pCurrentPlayer)
		{
			_LOG(SourceFLInfo, DebugError, "Error: no Lord, then reStart game");
			CurrentGameStatus = NewGame;
		}
		else
		{
			//���Ʒ�������
			_DealCardsToLord();
			CurrentGameStatus = DealedCardsToLordWithNoSort;
		}		
		break;
	case DealedCardsToLordWithNoSort:
		//״̬�����ѷ�����������������δ����
		//�������������� ����Ϊ��ǰ���
		_SortCardsForCurrentPlayer();
		CurrentGameStatus = WaitingForPlayerToPlayCards;
		break;
	case WaitingForPlayerToPlayCards:
		//״̬���ȴ���ҳ���
		//����ҷ�������ָ��
		pPlayingCardsTmp = NULL;
		pPlayingCardsTmp = _RequestCards(m_pCurrentPlayer);
		_PlayCards(pPlayingCardsTmp);
		pPlayingCardsTmp = NULL;

		CurrentGameStatus = CardsPlayedWithNoSort;
		break;
	case CardsPlayedWithNoSort:
		//״̬������Ѿ�����,����û������
		//����ǰ�������
		_SortCardsForCurrentPlayer();
		CurrentGameStatus = CardsPlayedWithSort;
		break;
	case CardsPlayedWithSort:
		//������Ϸ����
		//�����ǰ�����ȫ�����꣬����Ϸ����
		if (m_pCurrentPlayer->getCards()->getCount() == 0)
		{			
			std::string strLogInfo = "I am Winner!!";
			m_pCurrentPlayer->printInfoToLog(strLogInfo);

			//��ǰ�����ȫ������,Ӯ��//��Ϸ����
			CurrentGameStatus = CongratulationWinner;
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
			CurrentGameStatus = WaitingForPlayerToPlayCards;
		}
		break;
	case CongratulationWinner:
		CurrentGameStatus = GamdOver;
		break;
	case GamdOver:
		//״̬����Ϸ����
		_GameOver();
		CurrentGameStatus = ExitGame;
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
void CGameMainForm::_CleanEnv()
{

	//��������ϵ�������
	//��յ�����
	_UnInitializePokerCardList(&m_pListLordCards);	

	//����ϴγ��Ƽ�¼
	_UnInitializePokerCardList(&m_pListLastPlayedCards);

	//����ƺ�
	_UnInitializeDeck(&m_pListDeck);

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
/// ��ʼ��һ������ɫ��ֵ������е��Ƶķ���
/// PokerSuit PokerValue
/// </summary>
/// <returns></returns>
void CGameMainForm::_InitializePokerSuitValueDeck(CPokerCardList** ppPokerCardList)
{
	int suitIndex = 0;
	int valueIndex = 1; 
	int nIndexInInitList = 0;
	CPokerCard* pPorkerCard = NULL;

	//clear data
	CPokerCardList* pPokerCardList = NULL;
	if (NULL == ppPokerCardList)
	{
		return;
	}

	if (NULL != *ppPokerCardList)
	{
		pPokerCardList = *ppPokerCardList;
		pPokerCardList->clearAllData();
		delete pPokerCardList;
		pPokerCardList = NULL;
	}
	pPokerCardList = new CPokerCardList();
	*ppPokerCardList = pPokerCardList;
	
	//init data
	
	nIndexInInitList = 0;//0 - 53
	for (suitIndex = 0; suitIndex < 5; suitIndex++)//PokerSuit 0-4
	{
		for (valueIndex = 1; valueIndex <= 15; valueIndex++)//PokerValue 1-13  14 15
		{
			PokerSuit nPokerSuit = (PokerSuit)suitIndex;
			PokerValue nPokerValue = (PokerValue)valueIndex;

			//����Ǵ�С����ֵ����ȷ������
			if ((nPokerSuit == Other && nPokerValue <= King) || (nPokerSuit < Other && nPokerValue > King))
			{
				continue;
			}

			//����һ������
			pPorkerCard = new CPokerCard(nPokerSuit, nPokerValue);
			//Card����
	
			pPorkerCard->setIsFaceUp(false);//����Ϊ����
			pPorkerCard->setCanSelect(false);//����ѡ��
			pPorkerCard->setHasResponse(false);//���������Ӧ
			pPorkerCard->setIndexInInitList(nIndexInInitList);
			pPokerCardList->addCard(pPorkerCard);
			pPorkerCard = NULL;
			nIndexInInitList++;
			
		}//for (valueIndex = 1

	}//for (suitIndex = 0

}

/// <summary>
/// ϴ��
/// ����һ��ӵ��54��Ԫ�ص�һά�������θ�ֵΪ1------53�� ���54�Σ�ÿ�������һ�����֣��͵�i��λ�õ����ֽ���
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CGameMainForm::_ShuffleCards(CPokerCardList** ppPokerCardList)
{
	//std::rand() ����������ĺ���
	std::vector<int> vecPorksList;
	std::vector<int>::iterator iterVec;
	CPokerCard* pPokerCard = NULL;
	CPokerCardList* pPokerCardListTmp = NULL;

	int suitIndex = 0;
	int valueIndex = 0;
	int nIndexInInitList = 0;
	int nIndexAdd = 0;
	int nIndexRand = 0;
	CPokerCardList* pPokerCardList = NULL;
	pPokerCardList = *ppPokerCardList;

	//log
	_LOG(SourceFLInfo, DebugDebug, "before ShuffleCards:");
	pPokerCardList->printInfoToLog();



	nIndexInInitList = 0;//0 - 53
	for (suitIndex = 0; suitIndex < 5; suitIndex++)//PokerSuit 0-4
	{
		for (valueIndex = 1; valueIndex <= 15; valueIndex++)//PokerValue 1-13  14 15
		{
			PokerSuit nPokerSuit = (PokerSuit)suitIndex;
			PokerValue nPokerValue = (PokerValue)valueIndex;
			//����Ǵ�С����ֵ����ȷ������
			if ((nPokerSuit == Other && nPokerValue <= King) || (nPokerSuit < Other && nPokerValue > King))
			{
				continue;
			}
			vecPorksList.push_back(nIndexInInitList);
			nIndexInInitList++;
		}//for (valueIndex = 1
	}//for (suitIndex = 0

	//������ʱ���ǲ�����ǰ����
	std::srand( (unsigned int)time(NULL) ); //�����漴��������������  

	//����һ��ӵ��54��Ԫ�ص�һά�������θ�ֵΪ1------53�� ���54�Σ�ÿ�������һ�����֣��͵�i��λ�õ����ֽ���
	// using built-in random generator:
	// ����˳��
	std::random_shuffle ( vecPorksList.begin(), vecPorksList.end() );
	
	pPokerCardListTmp = new CPokerCardList();
	nIndexAdd = 0;
	iterVec = vecPorksList.begin();	
	while (iterVec != vecPorksList.end())
	{
		nIndexRand = *iterVec;
		pPokerCard = pPokerCardList->getNodeByIndex(nIndexRand);//Deck
		pPokerCard->setIndexInDeckList(nIndexAdd);
		pPokerCardListTmp->addCard(pPokerCard);//0-53 
		nIndexAdd++;
		iterVec++;
	}

	//vecPorksList���� 0-54 ������
	//pPokerCardList���� 0-54 ����
	//���Ըոպ�ȫ������ȡ��
	pPokerCardList->clear();
	delete pPokerCardList;
	pPokerCardList = NULL;
	
	//����ϴ�õ���
	*ppPokerCardList = pPokerCardListTmp;

	_LOG(SourceFLInfo, DebugDebug, "After ShuffleCards:");
	pPokerCardListTmp->printInfoToLog();

}

/// <summary>
/// ��ʼ��һ���Ƶķ���
/// </summary>
/// <returns></returns>
void CGameMainForm::_InitializeDeck(CPokerCardList** ppListDeck)
{	
	_UnInitializeDeck(ppListDeck);
	//����
	_InitializePokerSuitValueDeck(ppListDeck);
	
}//InitializeDeck


void CGameMainForm::_UnInitializeDeck(CPokerCardList** ppListDeck)
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

void CGameMainForm::_InitializePokerCardList(CPokerCardList** ppPokerCardList, CPokerCardList* pPokerCardList)
{
	CPokerCardList* pListTemp = NULL;

	_UnInitializePokerCardList(ppPokerCardList);
	*ppPokerCardList = NULL;

	if (NULL != pPokerCardList)
	{
		pListTemp = pPokerCardList;
		*ppPokerCardList = pListTemp;
	} 
	else
	{
		pListTemp = new CPokerCardList();
		*ppPokerCardList = pListTemp;
	}
	
}

void CGameMainForm::_UnInitializePokerCardList(CPokerCardList** ppPokerCardList)
{
	CPokerCardList* pListLastPlayedCards = NULL;

	if (NULL != ppPokerCardList)
	{
		pListLastPlayedCards = *ppPokerCardList;
	}

	if (NULL != pListLastPlayedCards)
	{
		pListLastPlayedCards->clear();
		delete pListLastPlayedCards;
		pListLastPlayedCards = NULL;

		*ppPokerCardList = NULL;
	}
}




/// <summary>
/// ����
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CGameMainForm::_DealCards(PlayerPosition nFirstGet)
{
	CPokerCard* pPokerCardRef = NULL;
	CGamePlayer* player = NULL;
	int nIndexPlayerGetCard = 0;

	//find the first one
	nIndexPlayerGetCard = m_pListThreePlayers->getIndexByPlayerPosition(nFirstGet);

	//��ʾ���ǣ����ڽ�����ע��Namescope

	//���Ƹ�����
	//û��17�� һ��51��
	for (int nIndex = 0; nIndex < 51; nIndex++)
	{
		//ȡ��һ����
		pPokerCardRef = m_pListDeck->getNodeByIndex(nIndex);
		//��������
		//��˳����
		player = m_pListThreePlayers->getNodeByIndex(nIndexPlayerGetCard % 3);//0
		nIndexPlayerGetCard++;

		player->addSingleCard(pPokerCardRef);
		//��Y�����ƶ�
		//��X�����ƶ�
	}
	//���ƶ�����ʼ

	//���������Ϊ������
	//��յ�����
	_UnInitializePokerCardList(&m_pListLordCards);
	_InitializePokerCardList(&m_pListLordCards, NULL);

	for (int nIndex = 51; nIndex < 54; nIndex++)
	{
		pPokerCardRef = m_pListDeck->getNodeByIndex(nIndex);	
		m_pListLordCards->addCard(pPokerCardRef);
	}

	//
	//Deck ��Ȼ����54���ƵĶ���

	//log
	_LOG(SourceFLInfo, DebugDebug, "After DealCards:");
	nIndexPlayerGetCard = m_pListThreePlayers->getIndexByPlayerPosition(nFirstGet);
	player = m_pListThreePlayers->getNodeByIndex(nIndexPlayerGetCard);//Lord Player
	player->printInfoToLogAllPlayerHandCards();//printf handHards to log


	//������
	_LOG(SourceFLInfo, DebugDebug, "After DealCards Three LordCards:");
	m_pListLordCards->printInfoToLog();
}


/// <summary>
/// ��������ҵ�������
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CGameMainForm::_SortCardsForAll(CGamePlayerList* pListPlayers)
{
	// MessageBox.Show("��ʼ����");
	//��ʼ����
	CGamePlayer* pGamePlayer = NULL;
	int nIndex = 0;
	for (nIndex = 0; nIndex < pListPlayers->getCount(); nIndex++)
	{
		pGamePlayer = pListPlayers->getNodeByIndex(nIndex);
		pGamePlayer->sortCards();
	}
}



/// <summary>
/// ��ʼѡ��һ������
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CGameMainForm::_ElectLord(CPokerCardList* pListDeck, CGamePlayerList* pListPlayers, PlayerPosition nPositionGetFirstCard)
{	
	//��ѧ��ƪ���ġ��Ҷ���ţ����˵һ����������ţ��ǰ�����������Ѿ���ţ�����ˣ��Ѿ���������ˡ�
	//�õ��������Ͼ���˵�����õ����Ժ󣬾�Ӧ����ÿ������Ƶ�·�ߡ�
	//�ȼ����Լ��ǵ�����Ȼ����ƣ������������Լ�Ӯ���ǾͲ���������
	int nIndexTest = 0;
	CGamePlayer* pTestPlayer = NULL;
	PlayerPosition nTestPlayerPosition = South;

	for (nIndexTest = 0; nIndexTest < pListPlayers->getCount(); nIndexTest++)
	{
		pTestPlayer = pListPlayers->getNodeByIndex(nIndexTest);
		nTestPlayerPosition = pTestPlayer->getPosition();
		//init 
		pTestPlayer->setIsLord(false);

		CAIElectLord* pAIElectLord = NULL;
		pAIElectLord = new CAIElectLord(pListDeck, nPositionGetFirstCard, nTestPlayerPosition);
		pAIElectLord->startTest();
		if (pAIElectLord->getTestResult())
		{
			//find one Lord
			std::string strLogInfo = "is the Winner in CAIElectLord Test Result, then set to Lord Player";

			pTestPlayer->setIsLord(true);//Lord Player
			pTestPlayer->printInfoToLog(strLogInfo);

			delete pAIElectLord;
			pAIElectLord = NULL;

			break;
		}
		delete pAIElectLord;
		pAIElectLord = NULL;
	}//for
	
}


/// <summary>
/// ���Ʒ�������
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CGameMainForm::_DealCardsToLord()
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

	//plordGamePlayer->getCards()->sortCards();
	//��ʼ����
	//story.Begin(this);
}


/// <summary>
/// �ҵ������ı��
/// </summary>
/// <returns></returns>
int CGameMainForm::_FindLordIndex()
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
int CGameMainForm::_FindCurrentPlayerIndex(CGamePlayer* pCurrentPlayer)
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
void CGameMainForm::_SortCardsForCurrentPlayer()
{
	if (m_pCurrentPlayer != NULL)
	{
		m_pCurrentPlayer->sortCards();
		//��ʼ��ĳ��Player���ư��յ�ǰ״̬��������Ķ���
		//AnimateSortCard(CurrentPlayer);
	}
	
}

/// <summary>
/// ����ҷ�������ָ��
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
CPokerCardList* CGameMainForm::_RequestCards(CGamePlayer* pCurrentPlayer)
{
	CPokerCardList* pPlayingCards = NULL;

	//((DispatcherTimer)sender).Stop();
	//            MessageBox.Show("����ҷ�������ָ��");
	//��ǰ��Ҳ�Ϊ��
	if (pCurrentPlayer != NULL)
	{         
		//������ҽ���
		//SetCurrentPlayer(CurrentPlayer);

		if (pCurrentPlayer->getPlayerType() == Human)
		{
			//����������
			_SetHumanPlayingCards(pCurrentPlayer);
		}
		else if (pCurrentPlayer->getPlayerType() == CPU)
		{
			//�������ǵ���
			//ѡ��Ҫ������
			pPlayingCards = pCurrentPlayer->AILogic->chooseCardsToPlay(m_pListLastPlayedCards);
		}
	}

	return pPlayingCards;
}


/// <summary>
/// ��������ҿ��Գ���
/// </summary>
void CGameMainForm::_SetHumanPlayingCards(CGamePlayer* pCurrentPlayer)
{
	CPokerCard* card = NULL;

	//����ѡ����
	for (int nIndex = 0; nIndex < pCurrentPlayer->getCards()->getCount(); nIndex++)
	{
		card = pCurrentPlayer->getCards()->getNodeByIndex(nIndex);
		card->setCanSelect(true);
		card->setHasResponse(true);
	}

	//���ư�ť�ɼ�
	//SetPlayCardsButton(true);
}


/// <summary>
/// ���Ʒ���,ִ�и÷�������ɳ��ƣ����г��ƶ�����������һ״̬
/// </summary>
/// <param name="playingCards">Ҫ�����ƣ���Ҫ��֤��ȷ��</param>
void CGameMainForm::_PlayCards(CPokerCardList* playingCards)
{
	//log
	std::string strLogInfo = "PlayingCards:";
	m_pCurrentPlayer->printInfoToLog(strLogInfo);
	if (NULL != playingCards)
	{
		playingCards->printInfoToLog();
	}


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

			//���ó����Ƶ�״̬
			cardRef->setIsSelected(false);
			cardRef->setCanSelect(false);
			cardRef->setHasResponse(false);
			cardRef->setIsFaceUp(true);

			m_pCurrentPlayer->getLastPlayedCards()->addCard(cardRef);
			m_pCurrentPlayer->removeSingleCard(cardRef);
		}
	}
	

	//���ƶ���
	//AnimatePlayingCards(CurrentPlayer, playingCards);

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
void CGameMainForm::_ClearOneRound()
{
	//MessageBox.Show("һ�ֳ��ƽ���");

	//�����������Ѿ������Ƽ�¼
	CGamePlayer* pGamePlayer = NULL;
	CPokerCard* card = NULL;
	for (int nIndex = 0; nIndex < m_pListThreePlayers->getCount(); nIndex++)
	{
		pGamePlayer = m_pListThreePlayers->getNodeByIndex(nIndex);

		for (int nIndexTwo = 0; nIndexTwo < pGamePlayer->getLastPlayedCards()->getCount(); nIndexTwo++)
		{
			card = pGamePlayer->getLastPlayedCards()->getNodeByIndex(nIndexTwo);
			card->setVisibility(Hidden);
		}
		//������Player��Pass��ʾ����ΪFalse
		//SetPassText(Players.IndexOf(player), false);
	}

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
void CGameMainForm::_NextPlayerPlayCards()
{
	//((DispatcherTimer)sender).Stop();
	//            MessageBox.Show("��һ����ҳ���");
	m_pCurrentPlayer = _GetNextPlayer();
	
}

/// <summary>
/// �����һ�����Ƶ����
/// </summary>
/// <returns></returns>
CGamePlayer* CGameMainForm::_GetNextPlayer()
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





