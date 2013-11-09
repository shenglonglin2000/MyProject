#ifndef  __DEF_CLASS_GAME_PLAYER__H__
#define  __DEF_CLASS_GAME_PLAYER__H__

#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)

class CPokerCard;
class CPokerCardList;
class CPokerCardListList;
class CAIComputer;

class CGamePlayer
{
public:
	CAIComputer* AILogic;

private:

	//�������
	PlayerType _playerType;

	/// <summary>
	/// ��ҵ�λ��
	/// </summary>
	PlayerPosition _position;

	/// <summary>
	/// ������ϵ���
	/// </summary>
	CPokerCardList* _cards;

	CPokerCardList* _lastPlayedCards;
	
	/// <summary>
	/// �Ƿ��ǵ���
	/// </summary>
	bool _isLord;

	/// <summary>
	/// ��һ�����
	/// </summary>
	CGamePlayer* _lastPlayerRef;
	
	/// <summary>
	/// ��һ�����
	/// </summary>
	CGamePlayer* _nextPlayerRef;

public:
	CGamePlayer(void);
	~CGamePlayer(void);


public:
	CPokerCardList* getCards();
	CPokerCardList* getLastPlayedCards();

public:
	//�������
	TA_TwoToOnePokerGameNet::PlayerType getPlayerType(); 
	void setPlayerType(TA_TwoToOnePokerGameNet::PlayerType nPlayerType);

	/// <summary>
	/// ��ҵ�λ��
	/// </summary>
	TA_TwoToOnePokerGameNet::PlayerPosition getPosition(); 
	void setPosition(TA_TwoToOnePokerGameNet::PlayerPosition nPosition);
	std::string getPositionString();

	/// <summary>
	/// �Ƿ��ǵ���
	/// </summary>
	bool getIsLord();
	void setIsLord(bool bIsLord);

	/// <summary>
	/// ��һ�����
	/// </summary>
	CGamePlayer* getLastPlayer();
	void setLastPlayer(CGamePlayer* pLastPlayerRef);




	/// <summary>
	/// ��һ�����
	/// </summary>
	CGamePlayer* getNextPlayer();
	void setNextPlayer(CGamePlayer* pNextPlayerRef);


public:
	//ΪPlayer���һ����
	void addSingleCard(CPokerCard* cardRef);

	//ΪPlayerɾ��һ����
	void removeSingleCard(CPokerCard* cardRef);

	/// <summary>
	/// ���ƽ�������
	/// </summary>
	void sortCards();


	/// <summary>
	/// �����Ƶķ���
	/// </summary>
	/// <param name="card"></param>
	void setCardUpDirection(PlayerPosition nGamePlayerPosition, CPokerCard* card);


	/// <summary>
	/// �����Ƶ�״̬
	/// </summary>
	/// <param name="card"></param>
	void setCardStatus(PlayerType nGamePlayerType, CPokerCard* card);



public:
	const char* getStringPlayerPosition(PlayerPosition nPokerSuit);

	void printInfoToLog(std::string& strLogInfo);

	void printInfoToLogAllPlayerHandCards();



};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_GAME_PLAYER__H__
