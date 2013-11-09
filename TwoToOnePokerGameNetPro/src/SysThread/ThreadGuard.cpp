/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/synchronisation/src/ThreadGuard.cpp $
* @author:  B Fountain
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
* 
* Implementation of the ThreadGuard class
*
*/

#include "ThreadGuard.h"


NS_BEGIN(TA_TwoToOnePokerGameNet)

//
// Constructor
//
ThreadGuard::ThreadGuard( IThreadLockable& lockable, bool log )
: m_lockable(lockable), m_log (log)
{

	if (m_log)
	{
// 		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugTrace,
// 			"ThreadGuard: Attempting lock acquire, address %lx", (unsigned long)&m_lockable);
	}

	m_lockable.acquire();

	if (m_log)
	{
// 		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugTrace,
// 			"ThreadGuard: Acquired lock, address %lx", (unsigned long)&m_lockable);
	}
}


//
// Destructor
//
ThreadGuard::~ThreadGuard()
{
	if (m_log)
	{
// 		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugTrace,
// 			"ThreadGuard: Releasing lock, address %lx", (unsigned long)&m_lockable);
	}

	m_lockable.release();
}

NS_END(TA_TwoToOnePokerGameNet)