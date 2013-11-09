#include "Logger.h"

#ifdef WIN32
#include <Windows.h>
#endif



NS_BEGIN(TA_TwoToOnePokerGameNet)


Logger::Logger()
{
}


Logger::~Logger()
{

}

int Logger::logToFile(int nStrLen,const std::string& file, int line, std::string& strLogLevel, va_list args, std::ostream& outfile)
{	
	std::string logMsg;
	// If it's a simple log, don't care whether file is empty or not, or what the line is.
	// If it's a full log, file must not be empty, file must not be NULL, and line
	// must be greater than 0.

	//TA_DEBUG_ASSERT ( false == file.empty(), "File is empty string" );
	//TA_DEBUG_ASSERT ( file.c_str() != NULL, "File is NULL" );
	//TA_DEBUG_ASSERT ( (line > 0 || line == -1) , "Line is <= 0" );

	// Construct the log message
	logMsg = _ConstructMessage (nStrLen, file, line, strLogLevel, args);

	// Ensure that an empty log message has not been provided
	//TA_DEBUG_ASSERT( logMsg != "", "Log string is empty");

	// Print the log message
	outfile << logMsg.c_str();
	outfile.flush();

	// Return the number of characters written
	return logMsg.size();
}


std::string Logger::_ConstructMsgBody(int nStrLen, va_list args)
{
	char* format = "";
	std::string testFormat;
	std::string strMsgBody;
	char *buff = NULL;
	int ret = -1;
	try
	{
		format = va_arg ( args, char* );
		testFormat = format;

		//TA_DEBUG_ASSERT ( testFormat.size() != 0, "Empty log message" );
		//TA_DEBUG_ASSERT ( format != "", "Empty log message" );
		//TA_DEBUG_ASSERT ( format, "Empty log message" );
		//TA_DEBUG_ASSERT ( testFormat.c_str() != NULL, "Empty log message" );
	}		
	catch ( ... )
	{
		throw std::exception("A char * was not passed in as the format string");
	}

	// Construct message from format string and args
	strMsgBody.clear();

	try
	{
		buff = new char[nStrLen];
		memset(buff, 0, nStrLen);

		ret = vsnprintf ( buff, nStrLen, format, args );
		if ( ret > nStrLen || ret < 0 )
		{
			std::ostringstream str;
			str << "ERROR: DebugMsgConstructionException: Message length exceeds " << nStrLen << " characters: " << format;
			strMsgBody = str.str();
			//throw TA_Base_Core::DebugMsgConstructionException(str.str());
		}
		else
		{
			strMsgBody = buff;
		}

		if (NULL != buff)
		{
			delete[] buff;
			buff = NULL;
		}
	}
	catch ( ... )
	{
		std::ostringstream str;
		str << "ERROR: DebugMsgConstructionException: Invalid argument types (was a std::string used?): " << format;
		strMsgBody = str.str();
	}

	//TA_DEBUG_ASSERT ( !strMsgBody.empty(), "Empty log message" );

	//message = message.insert ( 0, "    "); // Preprend indentation
	//message += "\n";

	if (NULL != buff)
	{
		delete[] buff;
		buff = NULL;
	}
	return strMsgBody;
}



unsigned int Logger::_PthreadSelf()
{
#ifdef WIN32
	return::GetCurrentThreadId();
#else
	return pthread_self();
#endif

}


std::string Logger::_ConstructMsgHeader(int nStrLen, const std::string& file, int line, std::string& strLogLevel)
{  
	std::string strLogMsgHeader;
	struct timeb timebuffer;
	struct tm newtime;
	std::string strLogMsgHeaderDateTime;///t[11/01/2013 14:44:07:906]
	std::string strLogMsgHeaderLogLevel;//[INFO]
	std::string strLogMsgHeaderThreadId;//[Thrd: 6248000]
	std::string strLogMsgHeaderFileName;//d:\googledrive1987.cpp:
	std::string strLogMsgHeaderLineNumber;   //234:

	pid_t		pid;
	unsigned int   threadID;

	char* pszBuffLogHeader = NULL;
	pszBuffLogHeader = new char[nStrLen];

	//construct log header

	//TA_DEBUG_ASSERT ( file.c_str() != NULL && !file.empty(), "Filename is NULL" );
	//TA_DEBUG_ASSERT ( line > 0 || line == -1, "Line number <= 0" );
	//TA_DEBUG_ASSERT ( strLogLevel.c_str() != NULL && !strLogLevel.empty(), "Level is NULL" );

	// Retrieve Time and Date
	ftime ( &timebuffer ); 

#if defined ( SOLARIS ) || defined( LINUX )
	newtime = *localtime_r ( &timebuffer.time, &newtime );
#else
	newtime = *localtime ( &timebuffer.time );
#endif
	//1
	strLogMsgHeaderDateTime.clear();
	if ( ( &newtime ) != NULL )
	{
		memset(pszBuffLogHeader, 0, nStrLen);
		sprintf ( pszBuffLogHeader, "[%02d/%02d/%02d %02d:%02d:%02d:%03d]", newtime.tm_mday, newtime.tm_mon + 1, newtime.tm_year + 1900, 
			newtime.tm_hour, newtime.tm_min, newtime.tm_sec, timebuffer.millitm );
		strLogMsgHeaderDateTime += "\t";
		strLogMsgHeaderDateTime = pszBuffLogHeader;
	}
	
	//2
	strLogMsgHeaderLogLevel.clear();
	strLogMsgHeaderLogLevel += "[";
	strLogMsgHeaderLogLevel += strLogLevel;
	strLogMsgHeaderLogLevel += "]";

	//3
	// Retrieve Thread Id
	memset(pszBuffLogHeader, 0, nStrLen);
	strLogMsgHeaderThreadId.clear();
#if defined ( SOLARIS ) || defined ( LINUX )
	//sprintf ( pszBuffLogHeader, "[Thrd: %d]", pthread_self() );
	//threadId = pszBuffLogHeader;
#endif
#if defined( WIN32 )
	//sprintf ( pszBuffLogHeader, "[Thrd: %d]", GetCurrentThreadId() ); 
	//threadId = pszBuffLogHeader;
#endif

	pid = getpid();
	threadID = _PthreadSelf();
	sprintf ( pszBuffLogHeader, "[PID:%lu][Thrd: %lu]", (unsigned int)pid, (unsigned int)threadID);
	strLogMsgHeaderThreadId = pszBuffLogHeader;

	strLogMsgHeaderFileName.clear();
	strLogMsgHeaderFileName = "  ";
	strLogMsgHeaderFileName += file;
	strLogMsgHeaderFileName += ":";

	// Convert line to string

	memset(pszBuffLogHeader, 0, nStrLen);
	sprintf ( pszBuffLogHeader, "%d:", line );
	strLogMsgHeaderLineNumber = pszBuffLogHeader;

	// Construct the log message
	
	strLogMsgHeader.clear();
	
	 // Indent so that the first line of the log message is obvious when word wrapping is enabled
	//strLogMsgHeader += "\t";
	strLogMsgHeader += strLogMsgHeaderDateTime;
	strLogMsgHeader	+= strLogMsgHeaderLogLevel;
	strLogMsgHeader	+= strLogMsgHeaderThreadId;
	strLogMsgHeader	+= strLogMsgHeaderFileName;
	strLogMsgHeader	+= strLogMsgHeaderLineNumber;


	if ( strLogMsgHeader.empty() || strLogMsgHeader.c_str() == NULL )
	{
		std::ostringstream str;
		str << file << " (" << line << "): An (impossible) error occurred while formatting the log message";
		std::string strErrorInfo = str.str();
		throw std::exception(strErrorInfo.c_str());
	}



	if (NULL != pszBuffLogHeader)
	{
		delete[] pszBuffLogHeader;
		pszBuffLogHeader = NULL;
	}


	return strLogMsgHeader;
}

std::string Logger::_ConstructMessage(int nStrLen, const std::string& file, int line, std::string& strLogLevel, va_list args)
{  
	std::string strLogMsgHeader;
	std::string strLogMsgBody;
	std::string strLogMsg; 	
	
	//construct log header
	strLogMsgHeader = _ConstructMsgHeader(DEF_INT_MinMesgBuffSize, file, line, strLogLevel);
	//construct log body 
	strLogMsgBody = _ConstructMsgBody(nStrLen, args);

	strLogMsg = strLogMsgHeader + strLogMsgBody;
	strLogMsg += "\n";

	return strLogMsg;
}




NS_END(TA_TwoToOnePokerGameNet)

