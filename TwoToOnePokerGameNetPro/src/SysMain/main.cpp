#include "vld.h"
#include <signal.h>

#include "CommonDef.h"
#include "CommonData.h"
#include "UtilityFun.h"
#include "DebugUtil.h"
		 
///
#include "ThreeCPUPlayGame.h"

using namespace TA_TwoToOnePokerGameNet;



//////////////////////////////////////////////////////////////////////////
//global var
bool g_b_main_continue = true;


//////////////////////////////////////////////////////////////////////////

void usr_signal(int SigNo)
{
	if (SigNo == SIGINT)
	{
		g_b_main_continue = false;
	}
	
#ifndef WIN32
	if (SigNo == SIGHUP)
	{
	}
#endif

	return;
}


void  setProgramSignal()
{
	signal(SIGINT, usr_signal);
#ifndef WIN32
	signal(SIGHUP, usr_signal);	//close putty
#endif

	return;
}

//////////////////////////////////////////////////////////////////////////




int main( int argc, char* argv[] )
{ 
	int nStop = 0;
	setProgramSignal();
	CUtilityFun::getInstance();
	TA_TwoToOnePokerGameNet::DebugUtil::getInstance();	

	DebugUtil::getInstance().setFile("./TwoToOnePokerGamePro.log");
	DebugUtil::getInstance().setLevel(DebugTrace);
	DebugUtil::getInstance().setMaxFiles(100);
	DebugUtil::getInstance().setMaxSize(20*1024*1024);

	_LOG(SourceFLInfo, DebugDebug, "main start");
	//////////////////////////////////////////////////////////////////////////

	CThreeCPUPlayGame* pThreeCPUPlayGame = NULL;
	pThreeCPUPlayGame = new CThreeCPUPlayGame();

	pThreeCPUPlayGame->start();

	while (false == pThreeCPUPlayGame->isFinishWork())
	{
		Thread::sleep(10000);
	}
	pThreeCPUPlayGame->terminateAndWait();

	delete pThreeCPUPlayGame;
	pThreeCPUPlayGame = NULL;

	//////////////////////////////////////////////////////////////////////////
	_LOG(SourceFLInfo, DebugDebug, "main end");
	CUtilityFun::removeInstance();
	DebugUtil::removeInstance();
	return 0;
}











