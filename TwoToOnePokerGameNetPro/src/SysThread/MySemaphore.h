#ifndef MYSEMAPHORE_H_INCLUDED
#define MYSEMAPHORE_H_INCLUDED


#include "semaphore.h"
#include "pthread.h"
#include "CommonDef.h"
#include "CommonData.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

class CMySemaphore
{
public:
	/** 
	* Semaphore
	*
	* Construct a new semaphore object
	*
	* @param initial	the initial count. Set to zero if you want the semaphore to
	*						block on the first call to wait
	*/
	CMySemaphore( unsigned int initial = 1 );

	/** 
	* ~Semaphore
	* 
	* semaphore desctructor
	*/
	virtual ~CMySemaphore();

	/** 
	* wait
	*
	* wait (and block) for the semaphore to be signalled, or continue if already signalled
	*
	* ie: If semaphore value is > 0 then decrement it and carry on. If it's
	* already 0 then block.
	*/
	void wait(void);

	/** 
	* trywait
	*
	* test to see if the semaphore will block on wait. This allows your thread to do something
	* else while it waits for a signal.
	*
	* ie: if semaphore value is > 0 then decrement it and return true.
	* If it's already 0 then return false.
	*
	* @return bool	false if the semaphore will block
	*/
	bool tryWait(void);

	/** 
	* post
	*
	* if any threads are blocked in wait(), wake one of them up. Otherwise
	* increment the value of the semaphore.
	*
	* @param void
	*/
	void post(void);

private:

	//
	// Disable the default copy and assignment constructors as shouldn't used.
	//
	CMySemaphore( const CMySemaphore& );
	CMySemaphore& operator=( const CMySemaphore& );

	sem_t			m_sem;
};
NS_END(TA_TwoToOnePokerGameNet)

#endif // MYSEMAPHORE_H_INCLUDED

