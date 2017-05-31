#pragma once
#include <string>

class CTimeSystem
{
public:
	CTimeSystem();
	//取得当前时间
	unsigned int						GetTimeNow(void) const { return m_dwTimeNow; }

public:
	//逻辑轮循函数
	virtual void						Tick();
    //取当前逻辑帧的时间点
	virtual unsigned int				GetFrameTime();
	//计算时差
	virtual unsigned int				CalSubTime(unsigned int timeStart, unsigned int timeEnd);
	//计算上一帧所用时间
	virtual unsigned int				GetLastFrameTime();
	//获取时间
	virtual std::string					GetDateTimeAsString();
	//获取时间
	virtual unsigned int				GetDateTime();
	//获取逻辑帧数
	virtual unsigned int				GetFrame() { return m_nFrame; }
	//获取逻辑计数时间
	virtual unsigned int				GetPastTime() { return m_dwPastTime; }
	//设置时钟倍速
	virtual void						SetTimeScale(float scale) { m_fTimeScale = scale; }

private:
	unsigned int		m_dwTimeNow;		//当前时间
	unsigned int		m_dwLastFrameTime;	//最后一帧间隔时间
	unsigned int		m_dwPastTime;		//从服务器启动到当前经历的时间
	unsigned int		m_nFrame;			//逻辑帧个数
	float				m_fTimeScale;		//时钟倍速
};

CTimeSystem &TimeSystem();