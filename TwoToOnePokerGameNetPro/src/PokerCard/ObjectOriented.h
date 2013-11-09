/************************************************************************/
/* ��������quitsort�㷨ʵ��                                                                     */
/************************************************************************/

#ifndef __DEF_CLASS_OBJECTORIENTED_H__
#define __DEF_CLASS_OBJECTORIENTED_H__



#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
//#include <unistd.h>

#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)
class CObjectOriented
{
public:
	CObjectOriented();
	~CObjectOriented();
public:
	static void Sort(int* array, int length);
	static std::vector<int> Sort(std::vector<int> list);
protected:
private:
	
};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_OBJECTORIENTED_H__
