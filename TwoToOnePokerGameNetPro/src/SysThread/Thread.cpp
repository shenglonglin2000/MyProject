#include "Thread.h"

// #ifdef WIN32
// #include <windows.h>
// #elif defined ( SOLARIS ) || defined ( LINUX )
// #include "pthread.h"
// #else
// #error Unsupported platform!
// #endif // WIN32


#include "TA_String.h"
#include "UtilityFun.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)


 

// runThread()
//
// This is a C-style function which is called from the thread object. It
// expects its parameter to be a pointer to a Thread object, which it
// will run
//
//static void* runThread(void* ptr)
void* runThread(void* ptr)
{
	//TA_DEBUG_ASSERT(ptr != NULL, "Null pointer passed to runThread()");

	//Thread* myThread = (Thread*) ptr;
	Thread* myThread = static_cast<Thread *>(ptr);
	myThread->m_ThreadState = THREAD_STATE_RUNNING;

	// terminateAndWait() can't set m_thread back to NULL until after this method finishes,
	// so m_thread MUST be valid here.
	//TA_Assert(myThread->m_thread != NULL);

	// printf("&i = %p\n", &i)	// printf Thread address
	_LOG(SourceFLInfo, DebugDebug, 
		"runThread(): myThread=%s about to call run()", 
		gPtrToStr(myThread).c_str() );

	try
	{
		myThread->run();
	}
	catch (std::exception& e)
	{
		std::string msg( "runThread(): " );
		msg.append( e.what() );
		_LOG( SourceFLInfo, DebugError, "std::exception  name=%s  msg=%s", 
		typeid(e).name(), msg.c_str() );
	}
	catch (...)
	{
		_LOG( SourceFLInfo, DebugError, "runThread(): Unknown exception" );
	}

	_LOG(SourceFLInfo, DebugDebug, 
		"runThread(): myThread=%s finished calling run()",  
		gPtrToStr(myThread).c_str());
	 
	myThread->m_ThreadState = THREAD_STATE_RUNEND; 
	return NULL;
}


Thread::Thread()
{
//m_priority(omni_thread::PRIORITY_NORMAL)

	m_ThreadState = THREAD_STATE_UNKNOWN;

	m_thread = (pthread_t *)malloc(sizeof(pthread_t));
	if(NULL == m_thread)
	{
		_LOG(SourceFLInfo, DebugError, "malloc error...");
	} 
	else
	{
		m_ThreadState = THREAD_STATE_NEW;
	}

}

Thread::~Thread()
{
	if(NULL != m_thread) 
	{
		_LOG(SourceFLInfo, DebugDebug,
			"Thread::~Thread(): this=%s destructor called", 
			gPtrToStr(this).c_str());

		free(m_thread);
		m_thread = NULL; 
	}

}


void Thread::start()
{
	int status = 0;		 

	// start() must not be called if the thread is already running.
	// terminateAndWait() must be called in between successive calls to start().
	if (THREAD_STATE_RUNNING ==	m_ThreadState
		|| THREAD_STATE_RUNEND == m_ThreadState)
	{
		_LOG(SourceFLInfo, DebugError,
			"Thread::start(): this=%s is already running.", gPtrToStr(this).c_str() );
		return;
	}

	_LOG(SourceFLInfo, DebugDebug, 
		"Thread::start(): this=%s about to start.", gPtrToStr(this).c_str() );

	// THREAD_STATE_NEW only occurs during the following two calls
	status = pthread_create(m_thread, NULL, runThread, this); 

	if(0 != status)
	{
		_LOG(SourceFLInfo, DebugError, "pthread_create [status=%d][m_thread=%s]", 
			status, gPtrToStr(m_thread).c_str());
	}


	_LOG(SourceFLInfo, DebugDebug, 
		"Thread::start(): this=%s has been started successfully. m_thread=%s", 
		gPtrToStr(this).c_str(), gPtrToStr(m_thread).c_str() );
}


void Thread::terminateAndWait()
{
	int status = 0; 

	if (NULL != m_thread)
	{
		_LOG(SourceFLInfo, DebugDebug, 
			"Thread::terminateAndWait(): this=%s about to call terminate()", 
			gPtrToStr(this).c_str() );

		terminate();

		_LOG(SourceFLInfo, DebugDebug, 
			"Thread::terminateAndWait(): this=%s about to wait.",
			 gPtrToStr(this).c_str() );

		status = pthread_join(*m_thread, NULL);
		if(status != 0)
		{
			_LOG(SourceFLInfo, DebugError, "pthread_join [status=%d][m_thread=%s]", 
				status, gPtrToStr(m_thread).c_str());
		}
		m_ThreadState = THREAD_STATE_TERMINATED;
		m_ThreadState = THREAD_STATE_RUNEND;


		_LOG(SourceFLInfo, DebugDebug, 
			"Thread::terminateAndWait(): this=%s has stopped.", 
			gPtrToStr(this).c_str() );
	}
}


void Thread::sleep(unsigned int milliSeconds)
{  
	//0.001 s = 1 Millisecond = 1000 Microseconds = 1000 000	Nanoseconds

	struct timespec delayVal; 
		
	long  nSleepMilliseconds = 0; 
	long  nSleepMicroseconds = 0;
	long  nSleepNanoseconds = 0; 
	long  nSleepSeconds = 0;
	
	nSleepSeconds =  milliSeconds / 1000;
		
	nSleepMilliseconds = milliSeconds % 1000;
	nSleepMicroseconds = nSleepMilliseconds * 1000;
	nSleepNanoseconds = nSleepMicroseconds * 1000;

	delayVal.tv_sec = nSleepSeconds;
	delayVal.tv_nsec = nSleepNanoseconds;	
	
	pthread_delay_np (&delayVal);
}


#if 0
void Thread::sleep(unsigned int milliSeconds)
{
	int Sec = 0;
	int uSec = 0;
	Sec = milliSeconds / 1000;
	uSec = milliSeconds % 1000;
	CUtilityFun::getInstance().mysys_boost_sleep(Sec, uSec);
}
#endif

 

void Thread::setPriority(int newPriority)
{
	//TA_DEBUG_ASSERT((newPriority >= 0)&&(newPriority < 3), "Invalid priority level");      

	m_priority = newPriority;
}


unsigned int Thread::getId() const
{
	return (m_thread != NULL) ? m_thread->x : static_cast< unsigned int >( -1 );
}


unsigned int Thread::getCurrentThreadId()
{
#ifdef WIN32
//	return ::GetCurrentThreadId();
#elif defined ( SOLARIS ) || defined ( LINUX )
// 	return pthread_self();
#else
#error Unsupported Platform!
#endif // WIN32
	unsigned int threadWin32Tmp = 0;
	pthread_t threadLinuxTmp;
	
	//threadWin32Tmp = ::GetCurrentThreadId();

	threadLinuxTmp = pthread_self();
	
	_LOG(SourceFLInfo, DebugDebug, 
		"fun getCurrentThreadId() [GetCurrentThreadId=%d][threadLinuxTmp.x=%d]", 
		threadWin32Tmp, threadLinuxTmp.x);
	
	return threadLinuxTmp.x;

}


EThreadState Thread::getCurrentState() const
{
	return m_ThreadState;
}
NS_END(TA_TwoToOnePokerGameNet)




