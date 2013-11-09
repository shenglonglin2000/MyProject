#include "MySemaphore.h"

#include "pthread.h"
#include "CommonDef.h"
#include "CommonData.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

CMySemaphore::CMySemaphore( unsigned int initial )
{
	
}


CMySemaphore::~CMySemaphore()
{

}


void CMySemaphore::wait(void)
{
	sem_wait(&m_sem);
}


bool CMySemaphore::tryWait(void)
{
	if ( 0 == sem_trywait(&m_sem) )
	{
		return false;
	}

	return true;
}


void CMySemaphore::post(void)
{
	sem_post(&m_sem);
}


NS_END(TA_TwoToOnePokerGameNet)


