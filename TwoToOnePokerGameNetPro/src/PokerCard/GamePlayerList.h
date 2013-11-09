#ifndef  __DEF_CLASS_GAME_PLAYER_LIST__H__
#define  __DEF_CLASS_GAME_PLAYER_LIST__H__

#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)

class CGamePlayer;

class CGamePlayerList
{
public:
	CGamePlayerList(void);
	~CGamePlayerList(void);

private:
	std::list<CGamePlayer*>*   m_pGamePlayerList;

public:
	void addGamePlayer(CGamePlayer* pGamePlayer);
	int  removeGamePlayer(CGamePlayer* pGamePlayer);
	int  getCount();
	void clearAllData();
	void clear();


public:
	//begin with 0
	CGamePlayer* getNodeByIndex(int nIndex);
	int getIndexByPlayerPosition(PlayerPosition nPlayerPosition);

};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_GAME_PLAYER_LIST__H__
