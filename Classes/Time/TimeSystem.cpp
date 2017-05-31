#include "TimeSystem.h"
#include "cocos2d.h"
#include <time.h>

USING_NS_CC;
CTimeSystem &TimeSystem()
{
	static CTimeSystem s;
	return s; 
}

CTimeSystem::CTimeSystem()
	: m_dwTimeNow(0)
	, m_dwLastFrameTime(0)
	, m_dwPastTime(0)
	, m_nFrame(0)
	, m_fTimeScale(1)
{
}

void CTimeSystem::Tick()
{
	//-------------------------------------------------------------
	//记录当前时间
	unsigned int nFrameTime = GetFrameTime();
	m_dwLastFrameTime = nFrameTime - m_dwTimeNow;
	m_dwLastFrameTime = (m_dwLastFrameTime > 50?50:m_dwLastFrameTime) * m_fTimeScale;
	m_dwPastTime += m_dwLastFrameTime;

	m_dwTimeNow = nFrameTime;
	m_nFrame++;
}

unsigned int CTimeSystem::GetFrameTime()
{
	timeval now;
	gettimeofday(&now, NULL);
	return static_cast<unsigned int>((now.tv_sec * 1000 + now.tv_usec / 1000)); 
}

unsigned int CTimeSystem::CalSubTime(unsigned int timeStart, unsigned int timeEnd)
{
	if(timeEnd < timeStart) 
		return ((unsigned int)0XFFFFFFFF-timeStart) + timeEnd;
	else 
		return timeEnd-timeStart;
}

unsigned int CTimeSystem::GetLastFrameTime()
{
	return m_dwLastFrameTime;
}

//获取时间
std::string CTimeSystem::GetDateTimeAsString()
{
	time_t rawTime;
	struct tm* timeInfo;
	
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	
	std::stringstream ss;
	ss << timeInfo->tm_year << ":" 
		<< timeInfo->tm_mon << ":" 
		<< timeInfo->tm_mday << ":" 
		<< timeInfo->tm_hour << ":"
		<< timeInfo->tm_min << ":"
		<< timeInfo->tm_sec;
	
	return ss.str();
}


//获取时间
unsigned int CTimeSystem::GetDateTime()
{
	time_t rawTime = 0;

	time(&rawTime);			// 获取时间，单位是毫秒 这个时间是 格林尼治时间

	rawTime += 8*3600;		// 因为北京是东八区时间，所以需要加上8小时

	return rawTime / ( 24 * 3600 );
}
