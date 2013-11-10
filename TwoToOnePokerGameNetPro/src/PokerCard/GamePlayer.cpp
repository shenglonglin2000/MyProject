

#include "GamePlayer.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"
#include "PokerCard.h"
#include "PokerCardList.h"
#include "PokerCardListList.h"
#include "AIComputer.h"
#include "DebugUtil.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

CGamePlayer::CGamePlayer(void)
{
	_cards = new CPokerCardList();
	_lastPlayedCards = new CPokerCardList();
	_playerType = Human;
	_position = South;
	_isLord = false;
	_lastPlayerRef = NULL;
	_nextPlayerRef = NULL;
	AILogic = NULL;
	AILogic = new CAIComputer();
	AILogic->setPlayer(this);


}


CGamePlayer::~CGamePlayer(void)
{
	_cards->clear();
	delete _cards;
	_cards = NULL;

	_lastPlayedCards->clear();
	delete _lastPlayedCards;
	_lastPlayedCards = NULL;

	_playerType = Human;
	_position = South;
	_isLord = false;
	_lastPlayerRef = NULL;
	_nextPlayerRef = NULL;

	delete AILogic;
	AILogic = NULL;
}



//�������
TA_TwoToOnePokerGameNet::PlayerType CGamePlayer::getPlayerType()
{
	return _playerType;
}
void CGamePlayer::setPlayerType(TA_TwoToOnePokerGameNet::PlayerType nPlayerType)
{
	_playerType = nPlayerType;
}

/// <summary>
/// ��ҵ�λ��
/// </summary>
TA_TwoToOnePokerGameNet::PlayerPosition CGamePlayer::getPosition()
{
	return _position;
}

std::string CGamePlayer::getPositionString()
{
	std::string strPlayerPosition = "";
	strPlayerPosition = getStringPlayerPosition(_position);

	return strPlayerPosition;
}


void CGamePlayer::setPosition(TA_TwoToOnePokerGameNet::PlayerPosition nPosition)
{
	_position = nPosition;
}

/// <summary>
/// �Ƿ��ǵ���
/// </summary>
bool CGamePlayer::getIsLord()
{
	return _isLord;
}
void CGamePlayer::setIsLord(bool bIsLord)
{
	_isLord = bIsLord;
}

/// <summary>
/// ��һ�����
/// </summary>
CGamePlayer* CGamePlayer::getLastPlayer()
{
	return _lastPlayerRef;
}
void CGamePlayer::setLastPlayer(CGamePlayer* pLastPlayerRef)
{
	_lastPlayerRef = pLastPlayerRef;
}




/// <summary>
/// ��һ�����
/// </summary>
CGamePlayer* CGamePlayer::getNextPlayer()
{
	return _nextPlayerRef;
}
void CGamePlayer::setNextPlayer(CGamePlayer* pNextPlayerRef)
{
	_nextPlayerRef = pNextPlayerRef;
}


CPokerCardList* CGamePlayer::getCards()
{
	return _cards;
}
CPokerCardList* CGamePlayer::getLastPlayedCards()
{
	return _lastPlayedCards;
}



//ΪPlayer���һ����
void CGamePlayer::addSingleCard(CPokerCard* cardRef)
{
	setCardUpDirection(_position, cardRef);
	setCardStatus(_playerType, cardRef);
	_cards->addCard(cardRef);
}

//ΪPlayerɾ��һ����
void CGamePlayer::removeSingleCard(CPokerCard* cardRef)
{
	_cards->removeCard(cardRef);
		
}

/// <summary>
/// ���ƽ�������
/// </summary>
void CGamePlayer::sortCards()
{
	_cards->sortCards();
	//_cards.Sort(CardsHelper.CompareCardsByValue);
}


/// <summary>
/// �����Ƶķ���
/// </summary>
/// <param name="card"></param>
void CGamePlayer::setCardUpDirection(PlayerPosition nGamePlayerPosition, CPokerCard* card)
{
	switch (nGamePlayerPosition)
	{
	case East:
		card->setUpDirection(Right);
		break;
	case West:
		card->setUpDirection(Left);
		break;
	case South:
		card->setUpDirection(Up);
		break;
	case North:
		card->setUpDirection(Down);
		break;
	}
}

/// <summary>
/// �����Ƶ�״̬
/// </summary>
/// <param name="card"></param>
void CGamePlayer::setCardStatus(PlayerType nGamePlayerType, CPokerCard* card)
{
	switch (nGamePlayerType)
	{
	case CPU:
		//���泯�£�����ѡ�񣬲���Ӧ���
		card->setIsFaceUp(false);
		card->setCanSelect(false);
		card->setHasResponse(false);
		break;
	case Human:
		//���泯��
		card->setIsFaceUp(true);
		card->setCanSelect(false);
		card->setHasResponse(false);
		break;
	}
}


void CGamePlayer::printInfoToLog(std::string& strLogInfo)
{
	std::string strPlayerPosition = "";
	strPlayerPosition = getStringPlayerPosition(_position);
	_LOG(SourceFLInfo, DebugDebug, "[Player=%s][isLord=%d] %s", strPlayerPosition.c_str(), _isLord, strLogInfo.c_str());			

}


void CGamePlayer::printInfoToLogAllPlayerHandCards()
{
	std::string strLogInfo;

	strLogInfo = "AllPlayerHandCards :";
	_LOG(SourceFLInfo, DebugDebug, "%s", strLogInfo.c_str());

	printInfoToLog(strLogInfo);
	_cards->printInfoToLog();

	/// ��һ�����
	if (NULL != _lastPlayerRef)
	{
		_lastPlayerRef->printInfoToLog(strLogInfo);
		_lastPlayerRef->getCards()->printInfoToLog();
	}

	/// ��һ�����
	if (NULL != _nextPlayerRef)
	{
		_nextPlayerRef->printInfoToLog(strLogInfo);
		_nextPlayerRef->getCards()->printInfoToLog();
	}

}



/*
struct PlayerPositionNameValue
{
const char*      pPlayerPositionName;
PlayerPosition   nPlayerPosition;
};
*/

static PlayerPositionNameValue nameValueMapping_PlayerPositionNameValue[] =
{
	{ "West",		West	 },
	{ "East",		East	 },
	{ "South",		South	 },
	{ "North",		North	 },
	{ NULL,			North	 },
};


const char* CGamePlayer::getStringPlayerPosition(PlayerPosition nPlayerPosition)
{	
	PlayerPositionNameValue* nv = nameValueMapping_PlayerPositionNameValue;

	while (nv->pPlayerPositionName != NULL)
	{
		if (nPlayerPosition == nv->nPlayerPosition)
		{
			return nv->pPlayerPositionName;
		}
		nv++;
	}

	return "UNKNOWN";
}

NS_END(TA_TwoToOnePokerGameNet)





