/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/utilities/src/TA_String.h $
* @author:  Ripple
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
*/
#ifndef TA_STRING_H_INCLUDED
#define TA_STRING_H_INCLUDED

// INTELLECTUAL PROPERTY NOTICE:
// Elements of the following library are based on work previously implemented by 
// the author, Lachlan Partington.  No restrictions are placed on Ripple Systems 
// relating to the use of this code, however the author asserts his right to use 
// these and similar implementations in his future work.


#if defined( WIN32 )
#pragma warning( disable : 4786 )
#endif // defined( WIN32 )


#include "CommonDef.h"
#include "CommonData.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)


template <class T>
struct AsPtr_t
{
	AsPtr_t(T* p) : m_p(p) {}
	T* m_p;
};

template <class T>
AsPtr_t<T> AsPtr(T* p)
{
	return AsPtr_t<T>(p);
}


template <class T>
std::ostream& operator<<(std::ostream& stm, const AsPtr_t<T>& obj)
{
	stm << "0x";
	stm << std::hex << obj.m_p << " (";

	if (NULL == obj.m_p)
	{
		try
		{
			stm << typeid(obj.m_p).name(); 
		}
		catch (...)
		{
		}
	}
	else
	{
		try
		{
			stm << typeid(*obj.m_p).name();
		}
		catch (...)
		{
		}
	}

	stm << ")";

	return stm;
}


template <class T>
struct AsDeletedPtr_t
{
	AsDeletedPtr_t(T* p) : m_p(p) {}
	T* m_p;
};




// Converts any type T to a string, providing an appropriate overload of
// operator<<(ostream&, T) exists.
// For example, it can be used for the basic types (char, short, etc),
// pairs, vectors, maps, etc.

// Note that it is preferable to add operator<< overloads rather than new
// versions of toString, because operator<< is more efficient for 
// building up strings.
template <class T>
std::string toString(const T& t)
{
	std::ostringstream stm;
	stm << t;
	return stm.str();    
}



template <class T>
std::string gPtrToStr(T* t)
{
	return toString(AsPtr(t));
}



NS_END(TA_TwoToOnePokerGameNet)

#endif // TA_STRING_H_INCLUDED
