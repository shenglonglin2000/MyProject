#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include "CommonDef.h"

#include "pthread.h"
#include "DebugUtil.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)

class Thread
{
public:
	Thread(); 
	virtual ~Thread();
	virtual void run() = 0;	
	virtual void terminate() = 0;

public:
	void start();
	void terminateAndWait();

public:	
	void setPriority(int newPriority);
	unsigned int getId() const;
	static void sleep(unsigned int milliseconds);
	static unsigned int getCurrentThreadId();
	
public:	
	EThreadState getCurrentState() const;
	
public:
	friend void * runThread(void * args);   
	
private: 
	pthread_t* m_thread; 
	
public:
	unsigned int    m_priority;
	EThreadState    m_ThreadState;
};
void* runThread(void * args);


NS_END(TA_TwoToOnePokerGameNet)

#endif  // THREAD_H_INCLUDED
