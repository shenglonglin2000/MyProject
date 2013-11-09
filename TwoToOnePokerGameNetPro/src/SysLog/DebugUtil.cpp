#include "DebugUtil.h"

#include "pthread.h"
#include <sys/timeb.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <sstream>
#include "Logger.h"

#include "CommonDef.h"
#include "CommonData.h"

//for pid
#if defined( WIN32 ) || defined( LINUX )
#include <process.h>
#else
#include <unistd.h>
#endif 


NS_BEGIN(TA_TwoToOnePokerGameNet)

DebugUtil* DebugUtil::m_instance = 0;


DebugUtil& DebugUtil::getInstance()
{
	// make this thread safe!

	// Fix for TD2689: m_lock was not being instantiated when DebugUtil was instantiated
	// from a static object. An accessor method is now used so that construction of the
	// lock is guaranteed.
	TA_TwoToOnePokerGameNet::ThreadGuard guard (getLock());

	if( m_instance == 0 )
	{
		m_instance = new DebugUtil();
	}

	return *m_instance;
}

void DebugUtil::removeInstance()
{
	{
		TA_TwoToOnePokerGameNet::ThreadGuard guard (getLock());

		if( m_instance != NULL )
		{
			delete m_instance;
		}
	}

	TA_TwoToOnePokerGameNet::ReEntrantThreadLockable* lock = &(getLock());
	delete lock;
}

DebugUtil::~DebugUtil()
{
	// Putting within try/catch so that pointers
	// will still be deleted if an exception is
	// thrown to prevent memory leaks

	try 
	{
		if ( m_outFile.is_open() )
		{
			m_outFile.flush();
			m_outFile.close();
		}
	}
	catch ( ... )
	{
	}

	delete m_logger;
	m_logger = NULL;
}


void DebugUtil::log(const std::string& file, int line, EDebugLevel level, ...)
{
	int nGetlevel = 0;
	std::string strLogLevel;  
	
	if ( level > m_currDebugLevel )
	{
		return;
	}

	//TA_DEBUG_ASSERT ( !file.empty(), "File name not provided" );
	
	// Line number can be -1 if it is unknown (as is possibly the case
	// with the FunctionEntry class).
	//TA_DEBUG_ASSERT ( line > 0 || line == -1, "Line number not provided" );
	TA_TwoToOnePokerGameNet::ThreadGuard guard (getLock());

	// Get the variable args
	va_list argPtr;
	
	//argPtr piont to level's pos
	va_start ( argPtr, level );

	try
	{
		// Ensure that args is pointing to something
		//TA_DEBUG_ASSERT ( argPtr != NULL, "Pointer to variable args is NULL" );

		//nGetlevel = va_arg ( args, int );
		// Reset the arg pointer back to the beginning
		//va_start ( args, level );


		// Check that the level is valid - getStringDebugLevel calls
		// //TA_DEBUG_ASSERT if the level does not exist
		strLogLevel = getStringDebugLevel( ( EDebugLevel )level );


		m_lastMsgLevel = ( EDebugLevel ) level;

		_LogMessage(DEF_INT_MinLogMesgSize, file, line, strLogLevel, argPtr);

		// Reset the arg pointer
		va_end ( argPtr );
	}
	catch ( ... )
	{
		// Write to stderr that an unknown exception has been caught. It is
		// caught here rather than rethrowing it because it seems inconvenient
		// to do a try/catch everywhere LOG() is called.

		std::cerr << "DebugUtil::log(): Caught an unknown exception. Message will not be logged." << std::endl;

		// Reset the arg pointer
		va_end ( argPtr );
	}

}



void DebugUtil::logLarge(const std::string& file, int line, int nStrlen, EDebugLevel level, ...)
{
	int nGetlevel = 0;
	std::string strLogLevel;  

	if ( level > m_currDebugLevel )
	{
		return;
	}

	//TA_DEBUG_ASSERT ( !file.empty(), "File name not provided" );
	//TA_DEBUG_ASSERT ( line > 0 || line == -1, "Line number not provided" );
	TA_TwoToOnePokerGameNet::ThreadGuard guard (getLock());

	// Get the variable args
	va_list argPtr;
	va_start ( argPtr, level );

	try
	{
		//TA_DEBUG_ASSERT ( argPtr != NULL, "Pointer to variable args is NULL" );
		strLogLevel = getStringDebugLevel( ( EDebugLevel )level );
 		m_lastMsgLevel = ( EDebugLevel ) level;
 		
 		if (nStrlen <= DEF_INT_MinLogMesgSize)
 		{
			_LogMessage(DEF_INT_MaxLogMesgSize, file, line, strLogLevel, argPtr);
 		}
 		else
 		{
			_LogMessage(nStrlen, file, line, strLogLevel, argPtr);
 		}
 		
		va_end ( argPtr );
	}
	catch ( ... )
	{
		std::cerr << "DebugUtil::logLarge(): Caught an unknown exception. Message will not be logged." << std::endl;
		va_end ( argPtr );
	}

}


void DebugUtil::setFile(const char* name)
{
	pid_t pid = getpid();

	ThreadGuard guard (getLock());

	// Close file if currently open
	//TD17314 make sure the setFile won't make the log file size small than config
	if( !m_filename.empty() && m_filename == name && m_sizeInBytes < m_maxSizeInBytes)
	{
		return;
	}
	if (m_outFile.is_open())
	{
		m_outFile.flush();
		m_outFile.close();
	}

	if( name == NULL || name[0] == '\0' )
	{
		// Notify that messages will be printed to stdout

		std::cerr << "Cannot set file to empty string. Log messages will be written to stdout." << std::endl;
		m_filename = "";
		return;
	}


	// BCF 19-Nov-2001 PR1028:
	// If the file exists, then open the file for reading and count the number
	// of lines which the file already contains. This is not quite the same as
	// the usual meaning of nLines (which actually measure the number of
	// *entries*), but gives us a good enough estimate to work with - we just
	// want to make sure that the logfile doesn't grow without bound if the
	// process is continually restarted.
	//
	// The 1000 character buffer may seem excessive, but the file will turn
	// bad if a line is longer than the given buffer, so best to be sure
	//
	// It is pretty inefficient to scan the entire file just to determine its
	// size, but this should only happen once in the process's life. If anyone
	// has a better idea please feel free give it a go (I tried taking the
	// difference of the begin and end positions, but it didn't work)

	try
	{
		m_sizeInBytes = 0;

		// Store filename in class variable
		m_filename = name;

		// Used to check if a new file needs to be opened.
		// setFile is only called externally so if m_filename
		// is different from m_defaultFilename then a debug
		// level specific file has been used in between.
		m_defaultFilename = name;

		std::string fileOpen = "";

		++m_fileCount;

		// Create the file. The filename will be the name with a number
		// appended onto the end before the extension. The number is a 
		// count of how many log files have been written to so far,
		// for the default log file. Specific level files are handled
		// seperately.  Eg. If the name is AlarmProcessor.log", the output file will 
		// be set to "AlarmProcessor.1.log".
		fileOpen = _CreateFilename(m_fileCount);

		// Check the size of the file
		m_sizeInBytes = _GetFileSize(fileOpen);

		if (m_sizeInBytes == -1)
		{
			// Couldn't acertain size of file so message logged to cerr
			return;
		}

		// BCF 16-Nov-2001 PR1028:
		// Open the file for appending (retain existing entries)

		m_outFile.open(fileOpen.c_str(),std::ios::out|std::ios::app);
		if (m_outFile.fail())
		{
			std::cerr << "DebugUtil::SetFile(): error opening file! - The reason is: "
				<< strerror( errno )
				<< std::endl;
			m_outFile.clear();
		}
		

 		log(SourceFLInfo, DebugInfo, "%s", 
 			"\n\t**********************************************************************************************************************\n"
 			"\t*                                                   START OF FILE                                                    *\n" 
			"\t**********************************************************************************************************************\n" );
	
		log(SourceFLInfo, DebugInfo, "Process ID = %d", pid);//Process Thread
	}
	catch ( ... )
	{
		// Write to stderr that an unknown exception has been caught. It is
		// caught here because a thread lock has been acquired so if an 
		// exception is thrown, the lock needs to be released.

		std::cerr << "Caught an unknown exception while attempting to set file."
			<< " Messages will be logged to stdout." << std::endl;
	}
}    


/*
struct DebugLevelNameValue
{
const char*   pLevelName;
EDebugLevel   nLevelValue;
};
*/



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


const char* DebugUtil::getStringDebugLevel(EDebugLevel level)
{
	EDebugLevel result = DebugFatal;
	DebugLevelNameValue* nv = nameValueMapping;

	//TA_DEBUG_ASSERT( nv != NULL, "Object is NULL" );
	
	while (nv->pLevelName != NULL)
	{
		if (level == nv->nLevelValue)
		{
			return nv->pLevelName;
		}
		nv++;
	}

	// Should never come down here, but just in case
	//TA_DEBUG_ASSERT ( false, "Undefined DebugLevel" );
	return "UNKNOWN";
}


EDebugLevel DebugUtil::getDebugLevelFromString(const char* levelStr)
{

	EDebugLevel result = DebugFatal;
	DebugLevelNameValue* nv = nameValueMapping;

	//TA_DEBUG_ASSERT( nv != NULL, "Object is NULL" );

	while (nv->pLevelName != NULL)
	{
		if (strcmp(levelStr, nv->pLevelName) == 0)
		{
			result = nv->nLevelValue;
			return result;
		}
		nv++;
	}

	// Should never come down here, but just in case
	//TA_DEBUG_ASSERT ( false, "Debug String is invalid" );
	return result;
}   




////////////////// PRIVATE FUNCTIONS //////////////////////
// Constructor
//
// NOTE: Typically the Linux boxes will be embedded processors with limited
//       memory and disc space.  Therefore the length of the log files will
//       nee to be somewhat smaller.
//
DebugUtil::DebugUtil()
{
	m_currDebugLevel = DebugInfo;
	m_lastMsgLevel = DebugDebug;
	m_filename.clear();
	m_defaultFilename.clear();
	m_firstSetFile = true;
	m_encodePidInFilenames = false;
	m_maxSizeInBytes = 300000;
	m_sizeInBytes = 0;
	m_maxFiles = 30;
	m_fileCount = 0;
	m_logger = NULL;

	//TA_DEBUG_ASSERT ( m_maxSizeInBytes != 0, "RPARAM_DEBUGFILEMAXSIZE == 0" );
	//TA_DEBUG_ASSERT ( m_maxFiles != 0, "RPARAM_DEBUGMAXFILES == 0" );

	// Putting initialisation in try/catch block to 
	// prevent memory leak if LoadDebugTypes() throws
	// an exception
	try 
	{
		m_logger = new Logger();

		//TA_DEBUG_ASSERT ( m_logger != NULL, "Object not created" );

		// More levels can be added dynamically if required
		// Initialise storage vectors
		m_filenameNumber = std::vector<int>(20, -1);
	}
	catch (...)
	{
		std::cerr << "There were problems on initialisation. There will be no logging" << std::endl;

		delete m_logger;
		m_logger = NULL;
	}
}


void DebugUtil::_SwapFiles()
{
	// This method is not thread locked because it is called by thread
	// locked methods.

	// Reset the line count (contains EOF character)

	m_sizeInBytes = 0; 
	if ( m_filename == "" )
	{
		return;
	}

	// m_fileCount started at 0 but was incremented in setFile. So
	// whatever m_fileCount is set to here is how many files there
	// are currently for the default file only.

	std::string newname;

	// Using the default file
	if (m_filename == m_defaultFilename)
	{
		++m_fileCount;

		// If we have reached our max files, then start
		// deleting the oldest file.

		if ( m_fileCount >= (m_maxFiles + 1) )
		{
			int rmFileCnt = m_fileCount - m_maxFiles;
			std::string prevname = _CreateFilename(rmFileCnt);

			//TA_DEBUG_ASSERT ( prevname.c_str() != NULL, "File string is NULL" );

			// Delete the oldest file 

			remove(prevname.c_str());
		}

		// Close the file

		m_outFile.flush();
		m_outFile.close();

		/*
		* TD4889 - Once the file has been closed, clear its internal flags
		*          to ensure that the object is clean.
		*/
		m_outFile.clear();

		// Create the new file and append a new file count

		newname = _CreateFilename(m_fileCount);

	}
	else
		// Using a specified file for the level
	{
		// Increment the file number, use the last message level
		// as an index, to avoid using a loop.
		++(m_filenameNumber[m_lastMsgLevel]);

		// If we have reached our max files, then start
		// deleting the oldest file.
		if ( m_filenameNumber[m_lastMsgLevel] >= (m_maxFiles + 1) )
		{
			int rmFileCnt = m_filenameNumber[m_lastMsgLevel] - m_maxFiles;
			std::string prevname = _CreateFilename(rmFileCnt);

			//TA_DEBUG_ASSERT ( prevname.c_str() != NULL, "File string is NULL" );

			// Delete the oldest file 			
			remove(prevname.c_str());
		}

		// Close the file		
		m_outFile.flush();
		m_outFile.close();

		/*
		* TD4889 - Once the file has been closed, clear its internal flags
		*          to ensure that the object is clean.
		*/
		m_outFile.clear();

		// Create the new file and append a new file count
		newname = _CreateFilename(m_filenameNumber[m_lastMsgLevel]);
	}

	//std::string newname = createFilename(m_fileCount);

	//TA_DEBUG_ASSERT ( newname.c_str() != NULL, "File string is NULL" );

	m_outFile.open(newname.c_str(),std::ios::out|std::ios::trunc);
	if (m_outFile.fail())
	{
		std::cerr << "Error opening file '" << newname << "'! - The reason is: "
			<< strerror( errno )
			<< std::endl;   
	}

	// File will contain an EOF character so increment file count - and
	// even if it doesn't, the size is only an approximation anyway to
	// prevent the file growing without a limit, so it doesn't matter if
	// it is a few bytes out

	++m_sizeInBytes;
}

void DebugUtil::_LogMessage(int nStrLen, const std::string file, int line, std::string& strLogLevel, va_list args)
{
	// Can't do std::ostream outfile = std::cout as the solaris compiler
	// says the copy constructor is private. Use a flag instead.

	bool writeToCout = true;
	int msgLen = 0;

	if ( m_outFile.is_open() )
	{
		// If the current log file has reached its maximum
		// size then write to a new file.

		if ( m_sizeInBytes >= m_maxSizeInBytes )
		{
			_SwapFiles();
		}

		// Check that there are no problems with the outstream

		if ( !m_outFile.good() )
		{
			std::cerr << "Error logging message to file. Message will now be logged to stdout." << std::endl;

			// Close the file so all messages will be logged to stdout from now on.
			// We don't want some messages logged to stdout and some to the file.
			try
			{

				/*
				* TD4889: Only close the file if it has been openned,
				*         otherwise don't bother.  The call to close()
				*         is actually where this defect shows itself.
				*/
				if( true == m_outFile.is_open() )
				{
					m_outFile.close();
					m_outFile.clear();
				}
			}
			catch ( ... )
			{
			}
			m_filename = "";
		}
		else // out stream is fine - write to file instead of cout
		{
			writeToCout = false;
		}
	}

	

	if (writeToCout)
	{
		msgLen = m_logger->logToFile(nStrLen,file, line, strLogLevel, args, std::cout);
	}
	else
	{
		msgLen = m_logger->logToFile(nStrLen,file, line, strLogLevel, args, m_outFile);
	}

	//TA_DEBUG_ASSERT ( msgLen != 0, "Log message was an empty string" );

	m_sizeInBytes += msgLen;
}


std::string DebugUtil::_CreateFilename(int fileNum) const
{
	std::ostringstream filename;
	pid_t pid = getpid();

	// Find where the extension in the filename starts
	// (ie look for the last '.').
	std::string::size_type dotPos = m_filename.rfind('.');

	if (dotPos != std::string::npos)
	{
		// Found a '.'. Take a copy of what's after it.
		std::string extension = m_filename.substr(dotPos + 1, m_filename.length());

		if (m_encodePidInFilenames == false)
		{

			if (atoi(extension.c_str()) == 0 && extension != "0")
			{
				// Extension is a normal string, not a number.
				// (This is most likely the case for Windows.)

				// Add everything before the '.' to the filename.
				filename << m_filename.substr(0, dotPos);

				// Add the '.' and the number.
				filename << "." << fileNum;

				// Add the extension.
				filename << "." << extension;
			}
			else
			{
				// Extension is a number. (Most likely Solaris)
				// Just add the log number to the end.   
				filename << m_filename << "." << fileNum;
			}

		}
		else
		{			
			if (atoi(extension.c_str()) == 0 && extension != "0")
			{
				// Extension is a normal string, not a number.
				// (This is most likely the case for Windows.)

				// Add everything before the '.' to the filename.
				filename << m_filename.substr(0, dotPos);

				// Add the process id
				filename << "." << pid;

				// Add the '.' and the number.
				filename << "." << fileNum;

				// Add the extension.
				filename << "." << extension;
			}
			else
			{
				// Extension is a number. (Most likely Solaris)
				// Just add the log number to the end.   
				filename << m_filename << "." << pid << "." << fileNum;
			}
		}
	}
	else
	{
		if (m_encodePidInFilenames == false)
		{
			// No extension at all. Add the log number to the end.
			filename << m_filename << "." << fileNum;
		}
		else
		{
			filename << m_filename << "." << pid;
		}
	}

	return filename.str();
}


TA_TwoToOnePokerGameNet::ReEntrantThreadLockable& DebugUtil::getLock()
{
	// Fix for TD2689: m_lock was not being instantiated when DebugUtil was instantiated
	// from a static object. An accessor method is now used so that construction of the
	// lock is guaranteed.

	// Fix for TD2689: This defect was a sequencing problem - we have no control over the 
	// order of when the static objects are deleted. This defect was caused by the static
	// lock created below being deleted midway through DebugUtil::getInstance() when it was
	// passed to ThreadGuard constructor. By the time ThreadGuard called aquire() on the lock, 
	// it had been deleted and hence the pure virtual acquire() in IThreadLockable was being
	// called, aborting the program. A solution around this was to create the lock as a pointer,
	// as static simple datatype members (pointers, ints, chars) are not destructed, hence this 
	// problem will be overcome. A side affect is a memory leak when the singleton is destroyed, 
	// but this is small compared to the application crashing.
	static TA_TwoToOnePokerGameNet::ReEntrantThreadLockable* lock = new TA_TwoToOnePokerGameNet::ReEntrantThreadLockable;
	return *lock;
}



bool DebugUtil::_PreLogChecks(EDebugLevel level)
{
	TA_TwoToOnePokerGameNet::ThreadGuard guard (getLock());
	
	// OK to log message
	bool foundFile = false;
	int intLevel = static_cast<int>(level);



	if (foundFile == false)
	{
		// Check to see if the current filename
		// is not the same as the default
		if (m_filename != m_defaultFilename)
		{
			return false;
		}
	}

	return false;
}


int DebugUtil::_GetFileSize(std::string& filename)
{
	//TA_DEBUG_ASSERT(!filename.empty(), "Filename is empty, cannot acertain size of file");

	// Check the size of the file
	std::ifstream inFile;
	inFile.open(filename.c_str());

	int fileSize = 0;

	if (!inFile.fail())
	{
		// Try to determine the size of the file

#if defined (SOLARIS) || defined (LINUX)
		struct stat buf;
#else
		struct _stat buf;
#endif
		int result;

#if defined (SOLARIS) || defined (LINUX)
		result = stat ( filename.c_str(), &buf );
#else
		result = _stat ( filename.c_str(), &buf );
#endif

		if( result != 0 )
		{
			// If we can't get the size of the file, don't want to risk 
			// writing to the file in case it exceeds the max size.
			// So write to stdout instead.

			std::cerr << "Cannot append to file: " << filename.c_str() 
				<< "Reason: Cannot determine size of log file." 
				<< "Log messages will be written to stdout" << std::endl;
			m_filename = "";
			return -1;
		}
		else
		{
			fileSize = buf.st_size;
		}

		inFile.clear();
		inFile.close();
	}

	return fileSize;
}


NS_END(TA_TwoToOnePokerGameNet)




