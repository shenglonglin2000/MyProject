///////////////////////////////////////////////////////////
//  CommonDef.h
//  Implementation of the Class CProFunRes
//  Created on:      20-2-2012 09:31:53
//  Original author: Shenglong.Lin
//  #include 
//  #define
//  enum
//  typedef
//  static const char*
///////////////////////////////////////////////////////////



#ifndef  __DEF__COMMON_DEF_H__
#define  __DEF__COMMON_DEF_H__



//////////////////////////////////////////////////////////////////////////
///#define
//////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------
// easy usage of the namespace identifier
#define  NS_BEGIN(name)			namespace name {
#define  NS_END(name)			};
//---------------------------------------------------------------




NS_BEGIN(TA_TwoToOnePokerGameNet) 




//---------------------------------------------------------------
#if !defined(EXTERN_C)
#ifdef __cplusplus
#define EXTERN_C			extern "C"
#else
#define EXTERN_C
#endif
#endif



//---------------------------------------------------------------
//typedef systype_
// for simply use the build-in type
typedef		unsigned char		systype_byte;
typedef     const char          systype_c_char;
typedef		unsigned char		systype_u_char;
typedef		unsigned int		systype_u_int;
typedef		unsigned short		systype_u_short;
typedef		unsigned long		systype_u_long;
//
typedef		unsigned char       systype_uint8;
typedef		unsigned int        systype_uint32;
typedef		int                 systype_int32;

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef     unsigned __int64       systype_ulong64;
typedef     signed   __int64       systype_long64;
#else
typedef     unsigned long long     systype_ulong64;
typedef     signed long long       systype_long64;
typedef     void*                  systype_HANDLE;
#endif
//---------------------------------------------------------------



//---------------------------------------------------------------
//class fun
#define DEF_DISABLE_COPY_AND_ASSIGNMENT(className)   private:   \
	className(const className&); \
	className& operator=(const className&);


//---------------------------------------------------------------
//-function
//---------------------------------------------------------------

#define DEF_DELETE_ARRAY(pArray)		if (pArray) { delete [](pArray); (pArray) = NULL; }
#define DEF_DELETE(p)					if (p) { delete (p); (p) = NULL; }

//---------------------------------------------------------------


//---------------------------------------------------------------
/*
int nInt32Value = 1;
u_char ucszInt32Value[4]  = {0};

nInt32Value = DEF_INT4KORR(ucszInt32Value);
DEF_INT4STORE(ucszInt32Value, nInt32Value);
*/

#define DEF_INT4KORR(A)	(unsigned int) (((unsigned int) ((unsigned char) (A)[0])) +\
	(((unsigned int) ((unsigned char) (A)[1])) << 8) +\
	(((unsigned int) ((unsigned char) (A)[2])) << 16) +\
	(((unsigned int) ((unsigned char) (A)[3])) << 24))
#define DEF_INT4STORE(T,A)       do { *((char *)(T))=(char) ((A));\
	*(((char *)(T))+1)=(char) (((A) >> 8));\
	*(((char *)(T))+2)=(char) (((A) >> 16));\
	*(((char *)(T))+3)=(char) (((A) >> 24)); } while(0)


#define DEF_INT8KORR(A)	((systype_ulong64) (((uint32) ((unsigned char) (A)[0])) +\
	(((systype_uint32)  ((unsigned char) (A)[1])) << 8) +\
	(((systype_uint32)  ((unsigned char) (A)[2])) << 16) +\
	(((systype_uint32)  ((unsigned char) (A)[3])) << 24)) +\
	(((systype_ulong64) (((systype_uint32)  ((unsigned char) (A)[4])) +\
	(((systype_uint32)  ((unsigned char) (A)[5])) << 8) +\
	(((systype_uint32)  ((unsigned char) (A)[6])) << 16) +\
	(((systype_uint32)  ((unsigned char) (A)[7])) << 24)))<<32))
#define DEF_INT8STORE(T,A)       do { *((char *)(T))=(char) ((A));\
	*(((char *)(T))+1)=(char) (((A) >> 8));\
	*(((char *)(T))+2)=(char) (((A) >> 16));\
	*(((char *)(T))+3)=(char) (((A) >> 24));\
	*(((char *)(T))+4)=(char) (((A) >> 32));\
	*(((char *)(T))+5)=(char) (((A) >> 40));\
	*(((char *)(T))+6)=(char) (((A) >> 48));\
	*(((char *)(T))+7)=(char) (((A) >> 56)); } while(0)


//---------------------------------------------------------------


//---------------------------------------------------------------
//log--------
									 
#define SourceFLInfo							__FILE__,__LINE__	

#define _LOG									DebugUtil::getInstance().log
#define _LogLarge								DebugUtil::getInstance().logLarge  


//log-------------------------------


enum def_int_Code_Value 
{
	DEF_INT_MaxLogMesgSize			=			1024*1024,
	DEF_INT_MinLogMesgSize			=			1024*200,
	DEF_INT_MinMesgBuffSize			=			512,
	
	
	DEF_INT_BUFFER_LEN_ADDEX		=           512,
	DEF_INT_BUFFER_LEN_READ			=			0xFFFF,//65535 64K
	DEF_INT_BUFFER_LEN_MAX			=			0xFFFFFFF,//268M
	
	DEF_INT_SingleLineMaxSize		=			1024*8,
	DEF_INT_NormalThreadSleep		=			100,
	DEF_INT_MonitorThreadSleep		=           1000,

	DEF_INT_LIMIT_TCP_TIMEOUT       =			500,

};




enum TCP_STATE_Code
{
	TCP_CONNECTED        = 100,
	TCP_DISCONNECTED     = 101,
	TCP_NODATAFORREAD    = 102,
};


//-------cfg file--------------------------------------------------------

#define					def_char_cfguse_lineCommont				';'
#define					def_char_cfguse_lineEnd				    '\r'
 
#define					def_str_cfguse_BlockBegin			 	"["
#define					def_str_cfguse_BlockEnd				    "]"
#define					def_str_cfguse_SubBlockBegin			"{"
#define					def_str_cfguse_SubBlockEnd				"}"
#define					def_str_cfguse_AttriMiddle			    "="
#define					def_str_cfguse_lineCommont				";"






//--------------------------------------------------------------- 
#define					defConfig_LogLevel						"loglevel"
#define					defConfig_LogFileSize					"logfilesize"
#define					defConfig_LogFileCount					"logfilecount"
#define					defConfig_LogFileName					"logfilename"






//---------------------------------------------------------------



NS_END(TA_TwoToOnePokerGameNet)

#endif  // __DEF__COMMON_DEF_H__











