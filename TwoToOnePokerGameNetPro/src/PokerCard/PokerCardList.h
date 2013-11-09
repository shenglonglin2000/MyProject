#ifndef  __DEF_CLASS_POKER_CARD_LIST__H__
#define  __DEF_CLASS_POKER_CARD_LIST__H__

#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)

class CPokerCard;

class CPokerCardList
{
public:
	CPokerCardList(void);
	~CPokerCardList(void);

public:
	CPokerCardList& CPokerCardList::operator =(const CPokerCardList& rhs);//�������캯��


private:
	std::list<CPokerCard*>*   m_pPokerCardList;
public:
	void addCard(CPokerCard* card);
	int  removeCard(CPokerCard* card);
	int  getCount() const;
	void addRange(CPokerCardList* pPokerCardList);

public:
	//begin with 0
	CPokerCard* getNodeByIndex(int nIndex) const;

public:
	void clearAllData();
	void clear();
	void sortCards();
	void printInfoToLog();



};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_POKER_CARD_LIST__H__
