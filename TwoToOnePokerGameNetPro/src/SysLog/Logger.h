#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED



#include "CommonDef.h"
#include "CommonData.h"

// Forward declaration


// Class Definition: Logger

NS_BEGIN(TA_TwoToOnePokerGameNet)

class Logger
{
public:

	/**
	* Logger
	* 
	* Constructor
	*
	*/
	Logger();

	/**
	* ~Logger
	* 
	* Destructor
	*
	*/
	virtual ~Logger();

	/**
	* log
	* 
	* Constructs the log message with the variable argument list
	* based on the log type and simpleLog flag and outputs the
	* complete message to the specified destination
	*
	* @return Size of the log message in bytes
	*
	* @param type		 Debug type
	* @param file		 Name of file
	* @param line		 Line number in file
	* @param args		 Pointer to a list of variable arguments
	* @param outfile	 Destination to output log message
	* @param simpleLog Flag indicating simple log or full log
	*
	* Pre: file is not empty
	*		 line is greater than 0
	*/
	int logToFile(int nStrLen, const std::string& file, int line, std::string& strLogLevel, va_list args, std::ostream& outfile);


private:	
	Logger(const Logger& oldLogger);
	const Logger& operator=(const Logger& rhs); 
	
	std::string _ConstructMessage(int nStrLen, const std::string& file, int line, std::string& strLogLevel, va_list args);							   
	std::string _ConstructMsgBody(int nStrLen, va_list args);
	std::string _ConstructMsgHeader(int nStrLen, const std::string& file, int line, std::string& strLogLevel);
	unsigned int _PthreadSelf();

};

NS_END(TA_TwoToOnePokerGameNet)

#endif // LOGGER_H_INCLUDED
