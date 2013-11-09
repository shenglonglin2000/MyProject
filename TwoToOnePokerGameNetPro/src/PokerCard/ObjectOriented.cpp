#include "ObjectOriented.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)

CObjectOriented::CObjectOriented()
{

}
CObjectOriented::~CObjectOriented()
{

}

void CObjectOriented::Sort(int* array, int length)
{
	//srand(time(0)+getpid());
	std::vector<int> ivector(length);
	for(int i = 0; i < length; i++)
		ivector[i] = array[i];

	ivector = CObjectOriented::Sort(ivector);

	for(int i = 0; i < length; i++)
		array[i] = ivector[i];
}

std::vector<int> CObjectOriented::Sort(std::vector<int> list)
{
	if (list.size() < 2)
		return list;

	// random pivot
	//int pivot = list[(int)((double)rand() / (RAND_MAX + 1) * list.size())];
	//srand(std::rand());

	// middle pivot
	int pivot = list[list.size() / 2];
	list.erase (list.begin() + list.size() / 2);
	std::vector<int> less;
	std::vector<int> greater;
	std::vector<int> result;
	for(int i = 0;i < (int)list.size();++i)
	{
		if (list[i] > pivot)
			greater.push_back(list[i]);
		else
			less.push_back(list[i]);
	}


	less = CObjectOriented::Sort(less);
	for(int i = 0;i < (int)less.size();++i)
		result.push_back(less[i]);

	result.push_back(pivot);

	greater = CObjectOriented::Sort(greater);
	for(int i = 0;i < (int)greater.size();++i)
		result.push_back(greater[i]);

	return result;
}
NS_END(TA_TwoToOnePokerGameNet)





