#ifndef THREADGUARD_H_INCLUDED
#define THREADGUARD_H_INCLUDED
/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/synchronisation/src/ThreadGuard.h $
* @author:  B Fountain
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
* 
* This class acquires access to the given lock on construction, excluding
* all other threads from access to code which is similarly locked. The lock
* is released on destruction.
* 
* To make methods synchronised (ala Java), inherit from IThreadLockable implementation 
* and instantiate a local variable of class ThreadGuard at the top of each method
* that you want to synchronise, using (*this) to initialise the lock.
*
*/

#include "CommonDef.h"
#include "IThreadLockable.h"



NS_BEGIN(TA_TwoToOnePokerGameNet)


class ThreadGuard
{
public:

	/** 
	* Constructor
	*
	* Constructs ThreadGuard object, automatically acquiring the 
	* IThreadLockable object passed to it.
	*
	* @param &IThreadLockable - The IThreadLockable this ThreadGuard
	*                           is to acquire/release    
	*/
	ThreadGuard( IThreadLockable& lockable, bool log = false );


	/** 
	* Destructor
	*
	* Automatically releases the IThreadLockable this ThreadGuard was
	* constructed with.
	*
	*/
	virtual ~ThreadGuard();


	/** 
	* acquire
	*
	* Acquire the lock.
	*
	*/
	inline void acquire()
	{
		m_lockable.acquire();
	}

	/** 
	* release
	*
	* Release the lock.
	* 
	*/
	inline void release()
	{
		m_lockable.release();
	}

private:

	//
	// Disable the default copy and assignment constructors as shouldn't used.
	//
	ThreadGuard( const ThreadGuard& );
	ThreadGuard& operator=( const ThreadGuard& );


	IThreadLockable& m_lockable;    // Reference to IThreadLockable this object 
	bool m_log;
};

NS_END(TA_TwoToOnePokerGameNet)

#endif // THREADGUARD_H_INCLUDED

