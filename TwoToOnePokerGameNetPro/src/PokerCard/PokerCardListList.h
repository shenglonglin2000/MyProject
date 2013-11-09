#ifndef  __DEF_CLASS_POKER_CARD_LIST_LIST__H__
#define  __DEF_CLASS_POKER_CARD_LIST_LIST__H__

#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)

class CPokerCardList;

class CPokerCardListList
{
public:
	CPokerCardListList(void);
	~CPokerCardListList(void);

private:
	std::list<CPokerCardList*>*   m_pPokerCardListList;

public:
	void addNode(CPokerCardList* pPokerCardList);
	void addRange(CPokerCardListList* pPokerCardListList);
	int  removeNode(CPokerCardList* pPokerCardList);

	int  getCount();

	void clear();
	void clearAllData();
	void printInfoToLog();


	//begin with 0
	CPokerCardList* getNodeByIndex(int nIndex);


};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_POKER_CARD_LIST_LIST__H__
