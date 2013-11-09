#ifndef DEBUGUTIL_H_INCLUDED
#define DEBUGUTIL_H_INCLUDED

/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/utilities/src/DebugUtil.h $
* @author:  Ripple
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* <description>
*
*/

#include "CommonDef.h"
#include "CommonData.h"
#include "ReEntrantThreadLockable.h"
#include "ThreadGuard.h"


// From windows.h
typedef struct _CONTEXT CONTEXT;


NS_BEGIN(TA_TwoToOnePokerGameNet)

class Logger;

class DebugUtil
{
public:

	/**
	* getInstance
	* 
	* Returns a single static instance of DebugUtil
	*
	* @return The one and only DebugUtil instance
	*/
	static DebugUtil& getInstance();

	static void removeInstance();

	/**
	* ~DebugUtil
	* 
	* Destructor
	*/
	virtual ~DebugUtil();

	/**
	* log
	* 
	* Outputs a log message with the time, date, thread ID, debug level
	* and location details. Any strings passed into ... must be passed
	* in as a const char* not a std::string. This is because they can 
	* only be read from ... as a char*.
	*
	* @param file Name of file
	* @param line Line number in file
	* @param type Debug type
	* @param ...  Variable arguments
	*
	* Pre: file is not an empty string
	*      line is greater than 0 or is -1 if unknown
	*/
	void log(const std::string& file, int line, EDebugLevel level, ...);
	void logLarge(const std::string& file, int line, int nStrlen, EDebugLevel level, ...);

	/**
	* setFile
	* 
	* Sets the log file to the file name specified.
	*
	* @param name Name of log file
	*
	* Pre: file is not an empty string
	*/
	void setFile(const char* name);

	/**
	* getStringDebugLevel
	* 
	* Convert enum representation of debug level to string.
	*
	* @param level Enum representation of debug level
	*/
	static const char* getStringDebugLevel(EDebugLevel level);

	/**
	* getDebugLevelFromString
	* 
	* Convert string to enum representation of debug level.
	*
	* @param level Enum representation of debug level
	*/
	static EDebugLevel  getDebugLevelFromString(const char* levelStr);

	/**
	* getLevel
	* 
	* Returns the current debug level
	*
	* @return Current debug level
	*/
	EDebugLevel getLevel() const
	{
		return m_currDebugLevel;
	}

	/**
	* setLevel
	* 
	* Sets the current debug level
	*
	* @param level New debug level
	*/
	void setLevel(EDebugLevel level)
	{
		m_currDebugLevel = level;
	}

	/**
	* getMaxSize
	* 
	* Returns the maximum size of the file in bytes
	*
	* @return Maximum size of file
	*/
	int getMaxSize() const
	{
		return m_maxSizeInBytes;
	}

	/**
	* setMaxSize
	* 
	* Sets the maximum size of the file
	*
	* @param maxSizeInBytes Maximum size of file in bytes
	*/
	void setMaxSize(int maxSizeInBytes)
	{
		m_maxSizeInBytes = maxSizeInBytes;
	}

	/**
	* setMaxFiles
	* 
	* Sets the maximum number of log files
	*
	* @param maxFiles Maximum number of log files
	*/
	void setMaxFiles(int maxFiles)
	{
		m_maxFiles = maxFiles;
	}



private:

	/**
	* DebugUtil
	* 
	* Constructor. This is a singleton so getInstance() calls
	* the constructor.
	*/
	DebugUtil();

	
	/**
	* logMessage
	* 
	* Outputs debug message to appropriate destination.
	*
	* @param file       Name of file
	* @param line       Line number in file
	* @param type       Debug type
	* @param args       Pointer to list of variable arguments 
	* @param simpleLog  Flag to indicate simple/full log
	* @param printCache Flag to indicate whether to print contents of cache
	*
	* Pre: file is not an empty string
	*      line is greater than 0
	*  nStrLen = #define MAXLOGMESGSIZE    1000000
	*#define MINLOGMESGSIZE    200000	
	*/
	void _LogMessage(int nStrLen, const std::string file, int line, std::string& strLogLevel, va_list args);


	/** 
	* createFilename
	*
	* Constructs a string containing the filename to log to. This is
	* simply the filename passed in to setFile(), but with a number 
	* before the extension (eg "logfile.txt" becomes "logfile.3.txt").
	*
	* @param fileNum  The number to place in the filename before the
	*                 extension (3 in the above example).
	*
	* @return A string containing the filename to log to.
	*
	*/
	std::string _CreateFilename(int fileNum) const;


	/**
	* preLogChecks
	*
	* This checks if the debug level is disabled from logging
	* by calling isLoggingDisabledForLevel.  It also does 
	* everything to ensure the correct file is used if a debug
	* level specific file has been specified.
	* 	  
	* @param level The debug level to against.
	* @return bool Flag to indicate if the logging has been
	*		disabled for the particular level.  True if disabled.
	*/
	bool _PreLogChecks(EDebugLevel level);


	/**
	* getFileSize
	*
	* This tries to acertiain the size of a file.
	*
	* @param filename The filename of the file.
	*/
	int _GetFileSize(std::string& filename);

	void _SwapFiles();

	// Fix for TD2689: m_lock was not being instantiated when DebugUtil was instantiated
	// from a static object. An accessor method is now used so that construction of the
	// lock is guaranteed.
	static TA_TwoToOnePokerGameNet::ReEntrantThreadLockable& getLock();
	static DebugUtil* m_instance;

private:
	EDebugLevel m_currDebugLevel;                           
	EDebugLevel m_lastMsgLevel;

	std::string m_filename;
	std::string m_defaultFilename;

	std::fstream m_outFile;

	unsigned int m_maxSizeInBytes;
	unsigned int m_sizeInBytes;

	int m_maxFiles;
	int m_fileCount;

	bool m_firstSetFile;
	bool m_encodePidInFilenames;

	Logger* m_logger;

	// Vector to hold the filename number
	std::vector<int> m_filenameNumber;

};


NS_END(TA_TwoToOnePokerGameNet)


#endif // DEBUGUTIL_H_INCLUDED

