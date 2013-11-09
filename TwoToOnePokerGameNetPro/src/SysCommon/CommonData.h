///////////////////////////////////////////////////////////
//  CProFunRes.h
//  Implementation of the Class CProFunRes
//  Created on:      20-2-2012 09:31:53
//  Original author: Shenglong.Lin
///////////////////////////////////////////////////////////



#ifndef  __DEF__COMMON_DATA_H__
#define  __DEF__COMMON_DATA_H__





//////////////////////////////////////////////////////////////////////////
///include
//////////////////////////////////////////////////////////////////////////


#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <typeinfo.h>

#include <time.h>
#include <sys/timeb.h>
#include <ctime>

#include <limits.h>
#include <float.h>
#include <memory.h>
//#include <memory>	 


#include <iostream>	
#include <string>
#include <sstream>
#include <vector>	
#include <map> 
#include <set>
#include <list>
#include <deque>
#include <utility> // for pair
#include <algorithm> // for lexicographical_compare
#include <iosfwd>    // needed by basic_path inserter and extractor
#include <stdexcept>
#include <cassert>

// random_shuffle example
#include <cstdlib>      // std::rand, std::srand

#include <fstream>
#include <signal.h>


#ifdef WIN32
#include <process.h>
#else
#include "pthread.h"
#endif
#include "pthread.h"

#include <direct.h>


#include "CommonDef.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)



//----------------------------------------------------------------------------

//log-------------------------------

// Debug levels are sequentially indexed starting at 0.  It is possible to iterate 
// through all debug levels by iterating through the integral sequence [ 0, countDebugLevels() ).
// The current debug level determines the highest debugging level that will be logged.
enum EDebugLevel
{
	DebugFatal		= 0L,	// fatal error conditions
	DebugError,		    // error conditions
	DebugWarn,		        // warning conditions
	DebugNorm,		        // normal conditions
	DebugInfo,		        // informational messages
	DebugDebug,		    // debug-level messages
	DebugSQL,              // SQL query strings
	DebugCORBA,            // for CORBA debug messages (only if other flags are enabled - see CorbaUtil)
	DebugMsg,              // for Messaging library debug messages
	DebugMsgPub,           // for Messaging library message sending and receiving debug messages
	DebugPeriodic,         // messages logged periodically by keepalives, etc
	DebugDatabase,      	// data access (SimpleDB) messages (other than SQL query strings)
	DebugExternal,      	// 3rd party (non-transactive) messages
	DebugTrace,		    // Function Entry/Exit Messages
};




struct DebugLevelNameValue
{
	const char*   pLevelName;
	EDebugLevel   nLevelValue;
};

/*


static DebugLevelNameValue nameValueMapping[] =
{
{ "DebugFatal",		DebugFatal		},
{ "DebugError",		DebugError		},
{ "DebugWarn",		DebugWarn		},
{ "DebugNorm",		DebugNorm		},
{ "DebugInfo",		DebugInfo		},
{ "DebugDebug",		DebugDebug		},
{ "DebugSQL",        DebugSQL         },
{ "DebugCORBA",      DebugCORBA       },
{ "DebugMsg",	    DebugMsg	        },
{ "DebugMsgPub",	    DebugMsgPub	    },
{ "DebugPeriodic",   DebugPeriodic    },
{ "DebugDatabase",	DebugDatabase	},
{ "DebugExternal",	DebugExternal	},
{ "DebugTrace",		DebugTrace		},
{ NULL,			DebugInfo		}
};
*/

//----------------------------------------------------------------------------




//thread----------------------------------
//thread.h use
enum EThreadState
{
	THREAD_STATE_NEW,

	THREAD_STATE_RUNNING,
	THREAD_STATE_TERMINATED,

	THREAD_STATE_RUNEND,
	THREAD_STATE_UNKNOWN                                     
};


//thread----------------------------------

//user  use
enum EThreadWorkState
{ 	
	WorkState_Begin,
	WorkState_Ready,

	WorkState_End,
};


//---------------------------------------------------------------


//==============================================================
/*
	declare some time constant for easy to use
*/
enum TimeConstantMac
{			   
	TIME_BASE_SECOND		= 1000,
	TIME_BASE_MIN			= 60   * TIME_BASE_SECOND,
	TIME_BASE_HOUR			= 60   * TIME_BASE_MIN,
	TIME_BASE_DAY			= 24   * TIME_BASE_HOUR,

	TIME_SECOND_TWO         = 2    * TIME_BASE_SECOND,
	TIME_SECOND_THREE       = 3    * TIME_BASE_SECOND,
	TIME_SECOND_FOUR        = 4    * TIME_BASE_SECOND,
	TIME_SECOND_FIVE		= 5    * TIME_BASE_SECOND,
	TIME_SECOND_TEN         = 10   * TIME_BASE_SECOND,
	TIME_SECOND_THIRTY      = 30   * TIME_BASE_SECOND,
            
	TIME_MIN_TWO			= 2  * TIME_BASE_MIN,
	TIME_MIN_THREE			= 3  * TIME_BASE_MIN,
	TIME_MIN_FIVE           = 5  * TIME_BASE_MIN,
	TIME_MIN_EIGHT          = 8  * TIME_BASE_MIN,
	TIME_MIN_TEN            = 10 * TIME_BASE_MIN,
	TIME_MIN_FIFTEEN        = 15 * TIME_BASE_MIN,
	TIME_MIN_TWENTY         = 20 * TIME_BASE_MIN,
	TIME_MIN_THIRTY         = 30 * TIME_BASE_MIN,
	TIME_MIN_FOURTY         = 40 * TIME_BASE_MIN,
	TIME_MIN_FIFTY          = 50 * TIME_BASE_MIN,

	TIME_HOUR_TWO           = 2 * TIME_BASE_HOUR
};


//---------------------------------------------------------------


//--datatype-------------------------------------------------------------
//syslist datatype
typedef enum tag_enumDataType
{
	DataType_NULL			  = 0,

	DataType_CConfigAttri,                   /*CConfigAttri*/
	DataType_CConfigBlock,                   /*CConfigBlock*/
	DataType_CConfigData,                    /*CConfigData*/

}enumDataType;
//---------------------------------------------------------------









//-------------------------------------------------------------------


typedef std::vector<std::string>							VrtLinesConT;	
typedef std::vector<std::string>::iterator					VrtLinesConIterT;

typedef std::map<unsigned int, std::string>					mapIDCodeConT;	
typedef std::map<unsigned int, std::string>::iterator		mapIDCodeConIterT;
typedef std::map<unsigned int, std::string>::value_type		mapIDCodeValueTypeT;

//-----------------------------------------------------------



NS_END(TA_TwoToOnePokerGameNet)

#endif  // __DEF__COMMON_DATA_H__








